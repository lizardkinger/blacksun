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
 *  File:       Matrix4x4.inl
 *  Created:    20.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATRIX4X4_INL_
#define MATRIX4X4_INL_

namespace BSMath
{
	
template<class T>
Matrix4x4<T>::Matrix4x4(const T& a)
 : _11(a), _12(a), _13(a), _14(a), _21(a), _22(a), _23(a), _24(a),
   _31(a), _32(a), _33(a), _34(a), _41(a), _42(a), _43(a), _44(a)
{
}

template<class T>
Matrix4x4<T>::Matrix4x4(const T& a11, const T& a12, const T& a13, const T& a14, 
		  const T& a21, const T& a22, const T& a23, const T& a24, 
		  const T& a31, const T& a32, const T& a33, const T& a34,
		  const T& a41, const T& a42, const T& a43, const T& a44)
 : _11(a11), _12(a12), _13(a13), _14(a14), _21(a21), _22(a22), _23(a23), _24(a24),
   _31(a31), _32(a32), _33(a33), _34(a34), _41(a41), _42(a42), _43(a43), _44(a44)
{	  	
}

template<class T>
Matrix4x4<T>::Matrix4x4(const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3)
 : _11(v1.x), _12(v1.y), _13(v1.z), _14(0.0),
   _21(v2.x), _22(v2.y), _23(v2.z), _24(0.0),
   _31(v3.x), _32(v3.y), _33(v3.z), _34(0.0),
   _41(0.0) , _42(0.0) , _43(0.0) , _44(1.0)
{
}

template<class T>
Matrix4x4<T>::Matrix4x4(const T* mat)
{
	assert(mat != NULL);
	memcpy( mat, val2, sizeof(Matrix4x4<T>));
}

template<class T>
Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& mat)
 : _11(mat._11), _12(mat._12), _13(mat._13), _14(mat._14), 
   _21(mat._21), _22(mat._22), _23(mat._23), _24(mat._24),
   _31(mat._31), _32(mat._32), _33(mat._33), _34(mat._34),
   _41(mat._41), _42(mat._42), _43(mat._43), _44(mat._44)
{
}	

template<class T>
void Matrix4x4<T>::set(const T& a11, const T& a12, const T& a13, const T& a14, 
	 const T& a21, const T& a22, const T& a23, const T& a24, 
	 const T& a31, const T& a32, const T& a33, const T& a34,
	 const T& a41, const T& a42, const T& a43, const T& a44)
{
	_11 = a11; _12 = a12; _13 = a13; _14 = a14;
	_21 = a21; _22 = a22; _23 = a23; _24 = a24;
	_31 = a31; _32 = a32; _33 = a33; _34 = a34;
	_41 = a41; _42 = a42; _43 = a43; _44 = a44;	 	
}

template<class T>
void Matrix4x4<T>::set(const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3)
{
   _11 = v1.x;   _12 = v1.y;   _13 = v1.z;
   _21 = v2.x;   _22 = v2.y;   _23 = v2.z;
   _31 = v3.x;   _32 = v3.y;   _33 = v3.z;
   
   _41 = _42 = _43 = _14 = _24 = _34 = static_cast<T>(0.0);
   _44 = static_cast<T>(1.0);
   
}

template<class T>
void Matrix4x4<T>::zero()
{
	_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = 
	_31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = static_cast<T>(0.0);	
}

template<class T>
void Matrix4x4<T>::identity()
{
	zero();
	_11 = _22 = _33 = _44 = static_cast<T>(1.0);
}

template<class T>
void Matrix4x4<T>::normalize()
{
	T maxVal = -99999999.0;
	for(int i=0; i<16; i++)
		if(val2[i]>maxVal)
			maxVal = val2[i];
	
	(*this) /= maxVal;
}

template<class T>
T Matrix4x4<T>::determinant() const
{
	return _14 * _23 * _32 * _41 - _13 * _24 * _32 * _41 - _14 * _22 * _33 * _41 + _12 * _24 * _33 * _41 +
		   _13 * _22 * _34 * _41 - _12 * _23 * _34 * _41 - _14 * _23 * _31 * _42 + _13 * _24 * _31 * _42 +
		   _14 * _21 * _33 * _42 - _11 * _24 * _33 * _42 - _13 * _21 * _34 * _42 + _11 * _23 * _34 * _42 +
		   _14 * _22 * _31 * _43 - _12 * _24 * _31 * _43 - _14 * _21 * _32 * _43 + _11 * _24 * _32 * _43 +
		   _12 * _21 * _34 * _43 - _11 * _22 * _34 * _43 - _13 * _22 * _31 * _44 + _12 * _23 * _31 * _44 +
		   _13 * _21 * _32 * _44 - _11 * _23 * _32 * _44 - _12 * _21 * _33 * _44 + _11 * _22 * _33 * _44;	
}

template<class T>
void Matrix4x4<T>::absolute()
{
	_11 = std::abs(_11); _12 = std::abs(_12);  _13 = std::abs(_13);  _14 = std::abs(_14);
	_21 = std::abs(_21); _22 = std::abs(_22);  _23 = std::abs(_23);  _24 = std::abs(_24);
	_31 = std::abs(_31); _32 = std::abs(_32);  _33 = std::abs(_33);  _34 = std::abs(_34);
	_41 = std::abs(_41); _42 = std::abs(_42);  _43 = std::abs(_43);  _44 = std::abs(_44);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getAbsolute() const
{
	return Matrix4x4<T>(std::abs(_11), std::abs(_12), std::abs(_13), std::abs(_14),
						std::abs(_21), std::abs(_22), std::abs(_23), std::abs(_24),
						std::abs(_31), std::abs(_32), std::abs(_33), std::abs(_34),
						std::abs(_41), std::abs(_42), std::abs(_43), std::abs(_44));
}

template<class T>
void Matrix4x4<T>::translation(const T& x, const T& y, const T& z)
{
	_14 = x;
	_24 = y;
	_34 = z;	
}

template<class T>
void Matrix4x4<T>::translation(const Vector3<T>& v)
{
	_14 = v.x;
	_24 = v.y;
	_34 = v.z;
}

template<class T>
void Matrix4x4<T>::transpose()
{
	Matrix4x4<T> temp = (*this);

	val[0][0] = temp.val[0][0];
	val[0][1] = temp.val[1][0];
	val[0][2] = temp.val[2][0];
	val[0][3] = temp.val[3][0];
	val[1][0] = temp.val[0][1];
	val[1][1] = temp.val[1][1];
	val[1][2] = temp.val[2][1];
	val[1][3] = temp.val[3][1];
	val[2][0] = temp.val[0][2];
	val[2][1] = temp.val[1][2];
	val[2][2] = temp.val[2][2];
	val[2][3] = temp.val[3][2];
	val[3][0] = temp.val[0][3];
	val[3][1] = temp.val[1][3];
	val[3][2] = temp.val[2][3];
	val[3][3] = temp.val[3][3];
}

template<class T>
void Matrix4x4<T>::inverse()
{
	Matrix4x4<T> m = (*this);
	
	T det = determinant();
	assert(det != static_cast<T>(0.0)); //Det = 0 -> Matrix can't be inversed
	
	_11 = m._23 * m._34 * m._42 - m._24 * m._33 * m._42 + m._24 * m._32 * m._43 - 
		  m._22 * m._34 * m._43 - m._23 * m._32 * m._44 + m._22 * m._33 * m._44;
	_12 = m._14 * m._33 * m._42 - m._13 * m._34 * m._42 - m._14 * m._32 * m._43 + 
		  m._12 * m._34 * m._43 + m._13 * m._32 * m._44 - m._12 * m._33 * m._44;
	_13 = m._13 * m._24 * m._42 - m._14 * m._23 * m._42 + m._14 * m._22 * m._43 - 
		  m._12 * m._24 * m._43 - m._13 * m._22 * m._44 + m._12 * m._23 * m._44;
	_14 = m._14 * m._23 * m._32 - m._13 * m._24 * m._32 - m._14 * m._22 * m._33 + 
		  m._12 * m._24 * m._33 + m._13 * m._22 * m._34 - m._12 * m._23 * m._34;
	_21 = m._24 * m._33 * m._41 - m._23 * m._34 * m._41 - m._24 * m._31 * m._43 + 
		  m._21 * m._34 * m._43 + m._23 * m._31 * m._44 - m._21 * m._33 * m._44;
	_22 = m._13 * m._34 * m._41 - m._14 * m._33 * m._41 + m._14 * m._31 * m._43 - 
		  m._11 * m._34 * m._43 - m._13 * m._31 * m._44 + m._11 * m._33 * m._44;
	_23 = m._14 * m._23 * m._41 - m._13 * m._24 * m._41 - m._14 * m._21 * m._43 + 
		  m._11 * m._24 * m._43 + m._13 * m._21 * m._44 - m._11 * m._23 * m._44;
	_24 = m._13 * m._24 * m._31 - m._14 * m._23 * m._31 + m._14 * m._21 * m._33 - 
		  m._11 * m._24 * m._33 - m._13 * m._21 * m._34 + m._11 * m._23 * m._34;
	_31 = m._22 * m._34 * m._41 - m._24 * m._32 * m._41 + m._24 * m._31 * m._42 - 
		  m._21 * m._34 * m._42 - m._22 * m._31 * m._44 + m._21 * m._32 * m._44;
	_32 = m._14 * m._32 * m._41 - m._12 * m._34 * m._41 - m._14 * m._31 * m._42 + 
		  m._11 * m._34 * m._42 + m._12 * m._31 * m._44 - m._11 * m._32 * m._44;
	_33 = m._12 * m._24 * m._41 - m._14 * m._22 * m._41 + m._14 * m._21 * m._42 - 
		  m._11 * m._24 * m._42 - m._12 * m._21 * m._44 + m._11 * m._22 * m._44;
	_34 = m._14 * m._22 * m._31 - m._12 * m._24 * m._31 - m._14 * m._21 * m._32 + 
		  m._11 * m._24 * m._32 + m._12 * m._21 * m._34 - m._11 * m._22 * m._34;
	_41 = m._23 * m._32 * m._41 - m._22 * m._33 * m._41 - m._23 * m._31 * m._42 + 
		  m._21 * m._33 * m._42 + m._22 * m._31 * m._43 - m._21 * m._32 * m._43;
	_42 = m._12 * m._33 * m._41 - m._13 * m._32 * m._41 + m._13 * m._31 * m._42 - 
		  m._11 * m._33 * m._42 - m._12 * m._31 * m._43 + m._11 * m._32 * m._43;
	_43 = m._13 * m._22 * m._41 - m._12 * m._23 * m._41 - m._13 * m._21 * m._42 + 
		  m._11 * m._23 * m._42 + m._12 * m._21 * m._43 - m._11 * m._22 * m._43;
	_44 = m._12 * m._23 * m._31 - m._13 * m._22 * m._31 + m._13 * m._21 * m._32 - 
		  m._11 * m._23 * m._32 - m._12 * m._21 * m._33 + m._11 * m._22 * m._33;
			
	(*this) = (*this) / det;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getInverse() const
{
	Matrix4x4<T> temp = (*this);
	temp.inverse();
	return temp;
}

template<class T>
bool Matrix4x4<T>::operator ==(const Matrix4x4<T>& m) const 
{
	return ((_11==m._11) && (_12==m._12) && (_13==m._13) && (_14==m._14) &&
		    (_21==m._21) && (_22==m._22) && (_23==m._23) && (_24==m._24) &&
		    (_31==m._31) && (_32==m._32) && (_33==m._33) && (_34==m._34) &&
		    (_41==m._41) && (_42==m._42) && (_43==m._43) && (_44==m._44));
}
	
template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator =(const Matrix4x4<T>& m)
{
	if(this == &m)
		return (*this);
		
	_11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
	_21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
	_31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
	_41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
	return (*this);
}	

template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator +=(const Matrix4x4& m)
{
	(*this) = (*this) + m;
	return (*this);		
}

template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator -=(const Matrix4x4& m)
{
	(*this) = (*this) - m;
	return (*this);		
}
	
template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator *=(const Matrix4x4& m)
{
	(*this) = (*this) * m;
	return (*this);		
}
	
template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator *=(const T& s)
{
	(*this) = (*this) * s;
	return (*this);		
}
	
template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator /=(const T& s)
{
	assert(s !=  static_cast<T>(0.0));
	
	_11 /= s; _12 /= s; _13 /= s; _14 /= s;
	_21 /= s; _22 /= s; _23 /= s; _24 /= s;
	_31 /= s; _32 /= s; _33 /= s; _34 /= s;
	_41 /= s; _42 /= s; _43 /= s; _44 /= s;
	
	return (*this);				
}		

template<class T>
Matrix4x4<T> Matrix4x4<T>::operator +(const Matrix4x4& m) const
{
	return Matrix4x4<T>(_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
						_21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
						_31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
						_41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::operator -(const Matrix4x4& m) const
{
	return Matrix4x4<T>(_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
						_21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
						_31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
						_41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44);
}
	
template<class T>
Matrix4x4<T> Matrix4x4<T>::operator *(const Matrix4x4& m) const
{
	return Matrix4x4<T>(
		val[0][0]*m.val[0][0] + val[0][1]*m.val[1][0] + val[0][2]*m.val[2][0] + val[0][3]*m.val[3][0],
		val[1][0]*m.val[0][0] + val[1][1]*m.val[1][0] + val[1][2]*m.val[2][0] + val[1][3]*m.val[3][0],
		val[2][0]*m.val[0][0] + val[2][1]*m.val[1][0] + val[2][2]*m.val[2][0] + val[2][3]*m.val[3][0],
		val[3][0]*m.val[0][0] + val[3][1]*m.val[1][0] + val[3][2]*m.val[2][0] + val[3][3]*m.val[3][0],

		val[0][0]*m.val[0][1] + val[0][1]*m.val[1][1] + val[0][2]*m.val[2][1] + val[0][3]*m.val[3][1],
		val[1][0]*m.val[0][1] + val[1][1]*m.val[1][1] + val[1][2]*m.val[2][1] + val[1][3]*m.val[3][1],
		val[2][0]*m.val[0][1] + val[2][1]*m.val[1][1] + val[2][2]*m.val[2][1] + val[2][3]*m.val[3][1],
		val[3][0]*m.val[0][1] + val[3][1]*m.val[1][1] + val[3][2]*m.val[2][1] + val[3][3]*m.val[3][1],

		val[0][0]*m.val[0][2] + val[0][1]*m.val[1][2] + val[0][2]*m.val[2][2] + val[0][3]*m.val[3][2],
		val[1][0]*m.val[0][2] + val[1][1]*m.val[1][2] + val[1][2]*m.val[2][2] + val[1][3]*m.val[3][2],
		val[2][0]*m.val[0][2] + val[2][1]*m.val[1][2] + val[2][2]*m.val[2][2] + val[2][3]*m.val[3][2],
		val[3][0]*m.val[0][2] + val[3][1]*m.val[1][2] + val[3][2]*m.val[2][2] + val[3][3]*m.val[3][2],

		val[0][0]*m.val[0][3] + val[0][1]*m.val[1][3] + val[0][2]*m.val[2][3] + val[0][3]*m.val[3][3],
		val[1][0]*m.val[0][3] + val[1][1]*m.val[1][3] + val[1][2]*m.val[2][3] + val[1][3]*m.val[3][3],
		val[2][0]*m.val[0][3] + val[2][1]*m.val[1][3] + val[2][2]*m.val[2][3] + val[2][3]*m.val[3][3],
		val[3][0]*m.val[0][3] + val[3][1]*m.val[1][3] + val[3][2]*m.val[2][3] + val[3][3]*m.val[3][3]);
}

template<class T>
Vector3<T> Matrix4x4<T>::operator *(const Vector3<T>& v) const
{
	T w = 1.0 / (v.x * _14 + v.y * _24 + v.z * _34 + _44);
	return Vector3<T>((v.x * _11 + v.y * _21 + v.z * _31 + _41) * w,
					  (v.x * _12 + v.y * _22 + v.z * _32 + _42) * w,
					  (v.x * _13 + v.y * _23 + v.z * _33 + _43) * w);
}
	
template<class T>
Matrix4x4<T> Matrix4x4<T>::operator *(const T& s) const
{
	return Matrix4x4<T>(_11 * s, _12 * s, _13 * s, _14 * s,
						_21 * s, _22 * s, _23 * s, _24 * s,
						_31 * s, _32 * s, _33 * s, _34 * s,
						_41 * s, _42 * s, _43 * s, _44 * s);
	return (*this);	
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::operator /(const T& s) const
{
	assert(s != static_cast<T>(s));
	return (*this) * (static_cast<T>(1.0)/s);	
}	

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeTranslation(const T& x, const T& y, const T& z)	
{
	Matrix4x4<T> res;
	
	res._14 = x;
	res._24 = y;
	res._34 = z;
	
	return res;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeScale(const T& x, const T& y, const T& z)
{
	Matrix4x4<T> res;
	
	res._11 = x;
	res._22 = y;
	res._33 = z;
	
	return res;
}	

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeRotateX(const T& rot)
{
	Matrix4x4<T> res;

	T c = cos(rot);
	T s = sin(rot);
	
	res._22 = c;
	res._23 = -s;
	res._32 = s;
	res._33 = c;
	
	return res;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeRotateY(const T& rot)
{
	Matrix4x4<T> res;

	T c = cos(rot);
	T s = sin(rot);
			
	res._11 = c;
	res._13 = s;
	res._31 = -s;
	res._33 = c;		
			
	return res;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeRotateZ(const T& rot)
{
	Matrix4x4<T> res;
	
	T c = cos(rot);
	T s = sin(rot);
	
	res._11 = c;
	res._12 = -s;
	res._21 = s;
	res._22 = c;
	
	return res;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeRotateArbitrary(const Vector3<T>& axle, const T& rot)
{
	Matrix4x4<T> res;
	
	Vector3<T> vAxle = axle;
	axle.normalize();
	
	T c = cos(rot);
	T s = sin(rot);
	T c2 = 1.0 - c;
	
	res._11 = (vAxle.x * vAxle.x) * c2 + c;
	res._12 = (vAxle.y * vAxle.x) * c2 + (vAxle.z * s);
	res._13 = (vAxle.z * vAxle.x) * c2 - (vAxle.y * s);
	
	res._21 = (vAxle.x * vAxle.y) * c2 - (vAxle.z * s);
	res._22 = (vAxle.y * vAxle.y) * c2 + c;
	res._23 = (vAxle.z * vAxle.y) * c2 + (vAxle.x * s);
	
	res._31 = (vAxle.x * vAxle.z) * c2 + (vAxle.y * s);
	res._32 = (vAxle.y * vAxle.z) * c2 - (vAxle.x * s);
	res._33 = (vAxle.z * vAxle.z) * c2 + c;	
	
	return res;
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeLookAt(const Vector3<T>& pos, const Vector3<T>& posLookAt,
		const Vector3<T>& worldUp)
{
	Vector3<T> dir = posLookAt - pos;
	Vector3<T> up = worldUp;
	
	dir.normalize();
	up.normalize();
	
	Vector3<T> s = dir ^ up;
	Vector3<T> u = s ^ dir;

	return Matrix4x4<T>(   s.x,    s.y,    s.z, -pos.x,
						   u.x,    u.y,    u.z, -pos.y,
						-dir.x, -dir.y, -dir.z, -pos.z,
						   0.0,    0.0,    0.0,    1.0); 
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makePerspectiveFov(const T& fovY, const T& aspect,
	const T& nearPlane, const T& farPlane)
{	
	T top = nearPlane * tan(fovY * PI/360.0);
	T right = top * aspect;
	
	return makeFrustum(-right, right, -top, top, nearPlane, farPlane);
};

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeFrustum(const T& left, const T& right,
		const T& bottom, const T& top, const T& nearPlane, const T& farPlane)
{
	Matrix4x4<T> res;
	
	res._11 = 2.0 * nearPlane / (right - left);
	res._13 = (right + left) / (right - left);
	res._22 = 2.0 * nearPlane / (top - bottom);
	res._23 = (top + bottom) / (top - bottom);
	res._33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
	res._34 = -2.0 * farPlane * nearPlane / (farPlane - nearPlane);
	res._43 = -1.0;
	res._44 = 0.0;	

	return res;	
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::makeOrthogonal(const T& left, const T& top, const T& right, 
		const T& bottom, const T& nearPlane, const T& farPlane)
{
	Matrix4x4<T> res;
	
	res._11 = 2.0 / (right - left);
	res._22 = 2.0 / (top - bottom);
	res._33 = -2.0 / (farPlane - nearPlane);
	res._14 = -(right + left) / (right - left);
	res._24 = -(top + bottom) / (top - bottom);
	res._34 = -(farPlane + nearPlane) / (farPlane - nearPlane); 
	
	return res;
}
	
}

#endif /*MATRIX4X4_INL_*/
