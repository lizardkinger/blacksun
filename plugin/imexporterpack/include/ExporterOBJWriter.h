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
 *  File:       ExporterOBJWriter.h
 *  Created:    09.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef EXPORTEROBJWRITER_H_
#define EXPORTEROBJWRITER_H_

#include "./../../renderer/include/Renderer.h"

/*! \file
* 
*  \brief Describes the class ExporterOBJWriter, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to write the mesh to OBJ
*
* The class exports the passed data of the scene to a OBJ-file
*/
class ExporterOBJWriter
{
public:
	/*! \brief Constructor
	*/
	ExporterOBJWriter() {};

	/*! \brief Exports the data to OBJ
	*
	* \param sFileName The file name
	* \param verts The groups of vertices that should be exported
	* \param indices The groups of indices that should be exported
	* \param materials The materials that should be exported 
	*/	
	bool execute(const string& sFileName, vector< vector<BSRenderer::Vertex> >& verts,
		vector< vector<int> >& indices, vector<BSRenderer::Material*>& materials);
		
private:
	/*! \brief Writes vertices into the file
	*
	* \param f The output file stream
	* \param verts The vertices that should be written 
	*/
	void writeVertices(ofstream& f, vector<BSRenderer::Vertex>& verts);

	/*! \brief Writes groups into the file
	*
	* \param f The output file stream
	* \param indices The groups of indices that should be written
	* \param materials The materials that should be written 
	*/
	void writeGroups(ofstream& f, vector< vector<int> >& indices, 
		vector<BSRenderer::Material*>& materials);
		
	/*! \brief Writes a material file
	*
	* \param sFileName The file name
	* \param materials The materials that should be written 
	*/		
	void writeMaterialFile(const string& sFileName, vector<BSRenderer::Material*>& materials);		

};

#endif /*EXPORTEROBJWRITER_H_*/
