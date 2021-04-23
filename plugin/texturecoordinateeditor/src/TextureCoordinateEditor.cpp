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
 *  File:       TextureCoordinateEditor.cpp
 *  Created:    22.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/TextureCoordinateEditor.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../scenegraph/include/Mesh.h"
#include "./../../scenegraph/include/Polygon.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../scenegraph/include/SelectionBuffer.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSScene;
using namespace BSRenderer;

static TextureCoordinateEditor* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new TextureCoordinateEditor();

	return static_cast<PlgInt*>(_instance);
}

TextureCoordinateEditor::TextureCoordinateEditor() : version(Version(1, 0, 0))
{
	m_sName = "Texture coordinate editor";
	m_sDescription = "A editor, which allows the user to texture a mesh confortable ";
	m_sDescription += "by manipulate the texture coordinates";
	
	m_pActionTexCoordEdit = NULL;
	m_texCoordEditor = new TexCoordEditor(0);
}

TextureCoordinateEditor::~TextureCoordinateEditor()
{
}

bool TextureCoordinateEditor::unloadPlg()
{
	if(m_pActionTexCoordEdit != NULL)
	{
		delete m_pActionTexCoordEdit;
		m_pActionTexCoordEdit = NULL;	
	}
	
	if(m_texCoordEditor != NULL)
	{
		delete m_texCoordEditor;
		m_texCoordEditor = NULL;	
	}	
		
	return true;
}

bool TextureCoordinateEditor::loadPlg()
{
	QMenu* pMenu = NULL;
	
	//Get the menu where to add the new menu items
	getMenu(&pMenu);

	m_pActionTexCoordEdit = pMenu->addAction("Texture coordinate editor", this, SLOT(execute()));
	
	return true;
}

void TextureCoordinateEditor::getMenu(QMenu** pMenu)
{
	QList<QAction*> pActions = BSGui::MainWindow::getInstance()->getMenuBar()->actions();

	//Walk through all menu-bar elements
	for(int i=0; i<pActions.size(); i++)
	{
		if(pActions[i]->text() == "Material")
		{
			(*pMenu) = pActions[i]->menu();
			break;	
		}	
	}
}

void TextureCoordinateEditor::execute()
{
	if(!m_texCoordEditor->isVisible())
	{
		m_texCoordEditor->fillFromScene();
		m_texCoordEditor->setVisible(true);
	}
}

