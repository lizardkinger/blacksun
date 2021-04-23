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

#ifndef IMPORTERMD2TYPES_H_
#define IMPORTERMD2TYPES_H_


/////////////// CONSTANTS ///////////////

const int MD2MAGICNUMBER = 844121161; /*!< \brief Magic number of MD2*/

const int MD2VERSION = 8; /*!< \brief MD2 version*/

/**							
* Constants for MD2 limits
*/
const int MD2MAXTRIANGLES = 4096; 	/*!< \brief MD2 triangle number limit*/
const int MD2MAXVERTICES = 2048; 	/*!< \brief MD2 vertex number limit*/
const int MD2MAXTEXCOORDS = 2048; 	/*!< \brief MD2 texture coordinate number limit*/
const int MD2MAXFRAMES = 512;	 	/*!< \brief MD2 frame number limit*/
const int MD2MAXSKINS = 32; 		/*!< \brief MD2 skin number limit*/


/////////////// STRUCTS ///////////////
/*!
* \brief The MD2 header structure
*/
struct MD2_HEADER
{
	int nMagicNumber;	/*!< \brief The magic number used to identify the file*/
	int nVersion;		/*!< \brief The file version number (must be 8)*/
	int nSkinWidth;		/*!< \brief The width in pixels of our image*/
	int nSkinHeight;	/*!< \brief The height in pixels of our image*/
	int nFrameSize;		/*!< \brief The size in bytes the frames are*/
	int nNumSkins;		/*!< \brief The number of skins associated with the model*/
	int nNumVertices;	/*!< \brief The number of vertices*/
	int nNumTexCoords;	/*!< \brief The number of texture coordinates*/
	int nNumTriangles;	/*!< \brief The number of faces (polygons)*/
	int nNumGlCommands;	/*!< \brief The number of gl commands*/
	int nNumFrames;		/*!< \brief The number of animated frames*/
	int nOffsetSkins;	/*!< \brief The offset in the file for the skin data*/
	int nOffsetTexCoords; 	/*!< \brief The offset in the file for the texture data*/
	int nOffsetTriangles; 	/*!< \brief The offset in the file for the face data*/
	int nOffsetFrames;		/*!< \brief The offset in the file for the frames data*/
	int nOffsetGlCommands; 	/*!< \brief The offset in the file for the gl commands data*/
	int nOffsetEnd;		/*!< \brief The end of the file offset*/
};

/*!
* \brief The MD2 skin structure
*/
struct MD2_SKIN
{
	char chSkinName[64]; /*!< \brief The skin name*/
};

/*!
* \brief The MD2 texture coordinate structure
*/
struct MD2_TEXCOORD
{
	short su; /*!< \brief U-Coordinate*/
	short sv; /*!< \brief V-Coordinate*/
};

/*!
* \brief The MD2 triangle structure
*/
struct MD2_TRIANGLE
{
	short sVertexIndex[3]; 	/*!< \brief Indices for vertices*/
	short sTexIndex[3];		/*!< \brief Indices for texture coordinates*/
};

/*!
* \brief The MD2 vertex structure
*/
struct MD2_VERTEX
{
	float fVertex[3]; /*!< \brief Vertex coordinates*/
	float fNormal[3]; /*!< \brief Normal coordinates*/
};

/*!
* \brief The MD2 vertex alias structure
*/
struct MD2_VERTEXALIAS
{
	BYTE byVertex[3];  			/*!< \brief Vertex*/
	BYTE byLightNormalIndex;    /*!< \brief Light normal index*/
};

/*!
* \brief The MD2 frame structure
*/
struct MD2_FRAME
{
	char chName[16]; 		 /*!< \brief Frame name*/
	MD2_VERTEX* pFinalVerts; /*!< \brief Frame vertices*/
};

/*!
* \brief The MD2 frame alias structure
*/
struct MD2_FRAMEALIAS
{
	float fScale[3]; 		/*!< \brief Scale*/
	float fTranslate[3]; 	/*!< \brief Translation*/
	char chName[16]; 		/*!< \brief Name*/
	MD2_VERTEXALIAS aliasVerts[1]; 	/*!< \brief Vertex alias*/
};

/*!
* \brief The MD2 keyframe structure
*/
struct MD2_KEYFRAME
{
	int nStart; /*!< \brief Start time*/
	int nEnd;	/*!< \brief End time*/
	char chName[16];  /*!< \brief Name*/
};

/*!
* \brief The MD2 key structure
*/
struct MD2_KEY
{
	int nNumKeys; /*!< \brief Number of keys*/
	MD2_KEYFRAME *pKey;  /*!< \brief Array of keys*/
};

/*!
* \brief The MD2 model structure
*/
struct MD2_MODELL
{
	MD2_HEADER Header; 				/*!< \brief File header*/
	vector<MD2_SKIN> Skins; 		/*!< \brief Array of the skins*/
	vector<MD2_VERTEX> Vertices; 	/*!< \brief Array of the vertices*/
	vector<MD2_TEXCOORD> TexCoords; /*!< \brief Array of the texcoords*/
	vector<MD2_TRIANGLE> Triangles; /*!< \brief Array of the triangles*/
	vector<MD2_FRAME> Frames; 		/*!< \brief Array of the frames*/
	MD2_KEY Key; 					/*!< \brief Key of the modell*/
};

#endif /*IMPORTERMD2TYPES_H_*/
