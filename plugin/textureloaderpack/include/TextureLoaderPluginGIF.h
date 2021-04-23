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
 *  File:       TextureLoaderPluginGIF.h
 *  Created:    20.02.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURELOADERPLUGINGIF_H_
#define TEXTURELOADERPLUGINGIF_H_

#include "./../../renderer/include/TextureLoaderPlugin.h"
#include "TextureLoaderPluginQImage.h"

/*! \file
* 
*  \brief Describes the class TextureLoaderPluginGIF, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The TextureLoader for GIF
*
* This plugin class provides a TextureLoader for Graphic Interchange Format (GIF)
*/
class TextureLoaderPluginGIF: public TextureLoaderPluginQImage
{
public:	
	/*!
	* \brief Returns the file extension which could be loaded by the correct loader
	* 
	* \return string The file extension (for example bmp, gif, ...)
	*/	
	virtual string getFileExtension() const
	{
		return "GIF";
	}

	/*!
	* \brief Returns the file description of the format which could be loaded by the correct loader
	* 
	* \return string The file extension description (for example 'Windows Bitmap')
	*/	
	virtual string getFileDescription() const
	{
		return "Graphic Interchange Format";	
	}

};
	
#endif /*TEXTURELOADERPLUGINGIF_H_*/
