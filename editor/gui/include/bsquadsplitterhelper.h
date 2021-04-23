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
*  File:       bsquadsplitterhelper.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 /** \file bsquadsplitterhelper.h
 * \brief Defines BSQuadSplitterHelper class.
 * 
 * \author Thomas Tischler.
  */
#ifndef BSQUADSPLITTERHELPER_H
#define BSQUADSPLITTERHELPER_H

#include <QtGui>

namespace BSGui
{

    /*! \brief A QuadSplitter Helper 
     *
     * This class represents the Splitter controlled by BSQuadSplitter. It is not meant to control this class directly use the BSQuadSplitterWidget class instead.
     */
class BSQuadSplitterHelper : public QSplitter
{
    
    Q_OBJECT
    
    public:
    /*!
     * \brief Constructs a BSQuadSplitterHandle.
     * 
     * \param parent The widgets parent.
     */
        BSQuadSplitterHelper(QWidget* parent = NULL);
    public slots:
        /*!
        * \brief Sets the position for the widget.
        * 
        * \param pos The new Position. This must be a global position and only the y component is used from pos.
        */
        void setPosition(QPoint pos);
        
        /*!
         * \brief Sets the position for the widget
         * 
         * \param pos The new position. This must be a local position.
        */
        void setPosition(int pos);
};

}


#endif
