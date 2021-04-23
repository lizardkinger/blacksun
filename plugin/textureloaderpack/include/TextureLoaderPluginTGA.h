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
 *  File:       TextureLoaderPluginTGA.h
 *  Created:    20.02.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURELOADERPLUGINTGA_H_
#define TEXTURELOADERPLUGINTGA_H_

#include "./../../renderer/include/TextureLoaderPlugin.h"

/*! \file
* 
*  \brief Describes the class TextureLoaderPluginTGA, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The TextureLoader for TGA
*
* This plugin class provides a TextureLoader for Targa Image (TGA)
*/
class TextureLoaderPluginTGA: public BSRenderer::TextureLoaderPlugin
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
	virtual string getFileExtension() const
	{
		return "TGA";
	}

	/*!
	* \brief Returns the file description of the format which could be loaded by the correct loader
	* 
	* \return string The file extension description (for example 'Windows Bitmap')
	*/	
	virtual string getFileDescription() const
	{
		return "Targa Image";	
	}
	
private:

	/*!
	* \brief The TGA Header
	*/
	struct TGAHeader
	{
		UCHAR header[12]; /*!< TGA file header*/
	};
	
	/*!
	* \brief The TGA Image informations
	*/
	struct TGAImage
	{
		UCHAR header[6]; 	/*!< First 6 useful bytes from the header*/
		UINT bytesPerPixel; /*!< Number of bytes per pixel*/
		UINT imageSize; 	/*!< sed to store the image size when setting aside ram*/
		UINT temp; 			/*!< Temporary variable*/
		UINT type;			/*!< TGA-type*/
		UINT height; 		/*!< Height of image*/
		UINT width; 		/*!< Width of image*/
		UINT bpp; 			/*!< Bits per pixel*/	
	};
	
	static const UCHAR cTGAHeaderUncompressed[12]; 	/*!< Uncompressed TGA header*/
	static const UCHAR cTGAHeaderCompressed[12]; 	/*!< Compressed TGA header*/
	
private:
	/*!
	* \brief Checks, whether the header is the header for a compressed TGA 
	* 
	* \param h The header which should be checked
	* \return bool True, if it's a compressed TGA header, otherwise false
	*/
	bool isCompressedHeader(const TGAHeader& h) const;
	
	/*!
	* \brief Checks, whether the header is the header for an uncompressed TGA 
	* 
	* \param h The header which should be checked
	* \return bool True, if it's an uncompressed TGA header, otherwise false
	*/
	bool isUncompressedHeader(const TGAHeader& h) const;
	
	/*!
	* \brief Loads a compressed TGA
	* 
	* Loads the texture and saves all important data into the passed variables
	* 
	* \param f The input stream of the file
	* \param pInfo The structure, where the informations of the loaded texture
	*	must be stored
	* \param pData A Byte-Array where the image data must be stored
	* \return bool true, if the texture was loaded correctly, otherwise false
	*/
	bool loadCompressed(ifstream& f, BSRenderer::TextureInfo& pInfo, BYTE** pData) const;

	/*!
	* \brief Loads an uncompressed TGA
	* 
	* Loads the texture and saves all important data into the passed variables
	* 
	* \param f The input stream of the file
	* \param pInfo The structure, where the informations of the loaded texture
	*	must be stored
	* \param pData A Byte-Array where the image data must be stored
	* \return bool true, if the texture was loaded correctly, otherwise false
	*/
	bool loadUncompressed(ifstream& f, BSRenderer::TextureInfo& pInfo, BYTE** pData) const;
};


#endif /*TEXTURELOADERPLUGINTGA_H_*/
