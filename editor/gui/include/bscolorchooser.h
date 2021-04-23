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
*  File:       bscolorchooser.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 
/** \file bscolorchooser.h
 * \brief Defines BSColorChooser class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSCOLORCHOOSER_H
#define BSCOLORCHOOSER_H

#include <QtGui>

namespace BSGui
{

    /*! \brief A color chooser 
     *
     * A class that lets the user pick a color 
     */
class BSColorChooser : public QLabel
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief Constructs an empty color chooser
        * 
        * \param parent The Widgets parent
        * \param f The Widgets window flags
        */
        BSColorChooser(QWidget* parent = NULL, Qt::WindowFlags f= 0) :
            QLabel(parent, f) {}
        /*!
        * \brief Constructs an color chooser with text
        * \param text The text that is displayed in the widget
        * \param parent The Widgets parent
        * \param f The WIdgets window flags
        */
        BSColorChooser(const QString & text, QWidget * parent = NULL, Qt::WindowFlags f = 0) :
            QLabel(text, parent, f) {}
        
        /*!
        * \brief Returns the current color of this color chooser
        * 
        * \return The current color
        */
        QColor getColor() const {return palette().color(QPalette::Button);}
        
        /*!
         * \brief Sets the current color of this color chooser
         * 
         * \param color The current color
        */
        void setColor(QColor color);
        
    protected:
        /*!
        * \brief Handels an mouse release event. 
        * It displayes a QColorDialog that lets the user choose the new color of the color chooser if the button that coused the event was the left mouse button.
        * 
        * \param event The mouse event.
        */
        void mouseReleaseEvent(QMouseEvent* event);
        
    signals:
        /*!
        * \brief This signal is emitted when the user changed the color. The parameter is the color that the user has chose.
        */
        void colorChanged(QColor);
};

}

#endif
