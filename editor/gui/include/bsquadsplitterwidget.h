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
*  File:       bsquadsplitterwidget.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bsquadsplitterwidget.h
 * \brief Defines BSQuadSplitterWidget class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSQUADSPLITTERWIDGET_H
#define BSQUADSPLITTERWIDGET_H

#include <QtGui>
#include "bsquadsplitter.h"

namespace BSGui
{
    
    /*! \brief A QuadSplitter
     *
     * This class represents a QuadSplitter. If you want to splitt 4 widgets at the same time this is the class you want to use.
     */

class BSQuadSplitterWidget : public QWidget
{
    public:
        /*!
        * \brief Constructs a BSQuadSplitterWidget.
        * 
        * \param parent The widgets parent.
        * \param topLeft The top left Widget.
        * \param bottomLeft The bottom left Widget.
        * \param topRight The top right Widget.
        * \param bottomRight The bottom right Widget.
        */
        BSQuadSplitterWidget(QWidget* topLeft, QWidget* bottomLeft, QWidget* topRight, QWidget* bottomRight, QWidget* parent = NULL);
        /*!
         * \brief Maximizes or restores the widget at the given position.
         * 
         * \param index The index of the widget to maxmize or restore.
         * \param status If this is true the widget is maxmized otherwise it is restored.
        */
        void setMaximized(int index, bool status);
        /*!
         * \brief Maximizes or restores the widget
         * 
         * \param widget The widget to maxmize or restore.
         * \param status If this is true the widget is maxmized otherwise it is restored.
        */
        void setMaximized(QWidget* widget, bool status);
        /*!
         * \brief Stores the state of the widget in the config file.
         */
        void storeState() const;
        /*!
         * \brief Restores the state of the widget from the config file.
         */
        void restoreState();
        //void setWidget(QWidget* widgetToAdd, int i);
        /*!
         * \brief Gets the currently maximized widget.
         * \return The currently maxmized widget. If no widget is maxmized NULL is returned.
         */
        QWidget* currentlyMaximized() const {return maximized;}
        
        /*!
         * \brief Gets the widget in the topLeft.
         * \return The currently maxmized widget. If no widget is maxmized NULL is returned.
         */
        QWidget* getTopLeft() const {return topLeft;}
        /*!
         * \brief Gets the currently maximized widget.
         * \return The currently maxmized widget. If no widget is maxmized NULL is returned.
         */
        QWidget* getTopRight() const {return topRight;}
        /*!
         * \brief Gets the currently maximized widget.
         * \return The currently maxmized widget. If no widget is maxmized NULL is returned.
         */
        QWidget* getBottomLeft() const {return bottomLeft;}
        /*!
         * \brief Gets the currently maximized widget.
         * \return The currently maxmized widget. If no widget is maxmized NULL is returned.
         */
        QWidget* getBottomRight() const {return bottomRight;}
        
        /*!
         * \brief Gets the widget at the given index.
         * \param index The index.
         * \return The widget at the given index.
         */
        QWidget* widget(const int index) const;
        /*!
         * \brief Gets the index of the given widget.
         * \param widget The Widget.
         * \return The index of the given widget.
        */
        int indexOf(const QWidget* widget) const;
        
    protected:
        /*!
        * \brief Handles the resize event.
        * \param event The event.
        */
        void resizeEvent(QResizeEvent* event);
        /*!
         * \brief Handles the change event.
         * \param event The event.
        */
        void changeEvent(QEvent* event);

    private:
        BSQuadSplitterHandle* quadHandle; /*!< \brief The splitter that splits the 2 BSQuadSplitterHelper helper */
        BSQuadSplitterHelper* left; /*!< \brief The left BSQuadSplitterHelper*/
        BSQuadSplitterHelper* right; /*!< \brief The right BSQuadSplitterHelper */
        BSQuadSplitter* quadSplitter; /*!< \brief The mouse offset */
        QWidget* maximized; /*!< \brief The currently maxmized widget */
        /*!
         * \brief Sets up the splitter etc..
        */
        void initUi();
        
        QByteArray leftState; /*!< \brief The state of the left BSQuadSplitterHelper */
        QByteArray rightState; /*!< \brief The state of the right BSQuadSplitterHelper */
        QByteArray quadSplitterState; /*!< \brief The state of the BSQuadSplitter */
        
        bool startMaximized; /*!< \brief This is true if the widget is restored with a maxmized widget */
        QWidget* topLeft; /*!< \brief The top left widget */
        QWidget* topRight; /*!< \brief The top right widget  */
        QWidget* bottomLeft; /*!< \brief The bottom left widget  */
        QWidget* bottomRight; /*!< \brief The bottom right widget  */
};

}

#endif
