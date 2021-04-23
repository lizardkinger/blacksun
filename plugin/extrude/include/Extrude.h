/***************************************************************************
 *   Copyright (C) 2006-07 by Philipp Gruber
 *   pgruber@fh-landshut.de
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
 *  File:       Extrude.hh
 *  Created:    24.04.2007
 *  Author:     Thomas Kuhndoerfer (lizardkingx)
 *
 **************************************************************************/

#ifndef EXTRUDE_H_ 
#define EXTRUDE_H_

#include "ExtrudeInterface.hh"
#include <SceneGraph.h>
#include "./../ui_ExtrudeSettings.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

class Extrude : public ExtrudeInterface
{
	Q_OBJECT

public: 
	Extrude();
	virtual ~Extrude();

	/** Returns the UID.
	 *
	 * \return The UID of this plugin.
	 */
	UID getUID()
	{
		return uid;
	}

	/** Returns the name.
	 *
	 * \return The name of the plugin.
	 */
	QString getName()
	{
		return m_sName;
	}

	/** Returns the author's name.
	 *
	 * \return The name of the author.
	 */
	QString getAutor()
	{
		return "Thomas Kuhndoerfer";
	}

	/** Returns the description.
	 *
	 * Hyperlinks (http://... or mailto:...) will be translated into 'active'
	 * hyperlinks, so the user can click it
	 *
	 * \return The description of the plugin.
	 */
	QString getDescription()
	{
		return m_sDescription;
	}

	/** Returns the version.
	 *
	 * \return The version of the plugin.
	 */
	Version getVersion()
	{
		return version;
	}

	/** The init-methode.
	 *
	 * This is the first function, that the pluginsystem call. In these
	 * function it's necessary for the plugin to communication with the facade.
	 * goes wrong in the initialisation, the function returns false. If these
	 * happen, the plugin will not load.
	 *
	 * \return Returns true, if plugin was intizialized correctly, otherwise false.
	 */
	bool loadPlg();

	bool installPlg()
	{
		return true;
	}

	bool uninstallPlg()
	{
		return true;
	}

	bool unloadPlg();

	void closePlg()
	{
	}
		
private: 
	Version version;
	QPushButton* button;

	QString m_sName;
	QString m_sDescription;
	
	Ui_ExtrudeSettings m_ui;
	
	void extrudePolygon(BSScene::Polygon* p, double dAmount);

public slots:
	void execute();
};

#endif /*EXTRUDE_H_*/
