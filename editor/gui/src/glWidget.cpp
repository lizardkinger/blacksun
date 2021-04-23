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

#include "./../include/glWidget.h"
#include "./../include/mainwindow.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../scenegraph/include/SceneAction2D.h"

	
using namespace BSScene;
using namespace BSRenderer;
	
namespace BSGui
{

    GLWidget::GLWidget(QWidget *parent, bool bOrtho, FillMode fm, 
    OrthogonalType orthoType)
    : BSRenderWidget(parent, bOrtho, fm, orthoType), showRubberBand(false), 
      showMoveLine(false), showScaleLine(false), showRotateArc(false), overpaintingColor(QColor(0,0,0))
    {
        actionMaximizeMe = new QAction("Maximized", this);
        actionMaximizeMe->setCheckable(true);
        
        insertAction(1, actionMaximizeMe);

        enableOverpainting();
        connect(actionMaximizeMe, SIGNAL(toggled(bool)), this, SLOT(setMaximized(bool)));
    }

    void GLWidget::setMaximized(bool status)
    {
        emit maximizeMe(this, status);
    }

	/*void GLWidget::mousePositionChanged(QPoint pos)
	{
		QPointF p;
		
		if(settings.viewMode != VIEWMODE_Perspective)
		{
			p = calcRenderPosition(pos);
		}
		
		MainWindow::getInstance()->getStatusBarPosLabel()->setText("X: " + QString::number(p.x(), 'f', 4) + "  Y: " + QString::number(p.y(), 'f', 4));
	}*/

    void GLWidget::mousePressEvent(QMouseEvent* event)
    {
        SceneManager *sm = SceneManager::getInstance();
        BSRenderWidget::mousePressEvent(event);
        int currentMouseProperty = getMouseProperty(event);
        lastMouseMoveProperty = currentMouseProperty;
        
        if(event->buttons() != Qt::NoButton)
        {
            if(currentMouseProperty == MouseSettings::SelectedAction)
            {
            
                if(settings.viewMode == VIEWMODE_Orthogonal)
                {
                    rubberBandOrigin = event->pos();
                    rubberBandDestination = event->pos();
                    showRubberBand = true;
                    
                    if(sm->getActionMode() == BSScene::SCENEACTION_Move)
                    {
                        moveLineOrigin = snapToGrid(event->pos());
                        moveLineDestination = snapToGrid(event->pos());
                        showMoveLine = true;
                    }
                    
                    if(sm->getActionMode() == BSScene::SCENEACTION_Rotate)
                    {
                        moveLineOrigin = snapToGrid(event->pos());
                        moveLineDestination = snapToGrid(event->pos());
                        showRotateArc = true;
                    }
                    
                    if(sm->getActionMode() == BSScene::SCENEACTION_Scale)
                    {
                        moveLineOrigin = snapToGrid(event->pos());
                        moveLineDestination = snapToGrid(event->pos());
                        showScaleLine = true;
                    }
                }
            }
            else if((currentMouseProperty == MouseSettings::AddToSelection ||
                    currentMouseProperty == MouseSettings::RemoveFromSelection)
                    && sm->getActionMode() == BSScene::SCENEACTION_Selection)
            {
                rubberBandOrigin = event->pos();
                rubberBandDestination = event->pos();
                showRubberBand = true;
            }
        }
    }
    
    void GLWidget::mouseMoveEvent(QMouseEvent* event)
    {
    	SceneManager *sm = SceneManager::getInstance();
        if(event->buttons() != Qt::NoButton)
        {
            
            int currentMouseProperty = getMouseProperty(event);
            lastMouseMoveProperty = currentMouseProperty;
            
            if(currentMouseProperty == MouseSettings::SelectedAction)
            {
                switch(sm->getActionMode())
                {
                    case BSScene::SCENEACTION_Creation:
                    {
                        break;
                    }
                    case BSScene::SCENEACTION_Rotate:
                    case BSScene::SCENEACTION_Move:
                    case BSScene::SCENEACTION_Scale:
                    {
                        moveLineDestination = snapToGrid(event->pos());
                        break;
                    }
                    default:
                    {
                        if(settings.viewMode == VIEWMODE_Orthogonal)
                        {
                            if(settings.viewMode == VIEWMODE_Orthogonal)
                            {
                                rubberBandDestination = event->pos();
                            }
                        }
                    }
                }
            }
            else if((currentMouseProperty == MouseSettings::AddToSelection || 
                    currentMouseProperty == MouseSettings::RemoveFromSelection)
                     && sm->getActionMode() == BSScene::SCENEACTION_Selection)
            {
                rubberBandDestination = event->pos();
            }
        }
        BSRenderWidget::mouseMoveEvent(event);
    }
    
    SceneAction2D GLWidget::buildSceneAction2D(Qt::MouseButton button,
                                               SceneAction2D_SelectionType selectionType, 
                                               QPoint eventPos)
    {
        SceneManager *sm = SceneManager::getInstance();
        
        QPointF startPos;
        QPointF endPos;
        SceneAction2D sa;
        if(sm->getActionMode() == BSScene::SCENEACTION_Selection)
        {
            startPos = calcRenderPosition(rubberBandOrigin);
            endPos = calcRenderPosition(eventPos);
        }
        else
        {
            startPos = calcRenderPosition(snapToGrid(rubberBandOrigin));
            endPos = calcRenderPosition(snapToGrid(eventPos));
        }
        
        SceneAxis current_axis;
        switch(getOrthoType())
        {
            case ORTHO_Front:
            case ORTHO_Back:
                current_axis = AXIS_z;
                break;
                        
            case ORTHO_Left:
            case ORTHO_Right:
                current_axis = AXIS_x;
                break;		
                        
            case ORTHO_Top:
            case ORTHO_Bottom:
                current_axis = AXIS_y;
                break;	        			
        }
        sa.topleft = startPos;
        sa.bottomright = endPos;
        sa.mouseButton = button;
        sa.axis = current_axis;
        sa.axis = current_axis;
        sa.selType = selectionType;
                
        switch(sm->getActionMode())
        {
            case BSScene::SCENEACTION_Rotate:
            {
                sa.rotateValue = rotateAngle;
            }
            break;
                        
            case BSScene::SCENEACTION_Scale:
            {
                double svX, svY, svZ;
                svX = svY = svZ = 0.0;
                    	
                switch(getOrthoType())
                {
                    case ORTHO_Front:
                    case ORTHO_Back:
                        svX = (moveLineDestination.x() - moveLineOrigin.x()) / 100.0; 
                        svY = (moveLineDestination.y() - moveLineOrigin.y()) / 100.0; 
                        break;
		                        
                    case ORTHO_Left:
                    case ORTHO_Right:
                        svZ = (moveLineDestination.x() - moveLineOrigin.x()) / 100.0; 
                        svY = (moveLineDestination.y() - moveLineOrigin.y()) / 100.0; 
                        break;		
		                        
                    case ORTHO_Top:
                    case ORTHO_Bottom:
                        svX = (moveLineDestination.x() - moveLineOrigin.x()) / 100.0; 
                        svZ = (moveLineOrigin.y() - moveLineDestination.y() ) / 100.0; 
                        break;	        			
                }
		                
                sa.scaleValueX = 1 + svX;
                sa.scaleValueY = 1 + svY;
                sa.scaleValueZ = 1 + svZ;                    	
                    	
                        
                      /*  qDebug() << "sa.scaleValueX: " << sa.scaleValueX;
                qDebug() << "sa.scaleValueY: " << sa.scaleValueY;
                qDebug() << "sa.scaleValueZ: " << sa.scaleValueZ;*/
            }
            break;
                    
            default: break;
        }
        return sa;
    }
    
    void GLWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        
        SceneManager *sm = SceneManager::getInstance();
        
        BSRenderWidget::mouseReleaseEvent(event);
        showRubberBand = false;
        showMoveLine = false;
        showRotateArc = false;
        showScaleLine = false;
        
        if(lastMouseMoveProperty == MouseSettings::SelectedAction)
        {

            if(settings.viewMode != VIEWMODE_Perspective)
            {
                sm->action2D(buildSceneAction2D(event->button(), SCENETYPE_Select, event->pos()));
            }
        }
        else if(lastMouseMoveProperty == MouseSettings::AddToSelection
                && sm->getActionMode() == BSScene::SCENEACTION_Selection)
        {
            if(settings.viewMode != VIEWMODE_Perspective)
            {
                sm->action2D(buildSceneAction2D(event->button(), SCENETYPE_AddToSelection, event->pos()));
            }
        }
        else if(lastMouseMoveProperty == MouseSettings::RemoveFromSelection
                && sm->getActionMode() == BSScene::SCENEACTION_Selection)
        {
            if(settings.viewMode != VIEWMODE_Perspective)
            {
                sm->action2D(buildSceneAction2D(event->button(), SCENETYPE_RemoveFromSelection, event->pos()));
            }
        }
        update();
    }
    
    QPoint GLWidget::getNearestGridPosition(QPoint pos) const
    {
        vector<QPoint> gridPoints;
        //r->getGridIntersectionPoints(gridPoints);
        if(settings.viewMode == VIEWMODE_Orthogonal)
        {
            m_pCamOrthogonal->getGridIntersectionPoints(gridPoints);
            QPoint nearestPoint;
            nearestPoint = gridPoints.at(0);
            QLine beginDestination(pos, nearestPoint);
            int nearestDistance = beginDestination.dx() * beginDestination.dx() + 
                                  beginDestination.dy() * beginDestination.dy();
            for(unsigned int i = 1 ; i < gridPoints.size() ; i++)
            {
                QLine newDestination(pos, gridPoints.at(i));
                int newDistance = newDestination.dx() * newDestination.dx() + 
                                  newDestination.dy() * newDestination.dy();
                if(newDistance < nearestDistance)
                {
                    nearestPoint = gridPoints.at(i);
                    nearestDistance = newDistance;
                }
            }
            return nearestPoint;
        }
        return pos;
    }
    
    QPoint GLWidget::snapToGrid(QPoint point)
    {
        if(MainWindow::getInstance()->getSnapToGridBox()->isChecked())
        {
            return getNearestGridPosition(point);
        }
        return point;
    }
    
    void GLWidget::enterEvent(QEvent* event)
    {
        setMouseTracking(true);
    }
    
    void GLWidget::leaveEvent(QEvent* event)
    {
        setMouseTracking(false);
       emit mouseMovedRenderPosition(QPointF(0,0));
    }
    
    void GLWidget::initializeGL()
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        
        BSRenderWidget::initializeGL();
        
        glPopAttrib();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
    
    void GLWidget::paintEvent(QPaintEvent* event)
    {
    	SceneManager *sm =SceneManager::getInstance();
    	const double fSmoothness = 1.5;
        QPainter painter;
        painter.begin(this);
       // QPen myPen;   
        painter.setPen(Qt::SolidLine);
        //myPen.setWidth(2);
        painter.setPen(overpaintingColor);
        //myPen.setColor(overpaintingColor);
        //painter.setPen(myPen);    
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        
        initRenderer();
        resetViewport();
        renderNow();
        
        glPopAttrib();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
        if(showRubberBand)
        {
            painter.save();
            QColor rectColor(selectionBoxColor);
            painter.setPen(rectColor);
            painter.drawRect(QRect(rubberBandOrigin, rubberBandDestination));
            rectColor.setAlpha(50);
            painter.fillRect(QRect(rubberBandOrigin, rubberBandDestination), rectColor);
            painter.restore();
        }
        if(showMoveLine || showScaleLine)
        {
            painter.drawLine(moveLineOrigin, moveLineDestination);
        }
        
        if(showRotateArc)
        {
        	
			Aabb aabb = sm->getSelBuffer().getBoundary();
			Vector center = aabb.getCentre();
			
			//qDebug() << "AABB-Center: " << aabb.getCentre().x << ", " << aabb.getCentre().y << ", " << aabb.getCentre().z;
	
			QPointF pointScene;
	        switch(getOrthoType())
            {
                case ORTHO_Front:
                case ORTHO_Back:
                	pointScene = QPointF(center.x,center.y);
                    break;
        			
                case ORTHO_Left:
                case ORTHO_Right:
                	pointScene = QPointF(center.z,center.y);
                    break;		
        			
                case ORTHO_Top:
                case ORTHO_Bottom:
                	pointScene = QPointF(center.x,center.z);
                    break;	        			
            }
            
			QPoint pointG = calcGUIPosition(pointScene);
			
        	qreal x= pointG.x();
        	qreal y= pointG.y();
        	qreal w= width() / 4.0;
        	qreal h= height() / 4.0;
        	
        	QRectF rectangle(x - w/2 ,y - h/2, w,h);

        	double lineX = moveLineDestination.x() - moveLineOrigin.x();

 			rotateAngle = lineX * fSmoothness;
 			
 			if(rotateAngle > 360)
 				rotateAngle = 360;
 			else if(rotateAngle < -360)
 				rotateAngle = -360;

 			int startAngle = 0 * 16;
 			int spanAngle = static_cast<int>(rotateAngle * 16);
 			
 			spanAngle = (-1 * spanAngle);
        	
        	painter.drawPie(rectangle,startAngle,spanAngle);
			
			sm->checkForRedraw(true);
        }
        
        QString stringToShow;
        
        if(settings.viewMode == VIEWMODE_Orthogonal)
        {
            if(m_pCamOrthogonal->getOrthogonalType() == ORTHO_Left)
            {
                stringToShow = "Left View";
            }
            
            if(m_pCamOrthogonal->getOrthogonalType() == ORTHO_Top)
            {
                stringToShow = "Top View";
            }
            
            if(m_pCamOrthogonal->getOrthogonalType() == ORTHO_Front)
            {
                stringToShow = "Front View";
            }
        }
        else
        {
            stringToShow = "3D View";
        }
        
        painter.drawText(QPoint(10,20), stringToShow);
        /*
        vector<QPoint> gridPoints;
        m_pCamOrthogonal->getGridIntersectionPoints(gridPoints);
        for(unsigned int i = 1 ; i < gridPoints.size() ; i++)
        {
        	painter.drawPoint(gridPoints.at(i));
        }
        */
        
        painter.end();
    }

}
