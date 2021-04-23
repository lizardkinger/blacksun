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
 *  File:       AABB.h
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef AABB_H_
#define AABB_H_

#include "Math.h"
#include "Vector3.h"
#include "Plane.h"

#include <vector>
using namespace std;

/*! \file
* 
*  \brief Describes the Template-Class AABB, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSMath
{
	
/* Forward declaration */	
template<class T>
class Vector3;

template<class T>
class Plane;

template<class T>
class Matrix4x4;

/*! \brief A template-class that represents an axis aligned bounding box
*
* It's a type independent axis aligned bounding box (AABB). Its a box, that
* represents a box spanned around a object, where this object is completly 
* inside. The box's limits are calculated by the minimum and maximum coordinate
* of the object, so that the box can be descripted by only two limit-positions.
*/	
template<class T>
class AABB
{	
public:
	/*!
	* \brief The default constructor
	* 
	* The bounding box will be initialized with very high limits
	*/	
	AABB()
	 : m_vMin(static_cast<T>(-999999.0), static_cast<T>(-999999.0), static_cast<T>(-999999.0)),
	   m_vMax(static_cast<T>( 999999.0), static_cast<T>( 999999.0), static_cast<T>( 999999.0))
	{	
	}

	/*!
	* \brief The constructor
	* 
	* The bounding box will be initialized with specific limits.
	* 
	* \param vMin The minimum limit
	* \param vMax The maximum limit
	*/	
	AABB(const Vector3<T>& vMin, const Vector3<T>& vMax)
	 : m_vMin(vMin), m_vMax(vMax)
	{
	}
	
	/*!
	* \brief The copy-constructor
	* 
	* \param aabb The AABB which should be copied
	*/
	AABB(const AABB<T>& aabb)
	 : m_vMin(aabb.m_vMin), m_vMax(aabb.m_vMax)
	{
	}
	
	/*!
	* \brief Set up the limits of the bounding box
	*
	* \param vMin The minimum limit
	* \param vMax The maximum limit
	*/		
	void set(const Vector3<T>& vMin, const Vector3<T>& vMax);
	
	/*!
	* \brief Set up the minimum limit of the bounding box
	*
	* \param vMin The minimum limit
	*/		
	void setMinimum(const Vector3<T>& vMin)
	{
		m_vMin = vMin;
	}
	
	/*!
	* \brief Returns the minimum limit of the bounding box
	*
	* \return Vector3<T> The minimum limit
	*/			
	Vector3<T> getMinimum() const
	{
		return m_vMin;
	}
	
	/*!
	* \brief Set up the maximum limit of the bounding box
	*
	* \param vMax The maximum limit
	*/		
	void setMaximum(const Vector3<T>& vMax)
	{
		m_vMax = vMax;
	}
	
	/*!
	* \brief Returns the maximum limit of the bounding box
	*
	* \return Vector3<T> The maximum limit
	*/			
	Vector3<T> getMaximum() const
	{
		return m_vMax;
	}
	
	/*!
	* \brief Set up the centre of the bounding box
	* 
	* Set up the centre of the bounding box, the box will be moved.
	*
	* \param vCentre The centre of the box
	*/			
	void setCentre(const Vector3<T>& vCentre);
	
	/*!
	* \brief Returns the centre of the bounding box
	*
	* \return Vector3<T> The centre of the box
	*/			
	Vector3<T> getCentre() const
	{
		return (m_vMax - m_vMin) * 0.5 + m_vMin;
	}
	
	/*!
	* \brief Returns the dimension of the bounding box
	*
	* \return Vector3<T> The dimension of the box
	*/			
	Vector3<T> getDimension() const
	{
		return m_vMax - m_vMin;	
	}
	
	/*!
	* \brief Inserts all six planes of the box into an array 
	*
	* Inserts all six planes of the box into an array in the order:\n
	* Right, Left, Top, Bottom, Front, Back
	* 
	* \param arr Reference to an Plane-Array
	*/
	void getPlanes(vector< Plane<T> >& arr) const;
	
	/*!
	* \brief Transforms the box with a matrix 
	*
	* All edges of the box will be transformed and the limits are
	* recalculated.
	* 
	* \param m The transformation matrix
	* \param bIgnoreTranslation If true, the translation of the matrix is ignored,
	*   the box will not be moved.
	*/	
	void transform(const Matrix4x4<T>& m, bool bIgnoreTranslation = false);

	/*!
	* \brief Returns the transformed box 
	*
	* All edges of the box will be transformed and the limits are
	* recalculated.
	* 
	* \param m The transformation matrix
	* \param bIgnoreTranslation If true, the translation of the matrix is ignored,
	*   the box will not be moved.
	* \return AABB<T> The transformed box
	*/	
	AABB<T> getTransform(const Matrix4x4<T>& m, bool bIgnoreTranslation = false) const;
	
	/*!
	* \brief Merges two axis aligned bounding boxes 
	*
	* The limits of the two boxes are calculated and will set up the new box
	* 
	* \param aabb The bounding box to merge with
	*/		
	void merge(const AABB<T>& aabb);

	/*!
	* \brief Merges this and an other bounding box and returns the result  
	*
	* The limits of the two boxes are calculated and will set up the new box
	* 
	* \param aabb The bounding box to merge with+
	* \return The merged bounding box
	*/		
	AABB<T> getMerge(const AABB<T>& aabb) const;
	
	/*! 
	* \brief Comparison operator 
	*/	
	bool operator == (const AABB<T>& a) const
	{
		return ((m_vMin==a.m_vMin) && (m_vMax==a.m_vMax));
	}
		
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator != (const AABB<T>& a) const
	{
		return !((m_vMin==a.m_vMin) && (m_vMax==a.m_vMax));
	}
	
	/*!
	* \brief Assign operator 
	*/		
	AABB<T>& operator = (const AABB<T>& a);	
	
private:
	Vector3<T> m_vMin; /*!< \brief The minimum of the box */
	Vector3<T> m_vMax; /*!< \brief The maximum of the box */
};
	
typedef AABB<double> Aabb;		
	
}

#include "AABB.inl"

#endif /*AABB_H_*/
