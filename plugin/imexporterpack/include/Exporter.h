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
 *  Module:     Plugin (BlackSun)
 *  File:       Exporter.h
 *  Created:    09.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef EXPORTER_H_
#define EXPORTER_H_

#include "./../../editor/scenegraph/include/SceneGraph.h"
#include "./../../renderer/include/Renderer.h"
#include <QtGui>

/*! \file
* 
*  \brief Describes the interface Exporter, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The interface for all exporter
*
* This interface is used for all exporter
*/
class Exporter : public QWidget
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	Exporter(QWidget* parent = 0)
	 : QWidget(parent)
	 {
	 }
	
	/*!
	* \brief Exports the mesh
	* 
	* Exports all meshes of the scene
	*/			
	virtual bool execute() = 0;

	/*!
	* \brief Returns the file extension which could be exported
	* 
	* \return string The file extension (for example md2, obj, ...)
	*/		
	virtual string getFileExtension() const = 0;
	
	/*!
	* \brief Returns the file description of the format which could be exported
	* 
	* \return string The file extension description
	*/	
	virtual string getFileDescription() const = 0; 		
};

#endif /*EXPORTER_H_*/

