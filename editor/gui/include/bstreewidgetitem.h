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
*  File:       bstreewidgetitem.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bstreewidgetitem.h
 * \brief Defines BSTreeWidgetItem class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSTREEWIDGETITEM_H
#define BSTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "../../pluginsystem/include/PlgMgr.h"
        
namespace BSGui
{
    
    /*! \brief This class is used in the pluginmanager.
     * If can hold an UID and an index see UIDRole and IndexRole
     */
class BSTreeWidgetItem : public QTreeWidgetItem
{
public:
    /*!
    * \brief Constructs a BSTreeWidgetItem.
    * 
    * \param strings The text for the columns.
    * \param type The type
    */
    BSTreeWidgetItem(const QStringList& strings, int type = UserType);
    /*!
     * \brief Copys a BSTreeWidgetItem.
     * 
     * \param other The item to copy.
     */
    BSTreeWidgetItem(const BSTreeWidgetItem& other);
    /*!
     * \brief Clones the Item.
     * 
     * \Return The clones item.
    */
    BSTreeWidgetItem* clone() const;
    /*!
     * \brief Frees all resources
    */
    virtual ~BSTreeWidgetItem();
    /*!
     * \brief Gets the requested data
     * \param column The column
     * \param role The Qt::ItemDataRole
     * \return The requested data
     */
    virtual QVariant data(int column, int role) const;
    /*!
     * \brief Sets the requested data
     * \param column The column
     * \param role The Qt::ItemDataRole
     * \param value The requested data
     */
    virtual void setData(int column, int role, const QVariant& value);
    /*!
     * \brief The item type.
     */
    enum    {
            UIDRole=33, /*!< The role for the UID*/
            IndexRole=34 /*!< The role for the index*/
            };
    
private:
    /*!< \brief The UID of the associated plugin */
    UID m_uid;
    /*!< \brief The index of the associated aboutStackWidget */
    int m_index;
};

}

#endif
