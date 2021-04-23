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
 *  File:       TextureLoaderPack.cpp
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/TextureLoaderPack.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../gui/include/mainwindow.h"

#include "../include/TextureLoaderPluginBMP.h"
#include "../include/TextureLoaderPluginGIF.h"
#include "../include/TextureLoaderPluginJPEG.h"
#include "../include/TextureLoaderPluginJPG.h"
#include "../include/TextureLoaderPluginPBM.h"
#include "../include/TextureLoaderPluginPGM.h"
#include "../include/TextureLoaderPluginPNG.h"
#include "../include/TextureLoaderPluginPPM.h"
#include "../include/TextureLoaderPluginTGA.h"
#include "../include/TextureLoaderPluginXBM.h"
#include "../include/TextureLoaderPluginXPM.h"

//using namespace BSScene;
using namespace BSRenderer;

static TextureLoaderPack* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new TextureLoaderPack();

	return static_cast<PlgInt*>(_instance);
}

TextureLoaderPack::TextureLoaderPack() : version(Version(1, 0, 0))
{
	m_sName = "Texture loader pack";
	m_sDescription = "Loads a number of different image file formats ";
	m_sDescription += "to use them as a texture.<br><br>";
	m_sDescription += "<strong>Supported file formats are:</strong>";

	m_loader.push_back(new TextureLoaderPluginBMP());
	m_loader.push_back(new TextureLoaderPluginGIF());
	m_loader.push_back(new TextureLoaderPluginJPG());
	m_loader.push_back(new TextureLoaderPluginJPEG());
	m_loader.push_back(new TextureLoaderPluginPBM());
	m_loader.push_back(new TextureLoaderPluginPGM());
	m_loader.push_back(new TextureLoaderPluginPNG());
	m_loader.push_back(new TextureLoaderPluginPPM());
	m_loader.push_back(new TextureLoaderPluginTGA());
	m_loader.push_back(new TextureLoaderPluginXBM());
	m_loader.push_back(new TextureLoaderPluginXPM());

	m_sDescription += "<table>";
	
	for(unsigned int i=0; i<m_loader.size(); i++)
	{
		m_sDescription += "<tr>";
		m_sDescription += "<td><b>";
		m_sDescription += m_loader[i]->getFileExtension().c_str();
		m_sDescription += "</b></td>";
		m_sDescription += "<td><em>";
		m_sDescription += m_loader[i]->getFileDescription().c_str();
		m_sDescription += "</em></td>";
		m_sDescription += "</tr>";
	}
	
	m_sDescription += "</table>";
}

TextureLoaderPack::~TextureLoaderPack()
{
}

bool TextureLoaderPack::unloadPlg()
{
	for(unsigned int i = 0; i < m_loaded.size(); i++)
	{
		Renderer::getInstance()->getTextureManager()->removeTextureLoader(m_loaded[i]);
	}	

	return true;
}

bool TextureLoaderPack::loadPlg()
{
	for(unsigned int i = 0; i < m_loader.size(); i++)
	{
		if(Renderer::getInstance()->getTextureManager()->addTextureLoader(m_loader[i]))
		{
			m_loaded.push_back(m_loader[i]->getFileExtension());
		}
	}
	return true;
}
