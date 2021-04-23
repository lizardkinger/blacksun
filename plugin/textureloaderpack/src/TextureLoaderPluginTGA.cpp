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
 *  File:       TextureLoaderPluginTGA.cpp
 *  Created:    20.02.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/TextureLoaderPluginTGA.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace BSRenderer;


const UCHAR TextureLoaderPluginTGA::cTGAHeaderUncompressed[12] = {0, 0, 2 , 0, 0, 0, 0, 0, 0, 0, 0, 0};
const UCHAR TextureLoaderPluginTGA::cTGAHeaderCompressed[12]   = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
bool TextureLoaderPluginTGA::execute(const string& sFileName, 
	TextureInfo& pInfo, BYTE** pData)
{
	ifstream f(sFileName.c_str(), ios::binary);
	
	//File openend?
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());
		f.close();
		return false;
	}
	
	//Read TGA header
	TGAHeader header;
	f.read(reinterpret_cast<char*>(&header), sizeof(header));
	
	//Header correctly read?
	if(f.rdstate())
	{
		LOG_Error("Can't read TGA file header");
		f.close();
		return false;	
	}
	
	//Save texture name	
	pInfo.sName = sFileName;
	
	//Check the TGA type
	if(isCompressedHeader(header))
	{
		//Load the compressed TGA file
		return loadCompressed(f, pInfo, pData);
	}
	else if(isUncompressedHeader(header))
	{
		//Load the uncompressed TGA file
		return loadUncompressed(f, pInfo, pData);
	}
	else
	{
		stringstream msg;
		msg << "Unsupported TGA type: " << static_cast<int>(header.header[2]);
		LOG_Error(msg.str()); 
		f.close();
		return false;
	}
    
	return false;
}

bool TextureLoaderPluginTGA::isCompressedHeader(const TGAHeader& h) const
{
	for(int i=0; i<12; i++)
	{
		if(h.header[i] != cTGAHeaderCompressed[i])
			return false;
	}
	
	return true;
}

bool TextureLoaderPluginTGA::isUncompressedHeader(const TGAHeader& h) const
{
	for(int i=0; i<12; i++)
	{
		if(h.header[i] != cTGAHeaderUncompressed[i])
			return false;
	}
	
	return true;	
}

bool TextureLoaderPluginTGA::loadUncompressed(ifstream& f, TextureInfo& pInfo, BYTE** pData) const
{
	TGAImage img;
	
	//Read TGA header
	f.read(reinterpret_cast<char*>(&img.header),sizeof(img.header));
	
	//Header correctly read?
	if(f.rdstate())
	{
		LOG_Error("Can't read TGA image data header");
		f.close();
		return false;	
	}	
	
	pInfo.nWidth  = img.header[1] * 256 + img.header[0]; //Width = (highbyte * 256 + lowbyte)
	pInfo.nHeight = img.header[3] * 256 + img.header[2]; //Height = (highbyte * 256 + lowbyte)
	pInfo.bHasAlpha	= img.header[4] == 32;
	
	img.width = pInfo.nWidth;						
	img.height = pInfo.nHeight;
	img.bpp = img.header[4];
	
	//Check, if all informations are valid
	if((img.bpp != 24) && (img.bpp !=32))
	{
		LOG_Error("Invalid texture informations 'Bits per Pixel'");
		f.close();
		return false;		
	}
	if((pInfo.nWidth <= 0) || (pInfo.nHeight <= 0))
	{
		LOG_Error("Invalid texture informations 'Dimension'");
		f.close();
		return false;		
	}
	
	//Compute total amount of memory needed to store the image data
	img.imageSize = ((img.bpp / 8) * pInfo.nWidth * pInfo.nHeight);
	
	//Allocate memory for image data
	(*pData) = new BYTE[img.imageSize];
	
	//Read TGA image data
	f.read(reinterpret_cast<char*>(*pData),img.imageSize*sizeof(BYTE));
		
	//Swap pixel data from BGR(A) to RGB(A)
	BYTE temp;
	BYTE bbp = img.bpp / 8;
	for(UINT i=0; i<(img.imageSize); i+=bbp)
	{
		temp = (*pData)[i];
		(*pData)[i] = (*pData)[i+2];
		(*pData)[i+2] = temp;
	}
	
	return true;
}
	
bool TextureLoaderPluginTGA::loadCompressed(ifstream& f, TextureInfo& pInfo, BYTE** pData) const
{
	TGAImage img;
	
	//Read TGA header
	f.read(reinterpret_cast<char*>(&img.header),sizeof(img.header));
	
	//Header correctly read?
	if(f.rdstate())
	{
		LOG_Error("Can't read TGA image data header");
		f.close();
		return false;	
	}	
	
	pInfo.nWidth  = img.header[1] * 256 + img.header[0]; //Width = (highbyte * 256 + lowbyte)
	pInfo.nHeight = img.header[3] * 256 + img.header[2]; //Height = (highbyte * 256 + lowbyte)
	pInfo.bHasAlpha	= img.header[4] == 32;
	
	img.width = pInfo.nWidth;						
	img.height = pInfo.nHeight;
	img.bpp = img.header[4];
	img.bytesPerPixel = img.bpp / 8;
	
	//Check, if all informations are valid
	if((img.bpp != 24) && (img.bpp !=32))
	{
		LOG_Error("Invalid texture informations 'Bits per Pixel'");
		f.close();
		return false;		
	}
	if((pInfo.nWidth <= 0) || (pInfo.nHeight <= 0))
	{
		LOG_Error("Invalid texture informations 'Dimension'");
		f.close();
		return false;		
	}
	
	//Compute total amount of memory needed to store the image data
	img.imageSize = ((img.bpp / 8) * pInfo.nWidth * pInfo.nHeight);
	
	//Allocate memory for image data
	(*pData) = new BYTE[img.imageSize];
	
	UINT nPixelCount = pInfo.nHeight * pInfo.nWidth; //Nuber of pixels in the image
	UINT nCurrentPixel = 0; //Current pixel being read
	UINT nCurrentByte = 0;  //Current byte 
	BYTE *buffer = new BYTE[img.bytesPerPixel];
	
	//Loop through all pixels
	do
	{
		BYTE chunkHeader = 0;
		
		//Read chunk header
		f.read(reinterpret_cast<char*>(&chunkHeader),sizeof(chunkHeader));
		
		//Header correctly read?
		if(f.rdstate())
		{
			stringstream msg;
			LOG_Error("Can't read RLE header"); 
			delete[] buffer;
			delete[] (*pData);
			f.close();
			return false;	
		}
				
		//ChunkHeader < 128: RAW color packets minus 1
		if(chunkHeader < 128)
		{
			chunkHeader++;
			
			//Read RAW color values
			for(short c = 0; c < chunkHeader; c++)
			{
				//Read one pixel
				f.read(reinterpret_cast<char*>(buffer),img.bytesPerPixel*sizeof(BYTE));
				
				//Data correctly read?
				if(f.rdstate())
				{
					stringstream msg;
					LOG_Error("Can't read image data");
					delete[] buffer;
					delete[] (*pData);
					f.close();
					return false;					
				}
				
				//Save the read color and swap from BGR to RGB
				(*pData)[nCurrentByte	 ] = buffer[2]; 
				(*pData)[nCurrentByte + 1] = buffer[1];
				(*pData)[nCurrentByte + 2] = buffer[0];
				
				//Its a 32-Bit TGA
				if(img.bpp == 32)
				{
					(*pData)[nCurrentByte + 3] = buffer[3]; //Copy the alpha-color-component
				}
				
				//Increase values
				nCurrentByte += img.bytesPerPixel;
				nCurrentPixel++;
				
				//To much bytes are read
				if(nCurrentPixel > nPixelCount)
				{
					stringstream msg;
					LOG_Error("To many pixels read");
					delete[] buffer;
					delete[] (*pData);
					f.close();
					return false;	
				}
			}
		}
		//next color reapeated chunkheader - 127 times
		else
		{
			chunkHeader -= 127;
			
			//Read one pixel
			f.read(reinterpret_cast<char*>(buffer),img.bytesPerPixel);
			
			//Data correctly read?
			if(f.rdstate())
			{
				stringstream msg;
				LOG_Error("Can't read image data");
				delete[] buffer;
				delete[] (*pData);
				f.close();
				return false;					
			}
			
			//Read RAW color values
			for(short c = 0; c < chunkHeader; c++)
			{
				//Save the read color and swap from BGR to RGB
				(*pData)[nCurrentByte	 ] = buffer[2]; 
				(*pData)[nCurrentByte + 1] = buffer[1];
				(*pData)[nCurrentByte + 2] = buffer[0];
				
				//Its a 32-Bit TGA
				if(img.bpp == 32)
				{
					(*pData)[nCurrentByte + 3] = buffer[3]; //Copy the alpha-color-component
				}
				
				//Increase values
				nCurrentByte += img.bytesPerPixel;
				nCurrentPixel++;
				
				//To much bytes are read
				if(nCurrentPixel > nPixelCount)
				{
					stringstream msg;
					LOG_Error("To many pixels read");
					delete[] buffer;
					delete[] (*pData);
					f.close();
					return false;	
				}	
			}			
						
		}
		
	}
	while(nCurrentPixel < nPixelCount);
	
	return true;
}	

