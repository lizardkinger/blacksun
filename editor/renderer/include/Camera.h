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
 *  File:       Camera.h
 *  Created:    12.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef CAMERA_H_
#define CAMERA_H_

#include "RendererTypes.h"

/*! \file
* 
*  \brief Describes the interface Camera, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief The camera-interface 
*
* An interface for all cameras. These cameras are used to combine the projection-
* and view-matrix to have a individual view on a scene
*/
class Camera
{
public:
	/*!
	* \brief The default constructor
	* 
	* Sets the viewport to 400x300 and set up all standard camera variables 
	*/	
	Camera() : m_nViewportWidth(400), m_nViewportHeight(300)
	{
		reset();	
	}
	
	/*!
	* \brief The destruktor
	*/		
	virtual ~Camera() {}

	/*!
	* \brief Returns a reference to the view matrix
	* 
	* \return Matrix& A reference to the view matrix
	*/	
	Matrix& getViewMatrix()
	{
		return m_mView;	
	}
	
	/*!
	* \brief Returns a reference to the projection matrix
	* 
	* \return Matrix& A reference to the projection matrix
	*/	
	Matrix& getProjectionMatrix()
	{
		return m_mProj;	
	}

	/*!
	* \brief Set the position of the camera
	* 
	* \param vPos The position
	*/	
	virtual void setPosition(const Vector& vPos)
	{
		m_mView.translation(vPos);
		m_vPos = vPos;
	}
	
	/*!
	* \brief Return the camera position
	* 
	* \return Vector The position
	*/		
	Vector getPosition() const
	{
		return m_mView.getTranslation();	
	}
	
	/*!
	* \brief Set the dimension of the viewport
	* 
	* \param nWidth The viewport width
	* \param nHeight The viewport height
	*/		
	virtual void setViewport(int nWidth, int nHeight)
	{
		m_nViewportWidth = nWidth;
		m_nViewportHeight = nHeight;	
	}
	
	/*!
	* \brief Returns the dimension of the viewport
	* 
	* \param nWidth Reference where the viewport width will be stored
	* \param nHeight Reference where the viewport height will be stored
	*/		
	void getViewport(int& nWidth, int& nHeight) const
	{
		nWidth = m_nViewportWidth;
		nHeight = m_nViewportHeight;	
	}
	
	/*!
	* \brief Set the clipping planes
	* 
	* \param dNearPlane The near clipping plane
	* \param dFarPlane The far clipping plane
	*/		
	virtual void setClippingPlanes(double dNearPlane = 0.1, double dFarPlane = 1000.0)
	{
		m_dNearPlane = dNearPlane;
		m_dFarPlane = dFarPlane;
	}
	
	/*!
	* \brief Set the speed of zooming via the mouse wheel
	* 
	* \param dWheelSpeed The zooming speed
	*/		
	virtual void setWheelSpeed(double dWheelSpeed)
	{
		m_dWheelSpeed = dWheelSpeed;
	}
	
	/*!
	* \brief Returns the speed of zooming via the mouse wheel
	* 
	* \return double The zooming speed
	*/		
	double getWheelSpeed() const
	{
		return m_dWheelSpeed;
	}
	
	/*!
	* \brief Set the field of view
	* 
	* \param dFOV The field of view
	*/		
	virtual void setFOV(double dFOV)
	{
		m_dFOV = dFOV;
	}
	
	/*!
	* \brief Returns the field of view
	* 
	* \return double The field of view
	*/		
	double getFOV() const
	{
		return m_dFOV;
	}
	
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
	virtual void mousePress(int nPosX, int nPosY) = 0;
	
	/*!
	* \brief Tells the camera, that the mouse was moved
	* 
	* \param nPosX The new x-coordinate of the mouse position
	* 	(in relation the the widget-origin)
	* \param nPosY The new y-coordinate of the mouse position 
	* 	(in relation the the widget-origin)
	*/	
	virtual void mouseMove(int nPosX, int nPosY) = 0;
	
	/*!
	* \brief Calls the camera, that the mouse wheel was rotated
	* 
	* \param nDelta The delta-value of the mouse wheel
	*/
	virtual void mouseWheel(int nDelta) = 0;
	
protected:
	Matrix m_mView; /*!< \brief The view-matrix */
	Matrix m_mProj; /*!< \brief The projection-matrix */
	Vector m_vPos;  /*!< \brief The positin of the camera */
	double m_dWheelSpeed; /*!< \brief The zooming speed */

	//Frustum m_frustum;
	//bool m_bFrustumUpToDate; 
	
	int m_nViewportWidth;  /*!< \brief The width of the viewport */
	int m_nViewportHeight; /*!< \brief The height of the viewport */
	
	double m_dFOV; /*!< \brief Field of view */	
	
	double m_dNearPlane; /*!< \brief The near clipping plane */
	double m_dFarPlane;  /*!< \brief The far clipping plane */
};

}

#endif /*CAMERA_H_*/
