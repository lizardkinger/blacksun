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
 *  Module:     Renderer (BlackSun)
 *  File:       RendererTypes.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef RENDERERTYPES_H_
#define RENDERERTYPES_H_

/*! \file
* 
*  \brief Collects all important includes, structures, constants and enumerations
*  \author Reinhard Jeschull
* 
*/

///////////////////////////////////////////////////////////
//  Standard-C++-Includes
///////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
using namespace std;

///////////////////////////////////////////////////////////
// Blacksun-Includes for core-lib
///////////////////////////////////////////////////////////
#include "../../core/include/Types.h"
#include "../../core/include/Log.h"

///////////////////////////////////////////////////////////
//  OpenGl-Includes
///////////////////////////////////////////////////////////
#include "../contrib/GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>

///////////////////////////////////////////////////////////
// Blacksun-Includes for math-lib
///////////////////////////////////////////////////////////
#include "../../math/include/Vector3.h"
#include "../../math/include/ColorRGB.h"
#include "../../math/include/Matrix4x4.h"
#include "../../math/include/AABB.h"
#include "../../math/include/Arcball.h"
#include "../../math/include/Util.h"
using namespace BSMath;

/*! \brief The renderer core of the editor
*
* The renderer core of the editor. It provides all classes to handle 
* confortable witch 3D-object and the rendering of scenes, loading textures, materials, ...
*/
namespace BSRenderer
{ 
	
///////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////
const int MAXTEXTURES = 4; /*!< \brief The maximum number of textures that can be combined together*/ 

///////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////

/*!
* \brief The vertex structure
*/
struct Vertex
{	
	/*!
	* \brief The constructor
	* 
	* \param pos The position
	* \param normal The normal vector
	* \param u The u-texture-coordinate for all texture stages
	* \param v The v-texture-coordinate for all texture stages 
	*/
	Vertex(const Vector& pos = Vector(0.0, 0.0, 0.0), 
		const Vector normal = Vector(0.0, 0.0, 0.0), double u = 0.0, double v = 0.0)
	 : vPos(pos), vNormal(normal)
	{
		dU[0] = u;
		dV[0] = v;
		for(int i=1; i<MAXTEXTURES; i++)
		{
			dU[i] = dU[0];
			dV[i] = dV[0];	
		}
	}

	/*!
	* \brief Set the texture coordinates of one or all texture stages
	* 
	* \param nStage The stage of the texture. Use an stage < 0 to use the coordinate
	* 	for all texture-stages 
	* \param u The u-texture-coordinate for one/all texture stage(s)
	* \param v The v-texture-coordinate for one/all texture stage(s) 
	*/	
	void setTexCoord(int nStage, double u, double v)
	{
		if(nStage < 0)
		{
			for(int i = 0; i < MAXTEXTURES; i++)
			{
				dU[i] = u;
				dV[i] = v;
			}
		}
		else
		{
			if(nStage >= MAXTEXTURES)
				nStage = MAXTEXTURES;
			
			dU[nStage] = u;
			dV[nStage] = v;			
		}	
	}
	
	Vector vPos; /*!< The position*/
	Vector vNormal; /*!< The normal vector*/
	double dU[MAXTEXTURES]; /*!< The u-texture-coordinate for each texture stage*/
	double dV[MAXTEXTURES]; /*!< The v-texture-coordinate for each texture stage*/
};

/*!
* \brief A texture information
*/
struct TextureInfo
{
	string sName; 	/*!< The (file) name of the texture*/
	int nWidth;		/*!< Width of the texture*/
	int nHeight;	/*!< Height of the texture*/
	bool bHasAlpha; /*!< Does the texture have a alpha-component*/
};

/*!
* \brief The OpenGl informations
*/
struct OGLInfo
{
	string sVendor; /*!< The vendor*/
	string sRenderer; /*!< The name of the renderer*/
	string sVersion; /*!< The OpenGL-version*/
	string sExtensions; /*!< All OpenGL-extension supported by the graphic card*/
};

///////////////////////////////////////////////////////////
// Enumerations
///////////////////////////////////////////////////////////

/*!
* \brief The type specifies how to render the scene
*/
enum FillMode
{
	FILL_Textured,  /*!< The objects are rendered textured*/
	FILL_Solid,     /*!< The objects are rendered without texture(s)*/
	FILL_Wireframe, /*!< The objects are rendered wireframe*/
	FILL_WireframeOverlay, /*!< The objects are rendered with texture and wireframe at once*/
	FILL_Point 	    /*!< The points of the edges are rendered*/
};

/*!
* \brief The view mode
*/
enum ViewMode
{
	VIEWMODE_Perspective, /*!< A perspective view*/
	VIEWMODE_Orthogonal	  /*!< A orthogonal view*/
};

/*!
* \brief The primitive-type
*/
enum PrimitiveType
{
	PRIM_Triangle,  /*!< The primitive is a triangle*/
	PRIM_Line, 		/*!< The primitive is a line*/
	PRIM_Point 		/*!< The primitive is a point*/
};

/*!
* \brief The type specifies how to render the data
*/
enum PolygonMode
{
	POLYMODE_Fill, 		/*!< Polygons are rendered filled*/
	POLYMODE_Wireframe, /*!< Polygons are rendered wireframe*/
	POLYMODE_Point 		/*!< Only render the points (edges) of the polygons*/
};

/*!
* \brief The type of texture blending
*/
enum TextureBlend
{
	TEXBLEND_One = 0, 	/*!< Use (0, 0, 0, 0) to blend */
	TEXBLEND_Zero, 		/*!< Use (1, 1, 1, 1) to blend */
	TEXBLEND_SrcColor, 	/*!< Use the RGB-part of the value */
	TEXBLEND_OneMinusSrcColor,  /*!< Use 1.0-RGB of the value */
	TEXBLEND_SrcAlpha,  		/*!< Use the alpha-part of the value */
	TEXBLEND_OneMinusSrcAlpha   /*!< Use 1.0 - alpha of the value */
};

/*!
* \brief The texture operation
*/
enum TextureOperation
{
	TEXOP_PrimaryColor = 0, /*!< Use color from glColor or similar */
	TEXOP_Texture,			/*!< Use color from texture */
	TEXOP_Constant,			/*!< Use color from glTexEnvfv*/
	TEXOP_Previous,			/*!< Use color from previous stage (or glColor if it is the first stage)*/
	TEXOP_Disable			/*!< Disable*/
};

/*!
* \brief The type of the operand
*/
enum TextureCombine
{
	TEXCOMB_ColorArg0 = 0, 	/*!< Use operand for Arg0 in the RGB-calucation */
	TEXCOMB_ColorArg1, 		/*!< Use operand for Arg1 in the RGB-calucation */
	TEXCOMB_ColorArg2, 		/*!< Use operand for Arg2 in the RGB-calucation */
	TEXCOMB_AlphaArg0, 		/*!< Use operand for Arg0 in the alpha-calucation */
	TEXCOMB_AlphaArg1, 		/*!< Use operand for Arg1 in the alpha-calucation */
	TEXCOMB_AlphaArg2, 		/*!< Use operand for Arg2 in the alpha-calucation */
	MAX_TEX_COMBINE 		/*!< The maximum operand types */
};

/*!
* \brief The type of the combine operand (blending)
*/
enum TextureCombineOperand
{
	TEXCOMBOP_RGB0 = 0, 	/*!< Use operand for Arg0 in the RGB-calucation */
	TEXCOMBOP_RGB1, 		/*!< Use operand for Arg1 in the RGB-calucation */
	TEXCOMBOP_RGB2, 		/*!< Use operand for Arg2 in the RGB-calucation */
	TEXCOMBOP_Alpha0, 		/*!< Use operand for Arg0 in the alpha-calucation */
	TEXCOMBOP_Alpha1, 		/*!< Use operand for Arg1 in the alpha-calucation */
	TEXCOMBOP_Alpha2, 		/*!< Use operand for Arg2 in the alpha-calucation */
	MAX_TEX_COMBINE_OPERAND	/*!< The maximum combine operand types */
};

/*!
* \brief The type specifies who to combine the color- and alpha-arguments together
*/
enum TextureCombineMethode
{
	TEXMET_Replace = 0,	/*!< Use Arg0 */
	TEXMET_Modulate, 	/*!< Combine via (Arg0 * Arg1) */
	TEXMET_Add, 		/*!< Combine via (Arg0 + Arg1) */
	TEXMET_AddSigned,   /*!< Combine via (Arg0 + Arg1 - 0.5) */
	TEXMET_Subtract, 	/*!< Combine via (Arg0 - Arg1) */
	TEXMET_Interpolate, /*!< Combine via [Arg0 * (Arg2) + Arg1 * (1-Arg2)] */
	TEXMET_Disable 		/*!< Use (0, 0, 0, 0) */
};

/*!
* \brief The type of repeating a texture
*/
enum TextureRepeat
{
	TEXREP_Repeat = 0,  /*!< Repeat the texture*/
	TEXREP_Clamp 		/*!< Clamp the texture*/
};

/*!
* \brief The type of realtime mirroring
*/
enum RealtimeMirror
{
	MIRROR_None = 0, /*!< No mirroring*/
	MIRROR_XY, /*!< Mirroring at the XY-plane */
	MIRROR_YZ, /*!< Mirroring at the YZ-plane */
	MIRROR_XZ, /*!< Mirroring at the XZ-plane */
};

/*!
* \brief The type of the orthogonal view
*/
enum OrthogonalType
{
	ORTHO_Front, /*!< Front orthogonal view*/
	ORTHO_Back,  /*!< Back orthogonal view*/
	ORTHO_Left,  /*!< Left orthogonal view*/
	ORTHO_Right, /*!< Right orthogonal view*/
	ORTHO_Top,   /*!< Top orthogonal view*/
	ORTHO_Bottom /*!< Bottom orthogonal view*/
}; 

}

#endif /*RENDERERTYPES_H_*/
