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
 *  File:       Arcball.inl
 *  Created:    12.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef ARCBALL_INL_
#define ARCBALL_INL_

namespace BSMath
{

template<class T>
Quaternion<T> Arcball<T>::drag(const T x, const T y)
{		
	m_vD = mapToSphere(x, y);
	Vector vTemp = m_vC ^ m_vD;
	
	if(vTemp.length() > EPSILON)
		return Quat(vTemp.x, vTemp.y, vTemp.z, m_vC * m_vD);
			
	//EPSILON, to avoid a assert because of a magnitude of 0
	return Quat(EPSILON, EPSILON, EPSILON, EPSILON);
}

template<class T>
Vector3<T> Arcball<T>::mapToSphere(const T x, const T y)
{
	T lenSqr = x*x + y*y;
	
	//Is the point outside the sphere?
	if(lenSqr > 1.0)
	{
		lenSqr = 1.0 / sqrt(lenSqr);
		
		//Return the point on the identity-sphere (r = 1.0)
		return Vector3<T>(x * lenSqr, y * lenSqr, 0.0);	
	}
	
	//Return a point inside the identity-sphere
	return Vector3<T>(x, y, sqrt(1.0 - lenSqr));	
}

}

#endif /*ARCBALL_INL_*/
