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
 *  File:       ImExporterPack.h
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef IMEXPORTERPACKINTERFACE_HH_
#define IMEXPORTERPACKINTERFACE_HH_

#include <PluginInterface.hh>
#include <QtGui>

/*! \file
* 
*  \brief Describes the class ImExporterPackInterface, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The interface for the helix generator
*
* The interface for the Im- and ExporterPack to combine the plugin base class with QObject
*/
class ImExporterPackInterface : public PlgInt, public QObject
{
public:
	static const UID uid = 117732UL; /*!< \brief UID of the plugin*/

    /*! \brief The default constructor
     */
	ImExporterPackInterface() {}

    /*! \brief The destructor
     */
	virtual ~ImExporterPackInterface() {}	
};

#endif /*IMEXPORTERPACKINTERFACE_HH_*/
