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
 *  File:       Vector3.h
 *  Created:    19.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef VECTOR3_INL_
#define VECTOR3_INL_

namespace BSMath
{

template<class T>
void Vector3<T>::set(const T& _x, const T& _y, const T& _z)
{
	x = _x;
	y = _y;
	z = _z;	
}

template<class T>
void Vector3<T>::negate()
{
	x *= -1;
	y *= -1;
	z *= -1;
}

template<class T>
void Vector3<T>::normalize()
{
	T len = length();
	assert(len!=static_cast<T>(0.0));
	len = static_cast<T>(1.0) / len;
	x *= len;
	y *= len;
	z *= len;
}

template<class T>
Vector3<T>& Vector3<T>::operator =(const Vector3<T>& v)
{
	if(this == &v)
		return (*this);
		
	x = v.x;
	y = v.y;
	z = v.z; 
	return *this;		
}

template<class T>
Vector3<T>& Vector3<T>::operator +=(const Vector3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z; 
	return *this;		
}

template<class T>
Vector3<T>& Vector3<T>::operator -=(const Vector3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z; 
	return *this;		
}

template<class T>
Vector3<T>& Vector3<T>::operator *=(const Vector3<T>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z; 
	return *this;		
}

template<class T>
Vector3<T>& Vector3<T>::operator *=(const T& v)
{
	x *= v;
	y *= v;
	z *= v; 
	return *this;		
}
	
template<class T>
Vector3<T>& Vector3<T>::operator /=(const Vector3<T>& v)
{
	assert((v.x!=static_cast<T>(0.0)) && 
		(v.y!=static_cast<T>(0.0)) && 
		(v.z!=static_cast<T>(0.0)));
	x /= v.x;
	y /= v.y;
	z /= v.z; 
	return *this;		
}

template<class T>
Vector3<T>& Vector3<T>::operator /=(const T& v)
{
	assert(v!=static_cast<T>(0.0));
	T temp = static_cast<T>(1.0)/v;
	x *= temp;
	y *= temp;
	z *= temp; 
	return *this;		
}	

template<class T>
Vector3<T> Vector3<T>::min(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(v1.x < v2.x ? v1.x : v2.x,
	 				  v1.y < v2.y ? v1.y : v2.y,
	  				  v1.z < v2.z ? v1.z : v2.z); 
}

template<class T>
Vector3<T> Vector3<T>::max(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(v1.x > v2.x ? v1.x : v2.x,
	 				  v1.y > v2.y ? v1.y : v2.y,
	  				  v1.z > v2.z ? v1.z : v2.z); 
}
	
template<class T> 	
Vector3<T> Vector3<T>::random(T length)
{
	Vector3<T> res(rand() / static_cast<T>(RAND_MAX),
				   rand() / static_cast<T>(RAND_MAX),
				   rand() / static_cast<T>(RAND_MAX));
	
	res *= (length / res.length());
			
	return res;
}
	
}

#endif /*VECTOR3_INL_*/
