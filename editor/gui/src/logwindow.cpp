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

        
#include "./../include/logwindow.h"
        
namespace BSGui
{
        
LogWindow::LogWindow(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.logTable->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui.logTable->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui.logTable->header()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui.logTable->header()->setResizeMode(4, QHeaderView::ResizeToContents);
    ui.logTable->header()->setResizeMode(5, QHeaderView::ResizeToContents);
    
    ui.logTable->header()->setResizeMode(1, QHeaderView::Stretch);
    ui.logTable->header()->setStretchLastSection (false);
    scrollBar = ui.logTable->verticalScrollBar();
    
    connect(BSCore::Log::getInstance(), SIGNAL(logChanged(BSCore::LogContents*)), this, SLOT(logChanged(BSCore::LogContents*)));
    
    connect(scrollBar, SIGNAL(rangeChanged(int, int)), this, SLOT(setSliderToMaximum(int, int)));
    
    ui.logTable->sortByColumn(5, Qt::AscendingOrder);
}

void LogWindow::logChanged(BSCore::LogContents* logEntry)
{
    
    
    QStringList listToInsert;
    listToInsert << logEntry->state
                 << logEntry->message
                 << QString::number(logEntry->line)
                 << logEntry->file
                 << logEntry->function
                 << logEntry->time;
    
    QColor logColor;
    QString state(logEntry->state);
    if (state == "OK")
    {
        logColor = Qt::darkGreen;
    }
    else if(state == "ERROR")
    {
        logColor = Qt::darkRed;
    }
    else if (state == "WARNING")
    {
        logColor = Qt::darkYellow;
    }
    else
    {
        logColor == Qt::black;
    }
    
    QTreeWidgetItem* itemToInsert = new QTreeWidgetItem(listToInsert);
    for(int i = 0; i < itemToInsert->columnCount(); i++)
    {
        itemToInsert->setData(i, Qt::ForegroundRole, logColor);
    }
    ui.logTable->insertTopLevelItem(ui.logTable->topLevelItemCount(), itemToInsert);
}

void LogWindow::setSliderToMaximum(int min, int max)
{
    scrollBar->setValue(max);
}

void LogWindow::closeEvent(QCloseEvent* event)
{
    emit windowClosed();
    event->accept();
}

}
