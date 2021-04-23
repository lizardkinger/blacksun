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
 *  File:       ManualEdit.cpp
 *  Created:    10.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/ManualEdit.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../scenegraph/include/Mesh.h"
#include "./../../scenegraph/include/Polygon.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../scenegraph/include/SelectionBuffer.h"
#include "./../../gui/include/mainwindow.h" 

using namespace BSScene;
using namespace BSRenderer;

static ManualEdit* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new ManualEdit();

	return static_cast<PlgInt*>(_instance);
}

ManualEdit::ManualEdit() : version(Version(1, 0, 0))
{
	m_sName = "Manual Edit";
	m_sDescription = "Allows to edit all data of the vertices manually";

	m_pActionManEdit = NULL;
	m_manualEdit = new ManEdit(0);
}

ManualEdit::~ManualEdit()
{
}

bool ManualEdit::unloadPlg()
{	
	if(m_pActionManEdit != NULL)
	{
		delete m_pActionManEdit;
		m_pActionManEdit = NULL;	
	}
	
	if(m_manualEdit != NULL)
	{
		delete m_manualEdit;
		m_manualEdit = NULL;	
	}
	
	return true;
}

bool ManualEdit::loadPlg()
{
	QMenu* pMenu = NULL;
	
	//Get the menu where to add the new menu items
	getMenu(&pMenu);

	m_pActionManEdit = pMenu->addAction("Manual Edit", this, SLOT(execute()), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_E));
	
	return true;
}

void ManualEdit::getMenu(QMenu** pMenu)
{
	QList<QAction*> pActions = BSGui::MainWindow::getInstance()->getMenuBar()->actions();

	//Walk through all menu-bar elements
	for(int i=0; i<pActions.size(); i++)
	{
		if(pActions[i]->text() == "Modify")
		{
			(*pMenu) = pActions[i]->menu();
			break;	
		}	
	}
}

void ManualEdit::execute()
{
	if(!m_manualEdit->isVisible())
	{
		m_manualEdit->createTableFromScene();
		m_manualEdit->setVisible(true);
	}
}

