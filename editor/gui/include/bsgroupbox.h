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
*  File:       bsgroupbox.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bsgroupbox.h
 * \brief Defines BSGroupBox class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSGROUPBOX_H
#define BSGROUPBOX_H

#include <QWidget>
#include <QGroupBox>

namespace BSGui
{
    
    /*! \brief A GroupBox 
     *
     * A QGroupBox that can hide its contents. 
    */

class BSGroupBox : public QGroupBox
{
    Q_OBJECT

    public:
        /*!
        * \brief Constructs a BSGLWidget.
        * 
        * \param text The text of the group box.
        * \param parent The widgets parent.
        */
        BSGroupBox(const QString& text, QWidget *parent = 0);
        
    private slots:
        /*!
        * \brief Hides or shows the content.
        * 
        * \param show If this is true the widget shows its contents and hides it if it is false.
        */
        void toggleChildWidgets(bool show);
        
    protected:
        /*!
        * \brief Handels a change event.
        * 
        * \param event The change event.
        */
        void changeEvent(QEvent* event);
        
    private:
        int prevMaximumHeight; /*!< \brief The maximum height that the widget had before it was minimized */
        int minimumHeight; /*!< \brief The height that the minimized widget has */
        QList<QWidget*> hiddenList; /*!< \brief The widget that a currently hidden */
};

}
#endif
