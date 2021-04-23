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
*  File:       bsflowlayout.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bsflowlayout.h
 * \brief Defines BSFlowLayout class.
 * 
 * \author Thomas Tischler.
 */
#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QtGui>

namespace BSGui
{
    /*! \brief A flow layout 
     *
     * A class that allows adding and removing from items without "holes" in the layout. 
     */
class BSFlowLayout : public QGridLayout
{
    public:
        /*!
        * \brief The Constructor of the Layout
        * 
        * \param parent The Layouts parent
        */
        BSFlowLayout(QWidget* parent = NULL);
        
        /*!
         * \brief Adds an Item to the layout
         * 
         * \param itemToAdd Item that should be added
        */
        void addItem(QLayoutItem* itemToAdd);
        
        /*!
         * \brief Removes the item from the layout and returns it. If there is no such item NULL is returned.
         * 
         * \param index The index of the Item that should be removes
         *
         * \return the item at the given index or NULL if there is no such item
         */
        QLayoutItem* takeAt(int index);
        
        /*!
         * \brief Gets the preferred Column Count. 
         * The preferred Column count is the number of of columns the layout will have. If you add an item and all places in the columns are full the item is inserted in a new row
         * 
         * \return The preferred column count
         */
        int getPreferredColumnCount() const {return preferredColumnCount;}
        
        /*!
         * \brief Sets the preferred Column Count. 
         * The preferred Column count is the number of of columns the layout will have. If you add an item and all places in the columns are full the item is inserted in a new row
         * 
         * \param count The preferred column count
        */
        void setPreferredColumnCount(int count){preferredColumnCount = count;}
        
    private:
        
        int preferredColumnCount; /*!< \brief The preferred column count */
};

}

#endif
