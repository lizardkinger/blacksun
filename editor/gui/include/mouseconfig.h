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
*  File:       mouseconfig.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 /** \file mouseconfig.h
 * \brief Defines MouseConfig class.
 * 
 * \author Thomas Tischler.
  */
 
#ifndef MOUSECONFIG_H
#define MOUSECONFIG_H

#include "configinterface.h"
#include "mousesettings.h"
#include <QtGui>
        
#include "../ui_mouseConfig.h"

namespace BSGui
{
    /*! \brief This widget allows to edit the mouse options.
     */
    class MouseConfig : public ConfigInterface
    {
    
        Q_OBJECT
    
        public:
            /*!
            * \brief The constructor.
            */
            MouseConfig();

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
    
            /*!
             * \brief The destructor.
            */
            ~MouseConfig();
            
    
        private:
            Ui_mouseConfig ui; /*!< \brief The ui class */
            
            /*!
             * \brief Fills an QComboBox with the appropriate contents based on its object name.
             * \param boxToFill The box that should be filled.
             */
            void fillBox(QComboBox* boxToFill);
            
            MouseSettings* settings; /*!< \brief The MouseSettings object */
            
            /*!
             * \brief Converts a QString to a MouseSettings::MouseAction.
             * \param string The string to convert.
             */
            MouseSettings::MouseAction StringToEnum(QString string);
            
            /*!
             * \brief Converts a objectName to a property name.
             * \param object The object wich name to read.
             */
            QByteArray objectNameToPropertyName(QObject* object);

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
            
            void checkForChanges();

    };

}

#endif
