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
 
#include "./../include/bsquadsplittermidhandle.h"

namespace BSGui
{
BSQuadSplitterMidHandle::BSQuadSplitterMidHandle(BSQuadSplitterHandle* parent, int size)
    :QWidget(parent), size(size)
{
    updateSize();
    
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    setCursor(Qt::SizeAllCursor);
    
    setPalette(QPalette(Qt::green));
    setAutoFillBackground(true);
}

void BSQuadSplitterMidHandle::updateSize()
{
    setMaximumSize(size, size);
    setMinimumSize(size, size);
}

void BSQuadSplitterMidHandle::mouseMoveEvent(QMouseEvent* event)
{
    QPoint correctedPos(event->globalX(), event->globalY() - mouseOffset);
    emit mouseMove(correctedPos);
    event->ignore();
}

void BSQuadSplitterMidHandle::setWidth(int width)
{
    this->size = width;
    updateSize();
}

void BSQuadSplitterMidHandle::setPosition(int pos)
{
    move(x(), pos - heightCorrection());
}

void BSQuadSplitterMidHandle::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::StyleChange)
    {
        setWidth(style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this));
    }
}

void BSQuadSplitterMidHandle::mousePressEvent(QMouseEvent* event)
{
    if(event->button() != Qt::LeftButton)
    {
        return;
    }
    mouseOffset = event->y() - heightCorrection();
    event->ignore();
}

int BSQuadSplitterMidHandle::heightCorrection()
{
    return height()/2 - size/2;
}

}
