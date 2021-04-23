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
 *  File:       ImporterOBJLoader.h
 *  Created:    08.04.2007 
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTEROBJLOADER_H_
#define IMPORTEROBJLOADER_H_

#include "./../../renderer/include/Renderer.h"
#include "ImporterOBJTypes.h"

/*! \file
* 
*  \brief Describes the class ImporterOBJLoader, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to load an OBJ-mesh
*
* The class loads an OBJ-mesh and stores the data into passed arrays
*/
class ImporterOBJLoader
{
public:
	/*! \brief Constructor
	* 
	* \param sMatDir The directory where the materials are stored 
	*/
	ImporterOBJLoader(const string& sMatDir)
	 : m_sMatDirectory(sMatDir)
	{
	}
	
	/*! \brief Destructor
	*/
	virtual ~ImporterOBJLoader()
	{
		release();	
	}
	
	/*! \brief Releases the mesh
	*/	
	void release();
	
	/*!
	* \brief Loads the mesh
	* 
	* \param sFileName The file name
	*/		
	bool execute(const string& sFileName);

	/*!
	* \brief Returns the number of materials
	* 
	* \return unsigned int The number of materials
	*/			
	unsigned int getNumMaterials() const
	{
		return m_materials.size();
	}
	
	/*!
	* \brief Returns a specific (sub-)mesh
	* 
	* Returns a specific (sub-)mesh and stored the data into the passed arrays
	* 
	* \param nIndex The index of the mesh that should be returned
	* \param indices The array where to store the indices
	* \param verts The array where to store the vertices
	* \param mat Reference to store the material
	* \param sTextureDirectory The texture directory
	* \param bDisableLoadingTextures If true, no textures are loaded. Otherwise texture are loaded normally
	*/	
	void getMesh(int nIndex, vector<int>& indices, vector<BSRenderer::Vertex>& verts,
		BSRenderer::Material& mat, const string& sTextureDirectory, bool bDisableLoadingTextures);

private:
	/*!
	* \brief Reads the vertex data
	* 
	* \param s The line where to load the vertex data
	*/
	void readVertexData(string& s);

	/*!
	* \brief Reads the face data
	* 
	* \param s The line where to load the face data
	*/
	void readFaceData(string& s);
	
	/*!
	* \brief Reads the material name
	* 
	* \param s The line where to load the material name
	*/	
	void readMaterialName(string& s);
	
	/*!
	* \brief Loads materials from a material file
	* 
	* \param s The file name
	*/	
	void loadMaterials(string& s);

	/*!
	* \brief Set up a specific (sub-)mesh
	* 
	* Returns a specific (sub-)mesh and stored the data into the passed arrays
	* 
	* \param nIndex The index of the mesh that should be returned
	* \param indices The array where to store the indices
	* \param verts The array where to store the vertices
	* \param mat Reference to store the material
	* \param sTextureDirectory The texture directory
	* \param bDisableLoadingTextures If true, no textures are loaded. Otherwise texture are loaded normally
	*/	
	void setUpMeshesAndMaterials(int nIndex, vector<int>& indices, vector<BSRenderer::Vertex>& verts,
		BSRenderer::Material& mat, const string& sTextureDirectory, bool bDisableLoadingTextures);	

	/*!
	* \brief Splits a string into tokens
	* 
	* \param s The string that should be splitted
	* \param c1 The first seperator character
	* \param c2 The second seperator character
	* \param res The array where to store the tokens
	*/			
	void split(const string& s, char c1, char c2, vector<string>& res);
	
	/*!
	* \brief Converts a string to a upper case string
	* 
	* \param s The string that should be converted
	* \return string The converted string
	*/		
	string toUpperCase(const string& s);
	
	/*!
	* \brief Returns the number of face data stored in the string
	* 
	* \param s The string
	* \return int The number of face data stored in the string
	*/	
	int getNumFaceData(string& s);
	
	/*!
	* \brief Creates a vertex
	* 
	* \param f The face
	* \param nVertID The vertex ID
	* \return Vertex The converted vertex
	*/		
	BSRenderer::Vertex createVertex(OBJ_FACE& f, int nVertID);

private:
	OBJ_MESH m_mesh; 					/*!< \brief The mesh*/
	vector<OBJ_MATERIAL> m_materials; 	/*!< \brief The materials*/
	string m_sMatDirectory; 			/*!< \brief The material directory*/

};

#endif /*IMPORTEROBJLOADER_H_*/
