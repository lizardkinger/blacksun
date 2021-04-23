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
 
#include "./../include/bsquadsplitterwidget.h"

namespace BSGui
{

    BSQuadSplitterWidget::BSQuadSplitterWidget(QWidget* topLeft, QWidget* bottomLeft, QWidget* topRight, QWidget* bottomRight, QWidget* parent)
    :QWidget(parent), maximized(NULL), startMaximized(false), topLeft(topLeft),
             topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight)
{
    initUi();
    left->addWidget(topLeft);
    left->addWidget(bottomLeft);
    
    
    right->addWidget(topRight);
    right->addWidget(bottomRight);
}

void BSQuadSplitterWidget::initUi()
{
    left = new BSQuadSplitterHelper();
    right = new BSQuadSplitterHelper();
    
    quadSplitter = new BSQuadSplitter(this);
    QLayout* layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(quadSplitter);
    layout->setMargin(0);
    layout->setSpacing(0);
    
    
    quadSplitter->addWidget(left);
    quadSplitter->addWidget(right);
    
    
    /*left->addWidget(first);
    left->addWidget(second);
    
    
    right->addWidget(third);
    right->addWidget(fourth);*/
    
    quadHandle = qobject_cast<BSQuadSplitterHandle*>(quadSplitter->handle(1));
    
    
    connect(quadHandle, SIGNAL(moveHorizontal(QPoint)), left, SLOT(setPosition(QPoint)));
    connect(quadHandle, SIGNAL(moveHorizontal(QPoint)), right, SLOT(setPosition(QPoint)));
    
    connect(left, SIGNAL(splitterMoved(int, int)), right, SLOT(setPosition(int)));
    connect(right, SIGNAL(splitterMoved(int, int)), left, SLOT(setPosition(int)));
    

    connect(left, SIGNAL(splitterMoved(int, int)), quadHandle->getMidHandle(), SLOT(setPosition(int)));
    //connect(right, SIGNAL(splitterMoved(int, int)), quadHandle->getMidHandle(), SLOT(setPosition(int)));
}

void BSQuadSplitterWidget::restoreState()
{
    QSettings settings;
    settings.beginGroup(objectName());
    
    right->restoreState(settings.value(objectName() + ":right").toByteArray());
    left->restoreState(settings.value(objectName() + ":left").toByteArray());
    quadSplitter->restoreState(settings.value(objectName() + ":mid").toByteArray());
    
    QWidget* widgetToMaximize = widget(settings.value(objectName() + ":maximized", -1).toInt());
    if (widgetToMaximize != NULL)
    {
        startMaximized = true;
        maximized = widgetToMaximize;
    }
    
    right->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
    left->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
    quadSplitter->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
    
    settings.endGroup();
}


int BSQuadSplitterWidget::indexOf(const QWidget* widget) const
{
    if(topLeft == widget)
    {
        return 0;
    }
    else if(bottomLeft == widget)
    {
        return 1;
    }
    else if(topRight == widget)
    {
        return 2;
    }
    else if(bottomRight == widget)
    {
        return 3;
    }
    return -1;
}

QWidget* BSQuadSplitterWidget::widget(const int index) const
{
    switch(index)
    {
        case 0:
        {
            return topLeft;
        }
        case 1:
        {
            return bottomLeft;
        }
        case 2:
        {
            return topRight;
        }
        case 3:
        {
            return bottomRight;
        }
    }
    return NULL;
}

void BSQuadSplitterWidget::storeState() const
{
    QSettings settings;
    settings.beginGroup(objectName());
    
    if (maximized != NULL)
    {
        settings.setValue(objectName() + ":right", rightState);
        settings.setValue(objectName() + ":left", leftState);
        settings.setValue(objectName() + ":mid", quadSplitterState);
    }
    else
    {
        settings.setValue(objectName() + ":right", right->saveState());
        settings.setValue(objectName() + ":left", left->saveState());
        settings.setValue(objectName() + ":mid", quadSplitter->saveState());
    }
    
    settings.setValue(objectName() + ":maximized", indexOf(maximized));
    settings.endGroup();
}

void BSQuadSplitterWidget::setMaximized(QWidget* widget, bool status)
{
    setMaximized(indexOf(widget), status);
}

void BSQuadSplitterWidget::setMaximized(int index, bool status)
{
    if(status && !startMaximized)
    {
        
        leftState = left->saveState();
        rightState = right->saveState();
        quadSplitterState = quadSplitter->saveState();
        
        quadSplitter->hide();
        layout()->addWidget(widget(index));
        
        maximized = widget(index);
    }
    else if(!status && !startMaximized)
    {

        QWidget* widgetToInsert = widget(index);
        
        if(index == 0)
        {
            left->insertWidget(0, widgetToInsert);
        }
        else if(index == 1)
        {
            left->insertWidget(1, widgetToInsert);
        }
        else if(index == 2)
        {
            right->insertWidget(0, widgetToInsert);
        }
        else if(index == 3)
        {
            right->insertWidget(1, widgetToInsert);
        }
        
        
        quadSplitter->show();
        left->restoreState(leftState);
        right->restoreState(rightState);
        quadSplitter->restoreState(quadSplitterState);
        
        quadHandle->getMidHandle()->setPosition(left->handle(1)->y());
        
        maximized = NULL;
    }
}

/*
void BSQuadSplitter::setHandleWidth(int width)
{
    QSplitter::setHandleWidth(width);
    left->setHandleWidth(width);
    right->setHandleWidth(width);
    midSplitter->getMidHandle()->setWidth(width);
}
*/

void BSQuadSplitterWidget::resizeEvent(QResizeEvent* event)
{
    if(quadSplitter->isVisible())
    {
        quadHandle->getMidHandle()->setPosition(left->handle(1)->y());
    }
    if(startMaximized && left->handle(1)->isVisible())
    {
        startMaximized = false;
        setMaximized(indexOf(maximized), true);
    }
    event->ignore();
}

void BSQuadSplitterWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::StyleChange)
    {
        right->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
        left->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
        quadSplitter->setHandleWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
        quadHandle->getMidHandle()->setPosition(left->handle(1)->y());
    }
    //event->ignore();
}

}

