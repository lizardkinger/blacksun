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
 *  File:       SphereGen.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/SphereGen.h"
#include <SceneGraph.h>
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static SphereGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new SphereGen();

	return static_cast<PlgInt*>(_instance);
}

SphereGen::SphereGen() : version(Version(1, 0, 0))
{
	m_sName = "Sphere-Generator";
	m_sDescription = "Generates a sphere";
	
	button = NULL;
}

SphereGen::~SphereGen()
{
}

bool SphereGen::unloadPlg()
{	
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
	
	return true;
}

bool SphereGen::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/GenSphere_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Sphere");
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

void SphereGen::execute()
{
	Mesh m;
	m.setName("Sphere");
	BSScene::Polygon p;
	vector<Vertex> verts;
	vector<int> indices;
	
	//Create sphere
	createSphere(indices, verts, m_ui.radius->value(), m_ui.rings->value(), 
		m_ui.segments->value());	

	//No indices -> no mesh
	if(indices.size() == 0)
		return;
	
	//Add vertices and indices to the mesh
	p.setVertices(verts);
	p.setIndices(indices);
	m.addPolygon(p);
	
	//Add new mesh to the scene
	SceneManager::getInstance()->insertObject(m);
}

void SphereGen::createSphere(vector<int>& indices, vector<Vertex>& verts,
	double dRadius, int nRings, int nSegments)
{
	if(isNearTo(dRadius, 0.0))
		return;
		
	if(nRings == 0 || nSegments == 0)
		return;
			
	//Calculate number of vertices and indices
	UINT nVertices = (nRings + 1) * (nSegments + 1);
	UINT nIndices = 6 * nRings * (nSegments + 1);
	
	indices.resize(nIndices);
	verts.resize(nVertices);
	
	//Angle per ring
	double dDeltaRingAngle = (PI / nRings);
	//Angle per segment
	double dDeltaSegAngle = (2.0 * PI / nSegments);
	
	UINT nCurIndex = 0;
	UINT nCurVertex = 0;
	
	//Walk through all rings of the sphere
	for(int r = 0; r <= nRings; r++)
	{
		double dR = sin(r * dDeltaRingAngle);
		double dY = cos(r * dDeltaRingAngle);
		
		//Walk through all ring-segments of the sphere
		for(int s = 0; s <= nSegments; s++)
		{
			double dX = dR * sin(s * dDeltaSegAngle);
			double dZ = dR * cos(s * dDeltaSegAngle);
			
			//Set up the new vertex on sphere
			verts[nCurVertex].vPos.set(dX * dRadius, dY * dRadius, dZ * dRadius);
			verts[nCurVertex].vNormal.set(dX, dY, dZ);
			verts[nCurVertex].setTexCoord(-1, static_cast<double>(s) / nSegments,
				1.0-static_cast<double>(r) / nRings);
			
			//Add indices (if its not the last ring)
			if(r != nRings) 
			{
				//The vertex-ID above the current vertex is v+nSegment, because every
				//ring has nSegments indices
				// 
				//nCurVertex+nSegments       nCurVertex+nSegments+1
				//                    X-----X
				//                    | F1 /|
				//                    |   / |
				//                    |  /  |
				//                    | / F2|
				//                    |/    |
				//                    X-----X
				//          nCurVertex       nCurVertex+1				 
 
				indices[nCurIndex + 0] = nCurVertex;
				indices[nCurIndex + 1] = nCurVertex + (nSegments);
				indices[nCurIndex + 2] = nCurVertex + (nSegments + 1);
				
				indices[nCurIndex + 3] = nCurVertex + (nSegments + 1);
				indices[nCurIndex + 4] = nCurVertex + 1;
				indices[nCurIndex + 5] = nCurVertex;
				
				nCurIndex += 6;
			} 
			
			nCurVertex++;
		}	
		
	}	
}
