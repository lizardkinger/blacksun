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
*  File:       glWidget.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/


/** \file glWidget.h
 * \brief Defines GLWidget class.
 * 
 * \author Thomas Tischler.
 */

	#ifndef GLWIDGET_H
	#define GLWIDGET_H

    #include "bsrenderwidget.h"

namespace BSGui
{
/*! \brief A widget to edit the scene.
* This widget allows not only to view the scene like the BSRenderWidget it allows to edit it.
*/
	class GLWidget : public BSRenderWidget
	{
	    Q_OBJECT
	
	public:
        /*!
        * \brief The Constructor of the Widget
        * 
        * \param parent The Widgets parent
        * \param bOrtho Tells if the view is orthogonal or not
        * \param fm The initial fill mode that should be used
        * \param orthoType If bOrtho is true this is the initial ortho type
        */
	    GLWidget(QWidget *parent = NULL, bool bOrtho = true, 
		FillMode fm = FILL_Textured, OrthogonalType orthoType = ORTHO_Front);
        /*!
         * \brief Destroys the Widget
         */ 
	    ~GLWidget(){}

        /*!
         * \brief Says if the widget is maximized.
         * \return True if the widget is maximized otherwise false.
         */ 
        bool getMaxmized() const {return actionMaximizeMe->isChecked();}

        /*!
         * \brief Gets the overpainting color.
         * \return The overpainting color.
         */ 
        QColor getOverpaintingColor() const {return overpaintingColor;}

        /*!
         * \brief Sets the overpainting color.
         * \param color The overpainting color.
         */ 
        void setOverpaintingColor(QColor color)
        {
            overpaintingColor = color;
            update();
        }
        /*!
         * \brief Gets the selection box color color.
         * \return The selection box color.
         */ 
        QColor getSelectionBoxColor() const {return selectionBoxColor;}
        /*!
         * \brief Sets the selection box color color.
         * \param color The selection box color.
         */ 
        void setSelectionBoxColor(QColor color)
        {
            selectionBoxColor = color;
            update();
        }

        /*!
         * \brief Gets the nearest point on the grid relative the given point.
         * \param point A point
         * \return The point on the grid.
         */ 
        QPoint getNearestGridPosition(QPoint point) const ;
        /*!
         * \brief Gets the nearest point on the grid relative the mouse only if snap to gris is enabled.
         * \param point Apoint
         * \return The point on the grid if snap to gris is enabled otherwise the input point.
         */ 
        QPoint snapToGrid(QPoint point);

    public slots:
        /*!
         * \brief Sets the widget to be maxmized without setting the corresponding action.
         * \param status The maxmized status
         */ 
        void setMaximized(bool status);
        /*!
         * \brief Sets the widget to be maxmized.
         * \param status The maxmized status
         */ 
        void setMaximizedAction(bool status){actionMaximizeMe->setChecked(status);}
	
	protected:
        /*!
         * \brief Hendels the mouse press event.
         * \param event The event.
         */ 
        void mousePressEvent(QMouseEvent* event);
        /*!
         * \brief Hendels the mouse move event.
         * \param event The event.
         */ 
        void mouseMoveEvent(QMouseEvent* event);
        /*!
         * \brief Hendels the mouse release event.
         * \param event The event.
         */ 
        void mouseReleaseEvent(QMouseEvent* event);

        /*!
         * \brief Hendels the enter event.
         * \param event The event.
         */ 
        void enterEvent(QEvent* event);
        /*!
         * \brief Hendels the leave event.
         * \param event The event.
         */ 
        void leaveEvent(QEvent* event);

        /*!
         * \brief Hendels the paint event.
         * \param event The event.
         */ 
        void paintEvent(QPaintEvent* event);

        /*!
         * \brief initializes the renderer.
         */ 
        void initializeGL();


	private:
        QAction* actionMaximizeMe; /*!< \brief The maximize action */

        QPoint rubberBandOrigin; /*!< \brief The rubber band origin */
        QPoint rubberBandDestination; /*!< \brief The rubber band destination */
        bool showRubberBand; /*!< \brief If this is true the rubberband is shown */

        QPoint moveLineOrigin; /*!< \brief The move line origin */
        QPoint moveLineDestination; /*!< \brief The move line destination */
        bool showMoveLine; /*!< \brief If this is true the move line is shown  */
        bool showScaleLine; /*!< \brief If this is true the scale line is shown  */
        bool showRotateArc; /*!< \brief If this is true the rotate arc is shown  */
        
        double rotateAngle; /*!< \brief The rorate angel */

        QColor overpaintingColor; /*!< \brief The overpainting color */
        QColor selectionBoxColor; /*!< \brief The selection box color */

        int lastMouseMoveProperty; /*!< \brief The last mouse move property */

        BSScene::SceneAction2D buildSceneAction2D(Qt::MouseButton button,
        BSScene::SceneAction2D_SelectionType selectionType, QPoint eventPos); /*!< \brief The scene action */

    signals:
        /*!
         * \brief This signal is emitted when the widgets wants to be maxmized.
         * The first parameter is a pointer to this object.
        *  The second parameter gives whether the widgets wants to be maxmized or restored. If it is true the widgets wants to be maximized.
        */
        void maximizeMe(GLWidget*, bool);
	};

}


	#endif
