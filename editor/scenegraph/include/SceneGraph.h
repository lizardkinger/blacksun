/***************************************************************************
 *   Copyright (C) 2006 by Thomas Kuhndörfer
 *   tkuhndo@fh-landshut.de
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
 *  Module:     Scenegraph (BlackSun)
 *  File:       SceneGraph.h
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

/** \file SceneGraph.h
 * \brief This is the a generic class for including the scenegraph.
 * 
 * \author Thomas Kuhndörfer.
 */

#include <QtCore>

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./SceneObject.h"
#include "./Polygon.h"
#include "./Mesh.h"
#include "./Scene.h"
#include "./SceneManager.h"
#include "./SelectionBuffer.h"

using namespace BSScene;  

#endif /*SCENEGRAPH_H_*/
