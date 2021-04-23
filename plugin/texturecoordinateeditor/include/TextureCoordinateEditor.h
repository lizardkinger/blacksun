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
 *  File:       TextureCoordinateEditor.h
 *  Created:    22.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURECOORDINATEEDITOR_H_
#define TEXTURECOORDINATEEDITOR_H_

#include "TextureCoordinateEditorInterface.hh"
#include "TexCoordEditor.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class TextureCoordinateEditor, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The texture coordinate editor
*
* This plugin class provides a dialog to edit the texture coordinates
*/
class TextureCoordinateEditor : public TextureCoordinateEditorInterface
{
	Q_OBJECT

public: 
	/*! The default constructor
	 */
	TextureCoordinateEditor();

	/*! The destructor
	 */
	virtual ~TextureCoordinateEditor();

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
		return "Reinhard Jeschull";
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

	/*! The install - function.
	 * 
	 * This is the first function, that the pluginsystem call.
	 * 
	 * \return Returns true, if plugin was installed correctly, otherwise false.
	 */
	bool installPlg()
	{
		return true;
	}

	/*! The uninstall - function.
	 * 
	 * This function is called, if the Plugin is uninstalled by the user. It
	 * can do here things (e.g. remove the plugin control in the gui), before
	 * the Konstruktor is called.
	 * 
	 * \return Returns true, if plugin was uninstalled correctly, otherwise false.
	 */
	bool uninstallPlg()
	{
		return true;
	}

	/*! The unload - function.
	 * 
	 * This function is called, if the Plugin is unloaded by the user. Here
	 * should the Plugin remove all it's control elements (e.g. from the GUI).
	 * 
	 * \return Returns true, if plugin was unloaded correctly, otherwise false.
	 */
	bool unloadPlg();

	/*! The close - function.
	 * 
	 * This function is called, if the application is finished from the user.
	 * The Plugin should do their clear jobs (e.g. let of memory)
	 */
	void closePlg()
	{
	}

private:
	/** \brief Returns the menu to add the plugin menu
	 *
	 * \param pMenu The pointer where to store the menu pointer
	 */
	void getMenu(QMenu** pMenu);
	
private: 
	Version version;		/*!< \brief The version of the plugin*/

	QString m_sName;		/*!< \brief Name of the plugin*/
	QString m_sDescription;	/*!< \brief Description of the plugin*/

	QAction* m_pActionTexCoordEdit; /*!< \brief Action for the menu item*/

	TexCoordEditor* m_texCoordEditor; /*!< \brief The texture coordinate editor dialog*/
public slots:
	void execute();	/*!< \brief Slot for executing this plugin*/
};

#endif /*TEXTURECOORDINATEEDITOR_H_*/
