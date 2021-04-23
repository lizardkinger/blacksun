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
#include "../include/mouseconfig.h"

namespace BSGui
{

    MouseConfig::MouseConfig()
    : ConfigInterface()
    {
        ui.setupUi(this);
        
        settings = MouseSettings::getInstance();
        
        QList<QComboBox*> childBoxses = findChildren<QComboBox*>();
        foreach(QComboBox* current, childBoxses)
        {
            fillBox(current);
            QByteArray propertyName = objectNameToPropertyName(current);
            current->setCurrentIndex(current->findData(settings->property(propertyName).toInt()));
            connect(current, SIGNAL(currentIndexChanged(int)), this, SLOT(checkForChanges()));
        }
    }

//Get the discription of the Config Dialog
    QString MouseConfig::getDescription() const
    {
        return QString("Change mouse options");
    }
//Get the Name of the Config Dialog
    QString MouseConfig::getShortName() const
    {
        return QString("Mouse");
    }
//Get the Icon of the Config Dialog
    QIcon MouseConfig::getIcon() const
    {
        return QIcon(":configdialog/media/exec.png");
    }


    void MouseConfig::dialogAccept()
    {
        QList<QComboBox*> childBoxses = findChildren<QComboBox*>();
        foreach(QComboBox* current, childBoxses)
        {
            QByteArray propertyName = objectNameToPropertyName(current);
            settings->setProperty(propertyName, current->itemData(current->currentIndex()));
        }
        
        emit statusChanged(false);
    }

    void MouseConfig::dialogReject()
    {
    }
    
    void MouseConfig::reset()
    {
        QList<QComboBox*> childBoxses = findChildren<QComboBox*>();
        foreach(QComboBox* current, childBoxses)
        {
            QByteArray propertyName = objectNameToPropertyName(current);
            current->setCurrentIndex(current->findData(settings->property(propertyName).toInt()));
        }
    }
    
    void MouseConfig::fillBox(QComboBox* boxToFill)
    {
        if(!boxToFill->objectName().endsWith("3DBox"))
        {
            boxToFill->addItem("Selected Action", MouseSettings::SelectedAction);
        }
        boxToFill->addItem("Move", MouseSettings::Move);
        if(boxToFill->objectName().endsWith("3DBox"))
        {
            boxToFill->addItem("Rotate", MouseSettings::Rotate);
        }
        if(!boxToFill->objectName().endsWith("3DBox"))
        {
            boxToFill->addItem("Add to selection", MouseSettings::AddToSelection);
        }
        if(!boxToFill->objectName().endsWith("3DBox"))
        {
            boxToFill->addItem("Remove from selection", MouseSettings::RemoveFromSelection);
        }
        boxToFill->addItem("Nothing", MouseSettings::Nothing);
    }
    
    
    void MouseConfig::checkForChanges()
    {
        QList<QComboBox*> childBoxses = findChildren<QComboBox*>();
        QComboBox* sendingBox = qobject_cast<QComboBox*>(sender());
        if(sendingBox != NULL)
        {
            QByteArray propertyName = objectNameToPropertyName(sendingBox);
            if(settings->property(propertyName).toInt() == sendingBox->itemData(sendingBox->currentIndex()).toInt())
            {
                emit statusChanged(false);
            }
            else
            {
                emit statusChanged(true);
            }
        }
    }
    
    QByteArray MouseConfig::objectNameToPropertyName(QObject* object)
    {
        return object->objectName().remove(object->objectName().length()-3, 3).toAscii();
    }

    MouseConfig::~MouseConfig()
    {
    }

}
