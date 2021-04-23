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
*  File:       bsquadsplitter.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

/** \file bsquadsplitter.h
 * \brief Defines BSQuadSplitter class.
 * 
 * \author Thomas Tischler.
 */
#ifndef BSQUADSPLITTER_H
#define BSQUADSPLITTER_H

#include <QtGui>
#include "bsquadsplitterhelper.h"
#include "bsquadsplitterhandle.h"

namespace BSGui
{

    /*! \brief A QuadSplitter 
     *
     * This class represents a splitter that splits two other splitters. It is not meant to control this class directly use the BSQuadSplitterWidget class instead.
     */
class BSQuadSplitterHandle;
    
class BSQuadSplitter : public QSplitter
{
    
    Q_OBJECT
    
    public:
    /*!
     * \brief Constructs a BSQuadSplitter.
     * 
     * \param parent The widgets parent.
    */
        BSQuadSplitter(QWidget* parent = NULL);
        
    protected:
    /*!
     * \brief Constructs a BSQuadSplitterMidHandle
    * \return the contsructed Handle
    */
        QSplitterHandle* createHandle();
};
    
}
#endif

