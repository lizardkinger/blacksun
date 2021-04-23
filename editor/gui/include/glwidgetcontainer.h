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
*  File:       glwidgetcontainer.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 /** \file glwidgetcontainer.h
 * \brief Defines GLWidgetContainer class.
 * 
 * \author Thomas Tischler.
  */

#ifndef GLWIDGETCONTAINER_H
#define GLWIDGETCONTAINER_H

#include <QtGui>
#include "glWidget.h"
#include "bsquadsplitterwidget.h"
        
namespace BSGui
{
        
    
/*! \brief This Widget represents a container for 4 glwidgets.
     * This widget manages 4 glwidgets. It emits the apropriate signals if an option of the managed GLWidgets chages and allows to change options on alle managed widgets simultaniously.
*/

class GLWidgetContainer : public QWidget
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief Constructs a GLWidgetContainer.
        * \param parent The parent of this widget.
        */
        GLWidgetContainer(QWidget* parent = NULL);
        /*!
         * \brief Stores the state of this widget in the config file.
         */
        void storeState() const ;
        /*!
         * \brief Restores the state of this widget from the config file.
        */
        void restoreState();
        
        /*!
         * \brief Gets the widget at the spicified position.
         * \param pos The position.
         * \return The widget.
         */
        GLWidget* getWidget(int pos) const {return widgetList.at(pos);}
        
        /*!
         * \brief Gets the number of widget.
         * \return The number of widgets.
         */
        int getNumberOfWidgets() const {return widgetList.count();}
        
        /*!
         * \brief Says if all widgets have the grid enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getGrid() const ;
        /*!
         * \brief Says if all widgets have wireframe mode enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getWireframe() const ;
        /*!
         * \brief Says if all widgets have wireframe overlay mode enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getWireframeOverlay() const ;
        /*!
         * \brief Says if all widgets have point mode enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getPointMode() const ;
        /*!
         * \brief Says if all widgets have textured mode enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getTextured() const ;
        /*!
         * \brief Says if all widgets have the normals enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getNormals() const ;
        /*!
         * \brief Says if all widgets have slod mode enabled.
         * \return This is true if it is enabled on all widgets otherwise false.
        */
        bool getSolid() const ;
        
        /*!
         * \brief Gets the selection box color.
         * \return The selection box color.
         */
        QColor getSelectionBoxColor() const ;
        /*!
         * \brief Sets the selection box color.
         * \param color The color.
         */
        void setSelectionBoxColor(QColor color);
        
        /*!
         * \brief Gets the overpainting color.
         * \return The selection box color.
         */
        QColor getOverpaintingColor();
        /*!
         * \brief Sets the overpainting box color.
         * \param color The color.
         */
        void setOverpaintingColor(QColor color);
        
        /*!
         * \brief Gets the currently maxmized widget.
         * \return The maxmized widget if there is no such widgets NULL is returned
         */
        GLWidget* getCurrentlyMaxmized() const {return currentlyMaximized;}
        
        /*!
         * \brief Destroys the widget.
         */
        virtual ~GLWidgetContainer();
                
    public slots:
        /*!
        * \brief Maxmizes a widget.
        * \param widget The widget that should be maxmized.
        * \param status If status is true the widget will be maxmized otherwise restored.
        */
        void setMaximized(GLWidget* widget, bool status);
        
        /*!
         * \brief Adds a glwidget to the container.
         */
        void addGLWidget(GLWidget* widgetToAdd);
        
        /*!
         * \brief Sets the grid on all glwidgets.
        * \param status If status is true it will be enabled otherwise disabled.
        */
        void setGrid(bool status);
        /*!
         * \brief Sets the wireframe on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setWireframe(bool status);
        /*!
         * \brief Sets the wireframe overlay on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setWireframeOverlay(bool status);
        /*!
         * \brief Sets the point mode on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setPointMode(bool status);
        /*!
         * \brief Sets the textured mode on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setTextured(bool status);
        /*!
         * \brief Sets the normals on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setNormals(bool status);
        /*!
         * \brief Sets the solid mode on all glwidgets.
         * \param status If status is true it will be enabled otherwise disabled.
        */
        void setSolid(bool status);
    
    private:
        
        BSQuadSplitterWidget* quadSplitter; /*!< \brief The quad splitter */
        
        QList<GLWidget*> widgetList; /*!< \brief The widget list */
        QList<QSplitter*> splitterList; /*!< \brief The splitter list */
        
        QList<QByteArray> splitterState; /*!< \brief The splitter state */
        
        GLWidget* currentlyMaximized; /*!< \brief The currently maxmized widget */
        
    private slots:
        /*!
        * \brief Sends the showGrid signal if all widgets have the same status for the grid set.
        * \param status The status.
        */
        void emitGrid(bool status);
        /*!
         * \brief Sends the showWireframe signal if all widgets have the same status for the wireframe set.
         * \param status The status.
        */
        void emitWireframe(bool status);
        /*!
         * \brief Sends the showWireframeOverlay signal if all widgets have the same status for the showWireframeOverlay set.
         * \param status The status.
        */
        void emitWireframeOverlay(bool status);
        /*!
         * \brief Sends the showPointsOnly signal if all widgets have the same status for the point mode set.
         * \param status The status.
        */
        void emitPointMode(bool status);
        /*!
         * \brief Sends the showTextured signal if all widgets have the same status for the textured mode set.
         * \param status The status.
        */
        void emitTextured(bool status);
        /*!
         * \brief Sends the showNormals signal if all widgets have the same status for the normals set.
         * \param status The status.
        */
        void emitNormals(bool status);
        /*!
         * \brief Sends the showSolid signal if all widgets have the same status for the solid mode set.
         * \param status The status.
        */
        void emitSolid(bool status);
        /*!
         * \brief Resets all views.
        */
        void resetViews();
        
    signals:
        /*!
        * \brief This signal is emitted when all glwidgets have the same grid status.
        * The param gives the status.
        */
        void showGrid(bool);
        /*!
         * \brief This signal is emitted when all glwidgets have the same normals status.
         * The param gives the status.
        */
        void showNormals(bool);
        
        /*!
         * \brief This signal is emitted when all glwidgets have the same textured mode status.
         * The param gives the status.
        */
        void showTextured(bool);
        /*!
         * \brief This signal is emitted when all glwidgets have the same wireframe mode status.
         * The param gives the status.
        */
        void showWireframe(bool);
        /*!
         * \brief This signal is emitted when all glwidgets have the same wireframe overlay status.
         * The param gives the status.
        */
        void showWireframeOverlay(bool);
        /*!
         * \brief This signal is emitted when all glwidgets have the same points mode status.
         * The param gives the status.
        */
        void showPointsOnly(bool);
        /*!
         * \brief This signal is emitted when all glwidgets have the same solid status.
         * The param gives the status.
        */
        void showSolid(bool);
};

}

#endif
