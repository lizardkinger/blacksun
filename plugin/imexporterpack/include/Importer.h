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
 *  File:       Importer.h
 *  Created:    28.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTER_H_
#define IMPORTER_H_

#include "./../../editor/scenegraph/include/SceneGraph.h"
#include "./../../renderer/include/Renderer.h"
#include <QtGui>

/*! \file
* 
*  \brief Describes the interface Importer, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The interface for all importer
*
* This interface is used for all importer
*/
class Importer : public QWidget
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	Importer(QWidget* parent = 0)
	 : QWidget(parent)
	 {
	 }
	
	/*!
	* \brief Imports the mesh
	* 
	* Imports a mesh and adds it to the scene
	*/			
	virtual bool execute() = 0;

	/*!
	* \brief Returns the file extension which could be imported
	* 
	* \return string The file extension (for example md2, obj, ...)
	*/		
	virtual string getFileExtension() const = 0;
	
	/*!
	* \brief Returns the file description of the format which could be imported
	* 
	* \return string The file extension description
	*/	
	virtual string getFileDescription() const = 0;  	
};

#endif /*IMPORTER_H_*/

