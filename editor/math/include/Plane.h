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
 *  File:       Plane.h
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef PLANE_H_
#define PLANE_H_

#include "Math.h"
#include "Vector3.h"
#include "Matrix4x4.h"

/*! \file
* 
*  \brief Describes the Template-Class Plane, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSMath
{
	
/* Forward declaration */	
template<class T>
class Vector3;

template<class T>
class Matrix4x4;

/*!
* \brief This type classifies a point in relation to a plane
*/
enum PointPlaneClassify
{
	PPC_Back,   /*!< The point is behind the plane*/
	PPC_Planar, /*!< The point is on the plane*/
	PPC_Front   /*!< The point is before the plane*/
};	

/*! \brief A template-class that represents an plane
*
* It's a type independent plane. Its stored in normal form:\n
* vNormal * vPoint + distance = 0     
*/
template<class T>
class Plane
{	
public:
	/*!
	* \brief The default constructor
	* 
	* Sets up a plane at the origin with a normal-vector of (0, 1, 0) 
	*/
	Plane()
	 : m_vNormal(static_cast<T>(0.0), static_cast<T>(1.0), static_cast<T>(0.0)),
	   m_dist(static_cast<T>(0.0))
	{
	}

	/*!
	* \brief The constructor
	* 
	* Sets up the plane with in parameter-form: a*px + b*py + c*pz + d = 0 
	* 
	* \param a The parameter a of the parameter-form
	* \param b The parameter b of the parameter-form
	* \param c The parameter c of the parameter-form
	* \param d The parameter d (distance) of the parameter-form
	*/	
	Plane(const T& a, const T& b, const T& c, const T& d)
	 : m_vNormal(a, b, c), m_dist(d)
	{
	}
	
	/*!
	* \brief The constructor
	* 
	* Sets up the plane with a normal-vector and the distance 
	* 
	* \param vNormal The normal-vector
	* \param d The distance
	*/
	Plane(const Vector3<T>& vNormal, const T& d)
	 : m_vNormal(vNormal), m_dist(d)
	{
	}
	
	/*!
	* \brief The constructor
	* 
	* Span a plane with three points 
	* 
	* \param vPoint0 The first point of the plane
	* \param vPoint1 The second point of the plane
	* \param vPoint2 The third point of the plane
	*/
	Plane(const Vector3<T>& vPoint0, const Vector3<T>& vPoint1,
		  const Vector3<T>& vPoint2);
		  
	/*!
	* \brief The constructor
	* 
	* Sets up the plane with a normal-vector and a point on the plane 
	* 
	* \param vPoint The point on the plane
	* \param vNormal The normal-vector of the plane
	*/		  
	Plane(const Vector3<T>& vPoint, const Vector3<T>& vNormal);
	
	/*!
	* \brief The copy-constructor
	* 
	* \param p The plane which should be copied
	*/	
	Plane(const Plane& p)
	 : m_vNormal(p.m_vNormal), m_dist(p.m_dist)
	{
	}	
	
	/*!
	* \brief Sets up the plane with in parameter-form: a*px + b*py + c*pz + d = 0 
	* 
	* \param a The parameter a of the parameter-form
	* \param b The parameter b of the parameter-form
	* \param c The parameter c of the parameter-form
	* \param d The parameter d (distance) of the parameter-form
	*/		
	void set(const T& a, const T& b, const T& c, const T& d);
	
	/*!
	* \brief Sets up the plane with a normal-vector and the distance 
	* 
	* \param vNormal The normal-vector
	* \param d The distance
	*/	
	void set(const Vector3<T>& vNormal, const T& d);
	
	/*!
	* \brief Span a plane with three points 
	* 
	* \param vPoint0 The first point of the plane
	* \param vPoint1 The second point of the plane
	* \param vPoint2 The third point of the plane
	*/	
	void set(const Vector3<T>& vPoint0, const Vector3<T>& vPoint1,
			 const Vector3<T>& vPoint2);
			 
	/*!
	* \brief Sets up the plane with a normal-vector and a point on the plane 
	* 
	* \param vPoint The point on the plane
	* \param vNormal The normal-vector of the plane
	*/				 
	void set(const Vector3<T>& vPoint, const Vector3<T>& vNormal);
	
	/*!
	* \brief Normalizes the normal-vector of the plane
	* 
	* The normal-vector is normalized and the distance is recalculated
	*/		
	void normalize();
	
	/*!
	* \brief Returns the plane with the normalized normal-vector 
	* 
	* \return Plane<T> The normalized plane
	*/		
	Plane<T> getNormalize() const;
	
	/*!
	* \brief Flips the normal-vector of the plane
	* 
	* The normal-vector is fliped and the distance is recalculated 
	* 
	*/		
	void flip();
	
	/*!
	* \brief Returns the plane with the fliped normal-vector 
	* 
	* \return Plane<T> The fliped plane
	*/	
	Plane<T> getFlip() const
	{
		return Plane<T>(-m_vNormal, static_cast<T>(-1.0)*m_dist);	
	}
	
	/*!
	* \brief Calculates the distance of a point to this plane 
	* 
	* \param vPoint The point which should be used to calc the distance from
	* \return T The distance of the point to this plane
	*/	
	T distanceFromPoint(const Vector3<T>& vPoint) const;
	
	/*!
	* \brief Classifies a point in relation to this plane 
	* 
	* \param vPoint The point which should be classified
	* \return PointPlaneClassify The classify of the point
	*/	
	PointPlaneClassify classifyPoint(const Vector3<T>& vPoint) const;
	
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator == (const Plane<T>& p) const
	{
		return ((m_vNormal==p.m_vNormal) && (m_dist==p.m_dist));
	}

	/*! 
	* \brief Comparison operator 
	*/			
	bool operator != (const Plane<T>& p) const
	{
		return !((m_vNormal==p.m_vNormal) && (m_dist==p.m_dist));
	}
	
	/*! 
	* \brief Assign operator 
	*/		
	Plane<T>& operator = (const Plane<T>& p);
	
private:
	Vector3<T> m_vNormal; /*!< \brief The normal-vector*/ 
	T m_dist;             /*!< \brief The distance to the origin*/
};
	
}

#include "Plane.inl"

#endif /*PLANE_H_*/
