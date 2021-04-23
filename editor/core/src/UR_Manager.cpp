/***************************************************************************
 *   Copyright (C) 2007 by Stefan Zeltner (Xexis)
 *   szeltne@fh-landshut.de
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
 *  Module:     Undo/Redo - Manager (BlackSun)
 *  File:       UR_Manager.cpp
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
 
#include "./../include/UR_Manager.h"
#include <QMessageBox>

namespace BSCore
{
UR_Manager* UR_Manager::m_instance = 0;

//Set version
BSPlgMgr::Version UR_Manager::version(1,0,0);

UR_Manager::UR_Manager() {}

UR_Manager::~UR_Manager() {}

UR_Manager* UR_Manager::getInstance()
{
	static Guardian g;
	
	if(m_instance == 0)
		m_instance = new UR_Manager;

	return m_instance;
}

void UR_Manager::redo()
{
	//QMessageBox::information(NULL, "REDO", "Redo - SLOT - Test");
	QUndoStack::redo();
}

void UR_Manager::undo()
{
	//QMessageBox::information(NULL, "UNDO", "Undo - SLOT - Test");
	QUndoStack::undo();
}

}
