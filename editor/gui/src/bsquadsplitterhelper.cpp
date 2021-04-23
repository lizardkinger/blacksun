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
 
 
#include "./../include/bsquadsplitterhelper.h"

namespace BSGui
{

BSQuadSplitterHelper::BSQuadSplitterHelper(QWidget* parent) 
    : QSplitter(Qt::Vertical, parent)
{
}

void BSQuadSplitterHelper::setPosition(int pos)
{
    int currentPos = sizes()[0];
    if (currentPos != pos)
    {
        moveSplitter(pos, 1);
    }
}

void BSQuadSplitterHelper::setPosition(QPoint pos)
{
    QPoint localPos = mapFromGlobal(pos);
    int currentPos = y();
    if (currentPos != localPos.y())
    {
        moveSplitter(localPos.y(), 1);
    }
}

}
