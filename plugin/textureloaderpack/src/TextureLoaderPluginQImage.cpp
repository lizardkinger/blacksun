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
 *  File:       TextureLoaderPluginQImage.cpp
 *  Created:    20.02.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/TextureLoaderPluginQImage.h"
#include <iostream>
#include <QImage>
using namespace std;
using namespace BSRenderer;

	
bool TextureLoaderPluginQImage::execute(const string& sFileName, 
	TextureInfo& pInfo, BYTE** pData)
{
	QImage img;
	
	//Load the texture file
	if(img.load(sFileName.c_str()) == false)
	{
		return false;	
	}
	
	//If it's a null-image, so return
	if(img.isNull())
	{
		return false;	
	}
	
	//Convert it into ARGB (32 Bit)
	img = img.convertToFormat(QImage::Format_ARGB32);
	
	//Mirror texture
    img = img.mirrored(false, true);
    
    //Convert from RGB to BGR 
    img = img.rgbSwapped();
	
	//Save texture informations
	pInfo.sName = sFileName;
	pInfo.nWidth = img.width();
	pInfo.nHeight = img.height();
	pInfo.bHasAlpha = img.hasAlphaChannel();

	//Copy the texture-data
	int nNumOfBytes = img.numBytes();
	if(pInfo.bHasAlpha)
	{
		(*pData) = new BYTE[nNumOfBytes];
		for(int i=0; i<nNumOfBytes; i++)
			(*pData)[i] = img.bits()[i];
	}
	else
	{
		(*pData) = new BYTE[nNumOfBytes];
		int t = 0;
		for(int i=0; i<nNumOfBytes; i++)
		{
			//Ignore every fourth byte 
			if(i%4 == 0)
				continue;
			
			t++;
			(*pData)[t] = img.bits()[i];
		}
	}
    
	return true;
}

