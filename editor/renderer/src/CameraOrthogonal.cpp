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
 *  Module:     Renderer (BlackSun)
 *  File:       CameraOrthogonal.cpp
 *  Created:    16.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/CameraOrthogonal.h"
#include "../include/Renderer.h"

namespace BSRenderer
{

CameraOrthogonal::CameraOrthogonal(int nWidth, int nHeight, OrthogonalType type)
: m_orthoType(type)
{
	setViewport(nWidth, nHeight);
	 
	reset();
}

CameraOrthogonal::~CameraOrthogonal()
{
	
}

void CameraOrthogonal::setViewport(int nWidth, int nHeight)
{
	Camera::setViewport(nWidth, nHeight);
	
	//Recalc projection matrix
	calcProjMatrix();
}

void CameraOrthogonal::getLimits(double& dLeft, double& dTop, double& dRight, 
	double& dBottom) const
{
	dLeft = m_dLeft;
	dTop = m_dTop;
	dRight = m_dRight;
	dBottom = m_dBottom;	
	
	double dRatio = static_cast<double>(m_nViewportWidth) / static_cast<double>(m_nViewportHeight);
	dTop /= dRatio;
	dBottom /= dRatio;
}

void CameraOrthogonal::setLimits(double dLeft, double dTop, double dRight, double dBottom)
{
	m_dLeft = dLeft;
	m_dTop = dTop;
	m_dRight = dRight;
	m_dBottom = dBottom;
	
	//Recalc projection matrix
	calcProjMatrix();	
}

void CameraOrthogonal::setClippingPlanes(double dNearPlane, double dFarPlane)
{ 
	//No changes, so return
	if((m_dNearPlane == dNearPlane) && (m_dFarPlane == dFarPlane))
		return;
		
	Camera::setClippingPlanes(dNearPlane, dFarPlane);
	
	//Recalc projection matrix
	calcProjMatrix();
}

void CameraOrthogonal::reset()
{
	Camera::reset();
	
	m_dLeft = m_dBottom = -3.0;
	m_dTop = m_dRight = 3.0;
	
	//setViewport(m_nViewportWidth, m_nViewportHeight);
	
	setPosition(Vector(0.0, 0.0, -(m_dFarPlane-1.0))); 
	calcViewMatrix();
	calcProjMatrix();
}

void CameraOrthogonal::getGridIntersectionPoints(vector<QPoint>& p) const
{
	//Get the limits
	double dL, dT, dR, dB;
	getLimits(dL, dT, dR, dB);
	
	//double dRatio = static_cast<double>(m_nViewportWidth) / static_cast<double>(m_nViewportHeight);
	//dT /= dRatio;
	//dB /= dRatio;	
		
	//Calculate distance between two (major) grid lines
	double dDist = calcDecade(std::max(dB - dT, dR - dL));
	
	//Calculate the start value of the lines		
	double dStartX = int(dL / dDist - 1.0) * dDist;
	double dStartY = int(dB / dDist - 1.0) * dDist;
	
	dDist /= static_cast<double>(Renderer::getInstance()->getSettings()->getGridSize());
	
	vector<int> vertical;
	vector<int> horizontal;
	
	//Vertical lines
	for(double x = dStartX; x <= dR; x += dDist)
	{
		vertical.push_back(static_cast<int>(((x-dStartX)/(dR-dL))*m_nViewportWidth));
	}
		
	//Horizontal lines
	for(double y = dStartY; y <= dT; y += dDist)
	{
		horizontal.push_back(static_cast<int>(((y-dStartY)/(dT-dB))*m_nViewportHeight));
	}
	
	//Create the intersection points
	for(unsigned int y=0; y<horizontal.size(); y++)
	{
		if(horizontal[y] < 0)
			horizontal[y] = 0;
		if(horizontal[y] > m_nViewportHeight)
			horizontal[y] = m_nViewportHeight;
			
		for(unsigned int x=0; x<vertical.size(); x++)
		{
			if(vertical[x] < 0)
				vertical[x] = 0;
			if(vertical[x] > m_nViewportWidth)
				vertical[x] = m_nViewportWidth;
				
			p.push_back(QPoint(vertical[x], m_nViewportHeight - horizontal[y]));
		}
	}
}

void CameraOrthogonal::mousePress(int nPosX, int nPosY)
{
	//Save the position of the mouse
	m_nLastMousePosX = nPosX;
	m_nLastMousePosY = nPosY;
}

void CameraOrthogonal::mouseMove(int nPosX, int nPosY)
{
	int nDeltaX = m_nLastMousePosX - nPosX;
	int nDeltaY = m_nLastMousePosY - nPosY;
	
	double dDeltaX = ((m_dRight - m_dLeft) / m_nViewportWidth) * nDeltaX;
	double dDeltaY = ((m_dBottom - m_dTop) / m_nViewportHeight) * nDeltaY;
	
	//Move the area
	m_dLeft += dDeltaX;
	m_dRight += dDeltaX;
	m_dTop += dDeltaY;
	m_dBottom += dDeltaY;

	//Last position is now current position
	m_nLastMousePosX = nPosX;
	m_nLastMousePosY = nPosY;	
	
	//Recalc projection matrix
	calcProjMatrix();
}

void CameraOrthogonal::mouseWheel(int nDelta)
{
	//'Zoom' by changing the position 
	double dScroll = -m_dWheelSpeed * (nDelta / 120.0);
	
	//Expand the area
	m_dLeft -= dScroll;
	m_dRight += dScroll;
	m_dTop += dScroll;
	m_dBottom -= dScroll;
	
	//Check for overlapping
	if(isNearTo(m_dLeft, m_dRight, 0.001) || isNearTo(m_dBottom, m_dTop, 0.001) ||
		(m_dLeft >= m_dRight) || (m_dBottom >= m_dTop))
	{
		//Redo expanding the area
		m_dLeft += dScroll;
		m_dRight -= dScroll;
		m_dTop -= dScroll;
		m_dBottom += dScroll;		
	}	
	
	//Recalc the projection matrix
	calcProjMatrix();
}

void CameraOrthogonal::calcViewMatrix()
{
	switch(m_orthoType)
	{
		case ORTHO_Front:  m_mView.identity(); break;
		case ORTHO_Back:   m_mView = Matrix::makeRotateY(degToRad(180.0));  break;
		case ORTHO_Left:   m_mView = Matrix::makeRotateY(degToRad(90.0));  break;
		case ORTHO_Right:  m_mView = Matrix::makeRotateY(degToRad(-90.0));   break;
		case ORTHO_Top:    m_mView = Matrix::makeRotateX(degToRad(90.0));   break;
		case ORTHO_Bottom: m_mView = Matrix::makeRotateX(degToRad(-90.0));  break;
	}
	m_mView.translation(m_vPos);
}

void CameraOrthogonal::calcProjMatrix()
{
	//Create a orthogonal matrix with the correct ratio
	double dRatio = static_cast<double>(m_nViewportWidth) / static_cast<double>(m_nViewportHeight);
	m_mProj = Matrix::makeOrthogonal(m_dLeft, m_dTop/dRatio, m_dRight, m_dBottom/dRatio, 
		m_dNearPlane, m_dFarPlane);
}

}
