/***************************************************************************
*   Copyright (C) 2006 by Thomas Tischler
*   ttischl1@fh-landshut.de
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
****************************************************************************
*
*  Module:     GUI (BlackSun)
*  File:       SplashScreen.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/



/** \file SplashScreen.h
 * \brief Defines SplashScreen class.
 * 
 * \author Thomas Tischler.
 */
 
#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

#include <QSplashScreen>


namespace BSGui
{

/*! \brief This Widget represents the splah screen.
*/
class SplashScreen : public QSplashScreen
{
	 
public:
    /*!
    * \brief The Constructor of the Widget.
    * 
    * \param f The window flags.
    * \param pixmap The pixmap of the splashscreen
    */
    SplashScreen(const QPixmap & pixmap = QPixmap(),Qt::WindowFlags f = 0);
    /*!
    * \brief The destructor.
    */
    ~SplashScreen();
};

}
#endif /*SPLASHSCREEN_H_*/
