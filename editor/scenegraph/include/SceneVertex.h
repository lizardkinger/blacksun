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
 *  File:       SceneVertex.h
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef SCENEVERTEX_H
#define SCENEVERTEX_H 

/** \file SceneVertex.h
 * \brief Defines the scenevertex class.
 * 
 * \author Thomas Kuhndörfer.
 */
 
///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./SceneObject.h"
#include "./SceneAction2D.h"


namespace BSScene   
{ 

// Forward declaration
class SelectionBuffer;



/*! \brief The SceneVertex object 
*
* A concrete class that represents vertices
*/
class SceneVertex : public SceneObject
{
public:

	/*!
	* \brief Constructor
	* 
	*/		
	SceneVertex();
	
	
	/*!
	* \brief Copy constructor
	* 
	* \param copy Another SceneVertex that will be copied
	*/			
	SceneVertex(const SceneVertex &copy);
	
	/*!
	* \brief Assignment constructor
	* 
	* \param copy Another SceneVertex that will be copied
	*/			
	SceneVertex&	operator=(const SceneVertex& copy);
	
	
	/*!
	* \brief Deonstructor
	* 
	*/		
	virtual ~SceneVertex();
	
	
	/*!
	* \brief Saves a sceneobject into a file
	* 
	* \param f The stream the data will be written to
	*/		
	void    save(QDataStream &f);
	
	
	/*!
	* \brief Loads a sceneobject from a file
	* 
	* \param f The stream the data will read from
	*/				
  	void    load(QDataStream &f);
  	
  	
	/*!
	* \brief Calls the Renderer to draw this object
	* 
	* \param r The rendering device
	*/	  	  	
  	void    render(Renderer& r);
	
	/*!
	* \brief Set the underlaying vertex of this object and calculates the AABB
	* 
	* \param v The new vertex for this SceneVertex
	*/	  	  	
  	void    setVertex(const Vertex &v) 
  	{ 
  		m_vertex = v; 	
  		calcBoundingBox(); 
  	}
  	 
	/*!
	* \brief Get the underlaying vertex of this object
	* 
	* \return The current vertex of this SceneVertex
	*/	  	 
  	Vertex  getVertex() 
  	{ 
  		return m_vertex;
  	}

	/*!
	* \brief Moves the object depending on a vector
	* 
	* \param v Translation vector for the scene object
	*/	
  	void 	move(const Vector &v); 


	/*!
	* \brief Rotates the object depending on a amount around a specific center
	* 
	* \param axis The rotation axis
	* \param center The center point for the rotation
	* \param amount The amount of rotation in degrees
	*/	  	
  	void    rotate(const SceneAxis axis,const Vector& center, const float amount);
  	
  	
	/*!
	* \brief Mirror the object
	* 
	* \param axis The mirror axis
	* \param center The center point for the mirroring
	*/	  	
  	void    mirror(const SceneAxis axis,const  Vector& center);
  	
	/*!
	* \brief Mirror the object  by world axis
	* 
	* \param axis The mirror axis
	*/	  	
  	void    mirrorWorld(const SceneAxis axis);  	
  	
	/*!
	* \brief Scales an object freely on each direction 
	* 
	*/
  	void    scale(const double ,const double ,const double ) 
  	{}


    /*!
	* \brief Hit test for selection in 2D view
	* 
	* \param boundary	Box that was generated by an selection area
	* \param axis	Axis of the view that will be the infinite component of the intersection test
	*/	
  	bool    intersects(const Aabb& boundary, const SceneAxis axis) ; 
  	

    /*!
	* \brief Selects intersecting vertices
	* 
	* \param aabb	Box that was generated by an selection area
	* \param sb		Reference to the SelectionBuffer to add selected items
	* \param axis	Axis of the view that will be the infinite component of the intersection test
	* \param selType Additional selection information
	*/	    
    void    getIntersectingVertices(const Aabb& aabb,SelectionBuffer& sb,const SceneAxis axis, const SceneAction2D_SelectionType selType);
    
	/*!
	* \brief Compares two scene objects
	* 
	* \param so The SceneObject for comparison
	* 
	* \return Return true if it's the same objects
	*/	
  	bool operator==(const SceneObject &so);
 
private:
	/*!
	* \brief Initializes a scenevertex 
	*/	
 	void 	init();
 	
 	
	/*!
	* \brief Initializes a scenevertex by another scenevertex
	* 
	* \param copy The scenevertex that will be copied
	*/	 	
	void 	init(const SceneVertex& copy);
	
	
	/*!
	* \brief Releases a scenevertex
	*/	 	
	void 	release();
	
	
    /*!
	* \brief Calculates the bounding box of this scenevertex
	* 
	*/	 	
	void    calcBoundingBox();
	
private:
	Vertex m_vertex; /*!< \brief The encapsulated vertex */
  
};

}

#endif /*SINGLEVERTEX_H_*/
