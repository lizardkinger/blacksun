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
 *  File:       AABB.inl
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef AABB_INL_
#define AABB_INL_

namespace BSMath
{

template<class T>
void AABB<T>::set(const Vector3<T>& vMin, const Vector3<T>& vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;	
}

template<class T>
void AABB<T>::setCentre(const Vector3<T>& vCentre)
{
	Vector3<T> vDelta = vCentre - getCentre();
	m_vMax += vDelta;
	m_vMin += vDelta;
}

template<class T>
void AABB<T>::getPlanes(vector< Plane<T> >& arr) const
{
	arr.push_back(Plane<T>(m_vMax, Vector3<T>( 1.0,  0.0,  0.0))); //Right
	arr.push_back(Plane<T>(m_vMin, Vector3<T>(-1.0,  0.0,  0.0))); //Left
	arr.push_back(Plane<T>(m_vMax, Vector3<T>( 0.0,  1.0,  0.0))); //Top
	arr.push_back(Plane<T>(m_vMin, Vector3<T>( 0.0, -1.0,  0.0))); //Bottom				
	arr.push_back(Plane<T>(m_vMax, Vector3<T>( 0.0,  0.0,  1.0))); //Front
	arr.push_back(Plane<T>(m_vMin, Vector3<T>( 0.0,  0.0, -1.0))); //Back
}

template<class T>
void AABB<T>::transform(const Matrix4x4<T>& m, bool bIgnoreTranslation)
{
	//Transform the 8 edges of the box
	Vector3<T> edges[8];

	//Front, bottom, left
	edges[0] = Vector3<T>(m_vMin.x, m_vMin.y, m_vMax.z) * m;
	//Front, bottom, right
	edges[1] = Vector3<T>(m_vMax.x, m_vMin.y, m_vMax.z) * m;
	//Front, top, left
	edges[2] = Vector3<T>(m_vMin.x, m_vMax.y, m_vMax.z) * m;
	//Front, top, right
	edges[3] = m_vMax * m;
	//Back, bottom, left
	edges[4] = m_vMin * m;
	//Back, bottom, right
	edges[5] = Vector3<T>(m_vMax.x, m_vMin.y, m_vMin.z) * m;
	//Back, top, left
	edges[6] = Vector3<T>(m_vMin.x, m_vMax.y, m_vMin.z) * m;
	//Back, top, right
	edges[7] = Vector3<T>(m_vMax.x, m_vMax.y, m_vMin.z) * m;
	
	//Get the new minimum edge of the transformed Box
	m_vMin = Vector3<T>::min(m_vMin, edges[0]);
	m_vMin = Vector3<T>::min(m_vMin, edges[1]);
	m_vMin = Vector3<T>::min(m_vMin, edges[2]);
	m_vMin = Vector3<T>::min(m_vMin, edges[3]);
	m_vMin = Vector3<T>::min(m_vMin, edges[4]);
	m_vMin = Vector3<T>::min(m_vMin, edges[5]);
	m_vMin = Vector3<T>::min(m_vMin, edges[6]);
	m_vMin = Vector3<T>::min(m_vMin, edges[7]);
	
	//Get the new maximum edge of the transformed Box
	m_vMax = Vector3<T>::max(m_vMax, edges[0]);
	m_vMax = Vector3<T>::max(m_vMax, edges[1]);
	m_vMax = Vector3<T>::max(m_vMax, edges[2]);
	m_vMax = Vector3<T>::max(m_vMax, edges[3]);
	m_vMax = Vector3<T>::max(m_vMax, edges[4]);
	m_vMax = Vector3<T>::max(m_vMax, edges[5]);
	m_vMax = Vector3<T>::max(m_vMax, edges[6]);
	m_vMax = Vector3<T>::max(m_vMax, edges[7]);	
	
	if(bIgnoreTranslation==false)
	{
		m_vMax += m.getTranslation();
		m_vMin += m.getTranslation();	
	}					
}

template<class T>
AABB<T> AABB<T>::getTransform(const Matrix4x4<T>& m, bool bIgnoreTranslation) const
{
	AABB<T> res = (*this);
	res.transform(m, bIgnoreTranslation);
	return res;
}

template<class T>
void AABB<T>::merge(const AABB<T>& aabb)
{
	m_vMin = Vector3<T>::min(m_vMin, aabb.m_vMin);
	m_vMax = Vector3<T>::max(m_vMax, aabb.m_vMax);
}

template<class T>
AABB<T> AABB<T>::getMerge(const AABB<T>& aabb) const
{
	return AABB<T>(Vector3<T>::min(m_vMin, aabb.m_vMin),
				   Vector3<T>::max(m_vMax, aabb.m_vMax));		
}

template<class T>
AABB<T>& AABB<T>::operator =(const AABB<T>& a)
{
	m_vMin = a.m_vMin;
	m_vMax = a.m_vMax;
	
	return *this;
}	
	
}

#endif /*AABB_INL_*/
