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
 *  File:       ImporterMD2.h
 *  Created:    28.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTERMD2_H_
#define IMPORTERMD2_H_

#include "Importer.h"

/*! \file
* 
*  \brief Describes the class ImporterMD2, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to import a MD2-mesh
*
* The class imports a MD2-mesh to the scene
*/
class ImporterMD2: public Importer
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	ImporterMD2(QWidget* parent = 0)
	 : Importer(parent)
	{
	}
	
	/*!
	* \brief Returns the file extension which could be imported
	* 
	* \return string The file extension (for example md2, obj, ...)
	*/		
	virtual string getFileExtension() const
	{
		return "MD2";	
	}
	
	/*!
	* \brief Returns the file description of the format which could be imported
	* 
	* \return string The file extension description
	*/		
	virtual string getFileDescription() const
	{
		return "Quake II MD2";	
	} 

public slots:
	/*!
	* \brief Imports the mesh
	* 
	* Imports a mesh and adds it to the scene
	*/	
	virtual bool execute();
	
};

#endif /*IMPORTERMD2_H_*/

