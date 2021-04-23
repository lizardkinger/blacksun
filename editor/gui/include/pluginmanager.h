/***************************************************************************
*   Copyright (C) 2006 by Thomas Tischler
*   ttischl1@fh-landshut.de
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
****************************************************************************
*
*  Module:     GUI (BlackSun)
*  File:       pluginmanager.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/



/** \file pluginmanager.h
 * \brief Defines PluginManager class.
 * 
 * \author Thomas Tischler.
 */

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDir>

#include "configinterface.h"
#include "bstreewidgetitem.h"
#include "../ui_pluginConfig.h"
#include "../../pluginsystem/include/PlgMgr.h"

namespace BSGui
{

    /*! \brief This widget represents the plugin manager.
     */

class PluginManager : public ConfigInterface
{

Q_OBJECT

public:
    /*!
    * \brief The constructor.
    */
    PluginManager();

    virtual ~PluginManager();
    
    /*!
    * \brief Get the discription of the Config Dialog.
    * \return The description of the dialog.
    */
    virtual QString getDescription() const ;
    /*!
    * \brief Get the Name of the Config Dialog.
    * \return The name of the dialog.
    */
    virtual QString getShortName() const ;
    /*!
    * \brief Get the Icon that is shown in the config Menu.
    * \return The icon of the dialog.
    */
    virtual QIcon getIcon() const ;

    Ui_pluginConfig ui; /*!< \brief The UI object */
    
public slots:
    /*!
    * \brief This is called when the Dialog was accepted.
    */
    virtual void dialogAccept();
    /*!
    * \brief This is called when the Dialog was rejected.
    */
    virtual void dialogReject();
    /*!
    * \brief This is called when the User clicked the reset Button.
    */
    virtual void reset();
    /*!
    * \brief This is called when the load button was clicked.
    */
    void loadButtonClicked();
    /*!
    * \brief This is called when the install button was clicked.
    */
    void installButtonClicked();
    /*!
    * \brief This is called when the unload button was clicked.
    */
    void unloadButtonClicked();
    /*!
    * \brief This is called when the uninstall button was clicked.
    */
    void uninstallButtonClicked();
    /*!
    * \brief This is called when aplugin was removed.
    * \param uid The UID of the plugin.
    */
    void removePlugin(UID uid);
    /*!
    * \brief This is called when a plugin was installed.
    * \param uid The UID of the plugin.
    */
    void pluginInstalled(UID uid);
    /*!
    * \brief This is called when the user clicks on an plugin.
    * \param current The current Item.
    * \param previous The previous Item.
    */
    void setAboutStackIndex(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    /*!
    * \brief This is called when a plugin changed its status.
    * \param uid The UID of the plugin.
    */
    void changeState(UID uid);
    /*!
    * \brief This is called when the search path was changed.
    */
    void changeSearchPath();


private:
    QList<UID> availablePlugins; /*!< \brief The availablePlugins */
    BSPlgMgr::PlgMgr* pluginManager; /*!< \brief The PlgMgr object */
    /*!
    * \brief Adds a plugin.
    * \param uid The UID of the plugin.
    */
    void addPlugin(UID pluginToAdd);
    /*!
    * \brief Initializes the plugin list.
    */
    void setupPluginList();
    //void regeneratePluginList();
    /*!
    * \brief Throws an error message.
    */
    void throwMessage(int pluginManagerFlag);
};

}

#endif
