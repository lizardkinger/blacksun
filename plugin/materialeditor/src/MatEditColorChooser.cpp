/***************************************************************************
 *   Copyright (C) 2006-07 by Reinhard Jeschull
 *   rjeschu@fh-landshut.de
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
 ***************************************************************************
 *
 *  Module:     Plugin (BlackSun)
 *  File:       MatEditColorChooser.cpp
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/MatEditColorChooser.h"

void MatEditColorChooser::mouseReleaseEvent(QMouseEvent* event)
{
	//Right button pressed?
	if(event->button() != Qt::LeftButton)
	{
		return;
	}

	//Show the color dialog to choose a color
	QRgb c = QColorDialog::getRgba(palette().color(QPalette::Button).rgba(), 0, this);
	//Convert it into QColor
    QColor currentColor(qRed(c), qGreen(c), qBlue(c), qAlpha(c));
    
    //Set new color, if color changed and color is a valid color
	if (currentColor != QColor::Invalid && currentColor != getColor())
	{
		setColor(currentColor);
	}
	event->accept();
}

void MatEditColorChooser::setColor(QColor color)
{
	//Set palette color
    setPalette(color); 
    
    //Emit a signal
    emit colorChanged(color);
}
