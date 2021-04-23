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
 *  File:       Importer3DSLoader.h
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTER3DSLOADER_H_
#define IMPORTER3DSLOADER_H_

#include "./../../renderer/include/Renderer.h"
#include "Importer3DSTypes.h"

/*! \file
* 
*  \brief Describes the class Importer3DSLoader, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to load an 3DS-mesh
*
* The class loads an 3DS-mesh and stores the data into passed arrays
*/
class Importer3DSLoader
{
public:
	/*! \brief Constructor
	*/
	Importer3DSLoader()
	{
	};

	/*! \brief Destructor
	*/	
	virtual ~Importer3DSLoader()
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
	* \brief Returns the number of meshes
	* 
	* \return unsigned int The number of meshes
	*/			
	unsigned int getNumMesh() const
	{
		return m_mesh.size();
	}	
	
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
	* \param bNegateVTextureCoordinate If true, the v-texture-coordinate is negated
	*/	
	void getMesh(int nIndex, vector<int>& indices, vector<BSRenderer::Vertex>& verts,
		BSRenderer::Material& mat, const string& sTextureDirectory,
		bool bDisableLoadingTextures, bool bNegateVTextureCoordinate);

private:
	/*!
	* \brief Reads a sub chunk
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/
	void readSubChunk(ifstream& f, A3DS_CHUNK& chunk);

	/*!
	* \brief Move to the next chunk
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/
	void moveToNextChunk(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Returns the next string
	*
	* \param f The input file stream
	* \return string The next string
	*/	
	string getNextString(ifstream& f);
	
	/*!
	* \brief Reads a chunk
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readChunk(ifstream& f, A3DS_CHUNK& chunk);

	/*!
	* \brief Read a color
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	* \return A3DS_COLOR The read color
	*/
	A3DS_COLOR readColor(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the shininess
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readShininess(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the specular power
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readSpecularPower(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the transperency
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readTransperency(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the mesh materials
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/
	void readMeshMaterials(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the faces
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readFaces(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the vertices
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readVertices(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the smoothing groups
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readSmoothingGroups(ifstream& f, A3DS_CHUNK& chunk);
	
	/*!
	* \brief Reads the texture coordinates
	*
	* \param f The input file stream
	* \param chunk The chunk where to store the read data
	*/	
	void readTexCoords(ifstream& f, A3DS_CHUNK& chunk);

	/*!
	* \brief Sets up a specific (sub-)mesh
	* 
	* Returns a specific (sub-)mesh and stored the data into the passed arrays
	* 
	* \param nIndex The index of the mesh that should be returned
	* \param indices The array where to store the indices
	* \param verts The array where to store the vertices
	* \param mat Reference to store the material
	* \param sTextureDirectory The texture directory 
	* \param bDisableLoadingTextures If true, no textures are loaded. Otherwise texture are loaded normally
	* \param bNegateVTextureCoordinate If true, the v-texture-coordinate is negated
	*/
	void setUpMeshesAndMaterials(int nIndex, vector<int>& indices, vector<BSRenderer::Vertex>& verts,
		BSRenderer::Material& mat, const string& sTextureDirectory, bool bDisableLoadingTextures,
		bool bNegateVTextureCoordinate);

private:
	std::vector<A3DS_MESH> m_mesh;			/*!< \brief The mesh*/
	std::vector<A3DS_MATERIAL> m_materials; /*!< \brief The materials*/
};

#endif /*IMPORTER3DSLOADER_H_*/
