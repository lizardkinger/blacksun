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
*  File:       screenshotmaker.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 
 /** \file screenshotmaker.h
 * \brief Defines ScreenshotMaker class.
 * 
 * \author Thomas Tischler.
  */
 
#include <QtGui>
#include "./../ui_screenshotMaker.h"
#include "bsrenderwidget.h"

#ifndef SCREENSHOTMAKER_H
#define SCREENSHOTMAKER_H

namespace BSGui
{

class ScreenshotMaker : public QDialog
{
/*! \brief This dialog allows the user to make screenshots from the scene.
*/
    Q_OBJECT
    
    public:
        /*!
        * \brief The Constructor of the Widget.
        * 
        * \param parent The Widgets parent.
        */
        ScreenshotMaker(QWidget* parent = NULL);
        /*!
         * \brief The destructor of the Widget.
         */
        virtual ~ScreenshotMaker();
        
    public slots:
        /*!
        * \brief This si called when the save button was clicked.
        */
        void saveButtonClicked();
        /*!
         * \brief Adds a GLWidget.
         * 
         * \param key The key of the widget you want to add.
         */
        void addGLWidget(const QString& key);
        
    private:
        Ui_screenshotMakerDialog ui; /*!< \brief The UI object of this widget */
        QDir screenshotDirectory; /*!< \brief The directory where the screenshot should be saved */
        QString projectName; /*!< \brief The name of the current project */
        QStringList supportedFormats; /*!< \brief The formats that are supported by this system */
        QStringList knownResolutions; /*!< \brief The standart resultions */
        /*!
         * \brief Sets the current filename to the project file name if the project file name is already in use projectName1 will be chossen etc..
         * 
         * \param fileName The project name.
         */
        void setFreeFile(QString fileName = QString());
        double aspectRatio; /*!< \brief The aspect ratio */
        bool lockWidthHeight; /*!< \brief If true aspect ratio is kept */
        /*!
         * \brief Sets the add and removes button to be enabled or disabled.
         */
        void checkAddRemoveButtons();
        
        /*!
         * \brief Reads the settings.
         */
        void readSettings();
        /*!
         * \brief Writes the settings.
         */
        void writeSettings();
        
        /*!
         * \brief Sets the current filename to the project file name if the project file name is already in use projectName1 will be chossen etc..
         * 
         * \return The current filename.
         */
        QString getCurrentFileType();
        
    private slots:
       /*!
        * \brief Sets the suffix to suffix
        * 
        * \param suffix The suffix.
        */
        void setSuffix(QString suffix);
        /*!
         * \brief Enables or disables the quality slider.
         * 
         * \param suffix The current format.
         */
        void setQuality(QString format);
        /*!
         * \brief Enables the save button if the destination is valid and disables the button otherwise.
        */
        void checkDestination();
        /*!
         * \brief Called when the file button was clicked.
         */
        void fileButtonClicked();
        /*!
         * \brief Called when the resultion combo box changed.
        */
        void resolutionComboChanged();
        /*!
         * \brief Called when the width box changed.
         */
        void widthBoxChanged();
        /*!
         * \brief Called when the height box changed.
         */
        void heightBoxChanged();
        /*!
         * \brief Called when the add button was clicked.
         */
        void addButtonClicked();
        /*!
         * \brief Called when the remove button was clicked.
         */
        void removeButtonClicked();
        
        /*!
         * \brief Called when the byFileExtension checkbox was clicked.         * 
         * \param status The status.
         */
        void byFileExtensionClicked(bool status);
        /*!
         * \brief Called when the autoFileExtension checkbox was clicked.         * 
         * \param status The status.
         */
        void autoFileExtensionClicked(bool status);
};

}

#endif
