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
*  File:       bsrenderwidget.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 /** \file bsrenderwidget.h
 * \brief Defines BSRenderWidget class.
 * 
 * \author Thomas Tischler.
  */
#ifndef BSRENDERWIDGET_H
#define BSRENDERWIDGET_H

#include <QtGui>
#include "./../../renderer/include/Renderer.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "bsglwidget.h"
#include "mouseconfig.h"

using namespace BSRenderer;

namespace BSGui
{

/*! \brief A Summary of important renderer settings.
 */
    
struct RendererSettings
{
    bool bRenderGrid; /*!< \brief True if the grid should be shown false otherwise */
    FillMode fillMode; /*!< \brief The fill mode that is currentyl used*/
    ViewMode viewMode; /*!< \brief The view mode that is currentyl used */
};

/*! \brief A Widget to view the scene.
 *
 * This class is able to view the scene.
*/
class BSRenderWidget : public BSGLWidget
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
        BSRenderWidget(QWidget *parent = NULL, bool bOrtho = true, 
                            FillMode fm = FILL_Textured, OrthogonalType orthoType = ORTHO_Front);
        /*!
         * \brief Destroys the Widget
         */ 
        ~BSRenderWidget();
        
        /*!
         * \brief Gives the minimum size hint
        *  \return The minimum size hint
        */ 
        QSize minimumSizeHint() const;
        /*!
         * \brief Gives the size hint
         *  \return The size hint
         */ 
        QSize sizeHint() const;
    
        /*!
         * \brief Tells if the grid is enabled
         * \return Is true if the grid is enabled otherwise false
         */ 
        bool getGrid() const {return actionShowGrid->isChecked();}
        /*!
         * \brief Tells if wireframe is enabled
         * \return Is true if wireframe is enabled otherwise false
         */ 
        bool getWireframe() const {return actionWireframe->isChecked();}
        /*!
         * \brief Tells if the wireframe overlay is enabled
         * \return Is true if the wireframe overlay is enabled otherwise false
         */ 
        bool getWireframeOverlay() const {return actionWireframeOverlay->isChecked();}
        /*!
         * \brief Tells if point mode is enabled
         * \return Is true if point mode is enabled otherwise false
         */ 
        bool getPointMode() const {return actionPointsOnly->isChecked();}
        /*!
         * \brief Tells if textured is enabled
         * \return Is true if textured is enabled otherwise false
         */ 
        bool getTextured() const {return actionTextured->isChecked();}
        /*!
         * \brief Tells if normals are shown
         * \return Is true if normals are shown otherwise false
         */ 
        bool getNormals() const {return actionShowNormals->isChecked();}
        /*!
         * \brief Tells if solid is enabled
         * \return Is true if solid is enabled otherwise false
         */ 
        bool getSolid() const {return actionSolid->isChecked();}
    
        /*!
         * \brief Tells the fill mode
         * \return The fill mode
         */ 
        FillMode getFillMode() const {return settings.fillMode;}
        /*!
         * \brief Tells the view mode
         * \return The view mode
         */ 
        ViewMode getViewMode() const {return settings.viewMode;}
        /*!
         * \brief Tells the orhto type
         * \return The ortho type
         */ 
        OrthogonalType getOrthoType() const {return m_pCamOrthogonal->getOrthogonalType();}
        /*!
         * \brief Gets the prespective camera
         * \return The perspective camera
         */ 
        CameraArcball* getCameraPerspective() const {return m_pCamPerspective;}
        /*!
         * \brief Gets the orthogonal camera
         * \return The orthogonal camera
         */ 
        CameraOrthogonal* getCameraOrthogonal() const  {return m_pCamOrthogonal;}
    
        /*!
         * \brief Resets the viewport
         */ 
        void resetViewport();
        
        /*!
         * \brief Adds an action to the context menu
         * \param pos The position where the action should be inserted
         * \param actionToInsert The action that should be inserted
         */ 
        void insertAction(int pos, QAction* actionToInsert);
        
        /*!
         * \brief Enables overpainting. 
        * If overpainting is enabled rendering will be done in BSRenderWidget::paintEvent()
         */ 
        void enableOverpainting(){overpaintingEnabled = true;}
        /*!
         * \brief disables overpainting. 
         * If overpainting is disabled rendering will be done in BSRenderWidget::paintgGL()
         */ 
        void disableOverpainting(){overpaintingEnabled = false;}
        /*!
         * \brief Tells if overpainting is enabled or diabled
         * \return If this is true overpainting is enabled otherwise disabled
         */ 
        bool isOverpaintingEnabled() const {return overpaintingEnabled;}
        
        /*!
         * \brief Gets the mouse settings property from a QMouseEvent
         * \param event The event.
         * \return This is the mouse settings property that can be used to get the mouse property from the MouseSettings class.
         */ 
        QByteArray getMouseSettingsProperty(QMouseEvent* event) const ;
        
        /*!
         * \brief Gets the mouse action from a QMouseEvent
         * \param event The event.
         * \return The mouse action from the event. It is equal to call: mouseSettings->property(getMouseSettingsProperty(event)).toInt();
         */ 
        int getMouseProperty(QMouseEvent* event) const ; 
        
        /*!
         * \brief Gets the current camera that is used by the widget
         * \return The current camera.
         */
        Camera* getCurrentCamera() const ;

    public slots:
        /*!
        * \brief Enables or disables the grid without modfifieng the corresponding action.
        * \param status If status is true the grid is enabled otherwise disabled.
        */
        void setGrid(bool status);
        /*!
         * \brief Sets the wireframe mode without modfifieng the corresponding action.
         * \param status If status is true the wireframe mode is enabled otherwise this function does nothing.
        */
        void setWireframe(bool status);
        /*!
         * \brief Sets the wireframe overlay mode without modfifieng the corresponding action.
         * \param status If status is true the wireframe overlay mode is enabled otherwise this function does nothing.
        */
        void setWireframeOverlay(bool status);
        /*!
         * \brief Sets the point mode without modfifieng the corresponding action.
         * \param status If status is true the point mode is enabled otherwise this function does nothing.
         */
        void setPointMode(bool status);
        /*!
         * \brief Sets the textured mode without modfifieng the corresponding action.
         * \param status If status is true the textured mode is enabled otherwise this function does nothing.
        */
        void setTextured(bool status);
        /*!
         * \brief Enables or disables the Normals without modfifieng the corresponding action.
         * \param status If status is true the Normals is enabled otherwise disabled.
        */
        void setNormals(bool status);
        /*!
         * \brief Sets the solid mode without modfifieng the corresponding action.
         * \param status If status is true the solid mode is enabled otherwise this function does nothing.
        */
        void setSolid(bool status);

        /*!
         * \brief Sets the wireframe mode
         * \param status If status is true the wireframe mode is enabled otherwise this function does nothing.
        */
        void setGridAction(bool status){actionShowGrid->setChecked(status);}
        /*!
         * \brief Sets the wireframe overlay mode
         * \param status If status is true the wireframe overlay mode is enabled otherwise this function does nothing.
        */
        void setWireframeAction(bool status){actionWireframe->setChecked(status);}
        /*!
         * \brief Sets the point mode
         * \param status If status is true the point mode is enabled otherwise this function does nothing.
        */
        void setWireframeOverlayAction(bool status){actionWireframeOverlay->setChecked(status);}
        /*!
         * \brief Sets the point mode
         * \param status If status is true the point mode is enabled otherwise this function does nothing.
        */
        void setPointModeAction(bool status){actionPointsOnly->setChecked(status);}
        /*!
         * \brief Sets the textured mode
         * \param status If status is true the textured mode is enabled otherwise this function does nothing.
        */
        void setTexturedAction(bool status){actionTextured->setChecked(status);}
        /*!
         * \brief Enables or disables the Normals.
         * \param status If status is true the Normals is enabled otherwise disabled.
        */
        void setNormalsAction(bool status){actionShowNormals->setChecked(status);}
        /*!
         * \brief Sets the solid mode
         * \param status If status is true the solid mode is enabled otherwise this function does nothing.
        */
        void setSolidAction(bool status){actionSolid->setChecked(status);}
        
        /*!
         * \brief Zooms to the given aabb box
        */
        void zoomTo(Aabb aabb);
	
    protected:
        /*!
        * \brief Initialize the renderer
        */
        void initializeGL();
        /*!
         * \brief Calls renderNow() if overpainting is true otherwise calls BSGLWidget::paintGL()
        */
        void paintGL();
        /*!
         * \brief Forces the renderer to render now
        */
        void renderNow();
        /*!
         * \brief Initialize the renderer
         */
        void initRenderer();
        /*!
         * \brief Sets the viewport
         * \param width The width for the viewport.
         * \param height The height for the viewport.
         */
        void resizeGL(int width, int height);
        /*!
         * \brief Handles the mouse press event.
         * \param event The event.
        */
        void mousePressEvent(QMouseEvent *event);
        /*!
         * \brief Handles the mouse move event.
         * \param event The event.
        */
        void mouseMoveEvent(QMouseEvent *event);
        /*!
         * \brief Handles the mouse release event.
         * \param event The event.
        */
        void mouseReleaseEvent(QMouseEvent* event);
        /*!
         * \brief Handles the mouse wheel event.
         * \param event The event.
        */
        void wheelEvent(QWheelEvent *event);
        /*!
         * \brief Handles the context menu event.
         * \param event The event.
        */
        void contextMenuEvent(QContextMenuEvent* event);
        /*!
         * \brief Calculates the a position in the widgets coordinate system from a point in the scenegraphs coordinate system.
         * \param p The Point in the widgets coordinate system.
         * \return The point in the scenegraphs coordinate system.
        */
        QPointF calcRenderPosition(const QPoint &p);
        /*!
         * \brief Calculates the a position in the scenegraphs coordinate system from a point in the widgets coordinate system.
         * \param p The Point in the scenegraphs coordinate system.
         * \return The point in the widgets coordinate system.
        */
        QPoint calcGUIPosition(const QPointF &p);
	

        QAction* actionShowGrid; /*!< \brief The action to show the grid */
        QAction* actionShowNormals; /*!< \brief The action to show the normals */

        QActionGroup* rendererModes; /*!< \brief The action group for the renderer modes */
        QAction* rendererModesSeparator; /*!< \brief The seperator in the context menu to seprate the renderer modes */
        QAction* actionWireframe; /*!< \brief The action for the wireframe mode */
        QAction* actionWireframeOverlay; /*!< \brief The action for the wireframe overlay mode */
        QAction* actionTextured; /*!< \brief The action for the textured mode */
        QAction* actionPointsOnly; /*!< \brief The action for the points only mode */
        QAction* actionSolid; /*!< \brief The action for the solid mode */
        
        RendererSettings settings; /*!< \brief The renderer settings struct */
        
        CameraArcball* m_pCamPerspective; /*!< \brief The perspective camera */
        CameraOrthogonal* m_pCamOrthogonal; /*!< \brief The orthogonal camera */
        
        MouseSettings* mouseSettings; /*!< \brief The MouseSettings object */
        


    private:
        Renderer* r; /*!< \brief The Renderer object */
        
        int m_nLastPosX;
        int m_nLastPosY;
        
        bool overpaintingEnabled; /*!< \brief shows if overpainting is enabled */
        

    signals:
        /*!
        * \brief This signal is emitted if the grid action changed.
        * The paramter is true if the grid is shown otherwise false
        */
        void showGrid(bool);
        

        /*!
         * \brief This signal is emitted if the normals action changed.
         * The paramter is true if the Normals are shown otherwise false
         */
        void showNormals(bool);

        /*!
         * \brief This signal is emitted if the textured action changed.
         * The paramter is true if the view mode is now textured otherwise false.
         */
        
        void showTextured(bool);
        /*!
         * \brief This signal is emitted if the grid action changed.
         * The paramter is true if the view mode is now wireframe otherwise false.
         */
        void showWireframe(bool);
        /*!
         * \brief This signal is emitted if the grid action changed.
         * The paramter is true if the view mode is now wireframe overlay otherwise false.
         */
        void showWireframeOverlay(bool);
        /*!
         * \brief This signal is emitted if the grid action changed.
         * The paramter is true if the view mode is now points only otherwise false.
         */
        void showPointsOnly(bool);
        /*!
         * \brief This signal is emitted if the grid action changed.
         * The paramter is true if the view mode is now solid otherwise false.
         */
        void showSolid(bool);

        /*!
         * \brief This signal is emitted if a mouse button was pressed.
         * The first parameter is the position where the button was pressed.
         * The second parameter is the mousebutton wich was pressed.
         */
        void mousePressed(QPoint, Qt::MouseButtons);
        /*!
         * \brief This signal is emitted if a mouse button was released
         * The first parameter is the position where the button was pressed.
         * The second parameter is the mousebutton wich was pressed.
         */
        void mouseReleased(QPoint, Qt::MouseButtons);
        /*!
         * \brief This signal is emitted if the mouse was moved on the widget.
         * The first parameter is the position where the button was moved.
         * The second parameter is the mousebutton wich was pressed.
         */
        void mouseMoved(QPoint, Qt::MouseButtons);
        /*!
         * \brief This signal is emitted if the mouse was moved on the widget.
         * The first parameter is the position where the button was moved.
         */
        void mouseMovedRenderPosition(QPointF);
        
};

}

#endif
