/***************************************************************************
 *   Copyright (C) 2006-07 by Stefan Zeltner
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
 *  Module:     Plugin (BlackSun)
 *  File:       Updater.h
 *  Created:    14.04.2007
 *  Author:     Stefan Zeltner
 *
 **************************************************************************/

#ifndef UPDATER_H_
#define UPDATER_H_

#include "Updater.hh"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class updater, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

/*! \brief The updater
*
* This plugin collect required information and start the updater - application.
*/
class Updater : public UpdaterInterface
{
	Q_OBJECT
private:
	QPushButton* button;	/*!< \brief The button for the toolbox*/
	const QString fileName;	/*!< \brief The paht of the updater - application*/
	QAction* updaterAction;	/*!< \brief The action for the menu*/

public: 
	/*! \brief The default constructor
	 */
	Updater();

	/*! \brief The destructor
	 */
	virtual ~Updater();

	/*! \brief Returns the UID.
	 *
	 * \return The UID of this plugin.
	 */
	UID getUID()
	{
		return uid;
	}

	/*! \brief Returns the name.
	 *
	 * \return The name of the plugin.
	 */
	QString getName()
	{
		return "Updater";
	}

	/*! \brief Returns the author's name.
	 *
	 * \return The name of the author.
	 */
	QString getAutor()
	{
		return "Stefan Zeltner";
	}

	/*! \brief Returns the description.
	 *
	 * Hyperlinks (http://... or mailto:...) will be translated into 'active'
	 * hyperlinks, so the user can click it
	 *
	 * \return The description of the plugin.
	 */
	QString getDescription()
	{
		return "A plugin for the online update.";
	}

	/*! \brief Returns the version.
	 *
	 * \return The version of the plugin.
	 */
	Version getVersion()
	{
		return Version(1, 0, 0);
	}

	/*! \brief The init-methode.
	 *
	 * This is the first function, that the pluginsystem call. In these
	 * function it's necessary for the plugin to communication with the facade.
	 * goes wrong in the initialisation, the function returns false. If these
	 * happen, the plugin will not load.
	 *
	 * \return Returns true, if plugin was intizialized correctly, otherwise false.
	 */
	bool loadPlg();

	/*! \brief The install - function.
	 * 
	 * This is the first function, that the pluginsystem call.
	 * 
	 * \return Returns true, if plugin was installed correctly, otherwise false.
	 */
	bool installPlg()
	{
		return true;
	}

	/*! \brief The uninstall - function.
	 * 
	 * This function is called, if the Plugin is uninstalled by the user. It
	 * can do here things (e.g. remove the plugin control in the gui), before
	 * the Konstruktor is called.
	 * 
	 * \return Returns true, if plugin was uninstalled correctly, otherwise false.
	 */
	bool uninstallPlg();

	/*! \brief The unload - function.
	 * 
	 * This function is called, if the Plugin is unloaded by the user. Here
	 * should the Plugin remove all it's control elements (e.g. from the GUI).
	 * 
	 * \return Returns true, if plugin was unloaded correctly, otherwise false.
	 */
	bool unloadPlg();

	/*! \brief The close - function.
	 * 
	 * This function is called, if the application is finished from the user.
	 * The Plugin should do their clear jobs (e.g. let of memory)
	 */
	void closePlg();

public slots:
	void startUpdate();	/*!< \brief Slot for executing this plugin*/
};

#endif /*UPDATER_H_*/
