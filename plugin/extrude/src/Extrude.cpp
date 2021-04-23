/***************************************************************************
 *   Copyright (C) 2006-07 by Gruber Philipp
 *   pgruber@fh-landshut.de
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
 *  File:       Extrude.cpp
 *  Created:    24.04.2007
 *  Author:     Thomas Kuhndoerfer (lizardkingx)
 *
 **************************************************************************/

#include "./../include/Extrude.h"
#include "./../../gui/include/mainwindow.h"
#include <iostream>


using namespace BSScene;
using namespace BSRenderer;


static Extrude* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Extrude();

	return static_cast<PlgInt*>(_instance);
}

Extrude::Extrude() : version(Version(1, 0, 0))
{
	m_sName = "Extrude-Plugin";
	m_sDescription = "Extrude a selected face";
	
	button = NULL;
}

Extrude::~Extrude()
{
}

bool Extrude::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
	
	return true;
}

bool Extrude::loadPlg()
{
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
	
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/Extrude_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Extrude");
	button->setCheckable(true);
	button->setAutoExclusive(true);
	
	QWidget* pluginSettings = new QWidget();
	pluginSettings->hide();
	m_ui.setupUi(pluginSettings);


    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", button);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("modificatorGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);

	connect(button, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
	connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
	
	return true;
}

void Extrude::execute()
{
	double amount = m_ui.amount->value();
	
	if(m_ui.directionInside->isChecked())
		amount *= -1;
	
 	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		switch( (*it)->getType())
		{ 
			case SCENETYPE_Mesh:
			{
	            Mesh *mesh = dynamic_cast<Mesh*>(*it);
	
				vector<BSScene::Polygon>* polys = mesh->getPolys();
	 		 
				//Walk through all polygons
				for(unsigned int p=0; p<polys->size(); p++)
				{			
					extrudePolygon(&((*polys)[p]), amount);	
				}
				
				SceneManager::getInstance()->notifyContentChanged(mesh->getType(),true);
			}break;
		
			case SCENETYPE_Polygon:
			{
				BSScene::Polygon *p = dynamic_cast<BSScene::Polygon*>(*it);
				extrudePolygon(p, amount);
				
				SceneManager::getInstance()->notifyContentChanged(p->getType(),true);
				
			}break;
		
			default: break;	
		}
   	}
}

void Extrude::extrudePolygon(BSScene::Polygon* p, double dAmount)
{
	if(p==NULL)
		return;
		
	vector<int> indices;
	vector<Vertex> verts;
	
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	
	Vertex newVerts[6];
	
	Vector vUp;
	Vector vNorm;
	int nFace = 0;
	
	for(unsigned int i=0; i<copyIndices.size(); i+=3)
	{	
		//First three vertices are the same
		newVerts[0] = copyVertices[copyIndices[i+0]];
		newVerts[1] = copyVertices[copyIndices[i+1]];
		newVerts[2] = copyVertices[copyIndices[i+2]];
		
	//	qDebug() << "Original Vertex 1: " << newVerts[0].vPos.x << "," << newVerts[0].vPos.y << "," << newVerts[0].vPos.z;
	//	qDebug() << "Original Vertex 2: " << newVerts[1].vPos.x << "," << newVerts[1].vPos.y << "," << newVerts[1].vPos.z;
	//	qDebug() << "Original Vertex 3: " << newVerts[2].vPos.x << "," << newVerts[2].vPos.y << "," << newVerts[2].vPos.z;
		
		//qDebug() << "Old normal: " << newVerts[0].vNormal.x << " , " << newVerts[0].vNormal.y << " , " << newVerts[0].vNormal.z;

		vNorm = (copyVertices[copyIndices[i+0]].vPos - copyVertices[copyIndices[i+2]].vPos) ^ 
				(copyVertices[copyIndices[i+1]].vPos - copyVertices[copyIndices[i+2]].vPos);
		
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		
		//qDebug() << "new normal: " << vNorm.x << " , " << vNorm.y << " , " << vNorm.z;
		
		// Up-Vector for the new vertices
		vUp = Vector(vNorm.x * dAmount, vNorm.y * dAmount, vNorm.z * dAmount);;
		
		//Position of the three new vertices		
		newVerts[3].vPos = newVerts[0].vPos + vUp;		
		newVerts[4].vPos = newVerts[1].vPos + vUp;		
		newVerts[5].vPos = newVerts[2].vPos + vUp;
		
		newVerts[3].vNormal = newVerts[4].vNormal = newVerts[5].vNormal = vNorm;
		
		vNorm.negate();	
		newVerts[0].vNormal = newVerts[1].vNormal = newVerts[2].vNormal = vNorm;
	
		
		//Add new indices
		int shift = nFace * 6;
		
		// Bottom
		indices.push_back(shift + 1);
		indices.push_back(shift + 0);
		indices.push_back(shift + 2);
		
    	indices.push_back(shift + 0);
		indices.push_back(shift + 1);
		indices.push_back(shift + 3);
		
		indices.push_back(shift + 1);
		indices.push_back(shift + 4);
		indices.push_back(shift + 3);
		
		indices.push_back(shift + 1);
		indices.push_back(shift + 5);
		indices.push_back(shift + 4);
		
		indices.push_back(shift + 1);
		indices.push_back(shift + 2);
		indices.push_back(shift + 5);
		
		indices.push_back(shift + 2);
		indices.push_back(shift + 3);
		indices.push_back(shift + 5);
		
		indices.push_back(shift + 2);
		indices.push_back(shift + 0);
		indices.push_back(shift + 3);
		
		// Top
		indices.push_back(shift + 4);
		indices.push_back(shift + 5);
		indices.push_back(shift + 3);	
		
		//Add new vertices
		qDebug() << "";
		for(int vert=0; vert<6; vert++)
		{
			//qDebug() << "Modified vertex: " << newVerts[vert].vPos.x << "," << newVerts[vert].vPos.y << "," << newVerts[vert].vPos.z;
			verts.push_back(newVerts[vert]);	
		}	
		
		nFace++;	
		
		//qDebug() << "Done with face nr." << nFace << endl;	
	}
		
	p->setIndices(indices);
	p->setVertices(verts);			
}




