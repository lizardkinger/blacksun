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
 *  File:       ImporterMD2Loader.h
 *  Created:    28.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMPORTERMD2LOADER_H_
#define IMPORTERMD2LOADER_H_

#include "./../../renderer/include/Renderer.h"
#include "ImporterMD2Types.h"

/*! \file
* 
*  \brief Describes the class ImporterMD2Loader, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The class to load a MD2-mesh
*
* The class loads a MD2-mesh and stores the data into passed arrays
*/
class ImporterMD2Loader
{
public:
	/*! \brief Constructor
	*/
	ImporterMD2Loader() {};
	
	/*! \brief Deonstructor
	*/
	virtual ~ImporterMD2Loader() {};
	
	/*!
	* \brief Loads the mesh
	* 
	* \param sFileName The file name
	* \param indices The array where to store the loaded indices
	* \param verts The array where to store the loaded vertices
	*/	
	virtual bool execute(const string& sFileName, vector<int>& indices, 
		vector<BSRenderer::Vertex>& verts);

private:
	/*!
	* \brief Reads a header from the file
	* 
	* \param f The input file stream
	* \param model The structure where to store the read data
	*/	
	bool readHeader(ifstream& f, MD2_MODELL& model);

	/*!
	* \brief Reads a skin from the file
	* 
	* \param f The input file stream
	* \param model The structure where to store the read data
	*/	
	void readSkin(ifstream& f, MD2_MODELL& model);

	/*!
	* \brief Reads a texture coordinate from the file
	* 
	* \param f The input file stream
	* \param model The structure where to store the read data
	*/	
	void readTexCoord(ifstream& f, MD2_MODELL& model);

	/*!
	* \brief Reads a triangle from the file
	* 
	* \param f The input file stream
	* \param model The structure where to store the read data
	*/	
	void readTriangle(ifstream& f, MD2_MODELL& model);

	/*!
	* \brief Reads frames and vertices from the file
	* 
	* \param f The input file stream
	* \param model The structure where to store the read data
	*/	
	void readFramesAndVertices(ifstream& f, MD2_MODELL& model);

	/*!
	* \brief Sets up the animation frames
	*
	* \param model The mesh
	*/	
	void setUpFrames(MD2_MODELL& model);
	
	/*!
	* \brief Creates the vertices from the animation
	* 
	* \param model The structure where the model data is stored
	* \param indices The array where to store the calculated indices
	* \param verts The array where to store the caluclated vertices
	* \param nAnimationKey The animation key
	* \param nFrame The frame number
	*/		
	void generateVertices(MD2_MODELL& model, vector<int>& indices, 
		vector<BSRenderer::Vertex>& verts, int nAnimationKey, int nFrame); 
	
	/*!
	* \brief Release the mesh
	* 
	* \param model The mesh that should be released
	*/			
	void releaseModel(MD2_MODELL& model); 	
};

#endif /*IMPORTERMD2LOADER_H_*/

