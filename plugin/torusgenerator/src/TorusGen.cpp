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
 *  File:       TorusGen.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/TorusGen.h"
#include <SceneGraph.h>
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static TorusGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new TorusGen();

	return static_cast<PlgInt*>(_instance);
}

TorusGen::TorusGen() : version(Version(1, 0, 0))
{
	m_sName = "Torus-Generator";
	m_sDescription = "Generates a torus";
	
	button = NULL;
}

TorusGen::~TorusGen()
{
}

bool TorusGen::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
		
	return true;
}

bool TorusGen::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/GenTorus_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Torus");
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

void TorusGen::execute()
{
	Mesh m;
	m.setName("Torus");
	BSScene::Polygon p;
	vector<Vertex> verts;
	vector<int> indices;
	
	//Create torus
	createTorus(indices, verts, m_ui.radiusOuter->value(), m_ui.radiusInner->value(), 
		m_ui.rings->value(), m_ui.segments->value(), m_ui.sliceStart->value(), 
		m_ui.sliceEnd->value());	
	
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

void TorusGen::createTorus(vector<int>& indices, vector<Vertex>& verts,
	double dRadiusOuter, double dRadiusInner, int nRings, int nSegments,
	double dStart, double dEnd)
{
	if(isNearTo(dRadiusInner, 0.0) || isNearTo(dRadiusOuter, 0.0))
		return;
		
	if(isNearTo(dStart, dEnd))
		return;
		
	if(nRings == 0 || nSegments == 0)
		return;
				
	//Calculate number of vertices and indices
	UINT nVertices = (nRings + 1) * (nSegments + 1);
	UINT nIndices = 6 * nRings * (nSegments + 1);
	
	indices.resize(nIndices);
	verts.resize(nVertices);
	
	//Angle per ring
	//double dDeltaRingAngle = (2.0 * PI / nRings);
	double dDeltaRingAngle = (degToRad(dEnd-dStart) / nRings);
	//Angle per segment
	double dDeltaSegAngle = (2.0 * PI / nSegments);
	
	UINT nCurIndex = 0;
	UINT nCurVertex = 0;
	Vector vInnerCentre;
	double dStartRad = degToRad(dStart);
	//Walk through all rings of the sphere
	for(int r = 0; r <= nRings; r++)
	{
		double dS = sin(r * dDeltaRingAngle + dStartRad);
		double dC = cos(r * dDeltaRingAngle + dStartRad);
		
		//Walk through all ring-segments of the sphere
		for(int s = 0; s <= nSegments; s++)
		{			
			//Height of inner radius for ring-segment s
			double dY = dRadiusInner * sin(s * dDeltaSegAngle);
			
			//X-Coordinate of new vertex in relation to the inner radius
			double dR = dRadiusInner * cos(s * dDeltaSegAngle);
			//Transform the X-Coordinate to X- and Z-Coordinate of complete torus
			double dX = (dRadiusOuter + dR) * dC;
			double dZ = (dRadiusOuter + dR) * dS;
			
			//Set up the new vertex on sphere
			vInnerCentre.set(dC * dRadiusOuter, 0.0, dS * dRadiusOuter);
			verts[nCurVertex].vPos.set(dX, dY, dZ);
			verts[nCurVertex].vNormal = verts[nCurVertex].vPos - vInnerCentre;
			if(verts[nCurVertex].vNormal != Vector(0.0, 0.0, 0.0))
				verts[nCurVertex].vNormal.normalize(); //Normlize to length 1
			if(dEnd < dStart)
				verts[nCurVertex].vNormal *= -1.0;
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
				indices[nCurIndex + 1] = nCurVertex + (nSegments + 1);
				indices[nCurIndex + 2] = nCurVertex + (nSegments);
				
				indices[nCurIndex + 3] = nCurVertex + (nSegments + 1);
				indices[nCurIndex + 4] = nCurVertex;
				indices[nCurIndex + 5] = nCurVertex + 1;
				
				nCurIndex += 6;
			} 
			
			nCurVertex++;
		}
		
	}
	
}
