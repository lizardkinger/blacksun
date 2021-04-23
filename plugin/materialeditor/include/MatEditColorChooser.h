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
 *  File:       MatEditColorChooser.h
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATEDITCOLORCHOOSER_H_
#define MATEDITCOLORCHOOSER_H_

#include <QtGui>

/*! \file
* 
*  \brief Describes the class MatEditColorChooser, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief A widget to choose a color (with alpha) and preview it with a label
*
* A label with Shows a standard dialog to choose a color with alpha by clicking on it.
*/
class MatEditColorChooser : public QLabel
{
    
    Q_OBJECT
    
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	* \param f The window flag
	*/
	MatEditColorChooser(QWidget* parent = NULL, Qt::WindowFlags f = 0)
	 : QLabel(parent, f)
	{
	}
	
	/*! \brief Constructor 
	*
	* \param text The text of the label
	* \param parent The parent widget
	* \param f The window flag
	*/	
	MatEditColorChooser(const QString & text, QWidget * parent = NULL, Qt::WindowFlags f = 0)
	 : QLabel(text, parent, f)
	 {
	 }
       
	/*! \brief Returns the last choosen color 
	*
	* \return QColor The choosen color
	*/        
	QColor getColor()
	{
		return palette().color(QPalette::Button);
	}
	
	/*! \brief Sets the color of the label 
	*
	* \param color The color
	*/	
	void setColor(QColor color);
        
protected:
	/*! \brief An event if the mouse-button was released on this widget
	*
	* \param event The mouse-event
	*/
	void mouseReleaseEvent(QMouseEvent* event);
        
signals:
	/*! \brief The signal for changing the color 
	*
	* \param color The color
	*/
	void colorChanged(QColor color);
	
};

#endif /*MATEDITCOLORCHOOSER_H_*/
