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
 *  File:       Quaternion.inl
 *  Created:    30.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef QUATERNION_INL_
#define QUATERNION_INL_

namespace BSMath
{
	
template<class T>
void Quaternion<T>::set(const T& _x, const T& _y, const T& _z, const T& _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;	
}	

template<class T>
void Quaternion<T>::computeW()
{
	T temp = 1.0 - (x * x) - (y * y) - (z * z);
	
	if(temp < 0.0)
	{
		w = 0.0;
	}
	else
	{
		w = -sqrt(temp);
	}
}

template<class T>
void Quaternion<T>::normalize()
{
	T len = magnitude();
	
	assert(len != static_cast<T>(0.0));
	
	(*this) *= (1.0/len);		
}

template<class T>
Quaternion<T> Quaternion<T>::getNormalize() const
{
	Quaternion<T> temp = (*this);
	
	temp.normalize();
	
	return temp;	
}	

template<class T>
void Quaternion<T>::conjugate()
{
	x *= (-1.0);
	y *= (-1.0);
	z *= (-1.0);
}	

template<class T>
void Quaternion<T>::invert()
{
	conjugate();
	T mag = magnitudeSqr();
	assert(mag != static_cast<T>(0.0));
	
	(*this) /= mag;	
}

template<class T>
Quaternion<T> Quaternion<T>::getInvert() const
{
	Quaternion<T> temp = (*this);
	
	temp.invert();
	
	return temp;
}

template<class T>	
Vector3<T> Quaternion<T>::rotateVector(const Vector3<T>& v) const
{
	Quaternion<T> r = (*this)*v*(~(*this));
	return ZFXVector(r.x, r.y, r.z); 	
}

template<class T>
Matrix4x4<T> Quaternion<T>::toMatrix() const
{
	Matrix4x4<T> res;

	T n = magnitudeSqr();
	T s = (n > 0.0) ? (2.0 / n) : 0.0;

	T xs = x * s;
	T ys = y * s;
	T zs = z * s;
	
	T wx = w * xs;
	T wy = w * ys;
	T wz = w * zs;
	
	T xx = x * xs;
	T xy = x * ys;
	T xz = x * zs;
	
	T yy = y * ys;
	T yz = y * zs;
	T zz = z * zs;

	res._11 = 1.0f - (yy + zz);
	res._12 = xy - wz;
	res._13 =  xz + wy;
	
	res._21 = xy + wz;
	res._22 = 1.0f - (xx + zz);
	res._23 =  yz - wx;
	
	res._31 = xz - wy;
	res._32 = yz + wx;
	res._33 = 1.0f - (xx + yy);

	return res;	
}

template<class T>
Vector3<T> Quaternion<T>::getEulerAngles() const
{
	return Vector3<T>(atan2(2.0*y*w - 2.0*x*z , 1.0 - 2.0*y*y - 2.0*z*z), 
	asin(2.0*x*y + 2.0*z*w),
	atan2(2.0*x*w - 2.0*y*z , 1.0 - 2.0*x*x - 2.0*z*z));	
}

template<class T>
void Quaternion<T>::getEulerAngles(T& pitch, T& yaw, T& roll) const
{
	pitch = atan2(2.0*y*w - 2.0*x*z , 1.0 - 2.0*y*y - 2.0*z*z);
	yaw   =  asin(2.0*x*y + 2.0*z*w);
	roll  = atan2(2.0*x*w - 2.0*y*z , 1.0 - 2.0*x*x - 2.0*z*z);
}

template<class T>
Quaternion<T>& Quaternion<T>::operator =(const Quaternion<T>& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	
	return (*this);
}
		
template<class T>
Quaternion<T>& Quaternion<T>::operator +=(const Quaternion<T>& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	
	return (*this);
}
		
template<class T>
Quaternion<T>& Quaternion<T>::operator -=(const Quaternion<T>& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	
	return (*this);		
}
		
template<class T>
Quaternion<T>& Quaternion<T>::operator *=(const Quaternion<T>& q)
{
	(*this) = (*this) * q;
	return (*this);	
}
		
template<class T>
Quaternion<T>& Quaternion<T>::operator *=(const T& s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	
	return (*this);		
}
		
template<class T>
Quaternion<T>& Quaternion<T>::operator /=(const T& s)
{
	assert(s != static_cast<T>(0.0));
	T mul = 1.0 / s;
	
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	
	return (*this);		
}	

template<class T>
Quaternion<T> Quaternion<T>::operator *(const Vector3<T>& v) const
{
	return Quaternion<T>(w * v.x + y * v.z - z * v.y,
                		 w * v.y + z * v.x - x * v.z,
                		 w * v.z + x * v.y - y * v.x,
              			-x * v.x - y * v.y - z * v.z);
}

template<class T>
Quaternion<T> Quaternion<T>::operator *(const Quaternion<T>& q) const
{
	return Quaternion<T>(w * q.x + x * q.w + y * q.z - z * q.y,
						 w * q.y + y * q.w + z * q.x - x * q.z,
						 w * q.z + z * q.w + x * q.y - y * q.x,
						 w * q.w - x * q.x - y * q.y - z * q.z);
}
 
template<class T>
Quaternion<T> Quaternion<T>::operator /(const T& s) const	
{
	assert(s != static_cast<T>(0.0));
	T mul = 1.0/s;
	
	return Quaternion<T>(x*mul, y*mul, z*mul, w*mul);		
}


template<class T>	
Quaternion<T> Quaternion<T>::makeFromVector(const Vector3<T>& v)
{
	Quaternion<T> res;
	
	T temp = 1.0 - v.lengthSqr();
	T quatW = 0.0f;
	if(temp >= 0.0)
		quatW = std::sqrt(temp);

	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	res.w = -quatW;
	
	res.normalize();		
	
	return res;	
}	

template<class T>	
Quaternion<T> Quaternion<T>::makeFromMatrix(const Matrix4x4<T>& m)
{
	Quaternion<T> res;
	
	T mul = 0.0;
    T diagonal = 1.0 + m._11 + m._22 + m._33;
    
	if(diagonal>EPSILON) 
	{
		mul = 1.0 / (sqrt(diagonal) * 2.0);
		res.x = (m._23 - m._32) * mul;
		res.y = (m._31 - m._13) * mul;
		res.z = (m._12 - m._21) * mul;
		res.w = 0.25 / mul;
	}        
	else
	{
		if((m._11 > m._22) && (m._11 > m._33))
		{
			mul  = 1.0 / (sqrt(1.0 + m._11 - m._22 - m._33) * 2.0);
			res.x = 0.25 / mul;
			res.y = (m._12 + m._21) * mul;
			res.z = (m._31 + m._13) * mul;
			res.w = (m._23 - m._32) * mul;
		} 
		else if(m._22 > m._33)
		{
			mul  = 1.0 / (sqrt(1.0 + m._22 - m._11 - m._33) * 2.0);
			res.x = (m._12 + m._21) * mul;
			res.y = 0.25 / mul;
			res.z = (m._23 + m._32) * mul;
			res.w = (m._31 - m._13) * mul;
		} 
		else
		{
			mul  = 1.0 / (sqrt(1.0 + m._33 - m._11 - m._22) * 2.0);
			res.x = (m._31 + m._13) * mul;
			res.y = (m._23 + m._32) * mul;
			res.z = 0.25 / mul;
			res.w = (m._12 - m._21) * mul;
		}
	}
			
	return res;	
}		
	
template<class T>	
Quaternion<T> Quaternion<T>::makeFromEuler(const T& pitch, const T& yaw, const T& roll)
{	
	T cYaw	 = cos(yaw 	 * 0.5);
	T sYaw	 = sin(yaw 	 * 0.5);
	T cPitch = cos(pitch * 0.5);
	T sPitch = sin(pitch * 0.5);
	T cRoll	 = cos(roll	 * 0.5);
	T sRoll	 = sin(roll	 * 0.5);

	return Quaternion<T>(cRoll * sPitch * cYaw + sRoll * cPitch * sYaw,
				cRoll * cPitch * sYaw - sRoll * sPitch * cYaw,
				sRoll * cPitch * cYaw - cRoll * sPitch * sYaw,
				cRoll * cPitch * cYaw + sRoll * sPitch * sYaw);
}

template<class T>	
Quaternion<T> Quaternion<T>::makeFromAxisAngle(const Vector3<T>& axis, const T& angle)
{	
	T temp = sin(angle / 2.0);
	
	return Quaternion<T>(axis.x * temp, axis.y * temp, axis.z * temp, cos(angle / 2.0));	
}

template<class T>	
Quaternion<T> Quaternion<T>::slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, 
	const T& f)
{
	if(q1 == q2) 
		return q1;

	Quaternion<T> quat1 = q1;
	Quaternion<T> quat2 = q2;
	
	T temp = (quat1.x * quat2.x) + (quat1.y * quat2.y) + (quat1.z * quat2.z) + (quat1.w * quat2.w);

	if(temp < 0.0)
	{
		quat2 = -quat2;
		temp *= (-1.0);
	}

	T scale0 = 1 - f;
	T scale1 = f;

	if((1 - temp) > 0.1)
	{
		T theta = acos(temp);
		T sinTheta = sin(theta);

		scale0 = sin((1.0 - f ) * theta) / sinTheta;
		scale1 = sin((f * theta) ) / sinTheta;
	}	

	return Quaternion<T>((scale0 * quat1.x) + (scale1 * quat2.x),
						 (scale0 * quat1.y) + (scale1 * quat2.y),
						 (scale0 * quat1.z) + (scale1 * quat2.z),
						 (scale0 * quat1.w) + (scale1 * quat2.w));		
}

template<class T>	
Quaternion<T> Quaternion<T>::random()
{
	T x0 = std::rand() / static_cast<T>(RAND_MAX);
	T r1 = sqrt(1.0 - x0);
	T r2 = sqrt(x0);
	T t1 = PI * 2.0 * (std::rand() / static_cast<T>(RAND_MAX));
	T t2 = PI * 2.0 * (std::rand() / static_cast<T>(RAND_MAX));
	T c1 = cos(t1);
	T s1 = sin(t1);
	T c2 = cos(t2);
	T s2 = sin(t2);
	
	return Quaternion<T>(s1 * r1, c1 * r1, s2 * r2, c2 * r2);
}
	
}

#endif /*QUATERNION_INL_*/
