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
 *  Module:     Renderer (BlackSun)
 *  File:       CacheObject.cpp
 *  Created:    04.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/RenderCache.h"

namespace BSRenderer
{
	
CacheObject::CacheObject(PolygonMode polymode, PrimitiveType type, int nMaterialID, 
	const Matrix& mat, bool bSwapFaceOrder)
: m_polymode(polymode), m_type(type), m_nMatID(nMaterialID), m_mat(mat),
  m_bSwapFaceOrder(bSwapFaceOrder)
{
	//Reserve memory for vertices
	m_verts.reserve(min(1000, RenderCache::getInstance()->getMaxVertsPerCache()));	
}

void CacheObject::addVertex(const Vertex& v)
{
	m_verts.push_back(v);	
}
	
}
