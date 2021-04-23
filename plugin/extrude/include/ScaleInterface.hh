/***************************************************************************
 *   Copyright (C) 2006-07 by Philipp Gruber
 *   pgruber@fh-landshut.de
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
 *  File:       ExtrudeInterface.hh
 *  Created:    24.04.2007
 *  Author:     Thomas Kuhndoerfer (lizardkingx)
 *
 **************************************************************************/

#ifndef SCALEINTERFACE_HH_
#define SCALEINTERFACE_HH_

#include <PluginInterface.hh>
#include <QtGui>

class ScaleInterface : public PlgInt, public QObject
{
public:
	static const UID uid = 9647832;

	ScaleInterface() {}
	virtual ~ScaleInterface() {}
};

#endif /*EXTRUDEINTERFACE_HH_*/
