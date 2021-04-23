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
 *  File:       ColorRGB.h
 *  Created:    24.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef COLORRGB_H_
#define COLORRGB_H_

#include "Math.h"
#include "Vector3.h"

/*! \file
* 
*  \brief Describes the Template-Class Color, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSMath
{

/* Forward declaration */	
template<class T>
class Vector3;

/*! \brief A template-class that represents a color
*
* It's a type independent color. All values must be between 0.0 and 1.0.
* The color is represented in the RGB(A) format for the red, green, blue
* and alpha components. 
*/		
template<class T>
class ColorRGB
{
public:

	union
	{
		T val[4];
		struct
		{
			T r, g, b, a;				
		};
	};
	
public:
	/*!
	* \brief The default constructor
	*/	
	ColorRGB()
	 : r(static_cast<T>(0.0)), g(static_cast<T>(0.0)), 
	   b(static_cast<T>(0.0)), a(static_cast<T>(1.0))
	{
	}
	
	/*!
	* \brief The copy-constructor
	* 
	* \param c The color which should be copied.
	*/	
	ColorRGB(const ColorRGB<T>& c)
	 : r(c.r), g(c.g), b(c.b), a(c.a)
	{
	}		
	
	/*!
	* \brief The constructor.
	* 
	* \param rgb The value for the red, green and blue color component
	* \param a The value for the alpha color component
	*/	
	ColorRGB(const T& rgb, const T& a = static_cast<T>(1.0))
	 : r(rgb), g(rgb), b(rgb), a(a)
	{	
	}	
	
	/*!
	* \brief The constructor.
	* 
	* \param _r The value for the red color component
	* \param _g The value for the green color component
	* \param _b The value for the blue color component  
	* \param _a The value for the alpha component
	*/			
	ColorRGB(const T& _r, const T& _g, const T& _b, const T& _a = static_cast<T>(1.0))
	 : r(_r), g(_g), b(_b), a(_a)
	{	
	}
		
	/*!
	* \brief The constructor.
	* 
	* \param v A vector to take the color components from  
	* \param _a The value for the alpha component
	*/		
	ColorRGB(const Vector3<T>& v, const T& _a = static_cast<T>(1.0))
	 : r(v.x), g(v.y), b(v.z), a(_a)
	{		
	}	
	
	/*!
	* \brief The constructor.
	* 
	* \param col A DWORD that stores the color
	*/		
	ColorRGB(const unsigned long& col)
	{
		set(col);			
	}
	
	/*!
	* \brief Set up the values of the color
	*
	* \param _r The red component of the color
	* \param _g The green component of the color
	* \param _b The blue component of the color
	* \param _a The alpha component of the color
	*/
	void set(const T& _r, const T& _g, const T& _b, const T& _a = static_cast<T>(1.0));
	
	/*!
	* \brief Set up the values of the color
	*
	* \param v A vector to take the color components from
	* \param _a The alpha component of the color
	*/		
	void set(const Vector3<T>& v, const T& _a = static_cast<T>(1.0));
	
	/*!
	* \brief Set up the values of the color
	*
	* \param col A DWORD that stores the color
	*/		
	void set(const unsigned long& col);

	/*!
	* \brief Inverts the color
	*
	* \param bInvertAlpha Specifies wheather the alpha component should be
	* 	inverted too 
	*/
	void invert(bool bInvertAlpha = false);
	
	/*!
	* \brief Returns the invert of this color
	*
	* \param bInvertAlpha Specifies wheather the alpha component should be
	* 	inverted too 
	* \return ColorRGB<T> The invert of the color
	*/		
	ColorRGB<T> getInvert(bool bInvertAlpha = false) const;
	
	/*!
	* \brief Returns the luminance
	*
	* \return T The luminance 
	*/		
	T luminance() const
	{
		return (r*0.39 + g*0.5 + b*0.11); 
	}

	/*!
	* \brief Returns the DWORD value of the color
	*
	* \return T The DWORD value of the color
	*/		
	unsigned long getDWORD() const;

	/*!
	* \brief Set up the values of the color from a DWORD
	*
	* \param col The DWORD that stores the color 
	*/			
	void fromDWORD(const unsigned long& col)
	{
		set(col);
	}

	/*!
	* \brief Returns an array of 4 floats, for example used for some OpenGl-functions
	*
	* This function returns an array of 4 floats. It will be used for OpenGl-functions
	* like glMaterial, which doesn't support double parameter. So, if this color
	* should be a Color<double>, this functions returns the float-values.
	*  
	* \param pArr The (already allocated) array to store the float values
	*/		
	void getFloatArray(float* pArr);
	
//	void fromHSV(const T& h, const T& s, const T& v, const T& a = 1.0);
//	void fromCMYK(const T& c, const T& m, const T& y, const T& k, const T& a = 1.0);
	
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
	operator const T* () const
	{
		return val;
	}
	
	
	/*! 
	* \brief Comparison operator 
	*/
	bool operator == (const ColorRGB<T>& c) const
	{
		return ((r==c.r) && (g==c.g) && (b==c.b));
	}
	
	/*!
	* \brief Comparison operator 
	*/		
	bool operator != (const ColorRGB<T>& c) const
	{
		return !((r==c.r) && (g==c.g) && (b==c.b));
	}

	/*!
	* \brief Assign operator 
	*/								
	ColorRGB<T>& operator =  (const ColorRGB<T>& c);
	
	/*!
	* \brief Assign operator for += 
	*/								
	ColorRGB<T>& operator += (const ColorRGB<T>& c);
	
	/*!
	* \brief Assign operator for -= 
	*/								
	ColorRGB<T>& operator -= (const ColorRGB<T>& c);
	
	/*!
	* \brief Assign operator for *= 
	*/								
	ColorRGB<T>& operator *= (const ColorRGB<T>& c);
	
	/*!
	* \brief Assign operator for *= 
	*/								
	ColorRGB<T>& operator *= (const T& s);
	
	/*!
	* \brief Assign operator for /= 
	*/								
	ColorRGB<T>& operator /= (const ColorRGB<T>& c);
	
	/*!
	* \brief Assign operator for /= 
	*/								
	ColorRGB<T>& operator /= (const T& s);

	/*!
	* \brief Unary operator 
	*/			
	ColorRGB<T> operator + () const
	{
		return (*this);
	}
	
	/*!
	* \brief Unary operator 
	*/			
	ColorRGB<T> operator - () const
	{
		return ColorRGB<T>(-r, -g, -b);
	}
	
	/*!
	* \brief Binary operator for addition 
	*/		
	ColorRGB<T> operator + (const ColorRGB<T>& c) const
	{
		return ColorRGB<T>(r+c.r, g+c.g, b+c.b);
	}
	
	/*!
	* \brief Binary operator for subtraction 
	*/				
	ColorRGB<T> operator - (const ColorRGB<T>& c) const
	{
		return ColorRGB<T>(r-c.r, g-c.g, b-c.b);
	}
	
	/*!
	* \brief Binary operator for multiplication 
	*/				 
	ColorRGB<T> operator * (const ColorRGB<T>& c) const
	{
		return ColorRGB<T>(r*c.r, g*c.g, b*c.b);
	}
	
	/*!
	* \brief Binary operator for multiplication 
	*/				 
	ColorRGB<T> operator * (const T& s) const
	{
		return ColorRGB<T>(r*s, g*s, b*s);
	}

	/*!
	* \brief Binary operator for devision 
	*/				
	ColorRGB<T> operator / (const ColorRGB<T>& c) const
	{
		return ColorRGB<T>(r/c.r, g/c.g, b/c.b);
	}

	/*!
	* \brief Binary operator for devision 
	*/				
	ColorRGB<T> operator / (const T& s) const
	{
		return ColorRGB<T>(r/s, g/s, b/s);
	}
	
	/*!
	* \brief Cast to DWORD 
	*/			
	operator unsigned long () const
	{
		return getDWORD();
	}		
	
	/*!
	* \brief Friend binary operator for multiplication
	*/			
	friend ColorRGB<T> operator * (const T& val, const ColorRGB<T>& c) { return ColorRGB<T>(val*c.r, val*c.g, val*c.b); }
	
	/*!
	* \brief Calculates a color with the minimum values of two colors. The
	* 	values are compared component-wise
	*
	* \param c1 The first color to compare for minimum calculation
	* \param c2 The second color to compare for minimum calculation
	* \return ColorRGB<T> The color with the minimum values
	*/			
	static ColorRGB<T> min(const ColorRGB<T>& c1, const ColorRGB<T>& c2);
	
	/*!
	* \brief Calculates a color with the maximum values of two colors. The
	* 	values are compared component-wise
	*
	* \param c1 The first color to compare for maximum calculation
	* \param c2 The second color to compare for maximum calculation
	* \return ColorRGB<T> The color with the maximum values
	*/			
	static ColorRGB<T> max(const ColorRGB<T>& c1, const ColorRGB<T>& c2);
	
	/*!
	* \brief Returns a random color
	*
	* \param bRandomAlpha Specifies wheather the alpha component should be
	* 	random too. Elsewhere, the alpha component is 1.0
	* \return ColorRGB<T> The random color
	*/			
	static ColorRGB<T> random(bool bRandomAlpha = false);
	
	/*!
	* \brief Returns an interpolated color
	*
	* Interpolates linear beween two colors with a specified factor. For 
	* example c1 = (0.0, 0.5, 0.2, 1.0) and c2 = (1.0, 0.9, 0.8, 1.0):\n
	* Factor f=0.0 : Result = c1\n
	* Factor f=1.0 : Result = c2\n
	* Factor f=0.5 : Result = Average of c1 and c2 = (0.5, 0.7, 0.5, 1.0)
	* 
	* \param c1 The first color to interpolate from
	* \param c2 The second color to interpolate from
	* \param f The interpolation factor
	* \return ColorRGB<T> The interpolated color
	*/			 		
	static ColorRGB<T> interpolateLinear(const ColorRGB<T>& c1, const ColorRGB<T>& c2, const T f)
	{
		return c1 + f * (c2 - c1);
	}	

public:
	static ColorRGB<T> white; 		 /*!< \brief Predefined white color */
	static ColorRGB<T> black; 		 /*!< \brief Predefined black color */
	static ColorRGB<T> red; 		 /*!< \brief Predefined red color */
	static ColorRGB<T> darkRed; 	 /*!< \brief Predefined dark red color */
	static ColorRGB<T> green; 		 /*!< \brief Predefined green color */
	static ColorRGB<T> darkGreen; 	 /*!< \brief Predefined dark green color */
	static ColorRGB<T> blue; 		 /*!< \brief Predefined blue color */
	static ColorRGB<T> darkBlue; 	 /*!< \brief Predefined dark blue color */
	static ColorRGB<T> cyan; 		 /*!< \brief Predefined cyan color */
	static ColorRGB<T> darkCyan; 	 /*!< \brief Predefined dark cyan color */
	static ColorRGB<T> magenta; 	 /*!< \brief Predefined magenta color */
	static ColorRGB<T> darkMagenta; /*!< \brief Predefined dark magenta color */
	static ColorRGB<T> yellow; 	 /*!< \brief Predefined yellow color */
	static ColorRGB<T> darkYellow;  /*!< \brief Predefined dark yellow color */
	static ColorRGB<T> gray; 		 /*!< \brief Predefined gray color */
	static ColorRGB<T> darkGray; 	 /*!< \brief Predefined dark gray color */
	static ColorRGB<T> lightGray; 	 /*!< \brief Predefined light gray color */
};

typedef ColorRGB<double> Color;
	
}

#include "ColorRGB.inl"

#endif /*COLORRGB_H_*/
