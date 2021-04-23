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
 *  File:       TexCoordWorkArea.cpp
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/TexCoordWorkArea.h"
#include "./../../editor/gui/include/bsglwidget.h"
#include <QtGui>
#include <iostream>

using namespace std;
using namespace BSGui;
using namespace BSScene;

TexCoordWorkArea::TexCoordWorkArea(QWidget *parent)
    : BSGLWidget(parent)
{
	m_pMesh = NULL;
	
	m_pCamPerspective = new CameraArcball(width(), height());
	m_pCamOrthogonal = new CameraOrthogonal(width(), height(), ORTHO_Front);
	
	m_pCamOrthogonal->setLimits(-0.5, 1.5, 1.5, -0.5);
	
	m_vm = VIEWMODE_Orthogonal;

	m_bShowGrid = true;	
	m_bShowRubberBand = false;
	
	//Create the textured quad
	double dDepth = -1.0;
	m_vertsTexQuad.push_back(Vertex(Vector(0.0, 0.0, dDepth), Vector(0.0, 0.0, 1.0), 0.0, 0.0));
	m_vertsTexQuad.push_back(Vertex(Vector(1.0, 0.0, dDepth), Vector(0.0, 0.0, 1.0), 1.0, 0.0));
	m_vertsTexQuad.push_back(Vertex(Vector(1.0, 1.0, dDepth), Vector(0.0, 0.0, 1.0), 1.0, 1.0));
	m_vertsTexQuad.push_back(Vertex(Vector(1.0, 1.0, dDepth), Vector(0.0, 0.0, 1.0), 1.0, 1.0));
	m_vertsTexQuad.push_back(Vertex(Vector(0.0, 1.0, dDepth), Vector(0.0, 0.0, 1.0), 0.0, 1.0));
	m_vertsTexQuad.push_back(Vertex(Vector(0.0, 0.0, dDepth), Vector(0.0, 0.0, 1.0), 0.0, 0.0));

	createRepeatMesh();

}

void TexCoordWorkArea::setMesh(Mesh* pMesh)
{
	m_pMesh = pMesh;
	convertMesh();	
}
	
void TexCoordWorkArea::setTextureStage(int nTexStage)
{
	m_nTexStage = nTexStage;
	convertMesh();
}

void TexCoordWorkArea::initializeGL()
{
	r = Renderer::getInstance();
	r->init();
}

void TexCoordWorkArea::convertMesh()
{
	m_vertsMesh.clear();
	m_indicesMesh.clear();
	m_vertAllocations.clear();
	m_vertsMeshSelection.clear();
	
	if(m_pMesh == NULL)
		return;
	
	vector<BSScene::Polygon>* polys = m_pMesh->getPolys();
 
	//Walk through all polygons
	for(vector<BSScene::Polygon>::iterator itPolys = polys->begin(); itPolys != polys->end(); ++itPolys)
	{			
		vector<Vertex>*	verts = itPolys->getVertices();
		vector<int>*	indices = itPolys->getIndices();
		
		int nIndexBias = m_vertsMesh.size();
		for(unsigned int v=0; v<verts->size(); v++)
		{
			m_vertsMesh.push_back((*verts)[v]);
			
			m_vertsMesh[m_vertsMesh.size()-1].vPos = Vector(m_vertsMesh[m_vertsMesh.size()-1].dU[m_nTexStage],
				m_vertsMesh[m_vertsMesh.size()-1].dV[m_nTexStage],0.0);
		}
		
		for(unsigned int v=0; v<verts->size(); v++)
		{
			m_vertAllocations.push_back(TexCoordVertex(&((*verts)[v]), &(m_vertsMesh[v])));
		}
		
		for(unsigned int i=0; i<indices->size(); i++)
		{
			m_indicesMesh.push_back((*indices)[i]+nIndexBias);	
		}		
	}
	
	for(unsigned int v=0; v<m_vertsMesh.size(); v++)
	{
		m_vertAllocations[v].pVertWorkArea = &(m_vertsMesh[v]);
	}
	
	update();		
}

void TexCoordWorkArea::createRepeatMesh()
{
	m_vertsTexQuadRepeat.clear();
    m_indicesTexQuadRepeat.clear();
    
    double dDepth = -1.0;
    double dStartX = 0.0;
    double dEndX = 0.0;
    double dStartY = 0.0;
    double dEndY = 0.0;
    m_pCamOrthogonal->getLimits(dStartX, dEndY, dEndX, dStartY);

	m_vertsTexQuadRepeat.push_back(Vertex(Vector(0.0, 0.0, dDepth), Vector(0.0, 0.0, 1.0), 0.0, 0.0));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(1.0, 0.0, dDepth), Vector(0.0, 0.0, 1.0), 1.0, 0.0));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(1.0, 1.0, dDepth), Vector(0.0, 0.0, 1.0), 1.0, 1.0));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(0.0, 1.0, dDepth), Vector(0.0, 0.0, 1.0), 0.0, 1.0));

	m_vertsTexQuadRepeat.push_back(Vertex(Vector(dStartX, dStartY, dDepth), Vector(0.0, 0.0, 1.0), dStartX, dStartY));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(dEndX,   dStartY, dDepth), Vector(0.0, 0.0, 1.0), dEndX,   dStartY));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(dEndX,   dEndY,   dDepth), Vector(0.0, 0.0, 1.0), dEndX,   dEndY));
	m_vertsTexQuadRepeat.push_back(Vertex(Vector(dStartX, dEndY,   dDepth), Vector(0.0, 0.0, 1.0), dStartX, dEndY));
	
	int ind[] = {5, 0, 4,
				 5, 1, 0,
				 5, 2, 1,
				 5, 6, 2,
				 7, 2, 6,
				 7, 3, 2,
				 7, 4, 3,
				 4, 0, 3};
	for(int i=0; i<24; i++)
	{
		m_indicesTexQuadRepeat.push_back(ind[i]);	
	}
	
    update();	
}

void TexCoordWorkArea::updateMesh()
{
	//Walk through all vertices
	for(int t=0; t<MAXTEXTURES; t++)
	{
		for(unsigned int v=0; v<m_vertAllocations.size(); v++)
		{
			m_vertAllocations[v].pVertMesh->dU[t] = m_vertAllocations[v].pVertWorkArea->dU[t];
			m_vertAllocations[v].pVertMesh->dV[t] = m_vertAllocations[v].pVertWorkArea->dV[t];					
		}
	}	
}

void TexCoordWorkArea::copyTexCoords(int nStageFrom, int nStageTo)
{
	//The stages are the same, do nothing
	if(nStageFrom == nStageTo)
		return;
		
	//Walk through all vertices
	for(unsigned int v=0; v<m_vertAllocations.size(); v++)
	{
		m_vertAllocations[v].pVertWorkArea->dU[nStageTo] = m_vertAllocations[v].pVertWorkArea->dU[nStageFrom];
		m_vertAllocations[v].pVertWorkArea->dV[nStageTo] = m_vertAllocations[v].pVertWorkArea->dV[nStageFrom];
		
		m_vertAllocations[v].pVertMesh->dU[nStageTo] = m_vertAllocations[v].pVertMesh->dU[nStageFrom];
		m_vertAllocations[v].pVertMesh->dV[nStageTo] = m_vertAllocations[v].pVertMesh->dV[nStageFrom];
						
		m_vertAllocations[v].pVertWorkArea->vPos.set(m_vertAllocations[v].pVertWorkArea->dU[nStageTo],
			m_vertAllocations[v].pVertWorkArea->dV[nStageTo], 0.0); 	
	}
	
	//Update view if current stage was changed
	if(m_nTexStage == nStageTo)
		update();			
}

void TexCoordWorkArea::paintGL()
{
	MaterialManager* pMatMgr = Renderer::getInstance()->getMaterialManager();
	
	QPainter painter;
	painter.begin(this);
	painter.setPen(Qt::SolidLine);
            
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
        
	Renderer::getInstance()->init();
	Renderer::getInstance()->setViewport(width(), height());
	m_pCamPerspective->setViewport(width(), height());
	m_pCamOrthogonal->setViewport(width(), height());

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
	r->setCameraPerspective(m_pCamPerspective);
	r->setCameraOrthogonal(m_pCamOrthogonal);
	r->clear();
	
	int nMatID = -1;
	
	if(m_pMesh != NULL)
	{
		if(m_pMesh->getPolys()->size()!=0)
		{
			nMatID = (*(m_pMesh->getPolys()->begin())).getMaterialID();
			Material* pMat = pMatMgr->getMaterial(nMatID);
			Material* pTexStageMat = pMatMgr->getPluginMaterial(0);
			pTexStageMat->setToDefault();
			pTexStageMat->setTexture(0, pMat->getTexture(m_nTexStage));
			nMatID = pMatMgr->getPluginMaterialID(0);
		}
	}

	r->setFillMode(FILL_Textured);
	r->render(PRIM_Triangle, m_vertsTexQuad, nMatID, m_mTransform, m_aabb);
	if(m_bShowRepeat)
		r->render(PRIM_Triangle, m_vertsTexQuadRepeat, m_indicesTexQuadRepeat, nMatID, m_mTransform, m_aabb);
	r->flush();

	if(m_bShowGrid)
	{
		r->setFillMode(FILL_Wireframe);
		r->renderGrid();
		r->flush();
	}

	r->setFillMode(FILL_Wireframe);	
	r->render(PRIM_Triangle, m_vertsMesh, m_indicesMesh, nMatID, m_mTransform, m_aabb);
	if(m_vertsMeshSelection.size()!=0)
	{
		vector<Vertex> vertsSelected;
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
			vertsSelected.push_back(*(m_vertsMeshSelection[v]->pVertWorkArea));
			
		r->renderSelection(PRIM_Point, vertsSelected, m_mTransform, nMatID, m_aabb);	
	}
	r->flush();

	//Restore realtime mirror
	r->getSettings()->setRealtimeMirror(rm);
            
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
        
	if(m_bShowRubberBand)
	{
		painter.drawRect(QRect(m_posRubberBandOrigin, m_posRubberBandDestination));
		painter.fillRect(QRect(m_posRubberBandOrigin, m_posRubberBandDestination), QBrush(QColor(255,255,255,50)));
	}
	
	//painter.drawText(QPoint(10,20), "TestString");
        
	painter.end();
}

void TexCoordWorkArea::resizeGL(int width, int height)
{
	r->setViewport(width, height);
	m_pCamPerspective->setViewport(width, height);
	m_pCamOrthogonal->setViewport(width, height);
	createRepeatMesh();
} 

void TexCoordWorkArea::update()
{    
	updateGL();	
}

void TexCoordWorkArea::mousePressEvent(QMouseEvent *event)
{
	if(QApplication::keyboardModifiers() == Qt::ControlModifier && event->buttons() == Qt::LeftButton)
	{
		m_bShowRubberBand = false;
		
		if(m_vm == VIEWMODE_Perspective)
			m_pCamPerspective->mousePress(event->x(), event->y());
		else
			m_pCamOrthogonal->mousePress(event->x(), event->y());
			
		createRepeatMesh();
	}
	else if(event->buttons() == Qt::LeftButton && m_mode == TEM_Select && QApplication::keyboardModifiers() != Qt::ControlModifier)
	{
		m_posRubberBandOrigin = event->pos();
		m_posRubberBandDestination = event->pos();
		m_bShowRubberBand = true;	
	}
	
	if(event != NULL)
	{
		m_nLastPosX = event->x();
		m_nLastPosY = event->y();
	}
    updateGL();
} 

void TexCoordWorkArea::mouseMoveEvent(QMouseEvent *event)
{
	if(QApplication::keyboardModifiers() == Qt::ControlModifier && event->buttons() == Qt::LeftButton)
	{	
		m_bShowRubberBand = false;
		if(m_vm == VIEWMODE_Perspective)
			m_pCamPerspective->mouseMove(event->x(), event->y());
		else
			m_pCamOrthogonal->mouseMove(event->x(), event->y());
			
		createRepeatMesh();
	}
	else if(event->buttons() == Qt::LeftButton && m_mode == TEM_Select && QApplication::keyboardModifiers() != Qt::ControlModifier)
	{
		m_posRubberBandDestination = event->pos();
	}
	else if(event->buttons() == Qt::LeftButton && m_mode == TEM_Move && QApplication::keyboardModifiers() != Qt::ControlModifier)
	{
		m_bShowRubberBand = false;
		
		double dCamStartX = 0.0;
		double dCamEndX   = 0.0;
		double dCamStartY = 0.0;
		double dCamEndY   = 0.0;
		
		m_pCamOrthogonal->getLimits(dCamStartX, dCamEndY, dCamEndX, dCamStartY);

		double dDeltaX = (dCamEndX - dCamStartX) / width();
		double dDeltaY = (dCamEndY - dCamStartY) / height();
		
		double dMoveX = (event->x() - m_nLastPosX) * dDeltaX;
		double dMoveY = -(event->y() - m_nLastPosY) * dDeltaY;		
		
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
		{
			m_vertsMeshSelection[v]->pVertWorkArea->vPos += Vector(dMoveX, dMoveY, 0.0);
			m_vertsMeshSelection[v]->pVertMesh->dU[m_nTexStage] += dMoveX;
			m_vertsMeshSelection[v]->pVertMesh->dV[m_nTexStage] += dMoveY;
		}
	}
	else if(event->buttons() == Qt::LeftButton && m_mode == TEM_Scale && QApplication::keyboardModifiers() != Qt::ControlModifier)
	{
		m_bShowRubberBand = false;
		
		double dCamStartX = 0.0;
		double dCamEndX   = 0.0;
		double dCamStartY = 0.0;
		double dCamEndY   = 0.0;
		
		m_pCamOrthogonal->getLimits(dCamStartX, dCamEndY, dCamEndX, dCamStartY);

		double dDeltaX = (dCamEndX - dCamStartX) / width();
		double dDeltaY = (dCamEndY - dCamStartY) / height();
		
		double dScaleX = (event->x() - m_nLastPosX) * dDeltaX;
		double dScaleY = -(event->y() - m_nLastPosY) * dDeltaY;
		
		Vector vMin( 999999.0,  999999.0,  999999.0);
		Vector vMax(-999999.0, -999999.0, -999999.0);
		Vector vCentre;
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
		{
			vMin = Vector::min(vMin, m_vertsMeshSelection[v]->pVertWorkArea->vPos);
			vMax = Vector::max(vMax, m_vertsMeshSelection[v]->pVertWorkArea->vPos);
		}
		vCentre = (vMin + vMax) * 0.5;
		double dDimWidth = vMax.x - vMin.x;
		double dDimHeight = vMax.y - vMin.y;
		
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
		{
			m_vertsMeshSelection[v]->pVertWorkArea->vPos -= vCentre;
			m_vertsMeshSelection[v]->pVertWorkArea->vPos.x *= (dDimWidth*0.5+dScaleX)/(dDimWidth*0.5);
			m_vertsMeshSelection[v]->pVertWorkArea->vPos.y *= (dDimHeight*0.5+dScaleY)/(dDimHeight*0.5);			
			m_vertsMeshSelection[v]->pVertWorkArea->vPos += vCentre;
			
			m_vertsMeshSelection[v]->pVertMesh->dU[m_nTexStage] = m_vertsMeshSelection[v]->pVertWorkArea->vPos.x;
			m_vertsMeshSelection[v]->pVertMesh->dV[m_nTexStage] = m_vertsMeshSelection[v]->pVertWorkArea->vPos.y;
		}				
	}
	else if(event->buttons() == Qt::LeftButton && m_mode == TEM_Rotate && QApplication::keyboardModifiers() != Qt::ControlModifier)
	{
		m_bShowRubberBand = false;
		
		double dCamStartX = 0.0;
		double dCamEndX   = 0.0;
		double dCamStartY = 0.0;
		double dCamEndY   = 0.0;
		
		m_pCamOrthogonal->getLimits(dCamStartX, dCamEndY, dCamEndX, dCamStartY);

		double dDeltaX = (dCamEndX - dCamStartX) / width();
		double dDeltaY = (dCamEndY - dCamStartY) / height();
		
		double dRotateX0 = m_nLastPosX * dDeltaX + dCamStartX;
		double dRotateY0 = (height() - m_nLastPosY) * dDeltaY + dCamStartY;
				
		double dRotateX1 = event->x() * dDeltaX + dCamStartX;
		double dRotateY1 = (height() - event->y()) * dDeltaY + dCamStartY;
		
		Vector vMin( 999999.0,  999999.0,  999999.0);
		Vector vMax(-999999.0, -999999.0, -999999.0);
		Vector vCentre;
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
		{
			vMin = Vector::min(vMin, m_vertsMeshSelection[v]->pVertWorkArea->vPos);
			vMax = Vector::max(vMax, m_vertsMeshSelection[v]->pVertWorkArea->vPos);
		}
		vCentre = (vMin + vMax) * 0.5;
		vCentre.z = 0.0;
		Vector v1 = Vector(dRotateX0, dRotateY0, 0.0) - vCentre;
		Vector v2 = Vector(dRotateX1, dRotateY1, 0.0) - vCentre;
		v1.normalize();
		v2.normalize();
		double dAngle = v1.angleWith(v2);
		
		//Calculte the normal vector
		Vector norm = v1 ^ Vector(0.0, 0.0, 1.0);
		if(norm != Vector(0.0, 0.0, 0.0))
			norm.normalize();
		
		//Create plane to classify the rotation direction
		Plane<double> plane(norm,  norm * v1);
		if(plane.classifyPoint(v2)==PPC_Back)
			dAngle *= -1.0;
	
		Matrix m = Matrix::makeRotateZ(dAngle);
		
		Vector vPos;
		for(unsigned int v=0; v<m_vertsMeshSelection.size(); v++)
		{
			vPos = m_vertsMeshSelection[v]->pVertWorkArea->vPos;
			m_vertsMeshSelection[v]->pVertWorkArea->vPos -= vCentre;			
			m_vertsMeshSelection[v]->pVertWorkArea->vPos = m_vertsMeshSelection[v]->pVertWorkArea->vPos * m;
			m_vertsMeshSelection[v]->pVertWorkArea->vPos += vCentre;
			
			m_vertsMeshSelection[v]->pVertMesh->dU[m_nTexStage] = m_vertsMeshSelection[v]->pVertWorkArea->vPos.x;
			m_vertsMeshSelection[v]->pVertMesh->dV[m_nTexStage] = m_vertsMeshSelection[v]->pVertWorkArea->vPos.y;
		}		
	}

	if(event != NULL)
	{
		m_nLastPosX = event->x();
		m_nLastPosY = event->y();
	}
    updateGL();
} 

void TexCoordWorkArea::mouseReleaseEvent(QMouseEvent *event)
{
	m_bShowRubberBand = false;
	
	if(m_mode == TEM_Select)
	{
		if(m_posRubberBandDestination == m_posRubberBandOrigin)
		{
			m_posRubberBandDestination -= QPoint(2,2);
			m_posRubberBandOrigin += QPoint(2,2);
		}
		
		double dCamStartX = 0.0;
		double dCamEndX   = 0.0;
		double dCamStartY = 0.0;
		double dCamEndY   = 0.0;
		
		m_pCamOrthogonal->getLimits(dCamStartX, dCamEndY, dCamEndX, dCamStartY);
		
		int nStartX = min(m_posRubberBandDestination.x(), m_posRubberBandOrigin.x());
		int nEndX   = max(m_posRubberBandDestination.x(), m_posRubberBandOrigin.x());
		int nStartY = min(height()-m_posRubberBandDestination.y(), height()-m_posRubberBandOrigin.y());
		int nEndY   = max(height()-m_posRubberBandDestination.y(), height()-m_posRubberBandOrigin.y());
		
		double dDeltaX = (dCamEndX - dCamStartX) / width();
		double dDeltaY = (dCamEndY - dCamStartY) / height();
		
		double dStartX = nStartX * dDeltaX + dCamStartX;
		double dEndX   = nEndX *   dDeltaX + dCamStartX;
		double dStartY = nStartY * dDeltaY + dCamStartY;
		double dEndY   = nEndY *   dDeltaY + dCamStartY;
		
		m_vertsMeshSelection.clear();
		Vector vPos;
		for(unsigned int v=0; v<m_vertAllocations.size(); v++)
		{
			vPos = m_vertAllocations[v].pVertWorkArea->vPos;
			
			if((vPos.x >= dStartX) && (vPos.x <= dEndX) && (vPos.y >= dStartY) && (vPos.y <= dEndY))
			{ 
				m_vertsMeshSelection.push_back(&(m_vertAllocations[v]));
			}				
		}
			
	}
	
	if(event != NULL)
	{
		m_nLastPosX = event->x();
		m_nLastPosY = event->y();
	}
	updateGL();
}

void TexCoordWorkArea::wheelEvent(QWheelEvent *event)
{
	if(m_vm == VIEWMODE_Perspective)
		m_pCamPerspective->mouseWheel(event->delta());
	else
		m_pCamOrthogonal->mouseWheel(event->delta());
	
	createRepeatMesh();
    updateGL();
}

void TexCoordWorkArea::resetCameras()
{	
	m_pCamPerspective->reset();
	m_pCamOrthogonal->reset();
	
	m_pCamOrthogonal->setLimits(-0.5, 1.5, 1.5, -0.5);
    createRepeatMesh();
    
	update();
}
