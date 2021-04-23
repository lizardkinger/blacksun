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
 *  File:       SceneAction2D.h
 *  Created:    13.05.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/


#ifndef SCENEACTION2D_H_
#define SCENEACTION2D_H_

/** \file SceneAction2D.h
 * \brief Defines the SceneAction2D class.
 * 
 * \author Thomas Kuhndörfer.
 */

///////////////////////////////////////////////////////////
// Includes for Qt
///////////////////////////////////////////////////////////
#include <QtGui>


namespace BSScene
{ 



///////////////////////////////////////////////////////////
// Enumerations
//////////////////////////////////////////////////////////

/*!
* \brief Defines the type of a selection done
*/	
enum SceneAction2D_SelectionType 	
{ 	
	SCENETYPE_Select, /*!< Add a single object to the selection but clean all previous selections  */
	
	SCENETYPE_AddToSelection, /*!< Add a single object to the existing selection */

	SCENETYPE_RemoveFromSelection /*!< Removes a selection region from the existing selection */
};

/*! \brief The SceneAction2D object 
*
* A concrete class for collecting information about a 2D action
*/
class SceneAction2D
{ 

public:

	/*!
	* \brief Calculates a vector from a source and target point
	* 
	* \param source Source point of the new vector
	* \param target Target point of the new vector
	* \param axis Axis information for filling the right vector components
	* 
	* \return The relative distance vector
	*/		
	static Vector getGUIDistance(const QPointF& source,const  QPointF& target,const  SceneAxis axis)
	{
		double val1 = target.x() - source.x();
		double val2 = target.y() - source.y();;
		Vector vecMove;
		
		switch(axis)
		{
			case AXIS_x:	
				vecMove = Vector(0,val2,val1);
				break;
			
			case AXIS_y:	
				vecMove = Vector(val1,0,-val2) ;
				break;		
			
			case AXIS_z:
				vecMove = Vector(val1,val2,0) ;
				break;		
		}
		
		return vecMove;
	}


	QPointF topleft; /*!< \brief Extremal point of a selection rectangle */   
	QPointF bottomright;  /*!< \brief Extremal point of a selection rectangle */   
	SceneAxis axis; /*!< \brief The axis the action took place */   
	Qt::MouseButton mouseButton; /*!< \brief The mouse button that created the action */   
	double scaleValueX; /*!< \brief Scale value in the X direction for scale action */   
	double scaleValueY; /*!< \brief Scale value in the Y direction for scale action */ 
	double scaleValueZ; /*!< \brief Scale value in the Z direction for scale action */ 
	double rotateValue; /*!< \brief Rotate value in degrees for rotate action */ 
	SceneAction2D_SelectionType selType; /*!< \brief Tells about the selection type */ 
};

}

#endif /*SCENEACTION2D_H_*/
