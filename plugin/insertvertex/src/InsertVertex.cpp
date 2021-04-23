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
 *  File:       InsertVertex.cpp
 *  Created:    09.05.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/InsertVertex.h"
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSScene;
using namespace BSRenderer;

static InsertVertex* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new InsertVertex();

	return static_cast<PlgInt*>(_instance);
}

InsertVertex::InsertVertex() : version(Version(1, 0, 0))
{
	m_sName = "Insert vertex";
	m_sDescription = "Inserts a vertex between two selected vertices and includes it ";
	m_sDescription += "in the mesh (faces will be split)";
	
	button = NULL;
}

InsertVertex::~InsertVertex()
{
}

bool InsertVertex::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
	
	return true;
}

bool InsertVertex::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/InsertVertex_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Insert vertex");
	
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", button);
    
    connect(button, SIGNAL(clicked()), this, SLOT(execute()));
	return true;
}

void InsertVertex::execute()
{
	//Set vor all points, without dublicates
	vector<Vector> tempPoints;
	vector<BSScene::Polygon*> polys;
	
	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SelectedVertex>& selVerts = sb.getSelectedVertices();

  	//Walk through all selected vertices
	for(list<SelectedVertex>::iterator it = selVerts.begin(); it != selVerts.end(); ++it) 
	{
		SelectedVertex *sVert = &(*it);
		
		if(sVert != NULL)
		{
			//Check, if the same position is already in the list
			bool bFoundPos = false;
			for(unsigned int v=0; v<tempPoints.size(); v++)
			{
				if(tempPoints[v] == sVert->getVertex()->vPos)
				{
					bFoundPos = true;
					break;
				}
			}
			
			//Check, if the same polygon is already in the list
			bool bFoundPoly = false;
			for(unsigned int p=0; p<polys.size(); p++)
			{
				if(polys[p] == sVert->getPolygon())
				{
					bFoundPoly = true;
					break;
				}
			}
			
			//Insertion needed?
			if(!bFoundPos)
			{
				tempPoints.push_back(sVert->getVertex()->vPos);
			}
			
			//Insertion needed?
			if(!bFoundPoly)
			{
				polys.push_back(sVert->getPolygon());	
			}
		}
	}
	
	//Are there exactly 2 vertices selected?
	if(tempPoints.size() != 2)
	{
		QMessageBox::critical(NULL, "Error", "Please select exactly two different vertices");
		return;
	}
	
	for(unsigned int p=0; p<polys.size(); p++)
	{
		vertexInsert(polys[p], tempPoints[0], tempPoints[1]);	
	}
	
	
	sb.reset();
    SceneManager::getInstance()->notifyContentChanged(BSScene::SCENETYPE_Vertex,false);	
}

void InsertVertex::vertexInsert(BSScene::Polygon* p, const Vector& v1, const Vector& v2)
{
	if(p==NULL)
		return;
	
	vector<int> indices;
	vector<int> facesToInsert;
	bool bNewVertCalculated = false;
	Vertex newVert;
	
	vector<int>* 	origInd  = p->getIndices();
	vector<Vertex>* origVert = p->getVertices();
	
	for(unsigned int i=0; i<origInd->size(); i+=3)
	{
		//Check if the face must be split
		if( (((*origVert)[(*origInd)[i+0]].vPos == v1) && ((*origVert)[(*origInd)[i+1]].vPos == v2)) ||
			(((*origVert)[(*origInd)[i+0]].vPos == v1) && ((*origVert)[(*origInd)[i+2]].vPos == v2)) ||
			(((*origVert)[(*origInd)[i+1]].vPos == v1) && ((*origVert)[(*origInd)[i+0]].vPos == v2)) ||
			(((*origVert)[(*origInd)[i+1]].vPos == v1) && ((*origVert)[(*origInd)[i+2]].vPos == v2)) ||
			(((*origVert)[(*origInd)[i+2]].vPos == v1) && ((*origVert)[(*origInd)[i+0]].vPos == v2)) ||
			(((*origVert)[(*origInd)[i+2]].vPos == v1) && ((*origVert)[(*origInd)[i+1]].vPos == v2)))
		{
			//Insert the face for splitting
			facesToInsert.push_back((*origInd)[i+0]);
			facesToInsert.push_back((*origInd)[i+1]);
			facesToInsert.push_back((*origInd)[i+2]);
			
			//Create new the new vertex that must be inserted
			if(!bNewVertCalculated)
			{
				bNewVertCalculated = true;
				Vertex vert1, vert2;
				
				if( (((*origVert)[(*origInd)[i+0]].vPos == v1) && ((*origVert)[(*origInd)[i+1]].vPos == v2)) ||
					(((*origVert)[(*origInd)[i+1]].vPos == v1) && ((*origVert)[(*origInd)[i+0]].vPos == v2)))
				{
					vert1 = (*origVert)[(*origInd)[i+0]]; 
					vert2 = (*origVert)[(*origInd)[i+1]];
				}
				else if( (((*origVert)[(*origInd)[i+1]].vPos == v1) && ((*origVert)[(*origInd)[i+2]].vPos == v2)) ||
					(((*origVert)[(*origInd)[i+2]].vPos == v1) && ((*origVert)[(*origInd)[i+1]].vPos == v2)))
				{
					vert1 = (*origVert)[(*origInd)[i+1]]; 
					vert2 = (*origVert)[(*origInd)[i+2]];
				}
				else if( (((*origVert)[(*origInd)[i+0]].vPos == v1) && ((*origVert)[(*origInd)[i+2]].vPos == v2)) ||
					(((*origVert)[(*origInd)[i+2]].vPos == v1) && ((*origVert)[(*origInd)[i+0]].vPos == v2)))
				{
					vert1 = (*origVert)[(*origInd)[i+0]]; 
					vert2 = (*origVert)[(*origInd)[i+2]];
				}
				
				newVert.vPos = (vert1.vPos + vert2.vPos) * 0.5;
				newVert.vNormal = (vert1.vNormal + vert2.vNormal) * 0.5;
				
				for(int t=0; t<MAXTEXTURES; t++)
				{
					newVert.dU[t] = (vert1.dU[t] + vert2.dU[t]) * 0.5;
					newVert.dV[t] = (vert1.dV[t] + vert2.dV[t]) * 0.5;									
				}						
			}	
		}
		else
		{
			//Insert the face without changes
			indices.push_back((*origInd)[i+0]);
			indices.push_back((*origInd)[i+1]);
			indices.push_back((*origInd)[i+2]);			
		}
	}
	
	
	if(facesToInsert.size()!=0)
		origVert->push_back(newVert);
	int nNewVertIndex = origVert->size()-1;
	
	//Add splitted faces	
	for(unsigned int f=0; f<facesToInsert.size(); f+=3)
	{
		//Insert new vertex between v0 and v1?
		if( (((*origVert)[facesToInsert[f+0]].vPos == v1) && ((*origVert)[facesToInsert[f+1]].vPos == v2)) ||
			(((*origVert)[facesToInsert[f+1]].vPos == v1) && ((*origVert)[facesToInsert[f+0]].vPos == v2)))
		{
			indices.push_back(facesToInsert[f+0]);
			indices.push_back(nNewVertIndex);
			indices.push_back(facesToInsert[f+2]);
			
			indices.push_back(facesToInsert[f+1]);
			indices.push_back(facesToInsert[f+2]);
			indices.push_back(nNewVertIndex);			
		}
		//Insert new vertex between v0 and v2?
		else if( (((*origVert)[facesToInsert[f+0]].vPos == v1) && ((*origVert)[facesToInsert[f+2]].vPos == v2)) ||
			(((*origVert)[facesToInsert[f+2]].vPos == v1) && ((*origVert)[facesToInsert[f+0]].vPos == v2)))
		{
			indices.push_back(facesToInsert[f+0]);
			indices.push_back(facesToInsert[f+1]);
			indices.push_back(nNewVertIndex);
			
			indices.push_back(facesToInsert[f+1]);
			indices.push_back(facesToInsert[f+2]);
			indices.push_back(nNewVertIndex);	
		}
		//Otherwise insert new vertex between v1 and v2
		else
		{
			indices.push_back(facesToInsert[f+0]);
			indices.push_back(facesToInsert[f+1]);
			indices.push_back(nNewVertIndex);
			
			indices.push_back(facesToInsert[f+2]);
			indices.push_back(facesToInsert[f+0]);
			indices.push_back(nNewVertIndex);
		}			
	}	
	
	p->setIndices(indices);
}

