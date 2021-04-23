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
 *  File:       TextureManager.cpp
 *  Created:    30.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#include "../include/TextureManager.h"

namespace BSRenderer
{
TextureManager* TextureManager::m_instance = NULL;

TextureManager* TextureManager::getInstance()
{
	static TextureManagerWaechter w;

	if(m_instance == NULL)
	{
		m_instance = new TextureManager();
	}
	
	return m_instance;
}	

int TextureManager::loadTexture(const string& sFileName, int nID)
{
	//Test, if texture is already loaded
	int nFound = getTextureID(sFileName);
	if(nFound != -1)
	{
		/*stringstream msg;
		msg << "Texture loaded: \"" << sFileName << "\"";
		LOG_Ok(msg.str()); */
		return nFound;
	}
		
	//Texture must be loaded
	//Split the extension from the file name
	string sExt = splitExt(sFileName);
	
	//Search a proper texture loader plugin
	map<string, TextureLoaderPlugin*>::iterator it = m_loader.find(sExt);
	if(it == m_loader.end())
	{
		stringstream msg;
		msg << "No texture loader plugin found to load extension \"" << sExt << "\"";
		LOG_Error(msg.str()); 
		return -1;
	}
	
	//A proper texture loader plugin was found, so load the texture
	TextureInfo info;
	BYTE* pData = NULL;
	UINT nBindID = 0;
	
	if(it->second->execute(sFileName, info, &pData) == false)
	{
		stringstream msg;
		msg << "Can't load texture: \"" << sFileName << "\"";
		LOG_Error(msg.str()); 
		return -1;	
	}
	
	//Texture loader plugin returned wrong texture informations
	if(pData==NULL)
	{
		stringstream msg;
		msg << "Texture loader plugin returned NULL for data: \"" << sFileName << "\"";
		LOG_Error(msg.str()); 
		return -1;
	}
	
	//Generate a new OpenGL-Texture
	glGenTextures(1, &nBindID);
	glBindTexture(GL_TEXTURE_2D, nBindID);
	if(info.bHasAlpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.nWidth, info.nHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info.nWidth, info.nHeight,
			GL_RGBA, GL_UNSIGNED_BYTE, pData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.nWidth, info.nHeight,
			0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, info.nWidth, info.nHeight,
			GL_RGB, GL_UNSIGNED_BYTE, pData);
	}
		
	//Its not longer used, so delete it
	delete[] pData;
	
	//Insert the texture
	if(nID == -1)
	{
		nID = 1;
	}
	
	while(m_textures.find(nID)!=m_textures.end())
	{
		nID++;
	}
	
	m_textures.insert(make_pair(nID, Texture(nBindID, info)));	
	
	stringstream msg;
	msg << "Texture loaded: \"" << sFileName << "\"";
	LOG_Ok(msg.str()); 
		
	return nID;
}

Texture* TextureManager::getTexture(int nID)
{
	map<int, Texture>::iterator it = m_textures.find(nID);
	
	if(it!=m_textures.end())
	{
		return &(it->second);
	}
	
	return NULL;
}

Texture* TextureManager::getTexture(const string& sFileName)
{
	map<int, Texture>::iterator it;
	
	for(it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		if(it->second.getTextureInfo()->sName == sFileName)
		{
			return &(it->second);
		}	
	}
	
	return NULL;
}

int TextureManager::getTextureID(const string& sFileName)
{
	map<int, Texture>::iterator it;
	
	for(it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		if(it->second.getTextureInfo()->sName == sFileName)
		{
			return it->first;
		}	
	}
	
	return -1;
}

bool TextureManager::addTextureLoader(TextureLoaderPlugin* pLoader)
{
	if(pLoader==NULL)
	{
		return false;
	}
		
	if(m_loader.find(pLoader->getFileExtension()) != m_loader.end())
	{
		return false;
	}
		
	m_loader.insert(make_pair(pLoader->getFileExtension(),pLoader));
	return true;	
}

void TextureManager::removeTexture(int nID)
{
	Texture* pTex = getTexture(nID);
	if(pTex != NULL)
		pTex->release();
		
	m_textures.erase(nID);	
}
	
string TextureManager::splitExt(const string& sFileName)
{
	string str = sFileName.substr(sFileName.find_last_of(".")+1, sFileName.length());
	for(UINT t=0; t<str.length(); t++)
	{
		str[t] = toupper(str[t]);
	}
	
	return str;
}	
	
}
