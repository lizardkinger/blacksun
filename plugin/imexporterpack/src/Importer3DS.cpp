/***************************************************************************
 *   Copyright (C) 2006-07 by Reinhard Jeschull
 *   rjeschu@fh-landshut.de
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *   (See COPYING for details.)
 ***************************************************************************
 *
 *  Module:     Plugin (BlackSun)
 *  File:       Importer3DS.cpp
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/Importer3DS.h"
#include "../include/Importer3DSLoader.h"
#include <QtGui>

using namespace BSRenderer;
using namespace BSScene;

Importer3DS::Importer3DS(QWidget* parent)
 : Importer(parent)
{
	//Initialize the UI
	m_ui.setupUi(this);
	
	//Connect slots 
	connect(m_ui.selectFileNameButton, SIGNAL(clicked()), this , SLOT(selectFileName()));
	connect(m_ui.selectTextureDirectoryButton, SIGNAL(clicked()), this , SLOT(selectTextureDirectory()));
	connect(m_ui.OKButton, SIGNAL(clicked()), this , SLOT(execute()));	
}

void Importer3DS::selectFileName()
{
	QString sFilter = getFileDescription().c_str();
	sFilter += " (*.";
	sFilter += getFileExtension().c_str();
	sFilter += ")";
	
	QString sFileName = QFileDialog::getOpenFileName(NULL,
		"Import model file", m_ui.fileNameLineEdit->text(),
		sFilter);	

	//QFileDialog aborted?
	if(sFileName == "")
		return;
		
	m_ui.fileNameLineEdit->setText(sFileName);		
}

void Importer3DS::selectTextureDirectory()
{
 	QString sDir = QFileDialog::getExistingDirectory(NULL, 
 		"Open texture directory", m_ui.textureDirectoryLineEdit->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);	

	//QFileDialog aborted?
	if(sDir == "")
		return;
		
	m_ui.textureDirectoryLineEdit->setText(sDir);	
}

bool Importer3DS::execute()
{	
	setVisible(false);          	

	QString sFileName = m_ui.fileNameLineEdit->text();
	if(sFileName == "")
		return true;

	Importer3DSLoader loader;
	if(loader.execute(sFileName.toStdString())==false)
		return false;

	MaterialManager* matMgr = Renderer::getInstance()->getMaterialManager();
	vector< vector<int> > indices; 
	vector< vector<Vertex> > verts;
	vector<int> materials;
	
	QString sTexDir = m_ui.textureDirectoryLineEdit->text();
	if(sTexDir=="")
	{
		int nIndex = max(sFileName.lastIndexOf("/"), sFileName.lastIndexOf("\\"));
		if(nIndex != -1)
			sTexDir = sFileName.left(nIndex);
	}

	//Create data from the 3ds-rawdata
	for(unsigned int i=0; i<loader.getNumMaterials(); i++)
	{
		vector<Vertex> v; 
		vector<int> ind;
		
		Material newMat;

		loader.getMesh(i, ind, v, newMat, sTexDir.toStdString(),
			m_ui.disableLoadingTexturesCheckBox->checkState() == Qt::Checked,
			m_ui.negateVTextureCoordinateCheckBox->checkState() == Qt::Checked);
		
		verts.push_back(v);
		indices.push_back(ind);
				
		//Set new specular power if its zero
		if(m_ui.setZeroSpecularPowerCheckBox->checkState() == Qt::Checked)
		{
			if(newMat.getSpecularFactor()==0.0)
				newMat.setSpecularFactor(m_ui.specularPowerSpinBox->value());
		}
		
		Material* pNewMat = new Material(newMat);
		
		materials.push_back(matMgr->addMaterial(pNewMat));
	}	
		
	//Scale model to a maximum size and transform it to the center
	if(m_ui.scaleToMaxSizeCheckBox->checkState() == Qt::Checked ||
		m_ui.centerMeshCheckBox->checkState() == Qt::Checked)
	{
		setMaxSizeAndCenter(verts, m_ui.maxSizeSpinBox->value(),
			m_ui.centerMeshCheckBox->checkState() == Qt::Checked);
	}
		
	//Split model-name from path
	QStringList list = sFileName.split("/");
	sFileName = list[list.size()-1];
	list = sFileName.split("\\");
	sFileName = list[list.size()-1];			
		
	//Add model(s) to scene
	for(unsigned int i=0; i<verts.size(); i++)
	{
		//Mesh has no indices or vertices?
		if((verts[i].size() == 0) || (indices[i].size() == 0))
			continue;
			
		Mesh m;
		QString sName = sFileName;
		if(verts.size() != 1)
			sName += QString(" ") + QString::number(i+1) + 
			QString("/") + QString::number(verts.size());
		m.setName(sName);
		BSScene::Polygon p;
		
		//Add vertices and indices to the mesh
		p.setVertices(verts[i]);
		p.setIndices(indices[i]); 
		p.setMaterialID(materials[i]);
		m.addPolygon(p);
		
		//Add new mesh to the scene
		SceneManager::getInstance()->insertObject(m);	
	}
	
	stringstream msg;
	msg << "Model loaded: \"" << sFileName.toStdString() << "\"";
	LOG_Ok(msg.str());	
	
	return true;
}

void Importer3DS::setMaxSizeAndCenter(vector< vector<Vertex> >& verts, double dSize, 
	bool dCenter) const
{
	Vector vMin( 999999,  999999,  999999);
	Vector vMax(-999999, -999999, -999999);
	
	for(unsigned m=0; m<verts.size(); m++)
	{
		for(unsigned v=0; v<verts[m].size(); v++)
		{
			vMin = Vector::min(vMin, verts[m][v].vPos);
			vMax = Vector::max(vMax, verts[m][v].vPos);				
		}	
	}
	
	Vector vDimension = vMax - vMin;
	Vector vCenter = vMin + vDimension * 0.5;
	double dMaxDim = max(vDimension.x, max(vDimension.y, vDimension.z));
	if(dMaxDim!=0.0) dMaxDim = 1.0 / dMaxDim;
	dMaxDim *= dSize;

	for(unsigned m=0; m<verts.size(); m++)
	{
		for(unsigned v=0; v<verts[m].size(); v++)
		{
			if(dCenter)
				verts[m][v].vPos -= vCenter;
				
			verts[m][v].vPos *= dMaxDim;				
		}	
	}
}
