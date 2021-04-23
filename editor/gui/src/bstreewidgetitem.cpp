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
        
#include "../include/bstreewidgetitem.h"
        
namespace BSGui
{
        
BSTreeWidgetItem::BSTreeWidgetItem(const QStringList& strings, int type)
    :QTreeWidgetItem(strings, type)
{
}

BSTreeWidgetItem::BSTreeWidgetItem(const BSTreeWidgetItem& other)
    :QTreeWidgetItem(other), m_uid(other.m_uid), m_index(other.m_index)
{
}

BSTreeWidgetItem* BSTreeWidgetItem::clone() const
{
    return new BSTreeWidgetItem(*this);
}

QVariant BSTreeWidgetItem::data(int column, int role) const
{
    if (role == BSTreeWidgetItem::UIDRole)
    {
        return QVariant(qulonglong(m_uid));
    }
    else if(role == BSTreeWidgetItem::IndexRole)
    {
        return QVariant(m_index);
    }
    return QTreeWidgetItem::data(column, role);
    
}

void BSTreeWidgetItem::setData(int column, int role, const QVariant& value)
{
    if (role == BSTreeWidgetItem::UIDRole)
    {
        m_uid = value.toULongLong();
    }
    else if (role == BSTreeWidgetItem::IndexRole)
    {
        m_index = value.toInt();
    }
    QTreeWidgetItem::setData(column, role, value);
}

BSTreeWidgetItem::~BSTreeWidgetItem()
{
}

}
