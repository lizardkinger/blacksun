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
 *  File:       MathConst.h
 *  Created:    26.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATHCONST_H_
#define MATHCONST_H_

/*! \file
* 
*  \brief Collects important algebraical constants
*  \author Reinhard Jeschull
* 
*/

namespace BSMath
{

const double EPSILON = 0.000001;                /*!< \brief A small number*/ 
const double E 		 = 2.71828182845904523536;  /*!< \brief Euler constant*/ 
const double LOG2_E  = 1.44269504088896340736;  /*!< \brief Base 2 logarithm of the Euler constant*/ 

const double LOG10_E = 0.434294481903251827651; /*!< \brief Base 10 logarithm of the Euler constant*/ 
const double LOGE_2  = 0.693147180559945309417; /*!< \brief Natural logarithm of 2*/ 
const double LOGE_10 = 2.30258509299404568402;  /*!< \brief Natural logarithm of 10*/ 
const double PI 	 = 3.14159265358979323846;  /*!< \brief Pi*/ 
const double SQRT_2  = 1.41421356237309504880;	/*!< \brief Squareroot of 2*/ 

}

#endif /*MATHCONST_H_*/
