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
*  File:       materialexplorer.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 /** \file materialexplorer.h
 * \brief Defines MaterialExplorer class.
 * 
 * \author Thomas Tischler.
  */
 
#ifndef MATERIALEXPLORER_H
#define MATERIALEXPLORER_H

#include <QtGui>
#include "./../../scenegraph/include/SceneManager.h"
#include "../../pluginsystem/include/PluginInterface.hh"
#include "../../pluginsystem/include/PlgMgr.h"

namespace BSGui
{
    /*! \brief This widget allows to view and rename materials.
     */
    class MaterialExplorer : public QListWidget
    {
    
        Q_OBJECT
    
        public:
            /*!
            * \brief The Constructor of the Widget
            * 
            * \param parent The Widgets parent
            */
            MaterialExplorer(QWidget* parent = NULL);
            
        public slots:
            /*!
            * \brief Adds a material 
            * 
            * \param materialNumber The material number.
            */
            void addMaterial(int materialNumber);
            /*!
             * \brief Removes a material 
             * 
             * \param materialNumber The material number.
            */
            void removeMaterial(int materialNumber);
            /*!
             * \brief Updates a material 
             * 
             * \param materialNumber The material number.
            */
            void updateMaterial(int materialNumber);
            
            /*!
             * \brief Sets the name of that material that is connected to the item 
             * 
             * \param item The item.
            */
            void setMaterialName(QListWidgetItem* item);
            
            /*!
             * \brief Sets the selected materials.
            */
            void setSelectedMaterial();
            
            /*!
             * \brief Renames the current item.
            */
            void renameCurrentItem();
            /*!
             * \brief Deletes the current item.
            */
            void deleteCurrentItem();
            /*!
             * \brief Renames the current item. 
            */
            void editCurrentItem();
            /*!
             * \brief Apply the current selected material on the objects that a currently in the selectionbuffer.
            */
            void applyCurrentItem();
            
        protected:
            /*!
            * \brief Handels the context menu event.
            * \param event The event.
            */
            void contextMenuEvent(QContextMenuEvent* event);
            
        private:
            MaterialManager* materialManager; /*!< \brief The material manager */
            BSScene::SceneManager* sceneManager; /*!< \brief The scene manager */
            /*!
             * \brief Finds the item to the correspoding material number.
             * \param materialNumber The material number.
             * \return The correspoding item.
             */
            QListWidgetItem* findMaterial(int materialNumber);
            QAction* actionDeleteCurrentItem; /*!< \brief The action to delete the current item */
            QAction* actionRenameCurrentItem; /*!< \brief The action to rename the current item */
            QAction* actionEditCurrentItem; /*!< \brief The action to edit the current item */
            QAction* actionApplyCurrentItem; /*!< \brief The action to apply the current item */
            
            /*!
             * \brief If the materialeditor is installed it gets a pointer to the material editor object other wise returns NULL.
             * \return The materialeditor object.
            */
            QObject* getMaterialEditorPlugin();
            
            static const unsigned long materialEditorMagic; /*!< \brief The material editor uid */
    };

}

#endif
