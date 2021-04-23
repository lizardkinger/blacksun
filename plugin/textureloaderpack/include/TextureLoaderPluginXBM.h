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
 *  File:       TextureLoaderPluginXBM.h
 *  Created:    20.02.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURELOADERPLUGINXBM_H_
#define TEXTURELOADERPLUGINXBM_H_

#include "./../../renderer/include/TextureLoaderPlugin.h"
#include "TextureLoaderPluginQImage.h"

/*! \file
* 
*  \brief Describes the class TextureLoaderPluginXBM, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The TextureLoader for XBM
*
* This plugin class provides a TextureLoader for X11 Bitmaps (XBM)
*/
class TextureLoaderPluginXBM: public TextureLoaderPluginQImage
{
public:
	/*!
	* \brief Returns the file extension which could be loaded by the correct loader
	* 
	* \return string The file extension (for example bmp, gif, ...)
	*/		
	virtual string getFileExtension() const
	{
		return "XBM";
	}

	/*!
	* \brief Returns the file description of the format which could be loaded by the correct loader
	* 
	* \return string The file extension description (for example 'Windows Bitmap')
	*/	
	virtual string getFileDescription() const
	{
		return "X11 Bitmap";	
	}

};


#endif /*TEXTURELOADERPLUGINXBM_H_*/
