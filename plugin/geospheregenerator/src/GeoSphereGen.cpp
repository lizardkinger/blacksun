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
 *  File:       GeoSphereGen.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/GeoSphereGen.h"
//#include <SceneGraph.h>
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static GeoSphereGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new GeoSphereGen();

	return static_cast<PlgInt*>(_instance);
}

GeoSphereGen::GeoSphereGen() : version(Version(1, 0, 0))
{
	m_sName = "GeoSphere-Generator";
	m_sDescription = "Generates a geo sphere";
	
	button = NULL;
}

GeoSphereGen::~GeoSphereGen()
{
}

bool GeoSphereGen::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
			
	return true;
}

bool GeoSphereGen::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/GenGeoSphere_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("GeoSphere");
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

void GeoSphereGen::execute()
{
	Mesh m;
	m.setName(tr("GeoSphere"));
	BSScene::Polygon p;
	vector<Vertex> verts;
	vector<int> indices;
	
	//Create geosphere
	createGeoSphere(indices, verts, m_ui.radius->value(), m_ui.depth->value());	

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

void GeoSphereGen::createGeoSphere(vector<int>& indices, vector<Vertex>& verts, 
		double dRadius, int nDepth)
{
	if(isNearTo(dRadius, 0.0))
		return;
		
	if(nDepth < 0)
		return;
		
	//Clear the arrays	
	indices.clear();
	verts.clear();

	//Save the positions of the base-icosahedron
	vector<Vector> vertsTemp;

	double dX = 0.525731112119133606;
	double dZ = 0.850650808352039932;
	
	vertsTemp.push_back(Vector(-dX, 0.0,  dZ));
	vertsTemp.push_back(Vector( dX, 0.0,  dZ));
	vertsTemp.push_back(Vector(-dX, 0.0, -dZ));
	vertsTemp.push_back(Vector( dX, 0.0, -dZ));

	vertsTemp.push_back(Vector(0.0,  dZ,  dX));
	vertsTemp.push_back(Vector(0.0,  dZ, -dX));
	vertsTemp.push_back(Vector(0.0, -dZ,  dX));
	vertsTemp.push_back(Vector(0.0, -dZ, -dX));

	vertsTemp.push_back(Vector( dZ,  dX, 0.0));
	vertsTemp.push_back(Vector(-dZ,  dX, 0.0));
	vertsTemp.push_back(Vector( dZ, -dX, 0.0));
	vertsTemp.push_back(Vector(-dZ, -dX, 0.0));
	
	//Save the indices for the the base-icosahedron
	static GLuint tindices[20][3] = {
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
	{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
	
	for(int i=0; i<20; i++)
	{
		subdivideGeoSphere(vertsTemp[tindices[i][0]],
				  vertsTemp[tindices[i][1]],
				  vertsTemp[tindices[i][2]],
				  nDepth, indices, verts, dRadius);	
	}
	
}

void GeoSphereGen::subdivideGeoSphere(Vector& v1, Vector& v2, Vector& v3, int nDepth,
	vector<int>& indices, vector<Vertex>& verts, double dRadius)
{
	if(nDepth==0)
	{
		indices.push_back(indices.size());
		indices.push_back(indices.size());
		indices.push_back(indices.size());
	
		verts.push_back(Vertex(v1 * dRadius, v1, calcSphereMapU(v1, dRadius), calcSphereMapV(v1, dRadius)));
		verts.push_back(Vertex(v3 * dRadius, v3, calcSphereMapU(v3, dRadius), calcSphereMapV(v3, dRadius)));
		verts.push_back(Vertex(v2 * dRadius, v2, calcSphereMapU(v2, dRadius), calcSphereMapV(v2, dRadius)));
	}
	else
	{
		Vector v12 = v1 + v2;
		Vector v23 = v2 + v3;
		Vector v31 = v3 + v1;
		
		v12.normalize();
		v23.normalize();
		v31.normalize();
		
		subdivideGeoSphere( v1, v12, v31, nDepth-1, indices, verts, dRadius);
		subdivideGeoSphere( v2, v23, v12, nDepth-1, indices, verts, dRadius);
		subdivideGeoSphere( v3, v31, v23, nDepth-1, indices, verts, dRadius);
		subdivideGeoSphere(v12, v23, v31, nDepth-1, indices, verts, dRadius);
	}
		
}

double GeoSphereGen::calcSphereMapU(Vector& v, double dRadius)
{
	if(v.y >= 0)
	{
		return acos(v.x / (dRadius * sin(PI * calcSphereMapV(v, dRadius)))) / (2 * PI);	
	}
	return (PI + acos(v.x / (dRadius * sin(PI * calcSphereMapV(v, dRadius))))) / (2 * PI);

	//return acos(v.x / (dRadius * sin(PI * calcSphereMapV(v, dRadius)))) / (2 * PI);
}

double GeoSphereGen::calcSphereMapV(Vector& v, double dRadius)
{
	return acos(v.z / dRadius) / PI;
}
