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
    

#include <QtGui>

#include "./../include/bsgroupbox.h"

namespace BSGui
{

BSGroupBox::BSGroupBox(const QString& text, QWidget *parent)
    : QGroupBox(text, parent), prevMaximumHeight(QWIDGETSIZE_MAX)
{
    setCheckable(true);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleChildWidgets(bool)));
    minimumHeight = qMax(style()->pixelMetric(QStyle::PM_IndicatorHeight), fontMetrics().height());
    minimumHeight += style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
}

void BSGroupBox::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::StyleChange)
    {
        minimumHeight = qMax(style()->pixelMetric(QStyle::PM_IndicatorHeight), fontMetrics().height());
        minimumHeight += style()->pixelMetric(QStyle::PM_DefaultFrameWidth)*2;
    }
}

void BSGroupBox::toggleChildWidgets(bool show)
{
    if(show)
    {
        setMaximumHeight(prevMaximumHeight);
    }
    else
    {
        prevMaximumHeight = maximumHeight();
        setMaximumHeight(minimumHeight);
    }
}

}
