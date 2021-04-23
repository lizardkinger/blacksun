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
 
#include "./../include/mousesettings.h"

namespace BSGui
{

MouseSettings* MouseSettings::m_instance = NULL;


MouseSettings* MouseSettings::getInstance()
{
    static MouseSettingsGuardian g;

    if(m_instance == NULL)
    {
        m_instance = new MouseSettings();
    }
	
    return m_instance;
}

MouseSettings::MouseSettings() 
    :QObject(NULL)
{
    
    defaultActions.insert("leftNoModifier2D", SelectedAction);
    defaultActions.insert("leftCtrlModifier2D", Move);
    defaultActions.insert("leftShiftModifier2D", AddToSelection);
    defaultActions.insert("leftCtrlShiftModifier2D", RemoveFromSelection);
    
    defaultActions.insert("middleNoModifier2D", Move);
    defaultActions.insert("middleShiftModifier2D", Nothing);
    defaultActions.insert("middleCtrlModifier2D", Nothing);
    defaultActions.insert("middleCtrlShiftModifier2D", Nothing);
    
    defaultActions.insert("leftNoModifier3D", Rotate);
    defaultActions.insert("leftCtrlModifier3D", Move);
    defaultActions.insert("leftShiftModifier3D", Nothing);
    defaultActions.insert("leftCtrlShiftModifier3D", Nothing);
        
    defaultActions.insert("middleNoModifier3D", Nothing);
    defaultActions.insert("middleShiftModifier3D", Nothing);
    defaultActions.insert("middleCtrlModifier3D", Nothing);
    defaultActions.insert("middleCtrlShiftModifier3D", Nothing);
    
    QSettings settings;
    settings.beginGroup("mousesettings");
    
    int begin = metaObject()->propertyOffset();
    for(int i = begin; i < metaObject()->propertyCount() ; i++)
    {
        QMetaProperty currentProperty = metaObject()->property(i);
        currentProperty.write(this, settings.value(currentProperty.name(),
                              defaultActions[currentProperty.name()]).toInt());
    }

    settings.endGroup();
    
}

MouseSettings::~MouseSettings()
{
    QSettings settings("FH-Landshut", "Blacksun");
    settings.beginGroup("mousesettings");
    
    int begin = metaObject()->propertyOffset();
    for(int i = begin; i < metaObject()->propertyCount() ; i++)
    {
        QMetaProperty currentProperty = metaObject()->property(i);
        settings.setValue(currentProperty.name(), currentProperty.read(this).toInt());
    }

    settings.endGroup();
}

}
