/***************************************************************************
 *   Copyright (C) 2006 by Thomas Kuhndörfer
 *   tkuhndo@fh-landshut.de
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
 *  Module:     Scenegraph (BlackSun)
 *  File:       SceneUtils.h
 *  Created:    10.05.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef SCENEUTILS_H_
#define SCENEUTILS_H_

/** \file SceneUtils.h
 * \brief Defines the sceneutil class.
 * 
 * \author Thomas Kuhndörfer.
 */


///////////////////////////////////////////////////////////
// Blacksun-Includes for Qt
///////////////////////////////////////////////////////////
#include <QtGui>
#include <QFile>
#include <QDataStream>

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./../../scenegraph/include/SceneObject.h"


namespace BSScene
{ 

/*! \brief The sceneutil object 
*
* A concrete class that have some shared functions
*/
class SceneUtils
{ 

public:

	/*!
	* \brief Creates a mathematical Plane object out of two vectors and a axis 
	* 
	* \param v1 Constructing vector of the plane number 1
	* \param v2 Constructing vector of the plane number 2
	* \param axis Axis of the view 
	*/		
	static Plane<double> create2DPlane(	const Vector& v1, 
										const Vector& v2,
										const SceneAxis axis);
	
	
	/*!
	* \brief Prints out debug information about a AABB
	* 
	* \param text Information about the context 
	* \param a The AABB itself
	* \param newline True if the line should printed on a new line
	*/		
	static void showAabb(	const QString &text,
							const Aabb& a,
							const  bool newline = false);
	
	
	/*!
	* \brief Prints out debug information about a vector
	* 
	* \param text Information about the context 
	* \param v The vector itself
	* \param newline True if the line should printed on a new line
	*/	
	static void showVec(	const QString &text,
						 	const Vector& v,  
						 	const bool newline = false);
	
  	/*!
	* \brief Expands a min and max vector according to a compare vector
	* 
	* \param comp The comparison vertex
	* \param vecMax The maximum vector
	* \param vecMin The minimum vector
	*/	
	static void 	compareMinMax(	const Vertex &comp,
									Vector &vecMax,
									Vector &vecMin) ;	
			
									
  	/*!
	* \brief Expands a min and max vector according to a compare vector
	* 
	* \param comp The comparison vector
	* \param vecMax The maximum vector
	* \param vecMin The minimum vector
	*/	
	static void 	compareMinMax(	const Vector &comp,
									Vector &vecMax,
									Vector &vecMin) ;	
									
	
	/*!
	* \brief Compares a min and a max vector and swap false maximums/minimums
	* 
	* \param vecMin The minimum vector
	* \param vecMax The maximum vector
	*/								
	static void getMinMax(Vector &vecMin,Vector &vecMax);															
};

}

#endif /*SCENEUTILS_H_*/
