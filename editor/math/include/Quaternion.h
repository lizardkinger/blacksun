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
 *  File:       Quaternion.h
 *  Created:    30.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Math.h"
#include "Vector3.h"
#include "Matrix4x4.h"

/*! \file
* 
*  \brief Describes the Template-Class Quaternion, their methodes and attributes.
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


/*! \brief A template-class that represents an quaternion
*
* A quaternion is used to describe a rotation withour the gimbal-lock.
* The quaternions have 4 values, one real part and three imaginary parts.
* The values x, y and z are representing the rotation axis, the value w
* the rotation.  
*/
template<class T>
class Quaternion
{
public:
	union
	{
		T val[4];
		struct
		{
			T x, y, z, w;				
		};
	};
	
public:
	/*!
	* \brief The default constructor
	* 
	* The default constructor set up an identity quaternion.
	*/	
	Quaternion()
	 : x(static_cast<T>(0.0)), y(static_cast<T>(0.0)), 
	   z(static_cast<T>(0.0)), w(static_cast<T>(1.0))
	{
	}

	/*!
	* \brief The constructor
	* 
	* Sets up the quaternion with the four components
	* 
	* \param _x The value for the x-part
	* \param _y The value for the y-part
	* \param _z The value for the z-part
	* \param _w The value for the w-part
	*/	 
	Quaternion(const T& _x, const T& _y, const T& _z, const T& _w = static_cast<T>(1.0))
	 : x(_x), y(_y), z(_z), w(_w)
	{
	}

	/*!
	* \brief The constructor
	* 
	* Sets up the quaternion by converting a rotation-matrix
	* 
	* \param m The rotation matrix
	*/	 	
	Quaternion(const Matrix4x4<T>& m)
	{
		(*this) = makeFromMatrix(m);
	}
	
	/*!
	* \brief The constructor
	* 
	* Sets up the values of the quaternion with a passed vector
	* 
	* \param v The vector where x-, y- and z-part are stored
	*/	 	
	Quaternion(const Vector3<T>& v)
 	: x(v.x), y(v.y), z(v.z), w(static_cast<T>(1.0))	
	{
	}

	/*!
	* \brief The copy-constructor
	* 
	* \param q The quaternion which should be copied
	*/
	Quaternion(const Quaternion<T>& q)
	 : x(q.x), y(q.y), z(q.z), w(q.w)
	{
	}

	/*!
	* \brief Sets up the quaternion with the four components
	* 
	* \param _x The value for the x-part
	* \param _y The value for the y-part
	* \param _z The value for the z-part
	* \param _w The value for the w-part
	*/	
	void set(const T& _x, const T& _y, const T& _z, const T& _w = static_cast<T>(1.0));

	/*!
	* \brief Sets up the quaternion from a vector
	* 
	* \param v The vector where x-, y- and z-part are stored
	*/	
	void fromVector(const Vector3<T>& v)
	{
		(*this) = makeFromVector(v);
	}
	
	/*!
	* \brief Sets up the quaternion by converting a rotation matrix
	* 
	* \param m The rotation matrix
	*/		
	void fromMatrix(const Matrix4x4<T>& m)
	{
		(*this) = makeFromMatrix(m);
	}

	/*!
	* \brief Sets up the quaternion with the rotation angles on the three axis
	* 
	* \param pitch The rotation angle on the x-axis in radians
	* \param yaw The rotation angle on the y-axis in radians
	* \param roll The rotation angle on the z-axis in radians
	*/		
	void fromEuler(const T& pitch, const T& yaw, const T& roll)
	{
		(*this) = makeFromEuler(pitch, yaw, roll);
	}
	
	/*!
	* \brief Sets up the quaternion with the rotation axis and rotation angle
	* 
	* \param axis The axis-vector
	* \param angle The rotation angle in radians
	*/	
	void fromAxisAngle(const Vector3<T> axis, const T& angle) { (*this) = makeFromAxisAngle(axis, angle); }
	
	/*!
	* \brief Computes the w-component
	* 
	* Computes the w-component from the x-, y-, and z-component
	*/	
	void computeW();
	
	/*!
	* \brief Sets up an identity quaternion
	* 
	* Sets up the identity quaternion (0, 0, 0, 1)
	*/	
	void identity()
	{
		set(static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(1.0));
	}
	
	/*!
	* \brief Normalizes the quaternion
	* 
	* Normalizes the quaternion, so that the magnitude is 1.0
	*/	
	void normalize();
	
	/*!
	* \brief Returns the normalized quaternion
	* 
	* \return Quaternion<T> The normalized quaternion
	*/	
	Quaternion<T> getNormalize() const;
			
	/*!
	* \brief Conjuages the quaternion
	* 
	* Conjugates the quaternion by inverting the x-, y-, and z-component
	*/			
	void conjugate();
	
	/*!
	* \brief Returns the conjuaged quaternion
	* 
	* \return Quaternion<T> The conjuagated quaternion
	*/	
	Quaternion<T> getConjugate() const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}
	
	/*!
	* \brief Inverts the quaternion
	* 
	* Inverts the quaternion
	*/	
	void invert();
	
	/*!
	* \brief Returns the inverted quaternion
	* 
	* \return Quaternion<T> The inverted quaternion
	*/	
	Quaternion<T> getInvert() const;
	
	/*!
	* \brief Returns the magnitude of the quaternion
	* 
	* \return T The magnitude
	*/	
	T magnitude()	 const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}
	
	/*!
	* \brief Returns the sqare of the magnitude
	* 
	* \return T The square of the magnitude
	*/
	T magnitudeSqr() const
	{
		return (x*x + y*y + z*z + w*w);
	}

	/*!
	* \brief Returns the Matrix, that represents the same rotation
	* 
	* Converts the quaternion into a matrix. Precondition: The 
	* quaternion must be a unit-quaternion
	* 
	* \return Matrix4x4<T> The converted Matrix
	*/		
	Matrix4x4<T> toMatrix() const;
	
	/*!
	* \brief Returns the rotation axis
	* 
	* \return Vector3<T> The rotation axis
	*/	
	Vector3<T> getAxis() const
	{
		return Vector3<T>(x, y, z);
	}
	
	/*!
	* \brief Returns the euler rotation angles
	* 
	* \return Vector3<T> A vector containing the three euler rotation angles
	*/
	Vector3<T> getEulerAngles() const;

	/*!
	* \brief Returns the euler rotation angles
	* 
	* \param pitch The pitch angle
	* \param yaw The yaw angle
	* \param roll The roll angle
	*/	
	void getEulerAngles(T& pitch, T& yaw, T& roll) const;
	
	/*!
	* \brief Rotates a vector and returns the result
	* 
	* \param v The vector that should be rotated
	* \return Vector3<T> The rotated vector
	*/	
	Vector3<T> rotateVector(const Vector3<T>& v) const;
	
	/*! 
	* \brief Cast to an Array of T 
	*/		
	operator T* ()
	{
		return val;
	}
	
	/*! 
	* \brief Cast to an Array of T 
	*/		
	operator const  T* () const
	{
		return val;
	}
	
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator == (const Quaternion<T>& q) const
	{
		return ((x==q.x) && (y==q.y) && (z==q.z) && (w==q.w));
	}

	/*! 
	* \brief Comparison operator 
	*/	
	bool operator != (const Quaternion<T>& q) const
	{
		return !((x==q.x) && (y==q.y) && (z==q.z) && (w==q.w));
	}

	/*!
	* \brief Assign operator 
	*/		
	Quaternion<T>& operator =  (const Quaternion<T>& q);
	
	/*!
	* \brief Assign operator for += 
	*/				
	Quaternion<T>& operator += (const Quaternion<T>& q);
	
	/*!
	* \brief Assign operator for -= 
	*/		
	Quaternion<T>& operator -= (const Quaternion<T>& q);
	
	/*!
	* \brief Assign operator for *= 
	*/		
	Quaternion<T>& operator *= (const Quaternion<T>& q);
	
	/*!
	* \brief Assign operator for *= 
	*/		
	Quaternion<T>& operator *= (const T& s);
	
	/*!
	* \brief Assign operator for /= 
	*/		
  	Quaternion<T>& operator /= (const T& s);
  	
	/*!
	* \brief Unary operator 
	*/	  	
	Quaternion<T> operator + () const
	{
		return (*this);
	}
	
	/*!
	* \brief Unary operator 
	*/		
	Quaternion<T> operator - () const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}

	/*!
	* \brief Binary operator for addition 
	*/		
	Quaternion<T> operator + (const Quaternion<T>& q) const
	{
		return Quaternion<T>(x+q.x, y+q.y, z+q.z, w+q.w);
	}
	
	/*!
	* \brief Binary operator for subtraction 
	*/		
	Quaternion<T> operator - (const Quaternion<T>& q) const
	{
		return Quaternion<T>(x-q.x, y-q.y, z-q.z, w-q.w);
	}
	
	/*!
	* \brief Binary operator for multiplication 
	*/		 
	Quaternion<T> operator * (const Vector3<T>& v) const;
	
	/*!
	* \brief Binary operator for multiplication 
	*/		
	Quaternion<T> operator * (const Quaternion<T>& q) const;
	
	/*!
	* \brief Binary operator for multiplication 
	*/		 
	Quaternion<T> operator * (const T& s) const
	{
		return Quaternion<T>(x*s, y*s, z*s, w*s);
	}
	
	/*!
	* \brief Binary operator for division 
	*/		
	Quaternion<T> operator / (const T& s) const;
	
	/*!
	* \brief Binary operator for conjugation 
	*/			
	Quaternion<T> operator ~ () const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}
	
	/*!
	* \brief Friend binary operator for multiplication
	*/		
	friend Quaternion<T> operator * (const T& val, const Quaternion<T>& q)
	{
		return (q * val);
	}
	
	/*!
	* \brief Friend binary operator for multiplication
	*/		
	friend Quaternion<T> operator * (const Vector3<T>& v, const Quaternion<T>& q)
	{
		return (q * v);
	}
		
	/*!
	* \brief Creates a quaternion from a vector
	* 
	* \param v The vector where x-, y- and z-part are stored
	* \return Quaternion<T> The quaternion
	*/			
	static Quaternion<T> makeFromVector(const Vector3<T>& v);
	
	/*!
	* \brief Creates a quaternion by converting a rotation matrix
	* 
	* \param m The rotation matrix
	* \return Quaternion<T> The Quaternion
	*/	
	static Quaternion<T> makeFromMatrix(const Matrix4x4<T>& m);
	
	/*!
	* \brief Creates a quaternion of rotation angles on the three axis
	* 
	* \param pitch The rotation angle on the x-axis in radians
	* \param yaw The rotation angle on the y-axis in radians
	* \param roll The rotation angle on the z-axis in radians
	* \return Quaternion<T> The Quaternion
	*/	
	static Quaternion<T> makeFromEuler(const T& pitch, const T& yaw, const T& roll);
	
	/*!
	* \brief Creates a quaternion of a rotation axis and rotation angle
	* 
	* \param axis The axis-vector
	* \param angle The rotation angle in radians
	* \return Quaternion<T> The Quaternion
	*/	
	static Quaternion<T> makeFromAxisAngle(const Vector3<T>& axis, const T& angle);
	
	/*!
	* \brief Returns an interpolated Quaternion
	*
	* Interpolates beween two quaternions with a specified factor
	* 
	* \param q1 The first quaternion to interpolate from
	* \param q2 The second quaternion to interpolate from
	* \param f The interpolation factor
	* \return Quaternion<T> The interpolated quaternion
	*/		
	static Quaternion<T> slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, const T& f);
	
		
	/*!
	* \brief Creates a uniform random rotation quaternion
	* 
	* \return Quaternion<T> The Quaternion
	*/		
	static Quaternion<T> random();
};
	
typedef Quaternion<double> Quat;
	
}

#include "Quaternion.inl"

#endif /*QUATERNION_H_*/
