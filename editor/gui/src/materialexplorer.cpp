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
 
#include "./include/materialexplorer.h"

namespace BSGui
{
    
    const unsigned long MaterialExplorer::materialEditorMagic = 112186UL;

MaterialExplorer::MaterialExplorer(QWidget* parent)
    : QListWidget(parent)
{
    materialManager = BSRenderer::MaterialManager::getInstance();
    sceneManager = BSScene::SceneManager::getInstance();
    connect(materialManager, SIGNAL(materialAdded(int)), this, SLOT(addMaterial(int)));
    connect(materialManager, SIGNAL(materialRemoved(int)), this, SLOT(removeMaterial(int)));
    connect(materialManager, SIGNAL(materialChanged(int)), this, SLOT(updateMaterial(int)));
    
    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(setMaterialName(QListWidgetItem*)));
    
    int materialNumber = materialManager->getStandardMaterialID();
    
    actionRenameCurrentItem = new QAction(tr("Rename"), this);
    actionDeleteCurrentItem = new QAction(tr("Delete"), this);
    actionEditCurrentItem = new QAction(tr("Edit"), this);
    actionApplyCurrentItem = new QAction(tr("Apply"), this);
    
    QListWidgetItem* itemToAdd = new QListWidgetItem;
    itemToAdd->setText(QString::fromStdString(materialManager->getMaterial(materialNumber)->getName()));
    itemToAdd->setData(Qt::UserRole, materialNumber);
    itemToAdd->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    addItem(itemToAdd);
    
    connect(actionDeleteCurrentItem, SIGNAL(triggered()), this, SLOT(deleteCurrentItem()));
    connect(actionRenameCurrentItem, SIGNAL(triggered()), this, SLOT(renameCurrentItem()));
    connect(actionEditCurrentItem, SIGNAL(triggered()), this, SLOT(editCurrentItem()));
    connect(actionApplyCurrentItem, SIGNAL(triggered()), this, SLOT(applyCurrentItem()));
    connect(&sceneManager->getSelBuffer(), SIGNAL(selectionChanged()), this, SLOT(setSelectedMaterial()));
    //connect(sceneManager, SIGNAL(sceneChanged()), this, SLOT(setSelectedMaterial()));
}

void MaterialExplorer::renameCurrentItem()
{
    editItem(currentItem());
}

void MaterialExplorer::deleteCurrentItem()
{
    QListWidgetItem* item = currentItem();
    int materialNumber = item->data(Qt::UserRole).toInt();
    materialManager->removeMaterial(materialNumber);
}

void MaterialExplorer::applyCurrentItem()
{
    QListWidgetItem* item = currentItem();
    BSScene::SelectionBuffer* selectionBuffer = &BSScene::SceneManager::getInstance()->getSelBuffer();
    selectionBuffer->setMaterial(item->data(Qt::UserRole).toInt());
    setSelectedMaterial();
}

void MaterialExplorer::editCurrentItem()
{
    QObject* materialEditor = getMaterialEditorPlugin();
    if(materialEditor != NULL)
    {
        QMetaObject::invokeMethod(materialEditor, "execute");
    }
}

void MaterialExplorer::contextMenuEvent(QContextMenuEvent* event)
{
    QObject* materialEditor = getMaterialEditorPlugin();
    QListWidgetItem* item = itemAt(event->pos());
    if(item != NULL)
    {
        QMenu* menu = new QMenu(this);
        menu->addAction(actionApplyCurrentItem);
        if(item->data(Qt::UserRole).toInt() != materialManager->getStandardMaterialID())
        {
            menu->addAction(actionRenameCurrentItem);
            menu->addAction(actionDeleteCurrentItem);
            if(materialEditor != NULL)
            {
                menu->addAction(actionEditCurrentItem);
            }
        }
        menu->exec(event->globalPos());
    }
}

void MaterialExplorer::addMaterial(int materialNumber)
{
    for(int i = 0 ; i < count() ; i++)
    {
        if(item(i)->data(Qt::UserRole).toInt() == materialNumber)
        {
            return;
        }
    }
    QListWidgetItem* itemToAdd = new QListWidgetItem;
    itemToAdd->setText(QString::fromStdString(materialManager->getMaterial(materialNumber)->getName()));
    itemToAdd->setData(Qt::UserRole, materialNumber);
    itemToAdd->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    addItem(itemToAdd);
}

void MaterialExplorer::removeMaterial(int materialNumber)
{
    delete findMaterial(materialNumber);
}

void MaterialExplorer::updateMaterial(int materialNumber)
{
    QListWidgetItem* itemToUpdate = findMaterial(materialNumber);
    if(itemToUpdate != NULL)
    {
        findMaterial(materialNumber)->setText(QString::fromStdString(materialManager->getMaterial(materialNumber)->getName()));
    }
}
QListWidgetItem* MaterialExplorer::findMaterial(int materialNumber)
{
    QListWidgetItem* itemToFind = NULL;
    for(int i = 0 ; i < count() ; i++)
    {
        itemToFind = item(i);
        if(itemToFind->data(Qt::UserRole).toInt() == materialNumber)
        {
            return itemToFind;
        }
    }
    return itemToFind;
}

void MaterialExplorer::setSelectedMaterial()
{
    
    for(int i = 0 ; i < count() ; i++)
    {
        item(i)->setData(Qt::ForegroundRole, Qt::black);
    }
    
    BSScene::SelectionBuffer* selectionBuffer = &BSScene::SceneManager::getInstance()->getSelBuffer();
    std::list<BSScene::SceneObject*> objectList = selectionBuffer->getSelectedObjects();
    for(std::list<BSScene::SceneObject*>::iterator i = objectList.begin() ; i != objectList.end() ; ++i)
    {
        int currentId = (*i)->getMaterialID();
        qDebug() << currentId;
        for(int i = 0 ; i < count() ; i++)
        {
            QListWidgetItem* current = item(i);
            if(current->data(Qt::UserRole).toInt() == currentId)
            {
                current->setData(Qt::ForegroundRole, Qt::blue);
                break;
            }
        }
    }
}

QObject* MaterialExplorer::getMaterialEditorPlugin()
{
    QObject* materialManager = NULL;
    PlgInt* interface = BSPlgMgr::PlgMgr::getInstance()->getPlgInstance(materialEditorMagic,Version(1,0,0));
    if(interface != NULL)
    {
        if(BSPlgMgr::PlgMgr::getInstance()->getStatus(materialEditorMagic))
        {
            materialManager = dynamic_cast<QObject*>(interface);
            if(materialManager != NULL)
            {
                const QMetaObject* materialManagerMeta = materialManager->metaObject();
                if(materialManagerMeta->indexOfMethod(QMetaObject::normalizedSignature("execute()")) > -1 )
                {
                    return materialManager;
                }
            }
        }
    }
    return NULL;
}

void MaterialExplorer::setMaterialName(QListWidgetItem* item)
{
    int materialNumber = item->data(Qt::UserRole).toInt();
    Material* materialToEdit = materialManager->getMaterial(materialNumber);
    materialToEdit->setName(item->data(Qt::DisplayRole).toString().toStdString());
}

}
