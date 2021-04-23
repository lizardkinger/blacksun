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
 *  File:       DynCompLoader.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef DYNCOMPLOADER_H_
#define DYNCOMPLOADER_H_

#ifdef WIN32
#include <windows.h>
#define DCL_OPEN_LIB(a) LoadLibraryA(a)
#define DCL_GET_OBJECT GetProcAddress
#define DCL_CLOSE_LIB FreeLibrary
#define DCL_INSTANCE HINSTANCE
#endif

#ifndef WIN32 // for Linux
#include <dlfcn.h>
#define DCL_OPEN_LIB(a) dlopen(a, RTLD_LAZY)
#define DCL_GET_OBJECT dlsym
#define DCL_CLOSE_LIB dlclose
#define DCL_INSTANCE void*
#endif

/*! \file
* 
*  \brief A file to access a shared library
*  \author Stefan Zeltner
* 
*/

/*! \brief A collection of functions for loading shared librarys
*
*/
namespace DynCompLoader
{
	
/*! \brief Open a shared library
 * 
 * \param file The path to the file
 * \return The library handle
 */
inline DCL_INSTANCE openLib(const char* file)
{
	return DCL_OPEN_LIB(file);
}
	
/*! \brief Get a instance from ComponentT from the library
 * 
 * \param handle The library handle
 * \param funcName The name from the function which create the instance
 * \return The created instance
 */
template <class ComponentT>
ComponentT createInstance(DCL_INSTANCE handle, const char* funcName)
{
	ComponentT instance = NULL;
	
	if(handle)
	{
		
		typedef ComponentT (*createfp)();
		createfp ci = (createfp)(DCL_GET_OBJECT(handle, funcName));
		
		if(ci)
		{
			instance = ci();
		}
		else
			return NULL;
	}
	else
		return NULL;
	
	return instance;
}
	
/*! \brief Close a shared library
 * 
 * \param handle The library handle
 */
inline void closeLib(DCL_INSTANCE handle)
{
	DCL_CLOSE_LIB(handle);
}

}
#endif /*DYNCOMPLOADER_H_*/
