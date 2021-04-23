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
 *  File:       ImporterOBJ.h
 *  Created:    08.04.2007 
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTEROBJ_H_
#define IMPORTEROBJ_H_



#include "Importer.h"
#include "./../../renderer/include/Renderer.h"
#include "../ui_ImporterOBJDialog.h"

/*! \file
* 
*  \brief Describes the class ImporterOBJ, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to import a OBJ-mesh
*
* The class imports a OBJ-mesh to the scene
*/
class ImporterOBJ: public Importer
{
	Q_OBJECT
	
public:	
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/	
	ImporterOBJ(QWidget* parent = 0);
	
	/*!
	* \brief Returns the file extension which could be imported
	* 
	* \return string The file extension (for example md2, obj, ...)
	*/	
	virtual string getFileExtension() const
	{
		return "OBJ";	
	}
	
	/*!
	* \brief Returns the file description of the format which could be imported
	* 
	* \return string The file extension description
	*/	
	virtual string getFileDescription() const
	{
		return "Lightwave OBJ";	
	}

public slots:
	/*!
	* \brief Imports the mesh
	* 
	* Imports a mesh and adds it to the scene
	*/	
	virtual bool execute();

private slots:
	/*! \brief Slot for selecting the file name
	*/
	void selectFileName();
	
	/*! \brief Slot for selecting the texture directory
	*/
	void selectTextureDirectory();

	/*! \brief Slot for selecting the material directory
	*/
	void selectMaterialDirectory();

private:
	/*!
	* \brief Scales the mesh to a specific maximum and center it
	* 
	* \param verts The groups of vertices of the mesh
	* \param dSize The max. dimension of the mesh
	* \param dCenter If true, the mesh is transformed to the center, otherwise it's not
	*   transformed
	*/
	void setMaxSizeAndCenter(vector< vector<BSRenderer::Vertex> >& verts, 
		double dSize, bool dCenter) const;
		
private:
	Ui_ImporterOBJDialog m_ui; /*!< \brief The UI-Object*/
		
};

#endif /*IMPORTEROBJ_H_*/
