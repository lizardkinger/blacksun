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
 *  File:       ImporterOBJTypes.h
 *  Created:    08.04.2007 
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/


#ifndef IMPORTEROBJTYPES_H_
#define IMPORTEROBJTYPES_H_

#include "./../../renderer/include/Renderer.h"

/////////////// STRUCTS ///////////////

/*!
* \brief The OBJ vertex structure
*/
struct OBJ_VERTEX
{
    /*! \brief The default constructor
     */	
	OBJ_VERTEX() : x(0), y(0), z(0) {}
	
	float x; /*!< \brief X-Coordinate*/
	float y; /*!< \brief Y-Coordinate*/
	float z; /*!< \brief Z-Coordinate*/
};

/*!
* \brief The OBJ normal structure
*/
struct OBJ_NORMAL
{
    /*! \brief The default constructor
     */	
	OBJ_NORMAL() : nx(0), ny(0), nz(0) {}
	
	float nx; /*!< \brief X-Coordinate*/
	float ny; /*!< \brief Y-Coordinate*/
	float nz; /*!< \brief Z-Coordinate*/
};

/*!
* \brief The OBJ texture coordinate structure
*/
struct OBJ_TEXCOORD
{
    /*! \brief The default constructor
     */	
	OBJ_TEXCOORD() : tu(0), tv(0) {}
	
	float tu; /*!< \brief U-Coordinate*/
	float tv; /*!< \brief V-Coordinate*/
};

/*!
* \brief The OBJ face structure
*/
struct OBJ_FACE
{
    /*! \brief The default constructor
     */
	OBJ_FACE() : nNumVerts(0), nNumNormals(0), nNumTexCoords(0),
		verts(NULL), normals(NULL), texcoords(NULL) {}
		
	int nNumVerts; 		/*!< \brief The number of vertices*/
	int nNumNormals; 	/*!< \brief The number of normals*/
	int nNumTexCoords; 	/*!< \brief The number of texture coordinates*/
	
	int* verts; 	/*!< \brief Vertex array*/
	int* normals; 	/*!< \brief Normal array*/
	int* texcoords; /*!< \brief Texture coordinates array*/
};

/*!
* \brief The OBJ material structure
*/
struct OBJ_MATERIAL
{
    /*! \brief The default constructor
     */	
	OBJ_MATERIAL() : sName("Default"), fAlpha(1.0),
		fShininess(10.0), nIllumination(0) {}
		
	string sName; 		/*!< \brief Name*/
	float cAmbient[3]; 	/*!< \brief Ambient color*/
	float cDiffuse[3]; 	/*!< \brief Diffuse color*/
	float cSpecular[3]; /*!< \brief Specular color*/
	float fAlpha; 		/*!< \brief Alpha value*/
	float fShininess; 	/*!< \brief Shininess factor*/
	int nIllumination; 	/*!< \brief Illumination type*/
	string sTexName; 	/*!< \brief Texture file name*/
};

/*!
* \brief The OBJ group structure
*/
struct OBJ_GROUP
{
	string sName;			/*!< \brief Name*/
	vector<OBJ_FACE> faces; /*!< \brief The faces*/
	string sMaterial; 		/*!< \brief The material name*/
};

/*!
* \brief The OBJ mesh structure
*/
struct OBJ_MESH
{
	string sMaterialFile;			/*!< \brief Materal file name*/
	vector<OBJ_VERTEX> vertices;	/*!< \brief Array of vertices*/
	vector<OBJ_NORMAL> normals;		/*!< \brief Array of normals*/
	vector<OBJ_TEXCOORD> texcoords; /*!< \brief Array of texture coordinates*/
	vector<OBJ_GROUP> groups;		/*!< \brief Array of groups*/
};

#endif /*IMPORTEROBJTYPES_H_*/
