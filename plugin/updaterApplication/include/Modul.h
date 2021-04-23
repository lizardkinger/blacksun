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
 *  File:       Modul.h
 *  Created:    14.04.2007
 *  Author:     Stefan Zeltner
 *
 **************************************************************************/

#ifndef MODUL_H_
#define MODUL_H_

#include <BSBaseTypes.h>

/*! \file
* 
*  \brief Describes the class Modul, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

/*! \brief The class Modul
*
* The Modul is a class which represents a core - component.
*/
class Modul
{
public:
	/*! \brief The constructor
	 *
	 * \param version The version
	 * \param name The name
	 * \param url The path
	 *
	 */	
	Modul(BSPlgMgr::Version version, QString name, QString url)
		: version(version), name(name), url(url)
	{
		type = 0;
	}
	
	/*! \brief The destructor
	 */
	virtual ~Modul() {}
	
	/*! \brief Comparing the version of two moduls
	 *
	 * \param other The other Modul
	 *
	 * \return Return True, if other is newer than this 
	 */	
	bool newVersion(const Modul& other)
	{
		return version < other.version;
	}
	
	/*! \brief Return the type of the Modul
	 *
	 * \return The type
	 */	
	int getTyp()
	{
		return type;
	}
	
	/*! \brief Checking the equality of two moduls
	 *
	 * \param other The other Modul
	 *
	 * \return The equality
	 */	
	virtual bool equal(Modul* other)
    {
    	return(type == other->type
    	    && name == other->name);
    }
	
	const BSPlgMgr::Version version;	/*!< \brief The version from the modul*/
	const QString name;					/*!< \brief The name from the modul*/
	const QString url;					/*!< \brief The location from the modul*/

protected:
	/*!< \brief The type
	 *  To identify the type of the modul.
	 *  The type of the base-class Modul is 0. 
	 * */
	int type;
};

#endif /*MODUL_H_*/
