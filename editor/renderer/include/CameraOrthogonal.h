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
 *  File:       CameraOrthogonal.h
 *  Created:    16.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef CAMERAORTHOGONAL_H_
#define CAMERAORTHOGONAL_H_

#include "RendererTypes.h"
#include "Camera.h"
#include <QPoint>

/*! \file
* 
*  \brief Describes the class CameraOrthogonal, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/

namespace BSRenderer
{

/*! \brief The orthogonal camera
*
* A orthogonal camera that can be used to show a scene for example in the 
* front/right view.
*/
class CameraOrthogonal: public Camera
{
public:
	/*!
	* \brief The constructor
	* 
	* Sets the viewport and set up a standard orthogonal camera 
	* 
	* \param nWidth The viewport width
	* \param nHeight The viewport height
	* \param type The type of the view
	*/	
	CameraOrthogonal(int nWidth, int nHeight, OrthogonalType type);
	
	/*!
	* \brief The destruktor
	*/		
	virtual ~CameraOrthogonal();
	
	/*!
	* \brief Set the type of the view
	* 
	* \param type The view type
	*/		
	void setOrthogonalType(OrthogonalType type)
	{
		m_orthoType = type;
		calcViewMatrix();	
	}
	
	/*!
	* \brief Return the type of the view
	* 
	* \return OrthogonalType The view type
	*/	
	OrthogonalType getOrthogonalType() const
	{
		return m_orthoType;	
	}

	/*!
	* \brief Return the limits of the orthogonal view
	* 
	* \param dLeft Reference where the left limit should be stored
	* \param dTop Reference where the top limit should be stored
	* \param dRight Reference where the right limit should be stored
	* \param dBottom Reference where the bottom limit should be stored
	*/		
	void getLimits(double& dLeft, double& dTop, double& dRight, double& dBottom) const;

	/*!
	* \brief Set the limits of the orthogonal view
	* 
	* \param dLeft Left limit
	* \param dTop Top limit
	* \param dRight Right limit
	* \param dBottom Bottom limit
	*/		
	void setLimits(double dLeft, double dTop, double dRight, double dBottom);
	
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
	* \brief Reset the camera to default
	* 
	* The camera will be reseted by restoring the variables to their default values
	*/		
	virtual void reset();

	/*!
	* \brief Returns all grid intersection points
	* 
	* \param p The array where the grid intersection points should be stored
	*/	
	void getGridIntersectionPoints(vector<QPoint>& p) const;
	
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
	* \brief Calls the camera, that the mouse wheel was rotated
	* 
	* \param nDelta The delta-value of the mouse wheel
	*/	
	virtual void mouseWheel(int nDelta);

private:
	/*!
	* \brief Calculates the view matrix
	* 
	* Calculates the matrix of the orthogonal type
	*/
	void calcViewMatrix();
	
	/*!
	* \brief Caluclates the projection matrix
	* 
	* Calculates a orthogonal matrix of viewport, clipping and limit informations
	*/		
	void calcProjMatrix();
	
private:
	OrthogonalType m_orthoType; /*!< \brief The type of the orthogonal view */

	double m_dLeft;   /*!< \brief The current left limit of the view */
	double m_dTop; 	  /*!< \brief The current top limit of the view */
	double m_dRight;  /*!< \brief The current right limit of the view */
	double m_dBottom; /*!< \brief The current bottom limit of the view */
	
	int m_nLastMousePosX; /*!< \brief The last x-coordinate of the mouse */
	int m_nLastMousePosY; /*!< \brief The last y-coordinate of the mouse */
};

}

#endif /*CAMERAORTHOGONAL_H_*/
