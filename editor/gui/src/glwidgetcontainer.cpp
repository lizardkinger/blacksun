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
        
#include "./include/glwidgetcontainer.h"

namespace BSGui
{
        
GLWidgetContainer::GLWidgetContainer(QWidget* parent) 
    : QWidget(parent), currentlyMaximized(NULL)
{
    
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    //setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
    
    layout->setMargin(0);
}

void GLWidgetContainer::setMaximized(GLWidget* widget, bool status)
{
    quadSplitter->setMaximized(widget, status);
    
}

void GLWidgetContainer::storeState() const
{
    quadSplitter->storeState();
}

void GLWidgetContainer::addGLWidget(GLWidget* widgetToAdd)
{
    widgetList.append(widgetToAdd);
    
    if (widgetList.count() == 4 )
    {
        quadSplitter = new BSQuadSplitterWidget(widgetList.at(0), widgetList.at(1), widgetList.at(2), widgetList.at(3), this);
        layout()->addWidget(quadSplitter);;
        quadSplitter->setObjectName("quadSplitter");
    }
    
    connect(widgetToAdd, SIGNAL(showGrid(bool)), this, SLOT(emitGrid(bool)));
    connect(widgetToAdd, SIGNAL(showWireframe(bool)), this, SLOT(emitWireframe(bool)));
    connect(widgetToAdd, SIGNAL(showWireframeOverlay(bool)), this, SLOT(emitWireframeOverlay(bool)));
    connect(widgetToAdd, SIGNAL(showTextured(bool)), this, SLOT(emitTextured(bool)));
    connect(widgetToAdd, SIGNAL(showPointsOnly(bool)), this, SLOT(emitPointMode(bool)));
    connect(widgetToAdd, SIGNAL(showNormals(bool)), this, SLOT(emitNormals(bool)));
    connect(widgetToAdd, SIGNAL(showSolid(bool)), this, SLOT(emitSolid(bool)));
    connect(widgetToAdd, SIGNAL(maximizeMe(GLWidget*, bool)), this, SLOT(setMaximized(GLWidget*, bool)));
}

void GLWidgetContainer::restoreState()
{
    quadSplitter->restoreState();
    if (quadSplitter->currentlyMaximized() != NULL)
    {
        qobject_cast<GLWidget*>(quadSplitter->currentlyMaximized())->setMaximizedAction(true);
    }
}

void GLWidgetContainer::emitGrid(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getGrid() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showGrid(status);
    }
    else
    {
        emit showGrid(false);
    }
}

void GLWidgetContainer::emitWireframe(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getWireframe() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showWireframe(status);
    }
    else
    {
        emit showWireframe(false);
    }
}

void GLWidgetContainer::emitWireframeOverlay(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getWireframeOverlay() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showWireframeOverlay(status);
    }
    else
    {
        emit showWireframeOverlay(false);
    }
}

void GLWidgetContainer::emitPointMode(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getPointMode() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showPointsOnly(status);
    }
    else
    {
        emit showPointsOnly(false);
    }
}

void GLWidgetContainer::emitTextured(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getTextured() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showTextured(status);
    }
    else
    {
        emit showTextured(false);
    }
}

void GLWidgetContainer::emitNormals(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getNormals() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showNormals(status);
    }
    else
    {
        emit showNormals(false);
    }
}

void GLWidgetContainer::emitSolid(bool status)
{
    bool sameStatus = true;
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if(widgetList.at(i)->getSolid() != status)
        {
            sameStatus = false;
            break;
        }
    }
    if(sameStatus)
    {
        emit showSolid(status);
    }
    else
    {
        emit showSolid(false);
    }
}

bool GLWidgetContainer::getGrid() const
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getGrid())
        {
            return false;
        }
    }
    return true;
}

bool GLWidgetContainer::getWireframe() const 
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getWireframe())
        {
            return false;
        }
    }
    return true;
}

bool GLWidgetContainer::getWireframeOverlay() const
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getWireframeOverlay())
        {
            return false;
        }
    }
    return true;
}

bool GLWidgetContainer::getPointMode() const
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getPointMode())
        {
            return false;
        }
    }
    return true;
}

bool GLWidgetContainer::getTextured() const
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getTextured())
        {
            return false;
        }
    }
    return true;
}

bool GLWidgetContainer::getNormals() const
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        if (!widgetList.at(i)->getNormals())
        {
            return false;
        }
    }
    return true;
}

void GLWidgetContainer::setGrid(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setGridAction(status);
    }
}

void GLWidgetContainer::setWireframe(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setWireframeAction(status);
    }
}

void GLWidgetContainer::setWireframeOverlay(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setWireframeOverlayAction(status);
    }
}

void GLWidgetContainer::setPointMode(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setPointModeAction(status);
    }
}

void GLWidgetContainer::setTextured(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setTexturedAction(status);
    }
}

void GLWidgetContainer::setNormals(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setNormalsAction(status);
    }
}

void GLWidgetContainer::setSolid(bool status)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setSolidAction(status);
    }
}

void GLWidgetContainer::setOverpaintingColor(QColor color)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setOverpaintingColor(color);
    }
}

QColor GLWidgetContainer::getOverpaintingColor()
{
    if(widgetList.count() > 1)
    {
        return widgetList.at(0)->getOverpaintingColor();
    }
    return QColor();
}

void GLWidgetContainer::setSelectionBoxColor(QColor color)
{
    for(int i = 0 ; i < widgetList.count() ; i++)
    {
        widgetList.at(i)->setSelectionBoxColor(color);
    }
}

QColor GLWidgetContainer::getSelectionBoxColor() const
{
    if(widgetList.count() > 1)
    {
        return widgetList.at(0)->getSelectionBoxColor();
    }
    return QColor();
}

void GLWidgetContainer::resetViews()
{
	foreach(GLWidget* current, widgetList)
	{
		current->getCurrentCamera()->reset();
	}
}

GLWidgetContainer::~GLWidgetContainer()
{
    storeState();
}

}
