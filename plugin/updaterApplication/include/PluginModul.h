/***************************************************************************
 *   Copyright (C) 2006-07 by Stefan Zeltner
 *   szeltne@fh-landshut.de
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
 *  File:       PluginModul.h
 *  Created:    14.04.2007
 *  Author:     Stefan Zeltner
 *
 **************************************************************************/

#ifndef PLUGINMODUL_H_
#define PLUGINMODUL_H_

#include "Modul.h"

/*! \file
* 
*  \brief Describes the class PluginModul, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

/*! \brief The class PluginModul
*
* The PluginModul is a class which represents a plugin - component.
*/
class PluginModul : public Modul
{
public:
	/*! \brief The constructor
	 *
	 * \param version The version
	 * \param name The name
	 * \param url The path
	 * \param uid The UID
	 *
	 */	
	PluginModul(BSPlgMgr::Version version, QString name, QString url, BSPlgMgr::UID uid)
		: Modul(version, name, url), uid(uid)
	{
		type = 1;
	}
	
	/*! \brief The destructor
	 */
	virtual ~PluginModul() {}
	
	/*! \brief Checking the equality of two moduls
	 *
	 * \param other The other Modul
	 *
	 * \return The equality
	 */	
	bool equal(Modul* other)
    {
    	return(type == other->getTyp())?
    	    uid == (dynamic_cast<PluginModul*> (other))->uid
    	  : false;
    	    
    }
	
	const BSPlgMgr::UID uid;	/*!< \brief The UID from the plugin-modul*/
	
};

#endif /*PLUGINMODUL_H_*/
