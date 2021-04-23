/***************************************************************************
 *   Copyright (C) 2006 by The Hunter   *
 *   hunter@localhost   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include "./../include/objectexplorer.h"

using namespace BSScene;

namespace BSGui
{
    ObjectExplorer::ObjectExplorer(QWidget* parent)
    : QListWidget(parent)
    {
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        SceneManager* sm = SceneManager::getInstance();
        sb = &sm->getSelBuffer();
        connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectItem()));
        connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemRenamed(QListWidgetItem*)));
        connect(sm, SIGNAL(meshInserted(QString)),this, SLOT(addMesh(QString)));
        connect(sm, SIGNAL(meshRemoved(QString)),this, SLOT(removeMesh(QString)));
        connect(sm, SIGNAL(meshChange()), this, SLOT(buildList()));
        
        actionRenameCurrentItem = new QAction(tr("Rename"), this);
        actionDeleteCurrentItem = new QAction(tr("Delete"), this);
        
        connect(actionDeleteCurrentItem, SIGNAL(triggered()), this, SLOT(deleteCurrentItem()));
        connect(actionRenameCurrentItem, SIGNAL(triggered()), this, SLOT(renameCurrentItem()));
        
        connect(sb, SIGNAL(meshAdded(QString)), this, SLOT(itemSelected(QString)));
        connect(sb, SIGNAL(meshRemoved(QString)), this, SLOT(itemDeselected(QString)));
    }
    
    void ObjectExplorer::buildList()
    {
        while(count() > 0)
        {
            delete takeItem(0);
        }
        vector<QString> meshNames = SceneManager::getInstance()->getScene().getMeshNames();
        for(unsigned int i = 0 ; i < meshNames.size() ; i++)
        {
            addMesh(meshNames.at(i));
        } 
    }
    
    void ObjectExplorer::addMesh(QString name)
	{
        /*if(findItems(name, Qt::MatchExactly).count() != 0)
        {
            //The Item is already in the list
            return;
        }*/
 		QListWidgetItem* itemToAdd = new QListWidgetItem;
 		
	    itemToAdd->setText(name);
        itemToAdd->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        itemToAdd->setData(Qt::UserRole, name);
        addItem(itemToAdd);
	}
    
    void ObjectExplorer::renameCurrentItem()
    {
        editItem(currentItem());
    }

    void ObjectExplorer::deleteCurrentItem()
    {
        SceneManager* sm = SceneManager::getInstance();
        BSScene::MenuHandler* mh = sm->getMenuHandler();
        mh->edit_delete();
    }
    
    void ObjectExplorer::contextMenuEvent(QContextMenuEvent* event)
    {
        QListWidgetItem* item = itemAt(event->pos());
        if(item != NULL)
        {
            QMenu* menu = new QMenu(this);
            menu->addAction(actionRenameCurrentItem);
            menu->addAction(actionDeleteCurrentItem);
            menu->exec(event->globalPos());
        }
    }
    	
    void ObjectExplorer::removeMesh(QString name)
    {
		for(int i=0; i<count(); i++)
		{
			QListWidgetItem * delItem = item( i );	
			
			QString meshToDelete = delItem->text();
			
            if(meshToDelete.compare(name)==0)
            	delete delItem;
		}
    }
    
    void ObjectExplorer::itemDeselected(QString name)
    {
        disconnect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectItem()));
        qDebug() << "Item" << name << "has been deselected by scenegraph";
        //qDebug() << sender();
        for(int i = 0 ; i < count() ; i++)
        {
            QListWidgetItem* current = item(i);
            if(current->text() == name && current->isSelected())
            {
                current->setSelected(false);
                break;
            }
        }
        connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectItem()));
    }
    
    void ObjectExplorer::itemSelected(QString name)
    {
        disconnect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectItem()));
        qDebug() << "Item" << name << "has been selected by scenegraph";
        for(int i = 0 ; i < count() ; i++)
        {
            QListWidgetItem* current = item(i);
            if(current->text() == name && !current->isSelected())
            {
                current->setSelected(true);
                break;
            }
        }
        connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectItem()));
    }
    
    void ObjectExplorer::selectItem()
    {
        disconnect(sb, SIGNAL(meshAdded(QString)), this, SLOT(itemSelected(QString)));
        disconnect(sb, SIGNAL(meshRemoved(QString)), this, SLOT(itemDeselected(QString)));
        
        SceneManager* sm = SceneManager::getInstance();
        SelectionBuffer &sb = sm->getSelBuffer();
        Scene &s = sm->getScene();
        
        //disconnect(&sb, SIGNAL(meshAdded(QString)), this, SLOT(itemSelected(QString)));
        //disconnect(&sb, SIGNAL(meshRemoved(QString)), this, SLOT(itemDeselected(QString)));
        
        sb.reset();
        
        
		foreach(QListWidgetItem* currentItem, selectedItems())
        {
            QString meshName = currentItem->text();
            s.selectMeshByName(sb,meshName);
        }
        
        connect(&sb, SIGNAL(meshAdded(QString)), this, SLOT(itemSelected(QString)));
        connect(&sb, SIGNAL(meshRemoved(QString)), this, SLOT(itemDeselected(QString)));
        
        sm->checkForRedraw(true);
    }
    void ObjectExplorer::itemRenamed(QListWidgetItem* item)
    {
        QString newName = item->text();
        QString oldName = item->data(Qt::UserRole).toString();
        
        SceneManager* sm = SceneManager::getInstance();
        SelectionBuffer &sb = sm->getSelBuffer();
        Scene &s = sm->getScene();
        
        // Rename the mesh
        s.renameMesh(oldName, newName);
        
        item->setData(Qt::UserRole, newName);
    }
}
