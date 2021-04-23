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
 *  File:       SelectedVertex.h
 *  Created:    03.04.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
#ifndef SELECTEDVERTEX_H_
#define SELECTEDVERTEX_H_

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./../../renderer/include/RendererTypes.h"

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./Polygon.h"


namespace BSScene
{

class SelectedVertex
{
	public:
		SelectedVertex(Vertex *vert, Polygon *poly):vert(vert),poly(poly) 
		{}
		
		virtual ~SelectedVertex()  
		{}
		
		Vertex* getVertex() 
		{ 
			return vert; 
		} 
		
		Polygon* getPolygon() 
		{ 
			return poly; 
		} 
		
		void set(Vertex *v, Polygon *p) 
		{ 
			vert = v; 
			poly = p; 
		}
	
	private:
		Vertex* vert;
		Polygon* poly;	
};

}

#endif /*SELECTEDVERTEX_H_*/
