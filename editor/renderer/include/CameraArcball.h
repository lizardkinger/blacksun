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
 *  File:       CameraArcball.h
 *  Created:    12.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef CAMERAARCBALL_H_
#define CAMERAARCBALL_H_

#include "RendererTypes.h"
#include "Camera.h"

/*! \file
* 
*  \brief Describes the class CameraArcball, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief The perspective arcball camera
*
* A perspective camera that can be handled via arcball rotation. The camera can be rotated
* around the scene by moving the mouse.
*/
class CameraArcball: public Camera
{
public:
	/*!
	* \brief The constructor
	* 
	* Sets the viewport and set up a standard arcball camera 
	* 
	* \param nWidth The viewport width
	* \param nHeight The viewport height
	*/	
	CameraArcball(int nWidth, int nHeight);
	
	/*!
	* \brief The destructor 
	*/	
	virtual ~CameraArcball()
	{
	}
	
	/*!
	* \brief Set the position of the camera
	* 
	* \param vPos The position
	*/	
	virtual void setPosition(const Vector& vPos)
	{
		Camera::setPosition(vPos);
		calcViewMatrix();
	}
	
	/*!
	* \brief Set the dimension of the viewport
	* 
	* \param nWidth The viewport width
	* \param nHeight The viewport height
	*/		
	virtual void setViewport(int nWidth, int nHeight);
	
	/*!
	* \brief Set the clipping planes
	* 
	* \param dNearPlane The near clipping plane
	* \param dFarPlane The far clipping plane
	*/	
	virtual void setClippingPlanes(double dNearPlane = 0.1, double dFarPlane = 1000);
	
	/*!
	* \brief Set the field of view
	* 
	* \param dFOV The field of view
	*/		
	virtual void setFOV(double dFOV);	
	
	/*!
	* \brief Reset the camera to default
	* 
	* The camera will be reseted by restoring the variables to their default values
	*/		
	virtual void reset();
	
	/*!
	* \brief Tells the camera, that the mouse button was pressed
	* 
	* \param nPosX The x-coordinate of the mouse position
	* 	(in relation the the widget-origin)
	* \param nPosY The y-coordinate of the mouse position 
	* 	(in relation the the widget-origin)
	*/	
	virtual void mousePress(int nPosX, int nPosY);
	
	/*!
	* \brief Tells the camera, that the mouse was moved
	* 
	* \param nPosX The new x-coordinate of the mouse position
	* 	(in relation the the widget-origin)
	* \param nPosY The new y-coordinate of the mouse position 
	* 	(in relation the the widget-origin)
	*/
	virtual void mouseMove(int nPosX, int nPosY);

	/*!
	* \brief Tells the camera, that the mouse was moved and should be translated
	* 
	* \param nPosX The new x-coordinate of the mouse position
	* 	(in relation the the widget-origin)
	* \param nPosY The new y-coordinate of the mouse position 
	* 	(in relation the the widget-origin)
	*/	
	void mouseMoveTranslate(int nPosX, int nPosY);
	
	/*!
	* \brief Calls the camera, that the mouse wheel was rotated
	* 
	* \param nDelta The delta-value of the mouse wheel
	*/
	virtual void mouseWheel(int nDelta);

private:
	/*!
	* \brief Calculates the view matrix
	* 
	* Calculates the matrix of the quaternion and applies the camera position
	*/
	void calcViewMatrix();
	
	/*!
	* \brief Caluclates the projection matrix
	* 
	* Calculates a perspective matrix (FOV) of viewport and clipping informations
	*/	
	void calcProjMatrix();	

private:
	Quat m_qRot; /*!< \brief The current rotation quaternion */
	Quat m_qRotOld; /*!< \brief The old rotation quaternion (last move) */
	Arcball<double> m_arcball; /*!< \brief The arcball */
	
	int m_nLastMousePosX; /*!< \brief The last x-coordinate of the mouse */
	int m_nLastMousePosY; /*!< \brief The last y-coordinate of the mouse */		
};

}

#endif /*CAMERAARCBALL_H_*/
