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
*  File:       bsquadsplittermidhandle.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

 /** \file bsquadsplittermidhandle.h
 * \brief Defines BSQuadSplitterMidHandle class.
 * 
 * \author Thomas Tischler.
  */
#ifndef BSQUADSPLITTERMIDHANDLE_H
#define BSQUADSPLITTERMIDHANDLE_H

#include <QtGui>
#include "bsquadsplitterhandle.h"

namespace BSGui
{
    
class BSQuadSplitterHandle;

/*! \brief A QuadSplitter Mid Handle
 *
 * This class represents the object in the mid of the QuadSplitter object. It is not meant to control this class directly use the BSQuadSplitterWidget class instead.
 */

class BSQuadSplitterMidHandle : public QWidget
{
    Q_OBJECT
            
    public:
        /*!
        * \brief Constructs a BSQuadSplitterHandle.
        * 
        * \param parent The widgets parent.
        * \param size The size of the splitter handle.
        */
        BSQuadSplitterMidHandle(BSQuadSplitterHandle* parent, int size);
        
        /*!
        * \brief Sets the size of the splitter handle.
        * 
        * \param width The size of the splitter handle.
        */
        void setWidth(int width);
        
    public slots:
        /*!
        * \brief Sets the position of the splitter handle.
        * 
        * \param pos The position of the splitter handle.
        */
        void setPosition(int pos);
    signals:
        /*!
        * \brief This signal is emitted when the handle was moved.
        * The first parameter gives the position to the splitter was moved.
        */
        void mouseMove(QPoint);
        
    protected:
        /*!
        * \brief Handles the mouse move event.
        * \param event The event.
        */
        void mouseMoveEvent(QMouseEvent* event);
        /*!
         * \brief Handles the mouse press event.
         * \param event The event.
         */
        void mousePressEvent(QMouseEvent* event);
        /*!
         * \brief Handles the change event.
         * \param event The event.
         */
        void changeEvent(QEvent* event);
        
    private:
        /*!
        * \brief Gives the height correction.
        * \return The Height correction.
        */
        int heightCorrection();

        int size; /*!< \brief The size of the handle */
                  
        /*!
         * \brief Updates the size.
         */
        void updateSize();
        
                
        int mouseOffset; /*!< \brief The mouse offset */
};

}

#endif
