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
 *  File:       Util.h
 *  Created:    25.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef UTIL_H_
#define UTIL_H_

#include "Math.h"

/*! \file
* 
*  \brief Provides some util functions
* 
*/

/*! \brief The algebraical core of the editor
*
* The algebraical core of the editor. It provides all classes to handle 
* confortable with all important mathematical objects used for 3D calculations
* and rendering.
*/
namespace BSMath
{
	
/*!
* \brief Generates a random decimal number.
* 
* Generates a random decimal number in a specified limit.  
* A decimal number in the interval [min, max] is returned.\n	  
* Warning: When a none-decimal datatype is passed for the limits, a 
* none-decimal datatyp will be the result. 
*
* \param min The lower limit of the generated number
* \param max The upper limit of the generated number
* \return T A random decimal number in the passed limits
*/	
template<class T> T randomDecimal(const T& min, const T& max);

/*!
* \brief Generates a random integer number.
* 
* Generates a random integer number in a specified limit.  
* A decimal number in the interval [min, max] is returned.\n	* 
*
* \param min The lower limit of the generated number
* \param max The upper limit of the generated number
* \return T A random integer number in the passed limits
*/		
template<class T> T randomInteger(const T& min, const T& max);

/*!
* \brief Converts an angle from degrees in radians 
*
* \param deg The angle which should be converted
* \return T The angle in radians
*/	
template<class T> T degToRad(const T& deg);

/*!
* \brief Converts an angle from radians in degrees 
*
* \param rad The angle which should be converted
* \return T The angle in degrees
*/		
template<class T> T radToDeg(const T& rad);

/*!
* \brief Checks whether a number is a pow of two 
*
* \param n The number which should be checked
* \return bool True, if the passed number is a pow of two, otherwise false
*/		
template<class T> bool isPow2(const T& n);

/*!
* \brief Returns the next pow of two of a number 
*
* \param n The number which next pow of two should be calculated
* \return unsigned long The next pow of two of the passed number. If the 
* 	passed number is a pow of two, the same number will be returned.
*/	
template<class T> unsigned long nextPow2(const T& n);

/*!
* \brief Returns the previous pow of two of a number 
*
* \param n The number which previous pow of two should be calculated
* \return unsigned long The previous pow of two of the passed number. If the 
* 	passed number is a pow of two, the same number will be returned.
*/		
template<class T> unsigned long prevPow2(const T& n);

/*!
* \brief Returns the cubic root a number 
*
* \param n The number which cubic root should be calculated
* \return T The cubic root of the number
*/	
template<class T> T cubicRoot(const T& n);	

/*!
* \brief Checks wheather two numbers are nearly equal. 
* 
* Checks wheather two numbers are nearly equal. A difference of
* EPSILON and smaller is allowed, all over will mean, that the two numbers
* are not equal.
*
* \param a The first number to compare
* \param b The second number to compare
* \return True, if the numbers are nearly the same (difference smaller or
*  equal than EPSILON). Otherwise false is returned.
*/	
template<class T> bool isNearTo(const T& a, const T& b);

/*!
* \brief Checks wheather two numbers are nearly equal. 
* 
* Checks wheather two numbers are nearly equal. A difference of
* a passed delta and smaller is allowed, all over will mean, that the two 
* numbers are not equal.
*
* \param a The first number to compare
* \param b The second number to compare
* \param delta The accuracy of the comparison
* \return True, if the numbers are nearly the same (difference smaller or
*  equal than delta). Otherwise false is returned.
*/		
template<class T> bool isNearTo(const T& a, const T& b, const T& delta);

/*!
* \brief Returns the decade of a number 
* 
* Returns the decade of a number: For example:\n
* 0.00123 => 0.001
* 483 => 100
* -1095 => 1000
*
* \param d The value
* \return The decade of the value (always positive)
*/		
template<class T> double calcDecade(const T& d);
		
}

#include "Util.inl"

#endif /*UTIL_H_*/
