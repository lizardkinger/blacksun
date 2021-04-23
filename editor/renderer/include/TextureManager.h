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
 *  File:       TextureManager.h
 *  Created:    30.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "RendererTypes.h"
#include "Material.h"
#include "TextureLoaderPlugin.h"

/*! \file
* 
*  \brief Describes the class TextureManager, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

typedef map<int, Texture>::iterator TextureIterator;
typedef map<string, TextureLoaderPlugin*>::const_iterator TextureLoaderPluginIterator;

/*! \brief The manager for the textures 
*
* Loads textures and avoids loading a texture more than once. To load different file types,
* the texture manager also manage the texture loader plugins.
*/
class TextureManager
{
public:
	/*!
	* \brief Returns the instance of the texture manager to give access to it
	*
	* \return TextureManager* The instance of the texture manager
	*/
	static TextureManager* getInstance();

	/*!
	* \brief Loads a texture from a file
	*
	* \param sFileName The file name of the texture which should be loaded
	* \param nID The ID that should be used. If there is already a texture
	*   on this ID, the next free ID will be used.
	* \return int The texture ID. If the were any errors, -1 will be returned
	*/	
	int loadTexture(const string& sFileName, int nID = -1);
		
	/*!
	* \brief Returns a pointer to the texture assigned to the ID
	*
	* \param nID The ID of the texture
	* \return Texture* Returns the pointer, if the ID was found,
	*   otherwise NULL will be returned.
	*/		
	Texture* getTexture(int nID);
	
	/*!
	* \brief Returns a pointer to the texture assigned to the file name
	*
	* \param sFileName The file name of the texture
	* \return Texture* Returns the pointer, if the file name was found,
	*   otherwise NULL will be returned.
	*/			
	Texture* getTexture(const string& sFileName);
	
	/*!
	* \brief Returns the ID assigned to the file name
	*
	* \param sFileName The file name of the texture
	* \return int Returns the IF, if a texture with the file name was found,
	*   otherwise -1 will be returned.
	*/		
	int getTextureID(const string& sFileName);
	
	/*!
	* \brief Removes a texture
	*
	* \param nID The ID of the texture that should be removed
	*/			
	void removeTexture(int nID);
	
	/*!
	* \brief Removes all textures
	*
	* Removes all textures and frees the texture in OpenGl
	*/			
	void removeAllTextures()
	{
		m_textures.clear();
	}
	
	/*!
	* \brief Adds a new texture loader plugin
	*
	* \param pLoader A pointer to the texture loader plugin that should be added
	* \return bool True, if the loader was added, otherwise false (a plugin for the
	* file extension already exists)
	*/			
	bool addTextureLoader(TextureLoaderPlugin* pLoader);
	
	/*!
	* \brief Removes a texture loader
	*
	* \param sExt The file extension of the texture loader that should be removed
	*/		
	void removeTextureLoader(const string& sExt)
	{
		m_loader.erase(sExt);	
	}
		
	/*!
	* \brief Returns the number of loaded textures
	*
	* \return UINT The number of loaded textures
	*/
	UINT getNumTextures() const
	{
		return m_textures.size();
	}

	/*!
	* \brief Returns an iterator to the first texture
	*
	* \return TexturInterator The iterator
	*/	
	TextureIterator beginTextures()
	{
		return m_textures.begin();	
	}
	
	/*!
	* \brief Returns an iterator to the last texture
	*
	* \return TexturInterator The iterator
	*/	
	TextureIterator endTextures()
	{
		return m_textures.end();	
	}
	
	/*!
	* \brief Returns an iterator to the first texture loader loader plugin
	*
	* \return TextureLoaderPluginIterator The iterator
	*/	
	TextureLoaderPluginIterator beginTextureLoaderPlugins()
	{
		return m_loader.begin();	
	}

	/*!
	* \brief Returns an iterator to the last texture loader loader plugin
	*
	* \return TextureLoaderPluginIterator The iterator
	*/		
	TextureLoaderPluginIterator endTextureLoaderPlugins()
	{
		return m_loader.end();	
	}
			
private:
	/*!
	* \brief The standard constructor
	*/
	TextureManager()
	{
	}

	/*!
	* \brief The destructor
	*/
	virtual ~TextureManager()
	{
	}
	
	/*! \brief The guard for the texture manager singleton 
	*
	* The guard for the texture manager singleton arrange that the destructor
	* of the singleton will be called
	*/
	class TextureManagerWaechter 
	{
	public:
		/*!
		* \brief The destructor
		*/
		virtual ~TextureManagerWaechter() 
		{
			if(TextureManager::m_instance != NULL)
				delete TextureManager::m_instance;
		}
	};
	friend class TextureManagerWaechter;

	/*!
	* \brief Splits the extension from the file name
	* 
	* \param sFileName The file name
	* \return string The exenstion of the file name
	*/
	string splitExt(const string& sFileName);

private:
	map<int, Texture> m_textures; /*!< \brief Map with all textures and their IDs */
	map<string, TextureLoaderPlugin*> m_loader; /*!< \brief Map with all texture loaders and their extension */
	
	static TextureManager* m_instance; /*!< \brief The instance of the texture manager singleton*/
};
	
}

#endif /*TEXTUREMANAGER_H_*/
