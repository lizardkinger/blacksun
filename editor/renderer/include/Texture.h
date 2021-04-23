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
 *  File:       Texture.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "RendererTypes.h"

/*! \file
* 
*  \brief Describes the class Texture, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief A two-dimensional texture 
*
* A two-dimensional texture. It will be returned by the TextureMangager and sets the texture
* in OpenGl. Informations about the texture is stored there too.
*/
class Texture
{
public:
	/*!
	* \brief The constructor
	* 
	* Its initialized with the Bind-ID (returned by OpenGl) and the texture informations
	* 
	* \param nBindID The binding ID (returned by OpenGl)
	* \param texInfo Informations of the textures
	*/	
	Texture(int nBindID, const TextureInfo& texInfo)
	 : m_nBindID(nBindID), m_texInfo(texInfo)
	 {	
	 }
		
	/*!
	* \brief Sets and activates the texture
	* 
	* \param nTexStage The stage where the texture should be activated.\n
	* 	Must be between 0 and (MAXTEXTURES-1)
	*/		
	void set(int nTexStage) const;
	
	/*!
	* \brief Returns the OpenGl texture binding ID
	* 
	* \return int The texture binding ID
	*/		
	int getBindID() const
	{
		return m_nBindID;
	}
	
	/*!
	* \brief Release the texture
	* 
	* Releases the texture and frees the textur memory via OpenGl
	*/
	void release() const;
	
	/*!
	* \brief Returns a pointer to the texture informations
	* 
	* \return TextureInfo* Pointer to the texture informations
	*/		
	TextureInfo* getTextureInfo()
	{
		return &m_texInfo;
	}

private:
	int m_nBindID; 			/*!< \brief The binding ID */
	TextureInfo m_texInfo;  /*!< \brief The texture informations */
};
	
}

#endif /*TEXTURE_H_*/
