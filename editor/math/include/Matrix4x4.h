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
 *  File:       Matrix4x4.h
 *  Created:    20.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATRIX4X4_H_
#define MATRIX4X4_H_

#include "Math.h"
#include "Vector3.h"
#include "Quaternion.h"

/*! \file
* 
*  \brief Describes the Template-Class Matrix4x4, their methodes and attributes.
* 
*/


namespace BSMath
{
	
/* Forward declarations */
template<class T>
class Vector3;

template<class T>
class Quaternion;


/*! \brief A template-class that represents a 4x4-matrix
*
* It's a type independent 4x4-matrix. The main use of this class is to
* simplify the handling with matrices, in particular with OpenGl. 
*/		
template<class T>
class Matrix4x4
{
public:
	union
	{
		T val[4][4];
		T val2[16];
		struct
		{
			T _11, _12, _13, _14;
			T _21, _22, _23, _24;
			T _31, _32, _33, _34;
			T _41, _42, _43, _44;			
		};
	};
	
public:
	/*!
	* \brief The default constructor
	* 
	* The default constructor set up an identity matrix.
	*/		
	Matrix4x4()	
	{
		identity();
	}
	
	/*!
	* \brief The constructor
	* 
	* This constructor set up all matrix elements to the same value.
	* 
	* \param a The matrix that should be copied
	*/			
	Matrix4x4(const T& a);
	
	/*!
	* \brief The constructor
	* 
	* This constructor set up all matrix elements individual
	*
	* \param a11 The value for the component [1][1]
	* \param a12 The value for the component [1][2]
	* \param a13 The value for the component [1][3]
	* \param a14 The value for the component [1][4]
	* \param a21 The value for the component [2][1]
	* \param a22 The value for the component [2][2]
	* \param a23 The value for the component [2][3]
	* \param a24 The value for the component [2][4]
	* \param a31 The value for the component [3][1]
	* \param a32 The value for the component [3][2]
	* \param a33 The value for the component [3][3]
	* \param a34 The value for the component [3][4]
	* \param a41 The value for the component [4][1]
	* \param a42 The value for the component [4][2]
	* \param a43 The value for the component [4][3]
	* \param a44 The value for the component [4][4]
	*/			
	Matrix4x4(const T& a11, const T& a12, const T& a13, const T& a14, 
			  const T& a21, const T& a22, const T& a23, const T& a24, 
			  const T& a31, const T& a32, const T& a33, const T& a34,
			  const T& a41, const T& a42, const T& a43, const T& a44);
			  
	/*!
	* \brief The constructor
	* 
	* This constructor set up all rows of the matrix with the passed
	* vectors.
	* 
	* \param v1 The vector with the three values for the first row
	* \param v2 The vector with the three values for the second row
	* \param v3 The vector with the three values for the third row
	*/							  
	Matrix4x4(const Vector3<T>& v1, const Vector3<T>& v2, 
			  const Vector3<T>& v3);
			  
	//Matrix4x4(const Vector4<T>& v1, const Vector4<T>& v2,
	//		  const Vector4<T>& v3,const Vector4<T>& v4);
	
	/*!
	* \brief The constructor
	* 
	* This constructor set up all matrix elements individual with the
	* passed array. The array must have 16 values and have to exist 
	* line-by-line.
	* 
	* \param mat The pointer to the array 
	*/					 
	Matrix4x4(const T* mat);
	
	/*!
	* \brief The copy-constructor
	* 
	* \param mat The matrix which should be copied.
	*/				
	Matrix4x4(const Matrix4x4<T>& mat);
	
	/*!
	* \brief The constructor
	* 
	* This constructor set up the elements by converting a quaternion
	* into a matrix.
	* 
	* \param quat The quaternion to convert
	*/				
	Matrix4x4(const Quaternion<T>& quat)
	{
		(*this) = quat.toMatrix();
	}
	
	/*!
	* \brief Set up the elements of the matrix
	* 
	* Set up all elements of the matrix with individual values.
	* 
	* \param a11 The value for the component [1][1]
	* \param a12 The value for the component [1][2]
	* \param a13 The value for the component [1][3]
	* \param a14 The value for the component [1][4]
	* \param a21 The value for the component [2][1]
	* \param a22 The value for the component [2][2]
	* \param a23 The value for the component [2][3]
	* \param a24 The value for the component [2][4]
	* \param a31 The value for the component [3][1]
	* \param a32 The value for the component [3][2]
	* \param a33 The value for the component [3][3]
	* \param a34 The value for the component [3][4]
	* \param a41 The value for the component [4][1]
	* \param a42 The value for the component [4][2]
	* \param a43 The value for the component [4][3]
	* \param a44 The value for the component [4][4]
	*/		
	void set(const T& a11, const T& a12, const T& a13, const T& a14, 
			 const T& a21, const T& a22, const T& a23, const T& a24, 
			 const T& a31, const T& a32, const T& a33, const T& a34,
			 const T& a41, const T& a42, const T& a43, const T& a44);
			 
	//void set(const Vector4<T>& v1, const Vector4<T>& v2, 
	//		 const Vector4<T>& v3, const Vector4<T>& v4);
	
	/*!
	* \brief Set up the rows of the matrix
	* 
	* \param v1 The vector with the three values for the first row
	* \param v2 The vector with the three values for the second row
	* \param v3 The vector with the three values for the third row
	*/		
	void set(const Vector3<T>& v1, const Vector3<T>& v2, 
			 const Vector3<T>& v3);	 
	
	/*!
	* \brief Set up the values of the matrix
	* 
	* Set up the elements of the matrix by converting a quaternion
	* 
	* \param q The quaternion to convert
	*/	
	void set(const Quaternion<T>& q)
	{
		(*this) = q.toMatrix();
	}					  
	
	/*!
	* \brief Set the matrix to a zero-matrix
	* 
	* All elements are set to zero, so that this matrix will be a
	* zero-matrix
	*/			
	void zero();
	
	/*!
	* \brief Set the matrix to a identity-matrix
	* 
	* Set the diagonal matrix elements to 1.0, all other elements to 0.0,
	* so that this matrix will be a identity-matrix
	*/		
	void identity();
	
	/*!
	* \brief Normalizes the matrix
	* 
	* Normalizes the matrix, so that all the values will be included
	* in the interval [0, 1]
	*/		
	void normalize();
	
	/*!
	* \brief Returns the determinant
	* 
	* \return T The determinant of the matrix
	*/			
	T determinant() const;
	
	/*!
	* \brief Absolute all elements of the matrix
	* 
	*/			
	void absolute();
	
	/*!
	* \brief Return the absolute of the matrix
	* 
	* \return Matrix4x4<T> The absolute of the matrix
	*/			
	Matrix4x4<T> getAbsolute() const;
	
	/*!
	* \brief Applies a scale matrix to this matrix
	* 
	* Applies a scale matrix by multiplying it to this matrix
	* 
	* \param s The scale factor for x-, y- and z-coordinate
	*/	
	void scale(const T& s)
	{
		scale(s, s, s);
	}
	
	/*!
	* \brief Applies a scale matrix to this matrix
	* 
	* Applies a scale matrix by multiplying it to this matrix
	* 
	* \param sx The scale factor for x-coordinate
	* \param sy The scale factor for y-coordinate
	* \param sz The scale factor for z-coordinate 
	*/			
	void scale(const T& sx, const T& sy, const T& sz)
	{
		(*this) *= makeScale(sx, sy, sz);	
	}
	
	/*!
	* \brief Applies a scale matrix to this matrix
	* 
	* Applies a scale matrix by multiplying it to this matrix
	* 
	* \param v The vector that stores the scale factor for x-, y- and 
	* 	z-coordinate 
	*/		
	void scale(const Vector3<T>& v)
	{
		scale(v.x, v.y, v.z);
	}
	
//	Vector3<T> getScale();
	
	/*!
	* \brief Set the translation in the matrix
	* 
	* Set the translation in the matrix, which is particularly used for 
	* OpenGl-matrizes. 
	* 
	* \param x The translation in x-axis
	* \param y The translation in y-axis
	* \param z The translation in z-axis
	*/		
	void translation(const T& x, const T& y, const T& z);
	
	/*!
	* \brief Set the translation in the matrix
	* 
	* Set the translation in the matrix, which is particularly used for 
	* OpenGl-matrizes. 
	* 
	* \param v The vector that stores the translation in x-, y-, and
	* 	z-axis
	*/				
	void translation(const Vector3<T>& v);
	
	/*!
	* \brief Returns the translation of the matrix
	* 
	* Returns the translation in the matrix, which is particularly stored in 
	* OpenGl-matrizes. 
	* 
	* \return Vector3<T> The vector that stores the translation
	*/				
	Vector3<T> getTranslation() const
	{
		return Vector3<T>(_14, _24, _34);
	}
	
	/*!
	* \brief Applies a X-rotation matrix to this matrix
	* 
	* Applies a rotation matrix around the X-axis by multiplying 
	* it to this matrix
	* 
	* \param rot The rotation angle in radians
	*/				
	void rotateX(const T& rot)
	{
		(*this) *= makeRotateX(rot);
	}
	
	/*!
	* \brief Applies a Y-rotation matrix to this matrix
	* 
	* Applies a rotation matrix around the Y-axis by multiplying 
	* it to this matrix
	* 
	* \param rot The rotation angle in radians
	*/			
	void rotateY(const T& rot)
	{
		(*this) *= makeRotateY(rot);
	}
	
	/*!
	* \brief Applies a Z-rotation matrix to this matrix
	* 
	* Applies a rotation matrix around the Z-axis by multiplying 
	* it to this matrix
	* 
	* \param rot The rotation angle in radians
	*/			
	void rotateZ(const T& rot)
	{
		(*this) *= makeRotateZ(rot);
	}
	
	/*!
	* \brief Applies a rotation matrix to this matrix
	* 
	* Applies a rotation matrix around all three axis by multiplying 
	* it to this matrix
	* 
	* \param rotX The rotation angle around the X axis in radians
	* \param rotY The rotation angle around the Y axis in radians
	* \param rotZ The rotation angle around the Z axis in radians
	*/			
	void rotateAround(const T& rotX, const T& rotY, const T& rotZ)
	{
		(*this) = (*this) * makeRotateX(rotX) * makeRotateY(rotY) * makeRotateZ(rotZ);	
	}
	
	/*!
	* \brief Applies a axle-rotation matrix to this matrix
	* 
	* Applies a rotation matrix around all three axis by multiplying 
	* it to this matrix
	* 
	* \param rot The vector that stores the rotation angles around the
	* 	three axis in radians
	*/					
	void rotateAround(const Vector3<T>& rot)
	{
		rotateAround(rot.x, rot.y, rot.z);
	}
	
	/*!
	* \brief Applies a rotation matrix to this matrix
	* 
	* Applies a rotation matrix around all a individual axle by multiplying 
	* it to this matrix
	* 
	* \param axle The direction of the axle
	* \param rot The rotation angle around the axle in radians
	*/					
	void rotateArbitrary(const Vector3<T>& axle, const T& rot)
	{
		(*this) = (*this) * makeRotateArbitrary(axle, rot);	
	}
	
//	Vector3<T> getRotation();

	/*!
	* \brief Transpose the matrix
	* 
	* Transposes this matrix by mirroring the elements on the main diagonal
	*/			
	void transpose();
	
	/*!
	* \brief Returns the transpose of this matrix
	* 
	* Transposes this matrix by mirroring the elements on the main diagonal
	* 
	* \return Matrix4x4<T> The transposed matrix
	*/					
	Matrix4x4<T> getTranspose() const
	{
		return Matrix4x4<T>(_11, _21, _31, _41, _12, _22, _32, _42,
							_13, _23, _33, _43, _14, _24, _34, _44);			
	}
	
	/*!
	* \brief Inverts the matrix
	* 
	* Inverts this matrix. Warning: Not all matrizes are invertible.
	*/			
	void inverse();
	
	/*!
	* \brief Returns the inverse of this matrix
	* 
	* Inverts this matrix. Warning: Not all matrizes are invertible.
	* 
	* \return Matrix4x4<T> the inverted matrix
	*/			
	Matrix4x4<T> getInverse() const;
	
	/*! 
	 * \brief Cast to an Array of T 
	 */			
	operator T* ()
	{
		return val2;
	}
	
	/*! 
	 * \brief Cast to an Array of T 
	 */			
	operator const  T* () const
	{
		return val2;
	}
		
	/*! 
	* \brief Comparison operator 
	*/					
	bool operator == (const Matrix4x4<T>& m) const;
	
	/*! 
	* \brief Comparison operator 
	*/			
	bool operator != (const Matrix4x4<T>& m) const
	{
		return !((*this)==m);	
	}
	
	/*!
	* \brief Assign operator 
	*/		
	Matrix4x4<T>& operator = (const Matrix4x4<T>& m);

	/*!
	* \brief Assign operator for += 
	*/
	Matrix4x4<T>& operator += (const Matrix4x4<T>& m);
	
	/*!
	* \brief Assign operator for -= 
	*/		
	Matrix4x4<T>& operator -= (const Matrix4x4<T>& m);
	
	/*!
	* \brief Assign operator for *= 
	*/		
	Matrix4x4<T>& operator *= (const Matrix4x4<T>& m);
	
	/*!
	* \brief Assign operator for *= 
	*/		
	Matrix4x4<T>& operator *= (const T& s);
	
	/*!
	* \brief Assign operator for /= 
	*/		
	Matrix4x4<T>& operator /= (const T& s);
	
	/*!
	* \brief Unary operator 
	*/		
	Matrix4x4<T> operator + () const
	{
		return (*this);
	}

	/*!
	* \brief Unary operator 
	*/		
	Matrix4x4<T> operator - () const 
	{
		return (*this)*(-1.0);
	}

	/*!
	* \brief Binary operator for addition 
	*/	
	Matrix4x4<T> operator + (const Matrix4x4<T>& m) const;
	
	/*!
	* \brief Binary operator for subtraction 
	*/			
	Matrix4x4<T> operator - (const Matrix4x4<T>& m) const;
	
	/*!
	* \brief Binary operator for multiplication 
	*/			
	Matrix4x4<T> operator * (const Matrix4x4<T>& m) const;
	
	/*!
	* \brief Binary operator for matrix*vector-multiplication 
	*/			
	Vector3<T>   operator * (const Vector3<T>& v) const;
	
	/*!
	* \brief Binary operator for multiplication 
	*/			
	Matrix4x4<T> operator * (const T& s) const;
	
	/*!
	* \brief Binary operator for devision 
	*/			
	Matrix4x4<T> operator / (const T& s) const;

	/*!
	* \brief Friend binary operator for vector*matrix-multiplication
	*/
	friend Vector3<T> operator * (const Vector3<T>& v, const Matrix4x4<T>& m)
	{
		return m * v;
	}
	
	/*!
	* \brief Friend binary operator for multiplication
	*/		
	friend Vector3<T> operator * (const T& s, const Matrix4x4<T>& m)
	{
		return m * s;
	}

	/*!
	* \brief Creates a translation matrix
	*  
	* \param trans The vector which stores the translation on x-, y- and
	* 	z-axis
	* \return Matrix4x4<T> The translation matrix
	*/	
	static Matrix4x4<T> makeTranslation(const Vector3<T>& trans)
	{
		return makeTranslation(trans.x, trans.y, trans.z);	
	}
	
	/*!
	* \brief Creates a translation matrix
	*  
	* \param x The translation on x-axis
	* \param y The translation on y-axis
	* \param z The translation on z-axis
	* \return Matrix4x4<T> The translation matrix
	*/		
	static Matrix4x4<T> makeTranslation(const T& x, const T& y, const T& z);

	/*!
	* \brief Creates a scaling matrix
	*  
	* \param scale The vector which stores the scaling factor for x-, y- and
	* 	z-axis
	* \return Matrix4x4<T> The scaling matrix
	*/		
	static Matrix4x4<T> makeScale(const Vector3<T>& scale)
	{
		return makeScale(scale.x, scale.y, scale.z);	
	}
	
	/*!
	* \brief Creates a scaling matrix
	*  
	* \param x The scaling factor for x-axis
	* \param y The scaling factor for y-axis
	* \param z The scaling factor for z-axis
	* \return Matrix4x4<T> The scaling matrix
	*/		
	static Matrix4x4<T> makeScale(const T& x, const T& y, const T& z);

	/*!
	* \brief Creates a X-rotation matrix
	*  
	* \param rot The rotation angle in radians
	* \return Matrix4x4<T> The X-rotation matrix
	*/		
	static Matrix4x4<T> makeRotateX(const T& rot);
	
	/*!
	* \brief Creates a Y-rotation matrix
	*  
	* \param rot The rotation angle in radians
	* \return Matrix4x4<T> The Y-rotation matrix
	*/		
	static Matrix4x4<T> makeRotateY(const T& rot);
	
	/*!
	* \brief Creates a Z-rotation matrix
	*  
	* \param rot The rotation angle in radians
	* \return Matrix4x4<T> The Z-rotation matrix
	*/		
	static Matrix4x4<T> makeRotateZ(const T& rot);
	
	/*!
	* \brief Creates a axle-rotation matrix
	* 
	* \param axle The direction of the axle
	* \param rot The rotation angle around the axle in radians
	* \return Matrix4x4<T> The axle-rotation matrix
	*/			
	static Matrix4x4<T> makeRotateArbitrary(const Vector3<T>& axle, 
			const T& rot);

	/*!
	* \brief Creates a look-at matrix
	* 
	* Creates a viewing matrix derived from a position, the point to look at
	* an the up vector
	* 
	* \param pos The position of the viewer
	* \param posLookAt The position to look at
	* \param worldUp The up vector of the viewer/cameara
	* \return Matrix4x4<T> The look-at matrix
	*/
	static Matrix4x4<T> makeLookAt(const Vector3<T>& pos, const Vector3<T>& posLookAt,
			const Vector3<T>& worldUp = Vector3<T>(0.0, 1.0, 0.0));

	/*!
	* \brief Creates a perspective projection matrix
	* 
	* Creates a viewing matrix with a speciefied viewing frustum
	* 
	* \param fovY The field of view angle in y-direction
	* \param aspect The aspect ratio width(x) to height(y)
	* \param nearPlane Distance to the near plane
	* \param farPlane Distance to the far plane 
	* \return Matrix4x4<T> The perspective projection matrix
	*/
	static Matrix4x4<T> makePerspectiveFov(const T& fovY, const T& aspect,
		const T& nearPlane, const T& farPlane);

	/*!
	* \brief Creates a perspective projection matrix
	* 
	* Creates a viewing matrix with a speciefied viewing frustum
	* 
	* \param left The coordinate of the left clipping plane
	* \param right The coordinate of the right clipping plane
	* \param bottom The coordinate of the bottom clipping plane
	* \param top The coordinate of the top clipping plane
	* \param nearPlane Distance to the near plane
	* \param farPlane Distance to the far plane 
	* \return Matrix4x4<T> The perspective projection matrix
	*/
	static Matrix4x4<T> makeFrustum(const T& left, const T& right,
		const T& bottom, const T& top, const T& nearPlane, const T& farPlane);

	/*!
	* \brief Creates a orthogonal projection matrix
	* 
	* Creates a viewing matrix for orghogonal projection
	* 
	* \param left X-coordinate for the left clipping plane
	* \param top Y-coordinate for the top clipping plane
	* \param right X-coordinate for the right clipping plane
	* \param bottom Y-coordinate for the bottom clipping plane
	* \param nearPlane Distance to the near plane
	* \param farPlane Distance to the far plane 
	* \return Matrix4x4<T> The orghogonal projection matrix
	*/
	static Matrix4x4<T> makeOrthogonal(const T& left, const T& top, 
			const T& right, const T& bottom, const T& nearPlane, const T& farPlane);
		
};
	
typedef Matrix4x4<double> Matrix;	
	
}

#include "Matrix4x4.inl"

#endif /*MATRIX4X4_H_*/
