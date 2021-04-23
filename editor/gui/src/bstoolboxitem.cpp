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
        
        
#include "./../include/bstoolboxitem.h"

namespace BSGui
{
        
BSToolBoxItem::BSToolBoxItem(QWidget* parent)
    :QWidget(parent)
{
}

bool BSToolBoxItem::addWidgetToGroupBox(const QString& objectName, QWidget* widgetToAdd)
{
    BSGroupBox* groupBox = findChild<BSGroupBox* >(objectName);
    if (groupBox != NULL)
    {
        QLayout* layout = groupBox->layout();
        if (layout != NULL)
        {
            layout->addWidget(widgetToAdd);
            return true;
        }
    }
    return false;
}

QStringList BSToolBoxItem::getGroupBoxNames() const
{
    QList<BSGroupBox* > groupBoxList = findChildren<BSGroupBox* >();
    
    QStringList nameList;
    
    for(int i = 0 ; i < groupBoxList.count() ; i++)
    {
        nameList << groupBoxList.at(i)->objectName();
    }
    return nameList;
}

}
