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
 *  File:       MatEditPreview.cpp
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/MatEditPreview.h"
#include "./../../editor/gui/include/bsglwidget.h"
#include <QtGui>
#include <iostream>

using namespace std;
using namespace BSGui;

MatEditPreview::MatEditPreview(QWidget *parent, FillMode fm, int* pnMat)
    : BSGLWidget(parent), m_pnMaterial(pnMat)
{	
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer->start(20);
	
	m_pCamPerspective = new CameraArcball(width(), height());
	m_pCamOrthogonal = new CameraOrthogonal(width(), height(), ORTHO_Front);
	
	m_vm = VIEWMODE_Perspective;
		
	m_fm = fm;
	m_nPrimitiveTyp = 1;
	m_bShowGrid = false;	
	
	//Create context menu
	m_contextMenu = new QMenu(this);
		
	m_actionRenderBox = new QAction("Render box", this);
	m_actionRenderBox->setCheckable(true);
	m_actionRenderSphere = new QAction("Render sphere", this);
	m_actionRenderSphere->setCheckable(true);
	m_actionRenderSphere->setChecked(true);
	m_actionRenderTorus = new QAction("Render torus", this);
	m_actionRenderTorus->setCheckable(true);
	m_actionRenderHelix = new QAction("Render helix", this);
	m_actionRenderHelix->setCheckable(true);
	m_actionRenderGrid = new QAction("Render grid", this);
	m_actionRenderGrid->setCheckable(true);
	m_actionRenderGeosphere = new QAction("Render geosphere", this);
	m_actionRenderGeosphere->setCheckable(true);
	m_actionSeperator = new QAction("Seperator", this);
	m_actionSeperator->setSeparator(true);
	m_actionResetView = new QAction("Reset view", this);			
	m_actionShowGrid = new QAction("Show grid", this);
	m_actionShowGrid->setCheckable(true);
	m_actionShowGrid->setChecked(m_bShowGrid);
        
	m_actionGroupPrimitive = new QActionGroup(this);
	m_actionGroupPrimitive->addAction(m_actionRenderBox);
	m_actionGroupPrimitive->addAction(m_actionRenderSphere);
	m_actionGroupPrimitive->addAction(m_actionRenderTorus);
	m_actionGroupPrimitive->addAction(m_actionRenderHelix);
	m_actionGroupPrimitive->addAction(m_actionRenderGrid);
	m_actionGroupPrimitive->addAction(m_actionRenderGeosphere);
	m_actionGroupPrimitive->setExclusive(true);   
	
	m_contextMenu->addAction(m_actionResetView);
	m_contextMenu->addAction(m_actionShowGrid);
	m_contextMenu->addAction(m_actionSeperator);     
	m_contextMenu->addAction(m_actionRenderBox);
	m_contextMenu->addAction(m_actionRenderSphere);
	m_contextMenu->addAction(m_actionRenderTorus);
	m_contextMenu->addAction(m_actionRenderHelix);
	m_contextMenu->addAction(m_actionRenderGrid);
	m_contextMenu->addAction(m_actionRenderGeosphere);
        
	//Connect context menu items
	connect(m_actionRenderBox, 			SIGNAL(toggled(bool)), this, SLOT(setRenderBox(bool)));
	connect(m_actionRenderSphere, 		SIGNAL(toggled(bool)), this, SLOT(setRenderSphere(bool)));
	connect(m_actionRenderTorus, 		SIGNAL(toggled(bool)), this, SLOT(setRenderTorus(bool)));
	connect(m_actionRenderHelix, 		SIGNAL(toggled(bool)), this, SLOT(setRenderHelix(bool)));
	connect(m_actionRenderGrid, 		SIGNAL(toggled(bool)), this, SLOT(setRenderGrid(bool)));
	connect(m_actionRenderGeosphere, 	SIGNAL(toggled(bool)), this, SLOT(setRenderGeosphere(bool)));
	connect(m_actionResetView, 			SIGNAL(triggered()),   this, SLOT(resetCameras()));        	
	connect(m_actionShowGrid, 			SIGNAL(toggled(bool)), this, SLOT(setShowGrid(bool)));
}

void MatEditPreview::initializeGL()
{
	m_time.start();
	r = Renderer::getInstance();
	r->init();

	createBox(m_indicesBox, m_vertsBox, 2.0, 2.0, 2.0);
	createSphere(m_indicesSphere, m_vertsSphere, 2.0, 48, 48);
	createTorus(m_indicesTorus, m_vertsTorus, 2.0, 1.00, 32, 32, 0.0, 360.0);
	createHelix(m_indicesHelix, m_vertsHelix, 2.0, 0.25, 0.25, 0.25, 4.0, 144, 32, 0.0, 360.0*4);
	createGrid(m_indicesGrid, m_vertsGrid, 4.0, 4.0, 4, 4);
	createGeoSphere(m_indicesGeoSphere, m_vertsGeoSphere, 2.0, 4);
} 

void MatEditPreview::paintGL()
{
	//Set up the clipping planes and fov for the cameras
	m_pCamPerspective->setClippingPlanes(r->getNearPlane(), r->getFarPlane());
	m_pCamPerspective->setFOV(r->getFOV());
	m_pCamPerspective->setWheelSpeed(r->getSettings()->getMouseWheelSpeed());
	m_pCamOrthogonal->setClippingPlanes(r->getNearPlane(), r->getFarPlane());
	m_pCamOrthogonal->setFOV(r->getFOV());
	m_pCamOrthogonal->setWheelSpeed(r->getSettings()->getMouseWheelSpeed());
	
	//Set no realtime mirror
	RealtimeMirror rm = r->getSettings()->getRealtimeMirror();
	r->getSettings()->setRealtimeMirror(MIRROR_None);
	
	r->setViewMode(m_vm);
	r->setFillMode(m_fm);
	r->setCameraPerspective(m_pCamPerspective);
	r->setCameraOrthogonal(m_pCamOrthogonal);
	r->clear();
	
	if(m_bShowGrid)
	{
		r->renderGrid();
	}

	m_mTransform.identity();

	switch(m_nPrimitiveTyp)
	{
		case 0:
		{
			r->render(PRIM_Triangle, m_vertsBox, m_indicesBox, *m_pnMaterial, m_mTransform, m_aabb);
		}break;
		case 1:
		{
			r->render(PRIM_Triangle, m_vertsSphere, m_indicesSphere, *m_pnMaterial, m_mTransform, m_aabb);
		}break;	
		case 2:
		{
			r->render(PRIM_Triangle, m_vertsTorus, m_indicesTorus, *m_pnMaterial, m_mTransform, m_aabb);
		}break;
		case 3:
		{
			r->render(PRIM_Triangle, m_vertsHelix, m_indicesHelix, *m_pnMaterial, m_mTransform, m_aabb);
		}break;
		case 4:
		{
			r->render(PRIM_Triangle, m_vertsGrid, m_indicesGrid, *m_pnMaterial, m_mTransform, m_aabb);
		}break;	
		default:
		{
			r->render(PRIM_Triangle, m_vertsGeoSphere, m_indicesGeoSphere, *m_pnMaterial, m_mTransform, m_aabb);
		}break;			
	}
	
	r->flush();
	
	//Restore realtime mirror
	r->getSettings()->setRealtimeMirror(rm);
		
	return;
}

void MatEditPreview::contextMenuEvent(QContextMenuEvent *event)
{
	event->accept();
	m_contextMenu->exec(event->globalPos());
}

void MatEditPreview::resizeGL(int width, int height)
{
	r->setViewport(width, height);
	m_pCamPerspective->setViewport(width, height);
	m_pCamOrthogonal->setViewport(width, height);
} 

void MatEditPreview::update()
{    
	updateGL();	
}

void MatEditPreview::mousePressEvent(QMouseEvent *event)
{
	if(m_vm == VIEWMODE_Perspective)
		m_pCamPerspective->mousePress(event->x(), event->y());
	else
		m_pCamOrthogonal->mousePress(event->x(), event->y());
    updateGL();
} 

void MatEditPreview::mouseMoveEvent(QMouseEvent *event)
{
	if(m_vm == VIEWMODE_Perspective)
		m_pCamPerspective->mouseMove(event->x(), event->y());
	else
		m_pCamOrthogonal->mouseMove(event->x(), event->y());
    updateGL();
} 

void MatEditPreview::wheelEvent(QWheelEvent *event)
{
	if(m_vm == VIEWMODE_Perspective)
		m_pCamPerspective->mouseWheel(event->delta());
	else
		m_pCamOrthogonal->mouseWheel(event->delta());
    updateGL();
}

void MatEditPreview::createBox(vector<int>& indices, vector<Vertex>& verts,
	double dWidth, double dHeight, double dDepth)
{
	indices.clear();
	verts.clear();	
	
	int Index[36] = {1,2,0,   3,1,0,   5,6,4,   7,5,4,   9,10,8,   11,9,8,
       13,14,12,   15,13,12,   17,18,16,   19,17,16,   21,22,20,   23,21,20};       
	for(int i=0; i<36; i++)	indices.push_back(Index[i]);
	
	//Left side
	verts.push_back(Vertex(Vector(-1.0,  1.0, -1.0), Vector(-1.0,  0.0,  0.0), 1.0, 0.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0,  1.0), Vector(-1.0,  0.0,  0.0), 0.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0,  1.0,  1.0), Vector(-1.0,  0.0,  0.0), 1.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0, -1.0), Vector(-1.0,  0.0,  0.0), 0.0, 0.0));
	
	//Back side
	verts.push_back(Vertex(Vector( 1.0,  1.0, -1.0), Vector( 0.0,  0.0, -1.0), 1.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0, -1.0), Vector( 0.0,  0.0, -1.0), 0.0, 0.0));
	verts.push_back(Vertex(Vector(-1.0,  1.0, -1.0), Vector( 0.0,  0.0, -1.0), 0.0, 1.0));
	verts.push_back(Vertex(Vector( 1.0, -1.0, -1.0), Vector( 0.0,  0.0, -1.0), 1.0, 0.0));
	
	//Up side
	verts.push_back(Vertex(Vector( 1.0,  1.0,  1.0), Vector( 0.0,  1.0,  0.0), 1.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0,  1.0, -1.0), Vector( 0.0,  1.0,  0.0), 0.0, 0.0));
	verts.push_back(Vertex(Vector(-1.0,  1.0,  1.0), Vector( 0.0,  1.0,  0.0), 0.0, 1.0));
	verts.push_back(Vertex(Vector( 1.0,  1.0, -1.0), Vector( 0.0,  1.0,  0.0), 1.0, 0.0));
	
	//Bottom side
	verts.push_back(Vertex(Vector( 1.0, -1.0, -1.0), Vector( 0.0, -1.0,  0.0), 1.0, 0.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0,  1.0), Vector( 0.0, -1.0,  0.0), 0.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0, -1.0), Vector( 0.0, -1.0,  0.0), 0.0, 0.0));
	verts.push_back(Vertex(Vector( 1.0, -1.0,  1.0), Vector( 0.0, -1.0,  0.0), 1.0, 1.0));
	
	//Front side
	verts.push_back(Vertex(Vector(-1.0,  1.0,  1.0), Vector( 0.0,  0.0,  1.0), 0.0, 1.0));
	verts.push_back(Vertex(Vector( 1.0, -1.0,  1.0), Vector( 0.0,  0.0,  1.0), 1.0, 0.0));
	verts.push_back(Vertex(Vector( 1.0,  1.0,  1.0), Vector( 0.0,  0.0,  1.0), 1.0, 1.0));
	verts.push_back(Vertex(Vector(-1.0, -1.0,  1.0), Vector( 0.0,  0.0,  1.0), 0.0, 0.0));
	
	//Right side
	verts.push_back(Vertex(Vector( 1.0,  1.0,  1.0), Vector( 1.0,  0.0,  0.0), 1.0, 1.0));
	verts.push_back(Vertex(Vector( 1.0, -1.0, -1.0), Vector( 1.0,  0.0,  0.0), 0.0, 0.0));
	verts.push_back(Vertex(Vector( 1.0,  1.0, -1.0), Vector( 1.0,  0.0,  0.0), 1.0, 0.0));
	verts.push_back(Vertex(Vector( 1.0, -1.0,  1.0), Vector( 1.0,  0.0,  0.0), 0.0, 1.0));
	
	for(int i=0; i<24; i++)
	{
		verts[i].vPos.x *= (dWidth * 0.5);
		verts[i].vPos.y *= (dHeight * 0.5);
		verts[i].vPos.z *= (dDepth * 0.5);	
	}	
}

void MatEditPreview::createSphere(vector<int>& indices, vector<Vertex>& verts,
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

void MatEditPreview::createTorus(vector<int>& indices, vector<Vertex>& verts,
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
			verts[nCurVertex].vNormal.normalize(); //Normlize to length 1
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

void MatEditPreview::createHelix(vector<int>& indices, vector<Vertex>& verts,
	double dRadiusOuterStart, double dRadiusInnerStart,
	double dRadiusOuterEnd, double dRadiusInnerEnd, double dHeight,
	int nRings, int nSegments, double dStart, double dEnd)
{
	if(isNearTo(dRadiusOuterStart, 0.0) || isNearTo(dRadiusInnerStart, 0.0) ||
		isNearTo(dRadiusOuterEnd, 0.0) || isNearTo(dRadiusInnerEnd, 0.0))
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
	double dDeltaRingAngle = (degToRad(dEnd-dStart) / nRings);
	//Angle per segment
	double dDeltaSegAngle = (2.0 * PI / nSegments);
	//Hight per ring
	double dDeltaHeight = dHeight / nRings;
	//Radius decrease/increase per ring
	double dDeltaRadiusOuter = (dRadiusOuterEnd - dRadiusOuterStart) / nRings;
	double dDeltaRadiusInner = (dRadiusInnerEnd - dRadiusInnerStart) / nRings;
	
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
			double dY = (dRadiusInnerStart + r * dDeltaRadiusInner) * sin(s * dDeltaSegAngle);
			
			//X-Coordinate of new vertex in relation to the inner radius
			double dR = (dRadiusInnerStart + r * dDeltaRadiusInner) * cos(s * dDeltaSegAngle);
			//Transform the X-Coordinate to X- and Z-Coordinate of complete torus
			double dX = (dRadiusOuterStart + r * dDeltaRadiusOuter + dR) * dC;
			double dZ = (dRadiusOuterStart + r * dDeltaRadiusOuter + dR) * dS;
			
			//Set up the new vertex on sphere
			vInnerCentre.set(dC * (dRadiusOuterStart + r * dDeltaRadiusOuter),
							 r * dDeltaHeight, 
							 dS * (dRadiusOuterStart + r * dDeltaRadiusOuter));
			verts[nCurVertex].vPos.set(dX, dY + r * dDeltaHeight, dZ);
			verts[nCurVertex].vNormal = verts[nCurVertex].vPos - vInnerCentre;
			verts[nCurVertex].vNormal.normalize(); //Normlize to length 1
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

void MatEditPreview::createGrid(vector<int>& indices, vector<Vertex>& verts,
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
	UINT nIndices = 6 * nSegmentsHeight * (nSegmentsWidth + 1);
	
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

void MatEditPreview::createGeoSphere(vector<int>& indices, vector<Vertex>& verts, 
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

void MatEditPreview::subdivideGeoSphere(Vector& v1, Vector& v2, Vector& v3, int nDepth,
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

double MatEditPreview::calcSphereMapU(Vector& v, double dRadius)
{
	if(v.y >= 0)
	{
		return acos(v.x / (dRadius * sin(PI * calcSphereMapV(v, dRadius)))) / (2 * PI);	
	}
	return (PI + acos(v.x / (dRadius * sin(PI * calcSphereMapV(v, dRadius))))) / (2 * PI);
}

double MatEditPreview::calcSphereMapV(Vector& v, double dRadius)
{
	return acos(v.z / dRadius) / PI;
}
