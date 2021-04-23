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
*  File:       bsquadsplitterhandle.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bsquadsplitterhandle.h
 * \brief Defines BSQuadSplitterHandle class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSQUADSPLITTERHANDLE_H
#define BSQUADSPLITTERHANDLE_H

#include <QtGui>
#include "bsquadsplittermidhandle.h"
#include "bsquadsplitter.h"

namespace BSGui
{

    
class BSQuadSplitterMidHandle;
class BSQuadSplitter;
    /*! \brief A QuadSplitter Handle 
    *
    * This class represents the handle of the BSQuadSplitter. It is not meant to control this class directly use the BSQuadSplitterWidget class instead.
    */
class BSQuadSplitterHandle : public QSplitterHandle
{
    
    Q_OBJECT
    
    public:
        /*!
        * \brief Constructs a BSQuadSplitterHandle.
        * 
        * \param parent The widgets parent.
        */
        BSQuadSplitterHandle(QSplitter* parent = NULL);
        /*!
         * \brief Gets the BSQuadSplitterMidHandle from this splitter.
         * 
         * \return The midHandle
        */
        BSQuadSplitterMidHandle* getMidHandle() const {return midHandle;}
        
    protected:
        BSQuadSplitterMidHandle* midHandle; /*!< \brief The mid handle */
    
    signals:
        /*!
        * \brief This signal is emitted when the horizontal part of the splitter was moved.
        * The paramter gives the position where the splitter was moved to.
        */
        void moveHorizontal(QPoint);
};


    
}
#endif

