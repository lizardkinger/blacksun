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
 *  File:       ImporterMD2.cpp
 *  Created:    28.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/ImporterMD2.h"
#include "../include/ImporterMD2Loader.h"
#include <QtGui>

using namespace BSRenderer;
using namespace BSScene;

bool ImporterMD2::execute()
{
	static int i=0;
	if(i==0)
	{
		QString sMessage = "";
		sMessage += "Steps to load a full textured MD2-model:\n";
		sMessage += "(1) Load a md2-mesh. A new material for this mesh will be created\n";
		sMessage += "(2) Install the TextureLoaderPack and the MaterialEditor\n";
		sMessage += "(3) Open the MaterialEditor, select the material and load a texture ";
		sMessage += "to by clicking on the combobox with '-No-'. The skin-texture has ";
		sMessage += "the same file-name as the model.\n";
		sMessage += "(4) Activate the texture by select the texture in the list\n";
		sMessage += "(5) Close the material editor";
		
		QMessageBox::information(NULL, "Info", sMessage);
		i=1;
	}
	
	QString sFilter = getFileDescription().c_str();
	sFilter += " (*.";
	sFilter += getFileExtension().c_str();
	sFilter += ")";
	
	QString sFileName = QFileDialog::getOpenFileName(NULL,
		"Import model file", QDir::currentPath(),
		sFilter);	

	//QFileDialog aborted?
	if(sFileName == "")
		return true;

	vector<int> indices; 
	vector<BSRenderer::Vertex> verts;
	int nMaterialID = -1;
	
	if(ImporterMD2Loader().execute(sFileName.toStdString(), indices, verts)==false)
		return false;
		
	//Split model-name from path
	QStringList list = sFileName.split("/");
	sFileName = list[list.size()-1];
	list = sFileName.split("\\");
	sFileName = list[list.size()-1];		
		
	//Create new material for the mesh for skin
	MaterialManager* pMatMgr = Renderer::getInstance()->getMaterialManager();
	nMaterialID = pMatMgr->createNewMaterial();
	QString sMatName = sFileName + "-Skin";
	pMatMgr->getMaterial(nMaterialID)->setName(sMatName.toStdString());
	 
	//Add model to scene
	Mesh m;
	m.setName(sFileName);
	BSScene::Polygon p;
	
	//Add vertices and indices to the mesh
	p.setVertices(verts);
	p.setIndices(indices);
	p.setMaterialID(nMaterialID);
	m.addPolygon(p);
	
	//Add new mesh to the scene
	SceneManager::getInstance()->insertObject(m);	
	
	stringstream msg;
	msg << "Model loaded: \"" << sFileName.toStdString() << "\"";
	LOG_Ok(msg.str());	
	
	return true;
}
