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
 *  File:       CacheObject.h
 *  Created:    04.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef CACHEOBJECT_H_
#define CACHEOBJECT_H_

#include "RenderCache.h"
#include "RendererTypes.h"

/*! \file
* 
*  \brief Describes the class CacheObject, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief A cache object 
*
* A cache object, that stores all data needed to render a speciefied number of vertices
* all at once
*/
class CacheObject
{
public:
	/*!
	* \brief The constructor
	* 
	* Initializes the cache object with its properties
	* 
	* \param polymode How the object should be rendered
	* \param type The type of the primitive(s)
	* \param nMaterialID The material-ID
	* \param mat The matrix of the object
	* \param bSwapFaceOrder Swap the face order?
	*/
	CacheObject(PolygonMode polymode, PrimitiveType type, int nMaterialID, const Matrix& mat,
		bool bSwapFaceOrder);
	
	/*!
	* \brief Adds a single vertex to the cache object
	* 
	* \param v The vertex that should be added
	*/	
	void addVertex(const Vertex& v);
	
	/*!
	* \brief Returns a reference to the vertex array
	* 
	* \return vector<Vertex>& A reference to the vertex array
	*/
	vector<Vertex>& getVerts()
	{
		return m_verts;
	}
	
	/*!
	* \brief Returns the number of vertices
	* 
	* \return int The number of vertices
	*/	
	int getNumVerts() const
	{
		return m_verts.size();
	}
	
	/*!
	* \brief Returns how the object should be rendered
	* 
	* \return PolygonMode How the object should be rendered
	*/	
	PolygonMode getPolygonMode() const
	{
		return m_polymode;
	}	
	
	/*!
	* \brief Returns the type of the primtive(s)
	* 
	* \return PrimtiveType The type of the primtive(s)
	*/	
	PrimitiveType getPrimitiveType() const
	{
		return m_type;
	}
	
	/*!
	* \brief Returns the materal-ID
	* 
	* \return int The material-ID
	*/	
	int getMaterial() const
	{
		return m_nMatID;
	}
	
	/*!
	* \brief Returns a reference to the matrix
	* 
	* \return Matrix& The reference to the matrix
	*/	
	Matrix& getMatrix()
	{
		return m_mat;
	}	

	/*!
	* \brief Returns if the face ordner must be swapped
	* 
	* \return bool True, if the face ordner must be swapped, otherwise false
	*/		
	bool getSwapFaceOrder() const
	{
		return m_bSwapFaceOrder;
	}
	
private:
	PolygonMode m_polymode; /*!< \brief How the object should be rendered */
	PrimitiveType m_type; /*!< \brief Type of the primitve */
	int m_nMatID; /*!< \brief The material-ID */
	Matrix m_mat; /*!< \brief The matrix */
	vector<Vertex> m_verts; /*!< \brief The vertex-array */
	bool m_bSwapFaceOrder; /*!< \brief Swap the face order? */
};
	
}

#endif /*CACHEOBJECT_H_*/
