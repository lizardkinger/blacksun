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
 *  File:       Plane.inl
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef PLANE_INL_
#define PLANE_INL_

namespace BSMath
{

template<class T>
Plane<T>::Plane(const Vector3<T>& vPoint0, const Vector3<T>& vPoint1,
	  const Vector3<T>& vPoint2)
{
	set(vPoint0, (vPoint2 - vPoint1) ^ (vPoint0 - vPoint1));
}

template<class T>
Plane<T>::Plane(const Vector3<T>& vPoint, const Vector3<T>& vNormal)
 : m_vNormal(vNormal)
{
	m_dist = (-vNormal)*vPoint;
}

template<class T>
void Plane<T>::set(const T& a, const T& b, const T& c, const T& d)
{
	m_vNormal.set(a, b, c);
	m_dist = d;
}

template<class T>
void Plane<T>::set(const Vector3<T>& vNormal, const T& d)
{
	m_vNormal = vNormal;
	m_dist = d;
}

template<class T>
void Plane<T>::set(const Vector3<T>& vPoint0, const Vector3<T>& vPoint1,
		 const Vector3<T>& vPoint2)
{
	set(vPoint0, (vPoint2 - vPoint1) ^ (vPoint0 - vPoint1));		
}

template<class T>
void Plane<T>::set(const Vector3<T>& vPoint, const Vector3<T>& vNormal)
{
	m_vNormal = vNormal;
	m_dist = (-vNormal)*vPoint;
}

template<class T>
void Plane<T>::normalize()
{
	T len = m_vNormal.length();
	assert(len != 0.0);
	len = static_cast<T>(1.0)/len;
	
	m_vNormal *= len;
	m_dist *= len;
}

template<class T>
Plane<T> Plane<T>::getNormalize() const
{
	Plane<T> res = (*this);
	res.normalize();
	return res;
}

template<class T>
void Plane<T>::flip()
{
	m_vNormal = -m_vNormal;
	m_dist = static_cast<T>(-1.0)*m_dist;	
}

template<class T>
T Plane<T>::distanceFromPoint(const Vector3<T>& vPoint) const
{
	return std::abs((m_vNormal * vPoint) - m_dist);
}

template<class T>
PointPlaneClassify Plane<T>::classifyPoint(const Vector3<T>& vPoint) const
{
	T f = (vPoint * m_vNormal) - m_dist;

	if(f > EPSILON)
	{
		return PPC_Front;
	}
	else if(f < -EPSILON)
	{
		return PPC_Back;
	}

	return PPC_Planar;
}

template<class T>
Plane<T>& Plane<T>::operator = (const Plane<T>& p)
{
	m_vNormal = p.m_vNormal;
	m_dist = p.m_dist;
	
	return *this;
}
	
}

#endif /*PLANE_INL_*/
