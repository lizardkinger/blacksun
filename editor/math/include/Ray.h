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
 *  File:       Ray.h
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef RAY_H_
#define RAY_H_

#include "Math.h"
#include "Vector3.h"
#include "Matrix4x4.h"

/*! \file
* 
*  \brief Describes the Template-Class Ray, their methodes and attributes.
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


/*! \brief A template-class that represents a ray
*
* It's a type independent ray. A ray is a combination of the origin
* and a direction.      
*/
template<class T>
class Ray
{	
public:
	/*!
	* \brief The default constructor
	* 
	* Sets up a ray at the origin with a direction of (1, 0, 0) 
	*/
	Ray()
	 : m_vDir(static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0))
	{
	}

	/*!
	* \brief The constructor
	* 
	* Sets up a ray with an orgin and a direction
	* 
	* \param vOrig The origin of the ray
	* \param vDir The direction of the ray
	*/	
	Ray(const Vector3<T>& vOrig, const Vector3<T>& vDir)
	 : m_vOrig(vOrig), m_vDir(vDir)
	{
	}
	
	/*!
	* \brief The copy-constructor
	* 
	* \param r The ray which should be copied
	*/	
	Ray(const Ray& r)
	 : m_vOrig(r.m_vOrig), m_vDir(r.m_vDir)
	{
	}
	
	/*!
	* \brief Sets up the ray with an orgin and a direction
	* 
	* \param vOrig The origin of the ray
	* \param vDir The direction of the ray
	*/		
	void set(const Vector3<T>& vOrig, const Vector3<T>& vDir);
	
	/*!
	* \brief Sets the origin of the ray
	* 
	* \param vOrig The origin of the ray
	*/		
	void setOrigin(const Vector3<T>& vOrig)
	{
		m_vOrig = vOrig;
	}
	
	/*!
	* \brief Returns the origin of the ray
	* 
	* \return Vector3<T> The origin of the ray
	*/			
	Vector3<T> getOrigin() const
	{
		return m_vOrig;
	}
	
	/*!
	* \brief Sets the direction of the ray
	* 
	* \param vDir The direction of the ray
	*/			
	void setDirection(const Vector3<T>& vDir)
	{
		m_vDir = vDir;
	}

	/*!
	* \brief Returns the direction of the ray
	* 
	* \return Vector3<T> The direction of the ray
	*/		
	Vector3<T> getDirection() const
	{
		return m_vDir;
	} 
	
	/*!
	* \brief Transforms the ray with a matrix 	
	* 
	* \param m The transformation matrix
	*/		
	void transform(const Matrix4x4<T>& m)
	{
		(*this) *= m;	
	}
	
	/*!
	* \brief Returns the transformed ray
	*  
	* \param m The transformation matrix
	* \return Ray<T> The transformed ray
	*/		
	Ray<T> getTransform(const Matrix4x4<T>& m) const
	{
		return (*this) * m;	
	}
	
	/*!
	* \brief Normalizes the ray 
	*
	* Normalizes the ray by normalizising the direction
	*/		
	void normalize()
	{
		m_vDir.normalize();	
	}
	
	/*!
	* \brief Returns the normalized ray 
	*
	* \return Ray<T> The normalized ray
	*/		
	Ray<T> getNormalize() const;
	
	/*!
	* \brief Flips the ray
	* 
	* The normal-vector is fliped 
	* 
	*/		
	void flip()
	{
		m_vDir *= static_cast<T>(-1.0);	
	}
	
	/*!
	* \brief Returns the fliped ray
	* 
	* \return Ray<T> The fliped ray 
	* 
	*/		
	Ray<T> getFlip()
	{
		return Ray<T>(m_vOrig, m_vDir * static_cast<T>(-1.0));	
	}
	
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator == (const Ray<T>& r) const
	{
		return ((m_vOrig==r.m_vOrig) && (m_vDir==r.m_vDir));
	}
	
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator != (const Ray<T>& r) const
	{
		return !((m_vOrig==r.m_vOrig) && (m_vDir==r.m_vDir));
	}
	
	/*! 
	* \brief Assign operator 
	*/	
	Ray<T>& operator = (const Ray<T>& r);
	
	/*!
	* \brief Assign operator for *= 
	*/		
	Ray<T>& operator *= (const Matrix4x4<T>& m);
	
	/*!
	* \brief Binary operator for multiplication
	*/	
	Ray<T> operator * (const Matrix4x4<T>& m) const;
	
private:
	Vector3<T> m_vOrig; /*!< \brief The origin of the ray*/ 
	Vector3<T> m_vDir;  /*!< \brief The direction of the ray*/ 	
};
	
}

#include "Ray.inl"

#endif /*RAY_H_*/
