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
*  File:       configbase.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file configbase.h
 * \brief Defines ConfigBase class.
 * 
 * \author Thomas Tischler.
 */
#ifndef CONFIGBASE_H
#define CONFIGBASE_H
#include <QtGui>
        
#include "../ui_ConfigBase.h"
#include "configinterface.h"
#include "./../../core/include/Log.h"
        
#include "pluginmanager.h"
#include "styleconfig.h"
#include "renderersettingsdialog.h"
#include "mouseconfig.h"

namespace BSGui
{

    /*! \brief This Widget represents the configuration dialog.
     * This dialog has no configuration options itself. It is a container for other configuration widgets.
     * See the ConfigInterface Class how to make your own config widget.
     */

class ConfigBase : public QDialog
{

    Q_OBJECT

public:
    /*!
    * \brief Constructs a new ConfigBase dialog.
    */
    ConfigBase();
    /*!
    * \brief Destroys the ConfigBase dialog.
    */
    virtual ~ConfigBase();

    /*!
    * \brief Add new configuration.
    * \param interfaceToAdd The ConfigInterface to add.
    */
    void addNewConfiguration(ConfigInterface* interfaceToAdd);

public slots:
    /*!
    * \brief Deplays the Item.
    * \param current The current Item.
    * \param previous The previous Item.
    */
    void displayItem(QListWidgetItem *current, QListWidgetItem *previous);
    /*!
    * \brief Aktivates the Apply button.
    */
    void aktivateApplyButton();
    /*!
    * \brief Deaktivates the Apply button.
    */
    void deaktivateApplyButton();
    
    /*!
    * \brief Aktivates the Reset button.
    */
    void aktivateResetButton();
    /*!
    * \brief Deaktivates the Apply button.
    */
    void deaktivateResetButton();

    /*! 
     * \brief Calls dialogAccept() on the current aktive dialog and calls deaktivateApplyButton().
     */
    void dialogAccepted();

    /*! 
     * \brief This is called when a config dialog has changed.
     * \param status Indicates if the dialog contains unsaved changes
     */
    void currentDialogModified(bool status);

    /*! 
     * \brief CThis calls the current aktive dialog reset slots.
     */
    void resetCurrentDialog();
    
private:
    Ui_ConfigBase ui; /*!< \brief The ui class */

    QList<ConfigInterface*> configInterfaceList; /*!< \brief The list of all ConfigInterface's */

    /*! 
     * \brief This is called when a config Dialog has changed.
     * \return Indicates if the dialog contains unsaved changes.
     */
    bool saveChanges();
};
}

#endif
