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
 *  Module:     Pluginsystem (BlackSun)
 *  File:       PlgMgr.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef PLGMGR_H_
#define PLGMGR_H_

#include "PluginInterface.hh"
#include "BSBaseTypes.h"
#include "BSPlugin.h"
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QMap>

/*! \file
* 
*  \brief The main file for the plugin system
*  \author Stefan Zeltner
* 
*/

/*! \brief The plugin system
*
*/
namespace BSPlgMgr
{
	
/*! \var typedef QMap<UID, BSPlugin*> PluginList
 * \brief The type definition for the PluginList.
 * 
 * The PluginList is a map with the UID as key and a BSPlugin* as value
 * 
 */
typedef QMap<UID, BSPlugin*> PluginList;

/*!
 * \brief The enum Types describes the plugin system flags which are possible.
 */
enum
{
	OK 					= 0x0000,
	NOT_INSTALLED		= 0x0001,
	ALREADY_RUNNING 	= 0x0002,
	ALREADY_STOPPED 	= 0x0004,
	ALREADY_INSTALLED	= 0x0008,
	FILE_NOT_FOUND		= 0x0010,
	VERSION_DISPUTE		= 0x0020,
	NOT_A_VALID_PLUGIN	= 0x0040,
	VERSION_ERROR		= 0x0080,
	IS_INSTALLED		= 0x0100,
	IS_LOADED			= 0x0200,
	PLG_SPECIFIC_ERROR	= 0x0400
};

/*! \brief The class PlgMgr
*
* This class represents the plugin manager
*/
class PlgMgr : public QObject
{
	Q_OBJECT
	
private:
	static PlgMgr* m_instance;	/*!< \brief The instance of the singleton*/
	PluginList m_plgList;		/*!< \brief A list with all installed plugins*/
	int errorFlag;				/*!< \brief a variable to store the flags*/
	
	/*! \brief Check a string for non-ASCII character
	 *
	 * \param str String for checking
	 */	
	bool onlyAscii(QString str);
	
	/*!
	 * \brief The constructor
	 */	
	PlgMgr();
	
	/*!
	 * \brief The destructor
	 */	
	virtual ~PlgMgr();

	/*! \brief The guard for the singleton 
	 *
	 * The guard for the singleton arrange that the destructor
	 * of the singleton will be called
	 */	
	class Guardian
	{
		public:
			/*!
			 * \brief The destructor
			 */
			virtual ~Guardian()
			{
				if(PlgMgr::m_instance != NULL)
				{
					delete PlgMgr::m_instance;
				}
			}
	};
	friend class Guardian;
	
	
public:
	/*!
	 * \brief The Version
	 */
	static Version version;
	
    /*! \brief getPlgInstance is uses for getting a handle from an another plugin
     * 
     * This function load plugins which this plugin need for their correct
     * work. To load the required plugin, you need the UID from the needed
     * plugin and it's minimal version. The specification say, that all plugins
     * had to be downwards compatible. If the PluginManager not found a plugin
     * with this UID or with a lower version, it returns NULL. It's recommend
     * to abort in this case the initPlg function when the plugin need the 
     * functions from the plugin which can't load. The editor shows to the
     * user, why the initialisation is aborted, also the plugin muss not give
     * the user an feedback.
     * 
     * \param uid The UID of the needed plugin
     * \param version The version of the needed plugin
     * \return The handle of the needed plugin
     */
	PlgInt* getPlgInstance(UID uid, const Version& version);
	
	/*! \brief Return the current value stored in errorFlag
	 * 
     * \return A flag to show the success
     */
     int getErrorFlag();
     
    /*!
     * \brief Clear the error flag
     */
     int clearErrorFlag();
	
	/*! \brief Install a plugin
	 * 
     * \param fileName The path to the plugin
     * \return A flag to show the success
     */
	int installPlg(QString fileName);
	
	/*! \brief Uninstall a plugin
	 * 
     * \param uid Uninstall the plugin with the UID uid
     * \return A flag to show the success
     */
	int uninstallPlg(UID uid);
	
	/*! \brief Get a version from a plugin
	 * 
     * \param uid Get the version from the selected plugin
     * \return The version
     */
	Version getVersion(UID uid);
	
	/*! \brief Get information about a plugin
	 * 
     * \param uid Get the information from the selected plugin
     * \return The about data string
     */
	QString getAboutData(UID uid);
	
	/*! \brief Get the name from a plugin
	 * 
     * \param uid Get the name from the selected plugin
     * \return The name
     */
	QString getName(UID uid);
	
	/*! \brief Get the current state from a plugin
	 * 
     * \param uid Get the state from the selected plugin
     * \return Return true, if the plugin is loaded, otherwise false
     */
	bool getStatus(UID uid);
	
	/*! \brief Get informations over the status and the version
	 * 
     * \param uid Select a plugin
     * \param version The version for comparison
     * \return A flag to show the results
     */
	int getInformation(UID uid, const Version& version);
	
	/*! \brief Get all installed plugins
	 * 
     * \return A list with the UIDs from all installed Plugins
     */
	QList<UID> getAvailablePlugins();
	
	/*!
	 * \brief Restore the state from all installed plugins from the last session
     */
	void restoreSavedStatus();
	
	/*! \brief Load a plugin
	 * 
     * \param uid Select a plugin
     * \return A flag to show the results
     */
	int loadPlugin(UID uid);
	
	/*! \brief Unload a plugin
	 * 
     * \param uid Select a plugin
     * \return A flag to show the results
     */
	int unloadPlugin(UID uid);
	
	/*!
	 * \brief Returns the instance of the undo/redo manager to give access to it
	 *
	 * \return The instance of the undo/redo manager
	 */
	static PlgMgr* getInstance();
	
signals:
	void stateChanged(UID uid);
	void setStatusBar(QString msg);
	void plgRemoved(UID uid);
	void plgInstalled(UID uid);
};

}

#endif /*PLGMGR_H_*/
