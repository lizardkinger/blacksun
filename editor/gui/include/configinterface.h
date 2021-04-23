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
*  File:       configinterface.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file configinterface.h
 * \brief Defines ConfigInterface class.
 * 
 * \author Thomas Tischler.
 */
#ifndef CONFIGINTERFACE_H
#define CONFIGINTERFACE_H
#include <QtGui>
        

namespace BSGui
{

    /*! \brief This Class acts as an abstract base class for all widgets that are displayed in the configuration dialog.
     */

class ConfigInterface : public QWidget
{
    Q_OBJECT
public:
    /*!
    * \brief The constructor.
    * \param parent The parent of this widget.
    */
    ConfigInterface(QWidget* parent = NULL);
    /*!
    * \brief The destructor.
    */
    virtual ~ConfigInterface() {}

    /*!
    * \brief Get the discription of the Config Dialog.
    * \return The description of the dialog.
    */
    virtual QString getDescription() const  = 0;
    /*!
    * \brief Get the Name of the Config Dialog.
    * \return The name of the dialog.
    */
    virtual QString getShortName() const = 0;
    /*!
    * \brief Get the Icon that is shown in the config Menu.
    * \return The icon of the dialog.
    */
    virtual QIcon getIcon() const  = 0;

    /*!
    * \brief Says if the dialoaf was modified.
    * \return Is true if the dialog was modified otherwise false.
    */
    bool isModified() {return configModified;}
    
public slots:
    /*!
    * \brief This is called when the Dialog was accepted.
    */
    virtual void dialogAccept() = 0;
    /*!
    * \brief This is called when the Dialog was rejected.
    */
    virtual void dialogReject() = 0;
    /*!
    * \brief This is called when the User clicked the reset Button.
    */
    virtual void reset() = 0;

signals:
    /*!
    * \brief This signal has to be emittet when the user changes something in the dialog that changed it's status.
    * The status indicates if the dialog contains unsaved changes made by the user
    * The parameter is true if the dialog contains unsaved changes
    * The parameter is false if the dialog does not contain unsaved changes
    */
    void statusChanged(bool);

private:
    bool configModified; /*!< \brief Is true if the dialog contains unsaved changes otherwise false */

private slots:
    /*!
    * \brief This is called when statusChanged signals was emitted.
    * \param status The status
    */
    void setModified(bool status);
    
};

}

#endif
