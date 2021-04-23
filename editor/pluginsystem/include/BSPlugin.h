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
 *  File:       PlgMgr.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef BSPLUGIN_H_
#define BSPLUGIN_H_

#include "PluginInterface.hh"
#include "DynCompLoader.h"
#include <QString>
#include <QByteArray>

/*! \file
* 
*  \brief Describes the class BSPlugin, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

namespace BSPlgMgr
{

/*! \brief The class BSPlugin
*
* This class represent the plugin
*/
class BSPlugin
{
private:
	static const char* FUNC_NAME;	/*!< \brief The name of the plugin access function*/
	QByteArray m_fileName;			/*!< \brief The path from the plugin*/
	bool m_state;					/*!< \brief The name of the plugin access function*/
	DCL_INSTANCE m_handle;			/*!< \brief The handle from the shared library*/
	PlgInt* m_pPlugin;				/*!< \brief The plugin object*/
	UID m_UID;						/*!< \brief The plugin UID*/
	
	/*! \brief The copy constructor
	 * 
	 *  Protect the class against copying
	 * 
     * \param The other BSPlugin
     */
	BSPlugin(const BSPlugin&) {}
	
	/*! \brief The assignment operator
	 * 
	 *  Protect the class against copying
	 * 
     * \param The other BSPlugin
     * \return A copy from this
     */
	BSPlugin& operator= (const BSPlugin&) {return *this;}
	
public:
	
	/*! \brief The constructor
	 * 
     * \param fileName The path to the plugin
     * \param state The new state
     */
	BSPlugin(QByteArray fileName, bool state = false);
	
	/*!
	 * \brief The destructor
	 */	
	virtual ~BSPlugin();
	
	/*! \brief Return the path to the plugin
	 * 
     * \return The path to the plugin
     */
	QByteArray getFileName() {return m_fileName;}
	
	/*! \brief Return the UID
	 * 
     * \return Returns the UID from the plugin
     */
	UID getUID() {return m_UID;}
	PlgInt* getInstance() {return m_pPlugin;}
	
	/*! \brief Set the plugin to a new state
	 * 
     * \param state The new state
     */
	void setState(bool state) {m_state = state;}
	
	/*! \brief Return the state from the plugin
	 * 
     * \return The state from the plugin
     */
	bool getState() {return m_state;}
};

}

#endif /*BSPLUGIN_H_*/
