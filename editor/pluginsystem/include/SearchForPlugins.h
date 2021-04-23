/***************************************************************************
 *   Copyright (C) 2007 by Stefan Zeltner (Xexis)
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
 *  Module:     Pluginsystem (BlackSun)
 *  File:       SearchForPlugins.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef SEARCHFORPLUGINS_H_
#define SEARCHFORPLUGINS_H_

#include "../include/PlgMgr.h"


/*! \file
* 
*  \brief Describes the class SearchForPlugins, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

namespace BSPlgMgr
{

/*! \brief The class SearchForPlugins
*
* This class search in their constructor for all available plugins in a defined folder
*/
class SearchForPlugins : public QObject
{
    Q_OBJECT
    
public: 
	/*! \brief The default constructor
	 * 
	 * The constructor search for every available plugin in the with is defined in the
	 * settings under PlgMgr/SearchPath. 
	 * 
	 */
	SearchForPlugins();

	/*! \brief The destructor
	 */
	virtual ~SearchForPlugins();

private:

	UID m_lastInstalledPlugin; /*!< \brief The UID from the last installed plugin*/

public slots:
	
	/*! \brief Get the UID from the last installed plugin from the plugin manager
	 * 
     * \param uid The last installed plugin
     */
	void getInstalledUid(UID uid);

};

}

#endif /*SEARCHFORPLUGINS_H_*/
