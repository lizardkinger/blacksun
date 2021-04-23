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
 *  File:       MakeHole.cpp
 *  Created:    11.04.07
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/MakeHole.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../scenegraph/include/Mesh.h"
#include "./../../scenegraph/include/Polygon.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../scenegraph/include/SelectionBuffer.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSScene;
using namespace BSRenderer;

static MakeHole* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new MakeHole();

	return static_cast<PlgInt*>(_instance);
}

MakeHole::MakeHole() : version(Version(1, 0, 0))
{
	m_sName = "MakeHole";
	m_sDescription = "Creates a hole in all selected";
	
	button = NULL;
}

MakeHole::~MakeHole()
{
}

bool MakeHole::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
			
	return true;
}

bool MakeHole::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/MakeHole_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Make Hole");
	button->setCheckable(true);
	button->setAutoExclusive(true);

	QWidget* pluginSettings = new QWidget();
	pluginSettings->hide();
	m_ui.setupUi(pluginSettings);
	
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", button);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("modificatorGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);

	connect(button, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
	connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
  
	return true;
}

void MakeHole::execute()
{	
 	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(unsigned int p=0; p<polys->size(); p++)
			{			
				createHole(&((*polys)[p]), m_ui.percentage->value()/100.0);	
			}
		}
   	}
   	
	SceneManager::getInstance()->notifyContentChanged(BSScene::SCENETYPE_Mesh,true);
}

void MakeHole::createHole(BSScene::Polygon* p, double dPercentage)
{
	if(p==NULL)
		return;
	
	vector<int> indices;
	vector<Vertex> verts;
	
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	
	Vertex newVerts[6];
	Vector vNorm;
	Vector vMiddle;	
	int nFace = 0;
	
	for(unsigned int i=0; i<copyIndices.size(); i+=3)
	{	
		/*                 V2
		 *                 / \
		 *                / | \                                            
		 *               /  |  \                           
		 *              /   |   \                         
		 *             /    |    \                        
		 *            /    / \    \                       
		 *           /    /V5 \    \      
		 *          / F1 /     \ F2 \               
		 *         /    /       \    \             
		 *        /    /         \    \             
		 *       /    /V3       V4\    \             
		 *      /    ---------------    \            
		 *     /  /        F3         \  \
		 *    -----------------------------
		 *  V0                            V1
		 *
		 */

		vNorm = (copyVertices[copyIndices[i+0]].vPos - copyVertices[copyIndices[i+2]].vPos) ^ 
				(copyVertices[copyIndices[i+1]].vPos - copyVertices[copyIndices[i+2]].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		
		vMiddle = (copyVertices[copyIndices[i+0]].vPos +
					copyVertices[copyIndices[i+1]].vPos + 
					copyVertices[copyIndices[i+2]].vPos) / 3.0;
		
		//First three vertices are the same
		newVerts[0] = copyVertices[copyIndices[i+0]];
		newVerts[1] = copyVertices[copyIndices[i+1]];
		newVerts[2] = copyVertices[copyIndices[i+2]];
		
		//Position of the three new vertices		
		newVerts[3].vPos = newVerts[0].vPos + (vMiddle - newVerts[0].vPos) * dPercentage;		
		newVerts[4].vPos = newVerts[1].vPos + (vMiddle - newVerts[1].vPos) * dPercentage;		
		newVerts[5].vPos = newVerts[2].vPos + (vMiddle - newVerts[2].vPos) * dPercentage;
			 
		//Normal of the three new vertices
		newVerts[3].vNormal = newVerts[4].vNormal = newVerts[5].vNormal = vNorm;
		
		//Texture coordinates for the three new vertices
		for(int v=0; v<3; v++)
		{
			for(int t=0; t<MAXTEXTURES; t++)
			{
				double dUMiddle = (newVerts[0].dU[t] + newVerts[1].dU[t] + newVerts[2].dU[t]) / 3.0;
				double dVMiddle = (newVerts[0].dV[t] + newVerts[1].dV[t] + newVerts[2].dV[t]) / 3.0;
				
				newVerts[3+v].dU[t] = newVerts[0+v].dU[t] + (dUMiddle - newVerts[0+v].dU[t]) * dPercentage;
				newVerts[3+v].dV[t] = newVerts[0+v].dV[t] + (dVMiddle - newVerts[0+v].dV[t]) * dPercentage;				
		
			}
		}
		
		//Add new indices
		indices.push_back(nFace * 6 + 0);
		indices.push_back(nFace * 6 + 1);
		indices.push_back(nFace * 6 + 3);
		
		indices.push_back(nFace * 6 + 1);
		indices.push_back(nFace * 6 + 4);
		indices.push_back(nFace * 6 + 3);
		
		indices.push_back(nFace * 6 + 1);
		indices.push_back(nFace * 6 + 5);
		indices.push_back(nFace * 6 + 4);
		
		indices.push_back(nFace * 6 + 1);
		indices.push_back(nFace * 6 + 2);
		indices.push_back(nFace * 6 + 5);
		
		indices.push_back(nFace * 6 + 2);
		indices.push_back(nFace * 6 + 3);
		indices.push_back(nFace * 6 + 5);
		
		indices.push_back(nFace * 6 + 2);
		indices.push_back(nFace * 6 + 0);
		indices.push_back(nFace * 6 + 3);
		
		//Add new vertices
		for(int vert=0; vert<6; vert++)
		{
			verts.push_back(newVerts[vert]);	
		}	
		
		nFace++;	
	}
	
	p->setIndices(indices);
	p->setVertices(verts);	
}

