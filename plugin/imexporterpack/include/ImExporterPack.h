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
 *  File:       ImExporterPack.h
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMEXPORTERPACK_H_
#define IMEXPORTERPACK_H_

#include "ImExporterPackInterface.hh"
#include "ImporterMD2.h"
#include "Importer3DS.h"
#include "ImporterOBJ.h"
#include "ExporterOBJ.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class ImExporterPack, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The Im- and Exporter-Pack
*
* This plugin class provides a number of Im- and Exporter to load and save model files
*/
class ImExporterPack : public ImExporterPackInterface
{
	Q_OBJECT

public: 
    /*! \brief The default constructor
     */
	ImExporterPack();
	
    /*! \brief The destructor
     */	
	virtual ~ImExporterPack();

	/** \brief Returns the UID.
	 *
	 * \return The UID of this plugin.
	 */
	UID getUID()
	{
		return uid;
	}

	/** \brief Returns the name.
	 *
	 * \return The name of the plugin.
	 */
	QString getName()
	{
		return m_sName;
	}

	/** \brief Returns the author's name.
	 *
	 * \return The name of the author.
	 */
	QString getAutor()
	{
		return "Reinhard Jeschull";
	}

	/** \brief Returns the description.
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

	/** \brief Returns the version.
	 *
	 * \return The version of the plugin.
	 */
	Version getVersion()
	{
		return version;
	}

	/** \brief The init-methode.
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
	bool uninstallPlg()
	{
		return true;
	}

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
	void closePlg()
	{
	}
	
private:
	/** \brief Returns the menu and actions to add the plugin menus
	 *
	 * \param pMenu The pointer where to store the menu pointer
	 * \param pImport The pointer where to store the importer action pointer
	 * \param pExport The pointer where to store the exporter action pointer
	 */
	void getMenuActions(QMenu** pMenu, QAction** pImport, QAction** pExport);

private: 
	Version version; 		/*!< \brief The version of the plugin*/

	QString m_sName; 		/*!< \brief Name of the plugin*/
	QString m_sDescription; /*!< \brief Description of the plugin*/

	ImporterMD2* m_importerMD2; /*!< \brief The MD2-Importer*/
	Importer3DS* m_importer3DS; /*!< \brief The 3DS-Importer*/
	ImporterOBJ* m_importerOBJ; /*!< \brief The OBJ-Importer*/
	ExporterOBJ* m_exporterOBJ;	/*!< \brief The OBJ-Exporter*/
	
	QMenu* m_pMenuImporter;		/*!< \brief Menu for the importer*/
	QMenu* m_pMenuExporter;		/*!< \brief Menu for the exporter*/

public slots:
	void executeImporterMD2(); /*!< \brief Slot for executing the MD2-Importer*/
	void executeImporter3DS(); /*!< \brief Slot for executing the 3DS-Importer*/
	void executeImporterOBJ(); /*!< \brief Slot for executing the OBJ-Importer*/
	void executeExporterOBJ(); /*!< \brief Slot for executing the OBJ-Exporter*/
};

#endif /*IMEXPORTERPACK_H_*/
