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
 *  Module:     Math (BlackSun)
 *  File:       Arcball.h
 *  Created:    12.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef ARCBALL_H_
#define ARCBALL_H_

#include "Math.h"
#include "Vector3.h"
#include "Quaternion.h"


/*! \file
* 
*  \brief Describes the Template-Class Arcball, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSMath
{
	
/* Forward declaration */	
template<class T>
class Vector3;

template<class T>
class Quaternion;

/*! \brief A template-class that can be used for arcball rotation
*
* It's a type independent arcball rotation, which converts mouse interactions
* into a rotation-quaternion.
*/	
template<class T>
class Arcball
{
public:
	/*!
	* \brief The default constructor
	*/	
	Arcball() {};
	
	/*!
	* \brief Tells the arcball, that the mouse was clicked
	* 
	* This methode must be call, if the mouse was clicked. For example
	* with the methode mousePressEvent in Qt.
	* 
	* \param x The current x-position of the mouse cursor (relativ position)
	* \param y The current y-position of the mouse cursor (relativ position)
	*/	
	void click(const T x, const T y)
	{
		m_vC = mapToSphere(x, y);
	}

	/*!
	* \brief Tells the arcball, that the mouse was moved/dragged
	* 
	* This methode must be call, if the mouse was moved/dragged. For example
	* with the methode mouseMoveEvent in Qt.
	* 
	* \param x The current x-position of the mouse cursor (relativ position)
	* \param y The current y-position of the mouse cursor (relativ position)
	* \return Quaternion<T> The quaternion which describes the rotation
	*/		
	Quaternion<T> drag(const T x, const T y);
	
private:

	/*!
	* \brief Maps a position into a identiy sphere
	* 
	* \param x The x-position of the position
	* \param y The y-position of the position
	* \return Vector3<T> The mapped position
	*/	
	Vector3<T> mapToSphere(const T x, const T y);
	
private:
	Vector3<T> m_vC; /*!< \brief The click vector */
	Vector3<T> m_vD; /*!< \brief The drag vector */
};
		
}

#include "Arcball.inl"

#endif /*AABB_H_*/
