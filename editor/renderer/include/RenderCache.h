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
 *  Module:     RenderCache (BlackSun)
 *  File:       RenderCache.h
 *  Created:    04.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#ifndef RENDERCACHE_H_
#define RENDERCACHE_H_

#include "Renderer.h"
#include "CacheObject.h"

/*! \file
* 
*  \brief Describes the class RenderCache, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief The render manager 
*
* The render manager collects all vertices that should be rendered. After sending all data
* to the manager, the scene is rendered clustered to improve the speed.
*/
class RenderCache
{
public:	
	/*!
	* \brief Returns the instance of the render cache to give access to it
	*
	* \return RenderCache* The instance of the render cache
	*/
	static RenderCache* getInstance();

	/*!
	* \brief Set the number of vertices for one cache object 
	*
	* \param nMaxVerts The number of vertices
	*/	
	void setMaxVertsPerCache(int nMaxVerts)
	{
		m_nMaxVertsPerCache = nMaxVerts;
	}

	/*!
	* \brief Returns the number of vertices for one cache object 
	*
	* \return int The number of vertices
	*/		
	int getMaxVertsPerCache() const
	{
		return m_nMaxVertsPerCache;
	}
	
	/*!
	* \brief Renders data 
	*
	* \param polymode How the polygons should be rendered
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param bSwapFaceOrder Swap the face order?
	*/	
	void render(PolygonMode polymode, PrimitiveType type, vector<Vertex>& verts, 
				int nMaterialID, const Matrix& mat, bool bSwapFaceOrder);

	/*!
	* \brief Renders indexed data 
	*
	* \param polymode How the polygons should be rendered
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param bSwapFaceOrder Swap the face order?
	*/	
	void render(PolygonMode polymode, PrimitiveType type, vector<Vertex>& verts, 
				vector<int>& indices, int nMaterialID, const Matrix& mat,
				bool bSwapFaceOrder);
	
	/*!
	* \brief Force to render all data now
	*
	* \return int Number of rendered vertices
	*/		
	int flush();
	
private:
	/*!
	* \brief The standard constructor
	*/
	RenderCache();
	
	/*!
	* \brief The destructor
	*/		
	virtual ~RenderCache()
	{
	}

	/*! \brief Render a cache object 
	*
	* \param pObject The cache object that should be rendered
	*/	
	void renderCacheObject(CacheObject* pObject);
	
	/*! \brief The guard for the render cache singleton 
	*
	* The guard for the render cache singleton arrange that the destructor
	* of the singleton will be called
	*/		
	class RenderCacheWaechter 
	{
	public:
		/*!
		* \brief The destructor
		*/
		virtual ~RenderCacheWaechter() 
		{
			if(RenderCache::m_instance != NULL)
				delete RenderCache::m_instance;
		}
	};
	friend class RenderCacheWaechter;

private:
	static RenderCache* m_instance; /*!< \brief The instance of the texture manager singleton*/
	
	int m_nMaxVertsPerCache; /*!< \brief The number of vertices that are collected in one cache object*/
	vector<CacheObject*> m_cacheObjects; /*!< \brief Array with all cach objects*/
};
	
}

#endif /*RENDERCACHE_H_*/
