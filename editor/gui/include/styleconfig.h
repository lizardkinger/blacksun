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
*  File:       styleconfig.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/


/** \file styleconfig.h
 * \brief Defines StyleConfig class.
 * 
 * \author Thomas Tischler.
 */

#ifndef STYLECONFIG_H
#define STYLECONFIG_H

#include "configinterface.h"
#include <QtGui>
        
#include "../ui_lookandfeel.h"

namespace BSGui
{
        /*! \brief This widget allows to change the styles of blacksun at runtime.
         */
class StyleConfig : public ConfigInterface
{
    
    Q_OBJECT
    
public:
    /*!
    * \brief The Constructor of the Widget.
    * 
    */
    StyleConfig();

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
    ~StyleConfig();
    
    private:
        Ui_lookandfeel ui; /*!< \brief The UI object */
        
        /*!
        * \brief Gets if the style was changed.
        * \return true if the style was changed false otherwise.
        */
        bool styleChanged();
        QStyle* previewStyle; /*!< \brief The current style */

    public slots:
        /*!
        * \brief Change the preview another style.
        * \param styleString The key of the new style.
        */
        void changePreview(QString styleString);
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

};

}

#endif
