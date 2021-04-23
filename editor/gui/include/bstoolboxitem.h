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
*  File:       bstoolboxitem.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bstoolboxitem.h
 * \brief Defines BSToolBoxItem class.
 * 
 * \author Thomas Tischler.
*/
#ifndef BSTOOLBOXITEM_H
#define BSTOOLBOXITEM_H

#include <QtGui>
#include "bsgroupbox.h"
namespace BSGui
{

    /*! \brief A abstract base class for items in the tool box
     *
     * If you want to add yout own items to the tool box you have to subclass this class.
     */
class BSToolBoxItem : public QWidget
{
    Q_OBJECT
    
    public:
        /*!
        * \brief Constructs a BSToolBoxItem.
        * 
        * \param parent The widgets parent.
        */
        BSToolBoxItem(QWidget* parent = 0);
        /*!
         * \brief Adds a given Widget to a given group box.
         * \param objectName The object name of the tool box.
         * \param widgetToAdd The widget that should be added.
         */
        virtual bool addWidgetToGroupBox(const QString& objectName, QWidget* widgetToAdd);
        /*!
         * \brief List all group box names.
         * \return A list of gorup box names.
        */
        QStringList getGroupBoxNames() const ;
        /*!
         * \brief This function has to be implemented in a subclass. It inserts a widget to the tool box item after a specific widget.
         * \param widgetToInsert The widget to insert.
         * \param prevWidget The widget after widgetToInsert has to be inserted. If this is NULL it is inserted at the beginning.
         */
        virtual void insertPluginWidget(QWidget* widgetToInsert, QWidget* prevWidget = NULL) = 0;
};

}
#endif
