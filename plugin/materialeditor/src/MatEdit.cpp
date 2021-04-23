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
 *  File:       MatEdit.cpp
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/MatEdit.h"
#include <SceneGraph.h>
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static MatEdit* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new MatEdit();

	return static_cast<PlgInt*>(_instance);
}

MatEdit::MatEdit() : version(Version(1, 0, 0))
{
	m_sName = "Material-Editor";
	m_sDescription = "Material-Editor";
	m_matEdit = new MaterialEditor(0);
	
	m_pActionMatEdit = NULL;
}

MatEdit::~MatEdit()
{
}

bool MatEdit::unloadPlg()
{
	if(m_pActionMatEdit!=NULL)
	{
		delete m_pActionMatEdit;
		m_pActionMatEdit = NULL;
	}
	
	if(m_matEdit!=NULL)
	{
		delete m_matEdit;
		m_matEdit = NULL;
	}
	
	return true;
}

bool MatEdit::loadPlg()
{
	QMenu* pMenu = NULL;
	
	//Get the menu where to add the new menu items
	getMenu(&pMenu);

	m_pActionMatEdit = pMenu->addAction("Material editor", this, SLOT(execute()));
	
	return true;
}

void MatEdit::getMenu(QMenu** pMenu)
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

void MatEdit::execute()
{
	if(!m_matEdit->isVisible())
	{
		m_matEdit->refreshEditor();
		m_matEdit->setVisible(true);
	}
}
