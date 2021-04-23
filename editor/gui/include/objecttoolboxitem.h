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
*  File:       objecttoolboxitem.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 
 /** \file objecttoolboxitem.h
 * \brief Defines ObjectToolBoxItem class.
 * 
 * \author Thomas Tischler.
  */

#ifndef OBJECTTOOLBOXITEM_H
#define OBJECTTOOLBOXITEM_H
        
#include "bstoolboxitem.h"
#include "./../ui_selectButtons.h"
#include "./../ui_objectExplorer.h"
#include "./../ui_rotateButtons.h"
#include "mainwindow.h"
#include "bsflowlayout.h"

namespace BSGui
{
    /*! \brief This widget represents the objekt tool box.
     */
class ObjectToolBoxItem : public BSToolBoxItem
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief The Constructor of the Widget
        * 
        * \param parent The Widgets parent
        */
        ObjectToolBoxItem(QWidget* parent = 0);
        
        /*!
         * \brief Adds a given Widget to a given group box.
         * \param objectName The object name of the tool box.
         * \param widgetToAdd The widget that should be added.
         */
        virtual bool addWidgetToGroupBox(const QString& objectName, QWidget* widgetToAdd);
        
        /*!
         * \brief Gets the preferred plugin widget position.
         * 
         * \return The position.
         */
        int getPreferredPluginWidgetPosition();
        
        Ui_selectButtons selectButtons_ui; /*!< \brief The selectButtons UI object */
        Ui_objectExplorer objectExplorer_ui; /*!< \brief The objectExplorers UI object */
        Ui_rotateButtons rotateButtons_ui; /*!< \brief The rotateButtons UI object */
                                           
        /*!
         * \brief Inserts the widget after a specic widget.
         * 
         * \param widgetToInsert The widget you want to insert.
         * \param prevWidget The widget after wich yout widget sould be inerted. If prevWidget is NULL is is inserted at the first position.
         */
        void insertPluginWidget(QWidget* widgetToInsert, QWidget* prevWidget = NULL);
        
    public slots:
       /*!
        * \brief Called when the appropriate button was clicked.
        */
        void objectButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void surfaceButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void vertexButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void selectButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void moveButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void rotateButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void scaleButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void extrudeButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void createPolyButtonClicked();
        /*!
         * \brief Called when the appropriate button was clicked.
        */
        void createVertButtonClicked();
        
    private:
        /*!
        * \brief Calles setupUi for the mode buttons.
        */
        void setupModeButtons();
        /*!
         * \brief Calles setupUi for the object buttons.
         */
        void setupObjectsButtons();
        /*!
         * \brief Calles setupUi for the modifier buttons.
        */
        void setupModifiersButtons();
        /*!
         * \brief Makes an QPushButton with a specific icon and button.
         * \param icon The path to the icon.
         * \param tooltip The tooltip.
         * \param stdSize True if the buttons should have the standard size of the buttons
        */
        QPushButton* makeIconButton(QString icon, QString tooltip, bool stdSize = true);
        
        /*!
         * \brief Adds the buttons to the correct button group.
         */
        void setupButtonGroups();
        
        BSGroupBox* selectButtonsGroupBox; /*!< \brief The select buttons group box*/
        BSGroupBox* modeButtonsGroupBox; /*!< \brief The mode buttons group box*/
        BSGroupBox* commonObjectsGroupBox; /*!< \brief The common objects group box*/
        BSGroupBox* modificatorGroupBox; /*!< \brief The modificator group box*/
        BSGroupBox* objectExplorerGroupBox; /*!< \brief The object rxplorer group box*/
                                            
        QButtonGroup* buttonGroup; /*!< \brief The button group for the modeButtonsGroupBox, commonObjectsGroupBox and modificatorGroupBox */
};

}
        
#endif
