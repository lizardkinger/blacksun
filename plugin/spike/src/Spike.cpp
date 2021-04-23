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
 *  File:       Spike.cpp
 *  Created:    13.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/Spike.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../scenegraph/include/Mesh.h"
#include "./../../scenegraph/include/Polygon.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../scenegraph/include/SelectionBuffer.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSScene;
using namespace BSRenderer;

static Spike* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Spike();

	return static_cast<PlgInt*>(_instance);
}

Spike::Spike() : version(Version(1, 0, 0))
{
	m_sName = "Spike";
	m_sDescription = "Creates a new vertex in all selected faces and ";
	m_sDescription += "moves this vertex by a specific way to generate a spike.";
	
	button = NULL;
}

Spike::~Spike()
{
}

bool Spike::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
			
	return true;
}

bool Spike::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/Spike_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Spike");
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

void Spike::execute()
{
	Vector vTransformPos = Vector(0.0, 0.0, 0.0);
	
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
				createSpike(&((*polys)[p]), m_ui.length->value(), 
					m_ui.transformByNormal->isChecked(), mesh->getAabb().getCentre());	
			}
			
			SceneManager::getInstance()->notifyContentChanged(mesh);
		}
   	}
}

void Spike::createSpike(BSScene::Polygon* p, double dLength, bool bTransformByNormal,
		Vector vTransformPos)
{
	if(p==NULL)
		return;
	
	vector<int> indices;
	vector<Vertex> verts;
	
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	
	Vertex newVerts[9];
	Vertex vertMiddle;
	Vector vNorm;		
	
	for(unsigned int i=0; i<copyIndices.size(); i+=3)
	{
		/*                 V2
		 *                /  \                                            
		 *               / *  \                           
		 *              /  *   \                         
		 *             /   *    \                        
		 *            /    *     \                       
		 *           /     *      \      
		 *          / F1   *   F3  \               
		 *         /       X        \             
		 *        /     *     *      \             
		 *       /    *    V3   *     \             
		 *      /   *             *    \            
		 *     /  *        F2       *   \
		 *    ----------------------------
		 *  V0                            V1
		 *
		 * Every Face (F1, F2, F3) needs three new vertices because of their
		 * individual normal vectors */     
		       		
		//Create the new middle vertex
		vertMiddle.vPos = (copyVertices[copyIndices[i+0]].vPos +
							copyVertices[copyIndices[i+1]].vPos + 
							copyVertices[copyIndices[i+2]].vPos)/3.0;
		if(bTransformByNormal)
		{
			vNorm = (copyVertices[copyIndices[i+0]].vPos - copyVertices[copyIndices[i+2]].vPos) ^ 
					(copyVertices[copyIndices[i+1]].vPos - copyVertices[copyIndices[i+2]].vPos);
		}
		else
		{
			vNorm = vertMiddle.vPos - vTransformPos;
		}
		
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		vNorm *= dLength;
		vertMiddle.vPos += vNorm;
							
		for(int t=0; t<MAXTEXTURES; t++)
		{
			vertMiddle.dU[t] = (copyVertices[copyIndices[i+0]].dU[t] +
								copyVertices[copyIndices[i+1]].dU[t] + 
								copyVertices[copyIndices[i+2]].dU[t])/3.0;
	
			vertMiddle.dV[t] = (copyVertices[copyIndices[i+0]].dV[t] +
								copyVertices[copyIndices[i+1]].dV[t] + 
								copyVertices[copyIndices[i+2]].dV[t])/3.0;				
		}
		
		
		//Vertices for face (V0, V1, V3)
		newVerts[0] = copyVertices[copyIndices[i+0]];
		newVerts[1] = copyVertices[copyIndices[i+1]];
		newVerts[2] = vertMiddle;
		
		vNorm = (newVerts[0].vPos - newVerts[2].vPos) ^ (newVerts[1].vPos - newVerts[2].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		
		newVerts[0].vNormal = newVerts[1].vNormal = newVerts[2].vNormal = vNorm;
		
		
		//Vertices for face (V1, V2, V3)
		newVerts[3] = copyVertices[copyIndices[i+1]];
		newVerts[4] = copyVertices[copyIndices[i+2]];
		newVerts[5] = vertMiddle;		

		vNorm = (newVerts[3].vPos - newVerts[5].vPos) ^ (newVerts[4].vPos - newVerts[5].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		
		newVerts[3].vNormal = newVerts[4].vNormal = newVerts[5].vNormal = vNorm;
		
		//Vertices for face (V2, V0, V3)
		newVerts[6] = copyVertices[copyIndices[i+2]];
		newVerts[7] = copyVertices[copyIndices[i+0]];
		newVerts[8] = vertMiddle;		
	
		vNorm = (newVerts[6].vPos - newVerts[8].vPos) ^ (newVerts[7].vPos - newVerts[8].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		
		newVerts[6].vNormal = newVerts[7].vNormal = newVerts[8].vNormal = vNorm;
		
		
		//Add new indices
		int nCurIndex = indices.size();
		for(int ind=0; ind<9; ind++)
		{
			indices.push_back(nCurIndex + ind);	
		}
		
		//Add new vertices
		for(int vert=0; vert<9; vert++)
		{
			verts.push_back(newVerts[vert]);	
		}		
	}

	p->setIndices(indices);
	p->setVertices(verts);	
}
