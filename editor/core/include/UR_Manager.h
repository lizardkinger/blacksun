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
 *  Module:     Undo/Redo - Manager (BlackSun)
 *  File:       UR_Manager.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef UR_MANAGER_H_
#define UR_MANAGER_H_

#include <QObject>
#include <QUndoStack>

///////////////////////////////////////////////////////////
// for the Versions - Info
///////////////////////////////////////////////////////////
#include "./../../pluginsystem/include/BSBaseTypes.h"


/*! \file
* 
*  \brief Describes the class UR_Manager, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

/*! \brief The core component
*
*/
namespace BSCore
{

/*! \brief The class UR_Manager
*
* This class manage the undo/redo system.
* 
* They store all actions which change the scene and can revert it.
*/
class UR_Manager : public QUndoStack
{
	Q_OBJECT

private:
	static UR_Manager* m_instance;	/*!< \brief The instance of the singleton*/
	
	/*! \brief The constructor
	 */	
	UR_Manager();
	
	/*! \brief The destructor
	 */
	virtual ~UR_Manager();

	/*! \brief The guard for the singleton 
	 *
	 * The guard for the singleton arrange that the destructor
	 * of the singleton will be called
	 */	
	class Guardian
	{
		public:
			/*!
			 * \brief The destructor
			 */
			virtual ~Guardian()
			{
				if(UR_Manager::m_instance != NULL)
				{
					delete UR_Manager::m_instance;
				}
			}
	};
	friend class Guardian;


public:
	/*!
	 * \brief The Version
	 */
	static BSPlgMgr::Version version;
	
	/*!
	 * \brief Returns the instance of the undo/redo manager to give access to it
	 *
	 * \return The instance of the undo/redo manager
	 */
	static UR_Manager* getInstance();

public slots:
	
	/*!
	 * \brief Redo a earlier executed undo
	 */
	void redo();
	
	/*!
	 * \brief Undo the last action from the undo stack
	 */
	void undo();
};
}
#endif /*UR_MANAGER_H_*/
