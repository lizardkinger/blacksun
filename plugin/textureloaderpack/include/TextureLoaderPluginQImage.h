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
 *  File:       TextureLoaderPluginQImage.h
 *  Created:    20.02.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURELOADERPLUGINQIMAGE_H_
#define TEXTURELOADERPLUGINQIMAGE_H_

#include "./../../renderer/include/TextureLoaderPlugin.h"

/*! \file
* 
*  \brief Describes the class TextureLoaderPluginQImage, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The TextureLoaderHelper
*
* This plugin class provides other texture loader plugins to load the file via QImage
*/
class TextureLoaderPluginQImage: public BSRenderer::TextureLoaderPlugin
{
public:
	/*!
	* \brief Loads the texture
	* 
	* Loads the texture and saves all important data into the passed variables
	* 
	* \param sFileName The file name of the texture
	* \param pInfo The structure, where the informations of the loaded texture
	*	must be stored
	* \param pData A Byte-Array where the image data must be stored
	* \return bool true, if the texture was loaded correctly, otherwise false
	*/	
	virtual bool execute(const string& sFileName, BSRenderer::TextureInfo& pInfo, 
		BYTE** pData);
	
	/*!
	* \brief Returns the file extension which could be loaded by the correct loader
	* 
	* \return string The file extension (for example bmp, gif, ...)
	*/		
	virtual string getFileExtension() const = 0;

	/*!
	* \brief Returns the file description of the format which could be loaded by the correct loader
	* 
	* \return string The file extension description (for example 'Windows Bitmap')
	*/		
	virtual string getFileDescription() const = 0;
};


#endif /*TEXTURELOADERPLUGINQIMAGE_H_*/
