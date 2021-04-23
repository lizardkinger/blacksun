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
 *  File:       CameraArcball.cpp
 *  Created:    12.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/CameraArcball.h"

namespace BSRenderer
{

CameraArcball::CameraArcball(int nWidth, int nHeight)
{
	setViewport(nWidth, nHeight);
	
	reset();
}

void CameraArcball::setViewport(int nWidth, int nHeight)
{
	Camera::setViewport(nWidth, nHeight);
	
	//Recalc the projection matrix
	calcProjMatrix();
}

void CameraArcball::setClippingPlanes(double dNearPlane, double dFarPlane)
{
	//No changes, so return
	if((m_dNearPlane == dNearPlane) && (m_dFarPlane == dFarPlane))
		return;
		
	Camera::setClippingPlanes(dNearPlane, dFarPlane);
	
	//Recalc the projection matrix
	calcProjMatrix();
}


void CameraArcball::setFOV(double dFOV)
{
	//No changes, so return
	if(m_dFOV == dFOV)
		return;
		
	Camera::setFOV(dFOV);
	
	//Recalc the projection matrix
	calcProjMatrix();
}

void CameraArcball::reset()
{
	Camera::reset();
	
	setPosition(Vector(0.0, 0.0, -5.0));
	m_qRot.set(-0.225, 0.0, 0.0, 1.0);
	calcViewMatrix();
}

void CameraArcball::mousePress(int nPosX, int nPosY)
{
	//Save the position of the mouse
	m_nLastMousePosX = nPosX;
	m_nLastMousePosY = nPosY;
		
	double dX = (static_cast<double>(nPosX) / (static_cast<double>(m_nViewportWidth - 1) * 0.5)) - 1.0;
	double dY = (static_cast<double>(nPosY) / (static_cast<double>(m_nViewportHeight - 1) * 0.5)) - 1.0;
	
	m_qRotOld = m_qRot;
	
	//Transmit click event to arcball
	m_arcball.click(-dX, dY);
		
	//Recalc the view matrix
	calcViewMatrix();	
}


void CameraArcball::mouseMove(int nPosX, int nPosY)
{
	//Save the position of the mouse
	m_nLastMousePosX = nPosX;
	m_nLastMousePosY = nPosY;
		
	double dX = (static_cast<double>(nPosX) / (static_cast<double>(m_nViewportWidth - 1) * 0.5)) - 1.0;
	double dY = (static_cast<double>(nPosY) / (static_cast<double>(m_nViewportHeight - 1) * 0.5)) - 1.0;
	
	//Transmit move event to arcball
	Quat qTemp = m_arcball.drag(-dX, dY);
	 
	//Calculate new rotation quaternion
	m_qRot = (m_qRotOld * qTemp).getNormalize();

	//Recalc the view matrix
	calcViewMatrix();
}

void CameraArcball::mouseMoveTranslate(int nPosX, int nPosY)
{ 
	m_vPos.x += 0.005 * (nPosX - m_nLastMousePosX);
	m_vPos.y -= 0.005 * (nPosY - m_nLastMousePosY);
	
	//Save the position of the mouse
	m_nLastMousePosX = nPosX;
	m_nLastMousePosY = nPosY;	
	
	//Recalc the view matrix
	calcViewMatrix();		
}

void CameraArcball::mouseWheel(int nDelta)
{
	//'Zoom' by changing the position 
	m_vPos.z += m_dWheelSpeed * (nDelta / 120.0);
	
	/*if(m_vPos.z < 0.0)
	{
		m_vPos.z = 0.0;	
	}*/

	//Recalc the view matrix	
	calcViewMatrix();
}

void CameraArcball::calcViewMatrix()
{
	//Vector a = m_qRot.getEulerAngles();
	//cout << a.x << " " << a.y << " " << a.z << endl;
	
	//Calcute the view matrix from the rotation quaternion
	m_mView = m_qRot.toMatrix();
	m_mView.transpose();
	m_mView.translation(m_vPos);	
}

void CameraArcball::calcProjMatrix()
{
	//Create a perspective matrix	
	m_mProj = Matrix::makePerspectiveFov(m_dFOV, 
		static_cast<double>(m_nViewportWidth)/static_cast<double>(m_nViewportHeight), 
		m_dNearPlane, m_dFarPlane);			
}

}
