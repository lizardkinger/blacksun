/***************************************************************************
 *   Copyright (C) 2006-07 by Reinhard Jeschull/Gruber Philipp
 *   rjeschu@fh-landshut.de/pgruber@fh-landshut.de
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
 *  File:       BlockGen.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull/Philipp Gruber (rjeschu/psirus)
 *
 **************************************************************************/

#include "./../include/BlockGen.h"


static BlockGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new BlockGen();

	return static_cast<PlgInt*>(_instance);
}

BlockGen::BlockGen() : version(Version(1, 0, 0))
{
	m_sName = "Block-Generator";
	m_sDescription = "Generates a block";
	
	createBlockButton = NULL;
}

BlockGen::~BlockGen()
{
}

bool BlockGen::unloadPlg()
{
	if(createBlockButton != NULL)
	{
		delete createBlockButton;
		createBlockButton = NULL;
	}
	
	return true;
}

bool BlockGen::loadPlg()
{
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
	LOG_Ok("Block-Test");
        
	int nButtonSize = 32;
	int nIconSize = 24;
	
	createBlockButton = new QPushButton(QIcon(":/media/GenBlock_32.png"), "");
	createBlockButton->setMaximumSize(nButtonSize, nButtonSize);
	createBlockButton->setMinimumSize(nButtonSize, nButtonSize);
	createBlockButton->setIconSize(QSize(nIconSize, nIconSize));
	createBlockButton->setToolTip("Block");
	createBlockButton->setCheckable(true);
	createBlockButton->setAutoExclusive(true);    
    
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", createBlockButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(createBlockButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
    
	return true;
}

void BlockGen::execute()
{
	int x = ui.x->value();
	int y = ui.y->value();
	int z = ui.z->value();
	
	double width = ui.width->value();;
	double height = ui.height->value();;
	double depth = ui.depth->value();;
	
	cout << "start" <<endl;
	Mesh m;
	m.setName(tr("Block"));
	BSScene::Polygon p;
	vector<Vertex> verts;
	vector<int> indices;
	UINT nCurIndex = 0;
	UINT nCurVertex = 0;
	
	//Create block
	createSide(indices, verts, width, depth,height, x, z, Top, nCurIndex, nCurVertex);	
	createSide(indices, verts, width, depth,height, x, z, Bottom, nCurIndex, nCurVertex);	
	createSide(indices, verts, width, height,depth, x, y, Front, nCurIndex, nCurVertex);	
	createSide(indices, verts, width, height,depth, x, y, Back, nCurIndex, nCurVertex);	
	createSide(indices, verts, depth, height,width, z, y, Left, nCurIndex, nCurVertex);	
	createSide(indices, verts, depth, height,width, z, y, Right, nCurIndex, nCurVertex);	
	
	//No indices -> no mesh
	if(indices.size() == 0)
		return;	
	
	//Add vertices and indices to the mesh
	p.setVertices(verts);
	p.setIndices(indices);
	m.addPolygon(p);
	
	//Add new mesh to the scene
	SceneManager::getInstance()->insertObject(m);
	LOG_Ok("Block added");
}

void BlockGen::createSide(vector<int>& indices, vector<Vertex>& verts,
		double dWidth, double dHeight, double dDepth, int nSegmentsWidth, int nSegmentsHeight,
		Sides side, UINT& nCurIndex, UINT& nCurVertex)
{
	cout<<side<<endl;
	
	if(isNearTo(dWidth, 0.0))
		return;
		
	if(isNearTo(dHeight, 0.0))
		return;
		
	if((nSegmentsWidth == 0) || (nSegmentsHeight == 0))
		return;
	
	for(int h = 0; h <= nSegmentsHeight; h++)
	{
		for(int w = 0; w <= nSegmentsWidth; w++)
		{
			switch(side)
			{
				case Top:	verts.push_back(Vertex(Vector(w * dWidth / nSegmentsWidth, dDepth/2, h * dHeight / nSegmentsHeight), Vector(0.0, 1.0, 0.0), 0.0,0.0));
							verts[nCurVertex].vPos.x -= 0.5 * dWidth;
							verts[nCurVertex].vPos.z -= 0.5 * dHeight;
							break;
				case Bottom:verts.push_back(Vertex(Vector(w * dWidth / nSegmentsWidth, -dDepth/2, h * dHeight / nSegmentsHeight), Vector(0.0, -1.0, 0.0), 0.0,0.0));
							verts[nCurVertex].vPos.x -= 0.5 * dWidth;
							verts[nCurVertex].vPos.z -= 0.5 * dHeight;
							break;
				case Front:	verts.push_back(Vertex(Vector(w * dWidth / nSegmentsWidth, h * dHeight / nSegmentsHeight, dDepth/2), Vector(0.0, 0.0, 1.0), 0.0,0.0));
							verts[nCurVertex].vPos.x -= 0.5 * dWidth;
							verts[nCurVertex].vPos.y -= 0.5 * dHeight;
							break;
				case Back:	verts.push_back(Vertex(Vector(w * dWidth / nSegmentsWidth, h * dHeight / nSegmentsHeight, -dDepth/2), Vector(0.0, 0.0, -1.0), 0.0,0.0));
							verts[nCurVertex].vPos.x -= 0.5 * dWidth;
							verts[nCurVertex].vPos.y -= 0.5 * dHeight;
							break;
				case Left:	verts.push_back(Vertex(Vector(-dDepth/2, h * dHeight / nSegmentsHeight, w * dWidth / nSegmentsWidth), Vector(-1.0, 0.0, 0.0), 0.0,0.0));
							verts[nCurVertex].vPos.z -= 0.5 * dWidth;
							verts[nCurVertex].vPos.y -= 0.5 * dHeight;
							break;
				case Right:	verts.push_back(Vertex(Vector(dDepth/2, h * dHeight / nSegmentsHeight, w * dWidth / nSegmentsWidth), Vector(1.0, 0.0, 0.0), 0.0,0.0));
							verts[nCurVertex].vPos.z -= 0.5 * dWidth;
							verts[nCurVertex].vPos.y -= 0.5 * dHeight;
							break;
				default:	LOG_Error("Error while creating Block");
			}
			
			verts[nCurVertex].setTexCoord(-1, static_cast<double>(w) / nSegmentsWidth,
				static_cast<double>(h) / nSegmentsHeight);
			
			//Add indices (if its not the last row)
			if(h != nSegmentsHeight && w != nSegmentsWidth) 
			{
				if(side == Front || side == Left || side == Bottom)
				{
					indices.push_back(nCurVertex);
					indices.push_back(nCurVertex + (nSegmentsWidth + 2));
					indices.push_back(nCurVertex + (nSegmentsWidth + 1));
					
					indices.push_back(nCurVertex + (nSegmentsWidth + 2));
					indices.push_back(nCurVertex);
					indices.push_back(nCurVertex + 1);
				}
				else
				{
					indices.push_back(nCurVertex);
					indices.push_back(nCurVertex + (nSegmentsWidth + 1));
					indices.push_back(nCurVertex + (nSegmentsWidth + 2));

					indices.push_back(nCurVertex + (nSegmentsWidth + 2));
					indices.push_back(nCurVertex + 1);
					indices.push_back(nCurVertex);
				}
				
				nCurIndex += 6;
			} 

			nCurVertex++;						
		}	
	} 
}
