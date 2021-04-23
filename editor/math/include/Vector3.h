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
 
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "Math.h"
#include "Matrix4x4.h"

/*! \file
* 
*  \brief Describes the Template-Class Vector3, their methodes and attributes.
* 
*/

namespace BSMath
{
	
/* Forward declaration */
template<class T>
class Matrix4x4;			

/*! \brief A template-class that represents a 3-dimensional vector
*
* It's a type independent 3-dimensional vector. 
*/		
template<class T>
class Vector3
{
public:
	union
	{
		T val[3];
		struct
		{
			T x, y, z;				
		};
	};
	
public:
	/*!
	* \brief The default constructor
	* 
	* Sets all values to 0, so that the standard vector is a zero-vector.
	*/	
	Vector3()
	 : x(static_cast<T>(0.0)), y(static_cast<T>(0.0)), z(static_cast<T>(0.0))
	{
	}	
	
	/*!
	* \brief The constructor
	* 
	* \param _x The value for the x-coordinate
	* \param _y The value for the y-coordinate
	* \param _z The value for the z-coordinate
	*/		 
	Vector3(const T& _x, const T& _y, const T& _z)
	 : x(_x), y(_y), z(_z)
	{ 
	}	
	
	/*!
	* \brief The copy-constructor
	* 
	* \param vec The vector which should be copied.
	*/			
	Vector3(const Vector3<T>& vec)
	 : x(vec.x), y(vec.y), z(vec.z)
	{
	}
	
	/*!
	* \brief Set up the values of the vector
	*
	* \param _x The x-coordinate of the vector
	* \param _y The y-coordinate of the vector
	* \param _z The z-coordinate of the vector
	*/		
	void set(const T& _x, const T& _y, const T& _z);
	
	/*!
	* \brief Set all values of the vector to 0.0
	*
	* The vector is set to a zero-vector, all values are 0.0 
	*/		
	void zero()
	{
		x = y = z = static_cast<T>(0.0);
	}
	
	/*!
	* \brief Returns the length of the vector
	*
	* \return T The length of the vector
	*/		
	T length() const
	{
		return static_cast<T>(sqrt(static_cast<double>(x*x + y*y + z*z)));
	}
	
	/*!
	* \brief Returns the square of the vector length.
	*
	* \return T The square of the vector length
	*/		
	T lengthSqr() const
	{
		return (x*x + y*y + z*z);
	}
	
	/*!
	* \brief Negates the vector
	* 
	* Negates the vector, so that is reversed. All coordinates will change 
	* their sign.
	*
	*/		
	void negate();
	
	/*!
	* \brief Normalizes the vector
	* 
	* Normalizes the vector, so that the length is 1.0
	*/		
	void normalize();
	
	/*!
	* \brief Returns the angle beween this vector and an other vector in 
	*  radians
	* 
	* \param v The other vector to compute the angle between them
	* \return T The angle between the vectors in radians
	*
	*/
	T angleWith(const Vector3<T>& v) const
	{
		return std::acos(angleCosineWith(v)); 
	}
	
	/*!
	* \brief Returns the cosine of the angle beween this vector and an other vector in 
	*  radians
	* 
	* \param v The other vector to compute the angle between them
	* \return T cos(angle) between the vectors
	*
	*/		
	T angleCosineWith(const Vector3<T>& v) const 
	{
		return (((*this)*v)/(length()*v.length())); 
	}
	
	/*!
	* \brief Overwrites the vector with the difference vector between 
	*  two other vectors 
	* 
	* Calculates the difference vector between two other vectors and assigns
	* the result to this vector
	*
	* \param v1 The first vector to calculate the difference
	* \param v2 The second vector to calculate the difference
	*/					
	void difference(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		*this = v1 - v2;
	}
	
	/*!
	* \brief Overwrites the vector with the cross-product of two vectors
	* 
	* Calculates the cross-product of two vectors and assigns the result
	* to this vector. The cross-product generates a vector that is
	* orthogonal to the two passed vectors. In case the two vectors
	* have the same direction, the result vector will be the zero-vector.
	*
	* \param v1 The first vector to calculate the cross-product
	* \param v2 The second vector to calculate the cross-product
	*/					
	void cross(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		*this = v1 ^ v2;
	}
	
	/*!
	* \brief Returns the dot-product of this vector and another vector
	*
	* \param v The other vector to calculate the dot-product
	* \return T The dot-product of this vector and the passed vector
	*/		
	T dotProduct(const Vector3<T>& v) const
	{
		return (*this) * v;
	}
	
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
	bool operator == (const Vector3<T>& v) const
	{
		return ((x==v.x) && (y==v.y) && (z==v.z));
	}
	
	/*! 
	* \brief Comparison operator 
	*/		
	bool operator != (const Vector3<T>& v) const
	{
		return !((x==v.x) && (y==v.y) && (z==v.z));
	}
	
	/*!
	* \brief Assign operator 
	*/			
	Vector3<T>& operator =  (const Vector3<T>& v);
	
	/*!
	* \brief Assign operator for += 
	*/			
	Vector3<T>& operator += (const Vector3<T>& v);
	
	/*!
	* \brief Assign operator for -= 
	*/			
	Vector3<T>& operator -= (const Vector3<T>& v);
	
	/*!
	* \brief Assign operator for *= (componentwise multiplication) 
	*/			
	Vector3<T>& operator *= (const Vector3<T>& v);
		
	/*!
	* \brief Assign operator for *= 
	*/			
	Vector3<T>& operator *= (const T& v);
	
	/*!
	* \brief Assign operator for /= 
	*/			
	Vector3<T>& operator /= (const Vector3<T>& v);
	
	/*!
	* \brief Assign operator for /= 
	*/			
	Vector3<T>& operator /= (const T& v);

	/*!
	* \brief Unary operator 
	*/		
	Vector3<T> operator + () const
	{
		return (*this);
	}
	
	/*!
	* \brief Unary operator 
	*/				
	Vector3<T> operator - () const
	{
		return Vector3<T>(-x, -y, -z);
	}
	
	/*!
	* \brief Binary operator for addition 
	*/			
	Vector3<T> operator + (const Vector3<T>& v) const
	{
		return Vector3<T>(x+v.x, y+v.y, z+v.z);
	}
	
	/*!
	* \brief Binary operator for subtraction 
	*/			
	Vector3<T> operator - (const Vector3<T>& v) const
	{
		return Vector3<T>(x-v.x, y-v.y, z-v.z);
	}
	
	/*!
	* \brief Binary operator for dot-product 
	*/			
	T operator * (const Vector3<T>& v) const
	{
		return (x*v.x + y*v.y + z*v.z);
	}
	
	//Vector3<T> operator * (const Matrix4x4<T>& m) const;
	//Vector3<T> operator * (const Quaternion<T>& q) const;
	
	/*!
	* \brief Binary operator for multiplication 
	*/			
	Vector3<T> operator * (const T& v) const
	{
		return Vector3<T>(x*v, y*v, z*v);
	}
	
	/*!
	* \brief Binary operator for subtraction 
	*/			
	Vector3<T> operator - (const T& v) const
	{
		return Vector3<T>(x-v, y-v, z-v);
	}
	
	/*!
	* \brief Binary operator for addition 
	*/			
	Vector3<T> operator + (const T& v) const
	{
		return Vector3<T>(x+v, y+v, z+v);
	}
	
	
	/*!
	* \brief Binary operator for devision 
	*/			
	Vector3<T> operator / (const Vector3<T>& v) const
	{
		return Vector3<T>(x/v.x, y/v.y, z/v.z);
	}
	
	/*!
	* \brief Binary operator for devision 
	*/			
	Vector3<T> operator / (const T& v) const 
	{
		return Vector3<T>(x/v, y/v, z/v);
	}
	
	/*!
	* \brief Binary operator for cross-product 
	*/			
	Vector3<T> operator ^ (const Vector3<T>& v) const
	{
		return Vector3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}

	/*!
	* \brief Friend binary operator for multiplication
	*/			
	friend Vector3<T> operator * (const T& val, const Vector3<T>& v)
	{
		return Vector3<T>(val*v.x, val*v.y, val*v.z);
	}
		
	/*!
	* \brief Calculates a vector with the minimum values of two colors. 
	* 
	* Calulates the component-wise minimum of two vectors.
	*
	* \param v1 The first vector to compare for minimum calculation
	* \param v2 The second vector to compare for minimum calculation
	* \return Vector3<T> The vector with the minimum values
	*/			
	static Vector3<T> min(const Vector3<T>& v1, const Vector3<T>& v2);
	
	/*!
	* \brief Calculates a vector with the maximum values of two colors. 
	* 
	* Calulates the component-wise maximum of two vectors.
	*
	* \param v1 The first vector to compare for maximum calculation
	* \param v2 The second vector to compare for maximum calculation
	* \return Vector3<T> The vector with the maximum values
	*/					
	static Vector3<T> max(const Vector3<T>& v1, const Vector3<T>& v2);
	
	/*!
	* \brief Returns a random vector
	*
	* \param length Specifies the length that the vector should have
	* \return Vector3<T> The random vector
	*/			
	static Vector3<T> random(T length = 1.0);
	
	/*!
	* \brief Returns an interpolated vector
	*
	* Interpolates linear beween two vector with a specified factor. For 
	* example v1 = (0.0, 5.0, 2.0) and v2 = (1.0, 1.8, 2.0):\n
	* Factor f=0.0 : Result = v1\n
	* Factor f=1.0 : Result = v2\n
	* Factor f=0.5 : Result = Average of v1 and v2 = (0.5, 3.4, 2.0)
	* 
	* \param v1 The first vector to interpolate from
	* \param v2 The second vector to interpolate from
	* \param f The interpolation factor
	* \return Vector3<T> The interpolated vector
	*/			
	static Vector3<T> interpolateLinear(const Vector3<T>& v1, const Vector3<T>& v2, const double f)
	{
		return v1 + f * (v2 - v1);
	}

};
	
typedef Vector3<double> Vector;
	
}

#include "Vector3.inl"

#endif /*VECTOR3_H_*/
