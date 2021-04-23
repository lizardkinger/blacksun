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
*  File:       renderersettingsdialog.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 
 /** \file renderersettingsdialog.h
 * \brief Defines RendererSettingsDialog class.
 * 
 * \author Thomas Tischler.
  */
 

#ifndef RENDERERSETTINGSDIALOG_H_
#define RENDERERSETTINGSDIALOG_H_

#include <QtGui>
#include <QtXml>
#include "./../ui_rendererSettings.h"
#include "configinterface.h"
#include "mainwindow.h"
#include "./../../renderer/include/Renderer.h"
#include "./../../renderer/include/RendererSettings.h"

namespace BSGui
{
        /*! \brief This widget represents a dieloag to edit the settings of the renderer.
         */
class RendererSettingsDialog : public ConfigInterface
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief The constructor.
        */
        RendererSettingsDialog();
        /*!
         * \brief The Destructor.
         */
        virtual ~RendererSettingsDialog() {}

        /*!
         * \brief Get the discription of the Config Dialog.
         * \return The description of the dialog.
         */
        virtual QString getDescription() const  {return QString("Renderer Settings");}
        /*!
         * \brief Get the Name of the Config Dialog.
         * \return The name of the dialog.
         */
        virtual QString getShortName() const  {return QString("Renderer");}
        /*!
         * \brief Get the Icon that is shown in the config Menu.
         * \return The icon of the dialog.
         */
        virtual QIcon getIcon() const {return QIcon(":configdialog/media/exec.png");}
        
        /*!
         * \brief Gets the color scheme in form of an string.
         * \return The xml document as a QString.
         */
        QString getColorScheme();
        /*!
         * \brief Sets the color scheme in form of an string.
         * \param colorScheme The xml document as a QString.
         */
        bool setColorScheme(const QString& colorScheme);
    
    public slots:
        /*!
        * \brief This is called when the Dialog was accepted.
        */
        virtual void dialogAccept();
        /*!
         * \brief This is called when the Dialog was rejected.
        */
        virtual void dialogReject() {}
        /*!
         * \brief This is called when the User clicked the reset Button.
        */
        virtual void reset();
        
        /*!
         * \brief Called when the saveColorButton was clicked.
         */
        void saveColorButtonClicked();
        /*!
         * \brief Called when the loadColorButton was clicked.
         */
        void loadColorButtonClicked();
        /*!
         * \brief Called when the resetColorsButton was clicked.
         */
        void resetColorsButtonClicked();
        /*!
         * \brief Called when one of the options where changed.
         */
        void optionsChanged();
        
        
    private:
        Ui_rendererSettings ui; /*!< \brief The UI class for this widget */
        
        /*!
         * \brief This reads all color options.
         */
        void setColors();
        /*!
         * \brief This reads all number options.
         */
        void setNumbers();
        /*!
         * \brief This reads all true/false options.
         */
        void setCheckboxses();

        /*!
         * \brief This sets all color options.
         */
        void writeColors();
        /*!
         * \brief This sets all numbers options.
         */
        void writeNumbers();
        /*!
         * \brief This sets all true/false options.
         */
        void writeCheckboxses();
        
        /*!
         * \brief Checks if an option was changed.
         * \return Is true if an option was changed otherwise false.
         */
        bool checkOptionsChanged();
        
        BSRenderer::RendererSettings* rendererSettings; /*!< \brief The RendererSettings object */
        BSRenderer::Renderer* renderer; /*!< \brief The Renderer object */
        
        /*!
         * \brief Gives the color from a QDomElement.
         * \param e The QDomElement.
         * \return Is color.
         */
        QColor colorFromElement(QDomElement e);
        
};
}

#endif
