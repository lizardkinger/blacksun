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
*  File:       objectexplorer.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
 /** \file objectexplorer.h
 * \brief Defines ObjectExplorer class.
 * 
 * \author Thomas Tischler.
  */
 
#ifndef OBJECTEXPLORER_H
#define OBJECTEXPLORER_H

#include <QtGui>
#include "./../../scenegraph/include/SceneManager.h"

namespace BSGui
{
    /*! \brief This widget allows to view, select, delete and rename meshes.
     */
class ObjectExplorer : public QListWidget
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief The Constructor of the Widget.
        * 
        * \param parent The Widgets parent.
        */
        ObjectExplorer(QWidget* parent = NULL);
        
    public slots:
        /*!
        * \brief Adds a Mesh.
        * 
        * \param name The Meshs name.
        */
    	void addMesh(QString name);
        /*!
         * \brief Removes a Mesh.
         * 
         * \param name The Meshs name.
         */
    	void removeMesh(QString name);
        /*!
         * \brief Deselectes an Mesh.
         * 
         * \param name The Meshs name.
         */
        void itemDeselected(QString name);
        /*!
         * \brief Selectes an Mesh.
         * 
         * \param name The Meshs name.
         */
        void itemSelected(QString name);
        /*!
         * \brief Selectes an Item.
         * 
         * \param added The items which where added.
         * \param removed The items which where removed.
         */
        void selectItem();
        /*!
         * \brief Renames an Item.
         * 
         * \param item The item.
         */
        void itemRenamed(QListWidgetItem* item);
        /*!
         * \brief Renames the current item.
         */
        void renameCurrentItem();
        /*!
         * \brief Deletes the current item.
         */
        void deleteCurrentItem();
        
        /*!
         * \brief Builds the mesh list.
         */
        void buildList();
        
    protected:
        /*!
        * \brief Handels the context menu event.
        * \param event The event.
        */
        void contextMenuEvent(QContextMenuEvent* event);
        
    private:
        BSScene::SelectionBuffer* sb; /*!< \brief The selection buffer object */
        
        QAction* actionDeleteCurrentItem; /*!< \brief Action to the delete the current Item */
        QAction* actionRenameCurrentItem; /*!< \brief Action to the rename the current Item */
                                          
};

}

#endif
