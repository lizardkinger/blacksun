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
 
#include "./../include/bsrenderwidget.h"

using namespace BSScene;

namespace BSGui
{
    
    BSRenderWidget::BSRenderWidget(QWidget *parent, bool bOrtho, FillMode fm, OrthogonalType orthoType)
    : BSGLWidget(parent), overpaintingEnabled(false)
    {
        
        settings.bRenderGrid = true;
        
        mouseSettings = MouseSettings::getInstance();
        
        SceneManager* sm =SceneManager::getInstance();     
        connect(sm,SIGNAL(sceneChanged()),this, SLOT(update()));
        
        m_pCamPerspective = new CameraArcball(width(), height());
        m_pCamOrthogonal = new CameraOrthogonal(width(), height(), orthoType);
        
        if(bOrtho)
            settings.viewMode = VIEWMODE_Orthogonal;
        else
            settings.viewMode = VIEWMODE_Perspective;
            
        settings.fillMode = fm;

        actionShowGrid = new QAction("Grid", this);
        actionShowGrid->setCheckable(true);
        actionShowGrid->setChecked(settings.bRenderGrid);

        actionShowNormals = new QAction("Normals", this);
        actionShowNormals->setCheckable(true);

        actionWireframe = new QAction("Wireframe", this);
        actionWireframe->setCheckable(true);
        if (settings.fillMode == FILL_Wireframe)
        {
            actionWireframe->setChecked(true);
        }
        else
        {
            actionWireframe->setChecked(false);
        }

        actionWireframeOverlay = new QAction("Wireframe", this);
        actionWireframeOverlay->setCheckable(true);
        if (settings.fillMode == FILL_Wireframe)
        {
            actionWireframe->setChecked(true);
        }
        else
        {
            actionWireframe->setChecked(false);
        }

        actionWireframeOverlay = new QAction("Wireframe Overlay", this);
        actionWireframeOverlay->setCheckable(true);
        if (settings.fillMode == FILL_WireframeOverlay)
        {
            actionWireframeOverlay->setChecked(true);
        }
        else
        {
            actionWireframeOverlay->setChecked(false);
        }

        actionTextured = new QAction("Textured", this);
        actionTextured->setCheckable(true);
        if (settings.fillMode == FILL_Textured)
        {
            actionTextured->setChecked(true);
        }
        else
        {
            actionTextured->setChecked(false);
        }

        actionPointsOnly = new QAction("Points only", this);
        actionPointsOnly->setCheckable(true);
        if (settings.fillMode == FILL_Point)
        {
            actionPointsOnly->setChecked(true);
        }
        else
        {
            actionPointsOnly->setChecked(false);
        }
        
        actionSolid = new QAction("Solid", this);
        actionSolid->setCheckable(true);
        if(settings.fillMode == FILL_Solid)
        {
            actionSolid->setChecked(true);
        }
        else
        {
            actionSolid->setChecked(false);
        }

        rendererModesSeparator = new QAction("Render Modes", this);
        rendererModesSeparator->setSeparator(true);

        rendererModes = new QActionGroup(this);
        rendererModes->addAction(actionWireframeOverlay);
        rendererModes->addAction(actionWireframe);
        rendererModes->addAction(actionTextured);
        rendererModes->addAction(actionPointsOnly);
        rendererModes->addAction(actionSolid);
        rendererModes->setExclusive(true);

        addAction(actionShowGrid);
        addAction(actionShowNormals);
        addAction(rendererModesSeparator);
        addAction(actionTextured);
        addAction(actionWireframe);
        addAction(actionWireframeOverlay);
        addAction(actionPointsOnly);
        addAction(actionSolid);


        int const minimumWidth = 1;
        int const minimumHight = 1;

        setMinimumHeight(minimumHight);
        setMinimumWidth(minimumWidth);

        connect(actionShowGrid, SIGNAL(toggled(bool)), this, SLOT(setGrid(bool)));
        connect(actionWireframe, SIGNAL(toggled(bool)), this, SLOT(setWireframe(bool)));
        connect(actionWireframeOverlay, SIGNAL(toggled(bool)), this, SLOT(setWireframeOverlay(bool)));
        connect(actionTextured, SIGNAL(toggled(bool)), this, SLOT(setTextured(bool)));
        connect(actionPointsOnly, SIGNAL(toggled(bool)), this, SLOT(setPointMode(bool)));
        connect(actionShowNormals, SIGNAL(toggled(bool)), this, SLOT(setNormals(bool)));
        connect(actionSolid, SIGNAL(toggled(bool)), this, SLOT(setSolid(bool)));

        connect(actionShowGrid, SIGNAL(toggled(bool)), this, SIGNAL(showGrid(bool)));
        connect(actionWireframe, SIGNAL(toggled(bool)), this, SIGNAL(showWireframe(bool)));
        connect(actionWireframeOverlay, SIGNAL(toggled(bool)), this, SIGNAL(showWireframeOverlay(bool)));
        connect(actionTextured, SIGNAL(toggled(bool)), this, SIGNAL(showTextured(bool)));
        connect(actionPointsOnly, SIGNAL(toggled(bool)), this, SIGNAL(showPointsOnly(bool)));
        connect(actionShowNormals, SIGNAL(toggled(bool)), this, SIGNAL(showNormals(bool)));
        connect(actionSolid, SIGNAL(toggled(bool)), this, SIGNAL(showSolid(bool)));
        
        BSScene::MenuHandler *mh = sm->getMenuHandler();
        connect(mh, SIGNAL(zoomToSelection(Aabb)), this, SLOT(zoomTo(Aabb)));

        update(); //We call update here to create the renderer Instance
    }
    
    void BSRenderWidget::insertAction(int pos, QAction* actionToInsert)
    {
        QList<QAction*> currentActions = actions();
        if(!currentActions.isEmpty() && currentActions.count() > pos)
        {
            QWidget::insertAction(currentActions[pos], actionToInsert);
        }
        else
        {
            addAction(actionToInsert);
        }
    }

    BSRenderWidget::~BSRenderWidget()
    {
        delete m_pCamPerspective;
        delete m_pCamOrthogonal;
    }

    QSize BSRenderWidget::minimumSizeHint() const
    {
        return QSize(50, 50);
    }

    QSize BSRenderWidget::sizeHint() const
    {
        return QSize(400, 400);
    }

    void BSRenderWidget::initializeGL()
    {
        initRenderer();
    }
    
    void BSRenderWidget::initRenderer()
    {
        r = Renderer::getInstance();
        r->init();
        //Set the Checkbox for the Normals
        actionShowNormals->setChecked(r->getSettings()->getShowNormals());
    }

    void BSRenderWidget::paintGL()
    {	
        if(!overpaintingEnabled)
        {
            renderNow();
        }
        else
        {
            BSGLWidget::paintGL();
        }
    }

    void BSRenderWidget::resizeGL(int width, int height)
    {
        //Set the viewports of the renderer and the cameras
        r->setViewport(width, height);
        m_pCamPerspective->setViewport(width, height);
        m_pCamOrthogonal->setViewport(width, height);
    }
    
    void BSRenderWidget::resetViewport()
    {
        //Set the viewports of the renderer and the cameras
        r->setViewport(QWidget::width(), QWidget::height());
        m_pCamPerspective->setViewport(QWidget::width(), QWidget::height());
        m_pCamOrthogonal->setViewport(QWidget::width(), QWidget::height());
    }
    
    QPointF BSRenderWidget::calcRenderPosition(const QPoint &p)
    { 	
    	double d1 = (double)p.x() / (double)width();
    	double d2 = (double)p.y() / (double)height();
    	
    	//qDebug() << "d1: " << d1 << " / d2: " << d2;
    	
    	double dLeft, dTop, dRight, dBottom;
    	m_pCamOrthogonal->getLimits(dLeft,dTop,dRight,dBottom);
    	
    	double view1 = dLeft + ((std::abs(dRight) + std::abs(dLeft)) * d1);
    	double view2 = dTop - ((std::abs(dTop) + std::abs(dBottom)) * d2);
    	
    	//qDebug() << "[Dimension] 1: " << dLeft << " till " << dRight << " / 2: " << dBottom << " till " << dTop;
    	//qDebug() << "[Scenepos] 1: " << view1 << " / 2: " << view2 << endl << endl;
    	
    	return QPointF(view1,view2);
    }
    
    QPoint BSRenderWidget::calcGUIPosition(const QPointF &view)
    {
    	//qDebug() << "Input: [Scenepos] x: " << view.x() << " / y: " << view.y();

    	double dLeft, dTop, dRight, dBottom;
    	m_pCamOrthogonal->getLimits(dLeft,dTop,dRight,dBottom);
    	
    	//qDebug() << "[Dimension] x: " << dLeft << " till " << dRight << " / y: " << dBottom << " till " << dTop;
    	
    	
		double dX = (view.x() - dLeft) /  (std::abs(dRight) + std::abs(dLeft)) ;
		double dY = -(view.y() - dTop) /  (std::abs(dTop) + std::abs(dBottom)) ;
		
		int pX = static_cast<int>(dX * (double)width());
		int pY = static_cast<int>(dY * (double)height());
		QPoint p(pX,pY);

    	//qDebug() << "Output: [GUI-Pos] x: " << "pX: " << p.x() << " / pY: " << p.y() << endl << endl;
    	return p;
    }
    
    QByteArray BSRenderWidget::getMouseSettingsProperty(QMouseEvent* event) const
    {
        QByteArray settingToGet;
        if(event->buttons() == Qt::LeftButton)
        {
            settingToGet += "left";
        }
        else if(event->buttons() == Qt::LeftButton)
        {
            settingToGet += "right";
        }
        else if(event->buttons() == Qt::MidButton)
        {
            settingToGet += "middle";
        }
        
        if(event->modifiers() == Qt::NoModifier)
        {
            settingToGet += "NoModifier";
        }
        else if(event->modifiers() == Qt::ControlModifier)
        {
            settingToGet += "CtrlModifier";
        }
        else if(event->modifiers() == Qt::ShiftModifier)
        {
            settingToGet += "ShiftModifier";
        }
        else if(event->modifiers().testFlag(Qt::ControlModifier) &&
                event->modifiers().testFlag(Qt::ShiftModifier))
        {
            settingToGet += "CtrlShiftModifier";
        }
        
        if(settings.viewMode == VIEWMODE_Perspective)
        {
            settingToGet += "3D";
        }
        else if(settings.viewMode == VIEWMODE_Orthogonal)
        {
            settingToGet += "2D";
        }
        return settingToGet;
    }
    
    int  BSRenderWidget::getMouseProperty(QMouseEvent* event) const
    {
        return mouseSettings->property(getMouseSettingsProperty(event)).toInt();
    }
    
    void BSRenderWidget::zoomTo(Aabb aabb)
 	{		
 		this->resetViewport();
 		
 		double dZoomMultiplier = 1.5; 
 		
 		aabb.setMaximum(aabb.getMaximum()*dZoomMultiplier);
		aabb.setMinimum(aabb.getMinimum()*dZoomMultiplier);
 
		if(settings.viewMode == VIEWMODE_Perspective)
		{
 			
		}
		else
        {        	
        	double sizeWidth = 0.0;
        	double sizeHeight = 0.0;
        	double newLeft = 0.0;
        	double newTop = 0.0;
        	double newRight = 0.0;
        	double newBottom = 0.0;        	
        	
        	switch(getOrthoType())
            {
                case ORTHO_Front:
                case ORTHO_Back:
                {
                	//Calc dimension of AABB
                    sizeWidth = aabb.getMaximum().x - aabb.getMinimum().x;
        			sizeHeight = aabb.getMaximum().y - aabb.getMinimum().y;

					//Width is bigger than the height => Height must be adjusted to
					//get a quadratic camera limit area
		        	if(sizeWidth > sizeHeight)
		        	{
		        		newLeft	 =	aabb.getMinimum().x;
		        		newRight = 	aabb.getMaximum().x;
		        		
		        		newTop = aabb.getMaximum().y + (sizeWidth - sizeHeight) * 0.5;
		        		newBottom = aabb.getMinimum().y - (sizeWidth - sizeHeight) * 0.5; 
		        	}
		        	// => Width must be adjusted
		        	else
		        	{
		        		newTop = aabb.getMaximum().y;
		        		newBottom = aabb.getMinimum().y;
		        		
		        		newLeft = aabb.getMinimum().x - (sizeHeight - sizeWidth) * 0.5;
		        		newRight = aabb.getMaximum().x - (sizeHeight - sizeWidth) * 0.5;
		        	}

                }break;
        			
                case ORTHO_Left:
                case ORTHO_Right:
                {
                    sizeWidth = aabb.getMaximum().z - aabb.getMinimum().z;
        			sizeHeight = aabb.getMaximum().y - aabb.getMinimum().y;
        			
					//Width is bigger than the height => Height must be adjusted to
					//get a quadratic camera limit area        			
		        	if(sizeWidth > sizeHeight)
		        	{
		        		newLeft	 =	aabb.getMinimum().z;
		        		newRight = 	aabb.getMaximum().z;
		        		
		        		newTop = aabb.getMaximum().y + (sizeWidth - sizeHeight) * 0.5;
		        		newBottom = aabb.getMinimum().y - (sizeWidth - sizeHeight) * 0.5; 
		        	}
		        	// => Width must be adjusted
		        	else
		        	{
		        		newTop = aabb.getMaximum().y;
		        		newBottom = aabb.getMinimum().y;
		        		
		        		newLeft = aabb.getMinimum().z - (sizeHeight - sizeWidth) * 0.5;
		        		newRight = aabb.getMaximum().z - (sizeHeight - sizeWidth) * 0.5;
		        	} 
        			
                }break;		
        			
                case ORTHO_Top:
                case ORTHO_Bottom:
                {
                    sizeWidth = aabb.getMaximum().x - aabb.getMinimum().x;
        			sizeHeight = aabb.getMaximum().z - aabb.getMinimum().z;
        			
					//Width is bigger than the height => Height must be adjusted to
					//get a quadratic camera limit area        			
		        	if(sizeWidth > sizeHeight)
		        	{
		        		newLeft	 =	aabb.getMinimum().x;
		        		newRight = 	aabb.getMaximum().x;
		        		
		        		newTop = aabb.getMaximum().z + (sizeWidth - sizeHeight) * 0.5;
		        		newBottom = aabb.getMinimum().z - (sizeWidth - sizeHeight) * 0.5; 
		        	}
		        	// => Width must be adjusted
		        	else
		        	{
		        		newTop = aabb.getMaximum().z;
		        		newBottom = aabb.getMinimum().z;
		        		
		        		newLeft = aabb.getMinimum().x - (sizeHeight - sizeWidth) * 0.5;
		        		newRight = aabb.getMaximum().x - (sizeHeight - sizeWidth) * 0.5;
		        	}            			
                }break;	        			
            }
             
            //Set the new limits
        	m_pCamOrthogonal->setLimits(newLeft, newTop, newRight, newBottom);
        }
        
        update();
 	}
    
    void BSRenderWidget::mousePressEvent(QMouseEvent* event)
    {
        
        int currentMouseProperty = getMouseProperty(event);
        
        if(currentMouseProperty == MouseSettings::Move)
        {
            if(settings.viewMode == VIEWMODE_Perspective)
                m_pCamPerspective->mousePress(event->x(), event->y());
            if(settings.viewMode == VIEWMODE_Orthogonal)
                m_pCamOrthogonal->mousePress(event->x(), event->y());
        }
        else if(currentMouseProperty == MouseSettings::Rotate)
        {
            if(settings.viewMode == VIEWMODE_Perspective)
                m_pCamPerspective->mousePress(event->x(), event->y());
        }
        BSGLWidget::mousePressEvent(event);
        emit mousePressed(event->pos(), event->button());
        update();
    }
	
    void BSRenderWidget::mouseMoveEvent(QMouseEvent* event)
    {
        bool mouseButtonPressed = false;
        if(event->buttons() != Qt::NoButton)
        {
            mouseButtonPressed = true;
        }
        
        if(mouseButtonPressed)
        {
            int currentMouseProperty = getMouseProperty(event);
            
            if(currentMouseProperty == MouseSettings::Move)
            {
                if(settings.viewMode == VIEWMODE_Perspective)
                    m_pCamPerspective->mouseMoveTranslate(event->x(), event->y());
                if(settings.viewMode == VIEWMODE_Orthogonal)
                    m_pCamOrthogonal->mouseMove(event->x(), event->y());
            }
            else if(currentMouseProperty == MouseSettings::Rotate)
            {
                if(settings.viewMode == VIEWMODE_Perspective)
                    m_pCamPerspective->mouseMove(event->x(), event->y());
            }
            
            BSGLWidget::mouseMoveEvent(event);
            update();
        }
        
        emit mouseMoved(event->pos(), event->buttons());
        emit mouseMovedRenderPosition(calcRenderPosition(event->pos()));
        
        //m_pCamPerspective->mouseMove(event->x(), event->y());
        //m_pCamOrthogonal->mouseMove(event->x(), event->y());
    }
    
    void BSRenderWidget::contextMenuEvent(QContextMenuEvent* event)
    {
        if (!QApplication::mouseButtons().testFlag(Qt::LeftButton))
        {
            QMenu menu(this);
            menu.addActions(actions());
            menu.exec(event->globalPos());
        }
    }
    
    void BSRenderWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        //if(event->button() == Qt::RightButton)
        //{
        //    displayContextMenu(event->globalPos());
        //    displayMenu = true;
        //}
        BSGLWidget::mouseReleaseEvent(event);
        emit mouseReleased(event->pos(), event->button());
    }

    void BSRenderWidget::wheelEvent(QWheelEvent* event)
    {
        //Send the event to the current camera
        if(settings.viewMode == VIEWMODE_Perspective)
            m_pCamPerspective->mouseWheel(event->delta());
        else
            m_pCamOrthogonal->mouseWheel(event->delta());
        
        update();
    }

    void BSRenderWidget::setGrid(bool status)
    {
        settings.bRenderGrid = status;
        update();
    }

    void BSRenderWidget::setWireframe(bool status)
    {
        if (status)
        {
            settings.fillMode = FILL_Wireframe;
        }
        update();
    }

    void BSRenderWidget::setWireframeOverlay(bool status)
    {
        if (status)
        {
            settings.fillMode = FILL_WireframeOverlay;
        }
        updateGL();
    }

    void BSRenderWidget::setPointMode(bool status)
    {
        if (status)
        {
            settings.fillMode = FILL_Point;
        }
        update();
    }

    void BSRenderWidget::setTextured(bool status)
    {
        if (status)
        {
            settings.fillMode = FILL_Textured;
        }
        update();
    }

    void BSRenderWidget::setNormals(bool status)
    {
        r->getSettings()->setShowNormals(status);
        update();
    }
    
    void BSRenderWidget::setSolid(bool status)
    {
        if(status)
        {
            settings.fillMode = FILL_Solid;
        }
        update();
    }
    
    Camera* BSRenderWidget::getCurrentCamera() const
    {
 		if(settings.viewMode == VIEWMODE_Perspective)
 			return m_pCamPerspective;
        if(settings.viewMode == VIEWMODE_Orthogonal)
        	return m_pCamOrthogonal;
        return NULL;
    }
    
    
    void BSRenderWidget::renderNow()
    {
        //Setup the glenvvars for this context
        r->getSettings()->setGlOptions();
            
        //Set up the clipping planes and fov for the cameras 
        m_pCamPerspective->setClippingPlanes(r->getNearPlane(), r->getFarPlane()); 
        m_pCamPerspective->setFOV(r->getFOV()); 
        m_pCamPerspective->setWheelSpeed(r->getSettings()->getMouseWheelSpeed()); 
        m_pCamOrthogonal->setClippingPlanes(r->getNearPlane(), r->getFarPlane()); 
        m_pCamOrthogonal->setFOV(r->getFOV()); 
        m_pCamOrthogonal->setWheelSpeed(r->getSettings()->getMouseWheelSpeed()); 
            //Set all important settings (neccessary, because all windows are sharing ONE renderer-instance)
        r->setViewMode(settings.viewMode);
        r->setFillMode(settings.fillMode);
        r->setCameraPerspective(m_pCamPerspective);
        r->setCameraOrthogonal(m_pCamOrthogonal);
        r->clear();
                
        if(settings.bRenderGrid)
        {
            r->renderGrid();
        }
            
        SceneManager *sm =SceneManager::getInstance();     
        sm->renderScene(*r);          
    
        //Force the renderer to render now
        //r->flush();	
    }
    
}
