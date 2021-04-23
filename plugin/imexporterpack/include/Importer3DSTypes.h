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
 *  File:       Importer3DSTypes.h
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/


#ifndef IMPORTER3DSTYPES_H_
#define IMPORTER3DSTYPES_H_

#include "./../../renderer/include/Renderer.h"

/////////////// CONSTANTS ///////////////
const int A3DS_PRIMARY_ID 	= 0x4D4D;
const int A3DS_EDIT3DS 		= 0x3D3D;
const int A3DS_KEYF3DS 		= 0xB000;

const int A3DS_VERSION 			= 0x0002;
const int A3DS_MESH_VERSION 	= 0x3D3E;
const int A3DS_KFVERSION 		= 0x0005;
const int A3DS_COLOR_F 			= 0x0010;
const int A3DS_COLOR_24 		= 0x0011;
const int A3DS_LIN_COLOR_24 	= 0x0012;
const int A3DS_LIN_COLOR_F 		= 0x0013;
const int A3DS_INT_PERCENTAGE 	= 0x0030;
const int A3DS_FLOAT_PERC 		= 0x0031;
const int A3DS_MASTER_SCALE 	= 0x0100;
const int A3DS_IMAGE_FILE 		= 0x1100;
const int A3DS_AMBIENT_LIGHT 	= 0X2100;

const int A3DS_NAMED_OBJECT 		= 0x4000;
const int A3DS_OBJ_MESH 			= 0x4100;
const int A3DS_MESH_VERTICES 		= 0x4110;
const int A3DS_VERTEX_FLAGS 		= 0x4111;
const int A3DS_MESH_FACES 			= 0x4120;
const int A3DS_MESH_MATER 			= 0x4130;
const int A3DS_MESH_TEX_VERT 		= 0x4140;
const int A3DS_MESH_SMOOTH_GROUP 	= 0x4150;
const int A3DS_MESH_XFMATRIX 		= 0x4160;
const int A3DS_MESH_COLOR_IND 		= 0x4165;
const int A3DS_MESH_TEX_INFO 		= 0x4170;
const int A3DS_HIERARCHY 			= 0x4F00;

const int A3DS_MAT 					= 0xAFFF;
const int A3DS_MAT_NAME 			= 0xA000;
const int A3DS_MAT_AMBIENT 			= 0xA010;
const int A3DS_MAT_DIFFUSE 			= 0xA020;
const int A3DS_MAT_SPECULAR 		= 0xA030;
const int A3DS_MAT_SHININESS 		= 0xA040;
const int A3DS_MAT_SPECULARPOWER 	= 0xA041;
const int A3DS_MAT_TRANSPERENCY 	= 0xA050;
const int A3DS_MAT_FALLOFF 			= 0xA052;
const int A3DS_MAT_EMISSIVE 		= 0xA080;
const int A3DS_MAT_SHADER 			= 0xA100;
const int A3DS_MAT_TEXMAP 			= 0xA200;
const int A3DS_MAT_TEXFILENAME 		= 0xA300;

const int A3DS_OBJ_LIGHT 	= 0x4600;
const int A3DS_OBJ_CAMERA 	= 0x4700;

const int A3DS_ANIM_HEADER 	= 0xB00A;
const int A3DS_ANIM_OBJ 	= 0xB002;
const int A3DS_ANIM_NAME 	= 0xB010;
const int A3DS_ANIM_POS 	= 0xB020;
const int A3DS_ANIM_ROT 	= 0xB021;
const int A3DS_ANIM_SCALE 	= 0xB022;

/////////////// STRUCTS ///////////////

/*!
* \brief The Autodesk 3DS chunk header structure
*/
struct A3DS_CHUNK
{
	/*! \brief The default constructor
	 */	
	A3DS_CHUNK() : ID(0), nLength(0), nBytesRead(0) {}
	
	short ID; 					/*!< \brief Chunk ID*/
	unsigned int nLength; 		/*!< \brief Length in bytes*/
	unsigned int nBytesRead; 	/*!< \brief Read bytes*/
};

/*!
* \brief The Autodesk 3DS vertex structure
*/
struct A3DS_VERTEX
{
	/*! \brief The default constructor
     */	
	A3DS_VERTEX() : x(0), y(0), z(0) {}
	
	float x; /*!< \brief X-Coordinate*/
	float y; /*!< \brief Y-Coordinate*/
	float z; /*!< \brief Z-Coordinate*/
};

/*!
* \brief The Autodesk 3DS triangle structure
*/
struct A3DS_FACE
{
	/*! \brief The default constructor
     */	
	A3DS_FACE() : nMatID(0) { nIndices[0] = nIndices[1] = nIndices[2] = 0; }
   
	unsigned int nIndices[3]; 		/*!< \brief Face indices*/
	A3DS_VERTEX vNormal;  			/*!< \brief Normal*/
	int nMatID; 					/*!< \brief Material ID*/
	unsigned long nSmoothingGroup; 	/*!< \brief Smoothing group ID*/
};

/*!
* \brief The Autodesk 3DS face structure
*/
struct A3DS_FILEFACE
{
	/*! \brief The default constructor
	*/	
	A3DS_FILEFACE() : nVis(0) { nIndices[0] = nIndices[1] = nIndices[2] = 0; }
	
	unsigned short nIndices[3]; /*!< \brief Indices*/
	unsigned short nVis;	 /*!< \brief nVis*/
};

/*!
* \brief The Autodesk 3DS color structure
*/
struct A3DS_COLOR
{
	/*! \brief The default constructor
	*/	
	A3DS_COLOR() : r(0), g(0), b(0) {}
	
	unsigned char r; /*!< \brief Red value*/
	unsigned char g; /*!< \brief Green value*/
	unsigned char b; /*!< \brief Blue value*/
};

/*!
* \brief The Autodesk 3DS texture coordinate structure
*/
struct A3DS_TEXCOORD
{
	/*! \brief The default constructor
	*/	
	A3DS_TEXCOORD() : tu(0), tv(0) {}
   
	float tu; /*!< \brief U-Coordinate*/
	float tv; /*!< \brief V-Coordinate*/
};

/*!
* \brief The Autodesk 3DS material structure
*/
struct A3DS_MATERIAL
{
	string sName; 			/*!< \brief Name*/
	A3DS_COLOR cDiffuse; 	/*!< \brief Diffuse color*/
	A3DS_COLOR cAmbient; 	/*!< \brief Ambient color*/
	A3DS_COLOR cSpecular; 	/*!< \brief Specular color*/
	A3DS_COLOR cEmissive; 	/*!< \brief Emissive color*/
	float fShininess; 		/*!< \brief Shininess factor*/
	float fTransparency; 	/*!< \brief Transparency factor*/
	float fSpecularPower; 	/*!< \brief Specular power*/
	string sTextureName; 	/*!< \brief Texture file name*/
};

/*!
* \brief The Autodesk 3DS mesh structure
*/
struct A3DS_MESH
{
	/*! \brief The default constructor
	*/	
	A3DS_MESH() : pFaces(0), pVertices(0), pTexCoords(0),
              nTotalFaces(0), nTotalVertices(0), nTotalTexCoords(0) {}

	string sName; /*!< \brief Name*/

	A3DS_FACE *pFaces;  		/*!< \brief Array of faces*/
	A3DS_VERTEX *pVertices;  	/*!< \brief Array of vertices*/
	A3DS_TEXCOORD *pTexCoords;  /*!< \brief Array of texture coordinates*/

	unsigned int nTotalFaces;  		/*!< \brief Number of faces*/
	unsigned int nTotalVertices;  	/*!< \brief Number of vertices*/
	unsigned int nTotalTexCoords;	/*!< \brief Number of texture coordinates*/
};


#endif /*IMPORTER3DSTYPES_H_*/
