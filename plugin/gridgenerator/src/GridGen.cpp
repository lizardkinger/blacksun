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
 *  File:       GridGen.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/GridGen.h"
#include <SceneGraph.h>
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static GridGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new GridGen();

	return static_cast<PlgInt*>(_instance);
}

GridGen::GridGen() : version(Version(1, 0, 0))
{
	m_sName = "Grid-Generator";
	m_sDescription = "Generates a grid";
	
	button = NULL;
}

GridGen::~GridGen()
{
}

bool GridGen::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
		
	return true;
}

bool GridGen::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/GenGrid_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Grid");
	button->setCheckable(true);
	button->setAutoExclusive(true);

	QWidget* pluginSettings = new QWidget();
	pluginSettings->hide();
	m_ui.setupUi(pluginSettings);
	
    BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", button);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);

	connect(button, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
	connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
  
	return true;
}

void GridGen::execute()
{
	Mesh m;
	m.setName(tr("Grid"));
	BSScene::Polygon p;
	vector<Vertex> verts;
	vector<int> indices;
	
	//Create grid
	createGrid(indices, verts, m_ui.width->value(), m_ui.height->value(), 
		m_ui.segmentsWidth->value(), m_ui.segmentsHeight->value());	
	
	//No indices -> no mesh
	if(indices.size() == 0)
		return;	
	
	vector<Vertex> v;
	vector<int> i;	
	i.push_back(0);
	i.push_back(1);
	i.push_back(2);
	
	//Add vertices and indices to the mesh
	for(unsigned int f=0; f<(indices.size()/3); f++)
	{
		v.clear();
		v.push_back(verts[indices[f*3+0]]);
		v.push_back(verts[indices[f*3+1]]);
		v.push_back(verts[indices[f*3+2]]);
		
		p.setVertices(v);
		p.setIndices(i);
		m.addPolygon(p);
	}
	
	//Add new mesh to the scene
	SceneManager::getInstance()->insertObject(m);
}

void GridGen::createGrid(vector<int>& indices, vector<Vertex>& verts,
		double dWidth, double dHeight, int nSegmentsWidth, int nSegmentsHeight)
{
	if(isNearTo(dWidth, 0.0))
		return;
		
	if(isNearTo(dHeight, 0.0))
		return;
		
	if((nSegmentsWidth == 0) || (nSegmentsHeight == 0))
		return;
		
	//Calculate number of vertices and indices
	UINT nVertices = (nSegmentsWidth + 1) * (nSegmentsHeight + 1);
	UINT nIndices = 6 * nSegmentsHeight * nSegmentsWidth;
	
	indices.resize(nIndices);
	verts.resize(nVertices);
	
	UINT nCurIndex = 0;
	UINT nCurVertex = 0;
	
	for(int h = 0; h <= nSegmentsHeight; h++)
	{
		for(int w = 0; w <= nSegmentsWidth; w++)
		{
			verts[nCurVertex].vPos.set(w * dWidth / nSegmentsWidth, 0.0, -h * dHeight / nSegmentsHeight);
			verts[nCurVertex].vPos.x -= 0.5 * dWidth;
			verts[nCurVertex].vPos.z += 0.5 * dHeight;
			verts[nCurVertex].vNormal = Vector(0.0, 1.0, 0.0);
			verts[nCurVertex].setTexCoord(-1, static_cast<double>(w) / nSegmentsWidth,
				static_cast<double>(h) / nSegmentsHeight);
			
			//Add indices (if its not the last row)
			if(h != nSegmentsHeight && w != nSegmentsWidth) 
			{
				indices[nCurIndex + 0] = nCurVertex;
				indices[nCurIndex + 1] = nCurVertex + (nSegmentsWidth + 2);
				indices[nCurIndex + 2] = nCurVertex + (nSegmentsWidth + 1);
				
				indices[nCurIndex + 3] = nCurVertex + (nSegmentsWidth + 2);
				indices[nCurIndex + 4] = nCurVertex;
				indices[nCurIndex + 5] = nCurVertex + 1;
				
				nCurIndex += 6;
			} 
			
			nCurVertex++;						
		}	
	} 
}
