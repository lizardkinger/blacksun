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
*  File:       guialgoconst.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/


/** \file guialgoconst.h
 * \brief Defines some methods and constants useful for the gui.
 * 
 * \author Thomas Tischler.
 */

#ifndef GUIALGOCONST_H
#define GUIALGOCONST_H

#include "./../../math/include/ColorRGB.h"
#include <QColor>
#include <QSize>

namespace BSGui
{

const QSize standardIconSize = QSize(24, 24); /*!< \brief The standard size of the icons */
const QSize standardButtonSize = QSize(32, 32); /*!< \brief The standard size of the buttons */

/*!
 * \brief Converts a color to QColor.
 * \param theColor The Color.
 * \return The QColor.
 */
QColor ColorToQColor(BSMath::Color theColor);
/*!
 * \brief Converts a QColor to Color.
 * \param theColor The QColor.
 * \return The Color.
 */
BSMath::Color QColorToColor(QColor theColor);

}

#endif
