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
*  File:       bsglwidget.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 /** \file bsglwidget.h
 * \brief Defines BSGLWidget class.
 * 
 * \author Thomas Tischler.
  */
#ifndef BSGLWIDGET_H
#define BSGLWIDGET_H

#include <QGLWidget>
#include <QtGui>

namespace BSGui
{
    /*! \brief A QGLWidget that shares one GLContext. 
    */
class BSGLWidget : public QGLWidget
{
    public:
        /*!
        * \brief Constructs a BSGLWidget
        * 
        * \param parent The parent id this widget
        * \param f The window Flags fpr this widget
        */
        BSGLWidget(QWidget* parent = NULL, Qt::WindowFlags f = 0);
        
        /*!
         * \brief gets the shared widget
         * 
         * \return The shared widgets of all BSGLWidgets
        */
        static QGLWidget* getShared();
        
    private:
        static QPointer<QGLWidget> shared; /*!< \brief The shared widget */
};

}

#endif
