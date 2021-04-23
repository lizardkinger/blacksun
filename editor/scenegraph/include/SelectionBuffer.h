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
 *  File:       SelectionBuffer.h
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 * 
 **************************************************************************/
 
#ifndef SELECTIONBUFFER_H_
#define SELECTIONBUFFER_H_

/** \file SelectionBuffer.h
 * \brief Defines the SelectionBuffer class.
 * 
 * \author Thomas Kuhndörfer.
 */

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./SceneObject.h"
#include "./Polygon.h"
#include "./Mesh.h"
#include "./SelectedVertex.h"
#include "./SceneVertex.h"
#include "./SceneAction2D.h"

///////////////////////////////////////////////////////////
// Blacksun-Includes for Qt
///////////////////////////////////////////////////////////
#include <QtCore>

namespace BSScene
{

// Forward declaration
class Polygon;


/*! \brief The SelectionBuffer
*
* A concrete class that manages selected scene objects
*/
class SelectionBuffer : public QObject
{
    
Q_OBJECT
    
public:

	/*!
	* \brief Constructor
	* 
	*/		
	SelectionBuffer();
	
	/*!
	* \brief Deconstructor
	* 
	*/			
	virtual ~SelectionBuffer();
	
	
	/*!
	* \brief Resets the state of the SelectionBuffer to initialized. All selected scene information are getting lost 
	* 
	*/	  	
  	void reset();

	/*!
	* \brief Deletes all scene objects that are currently selected
	* 
	*/	 
  	void deleteSelectedObjects();
  	
  	
	/*!
	* \brief Copys all scene objects that are currently selected
	* 
	*/	   	
  	void copySelectedObjects();


	/*!
	* \brief Calls the blacksun renderer to render the scene
	* 
	* \param r Reference to the blacksun renderer
	*/	     
  	void render(Renderer& r);
  	
	/*!
	* \brief Calls the blacksun renderer to render the bounded vertices
	* 
	* \param r Reference to the blacksun renderer
	*/	       	
  	void renderVertices(Renderer& r);
  	
	/*!
	* \brief Moves all scene objects that are currently selected
	* 
	* \param v Translation vector for the scene object* 
	*/	   
  	void move(const Vector& v);

	/*!
	* \brief Add a scene objects to the current selection
	* 
	* \param so The sceneobject that will be added to the selection
	* \param selType Additional selection information	 
	*/	   
  	void addSceneObject( SceneObject* so,const SceneAction2D_SelectionType selType);
  	
  	/*!
	* \brief Add a single scene vertex to the current selection
	* 
	* \param v The scene vertex that will be added to the selection
	*/	  
  	//void addSceneVertex(const SceneVertex* v);
  	
  	/*!
	* \brief Add a single scene vertex to the current selection
	* 
	* \param v The scene vertex that will be added to the selection
	* \param p The polygon of the vertex
	*/	  
  	void addSingleVertex( Vertex* v,  Polygon* p);
  	
  	
	/*!
	* \brief Deselects a scene objects from the current selection
	* 
	* \param so The sceneobject that will be removed to the selection
	*/	   
  	void deselectSceneObject(SceneObject* so);
  	  	
  	  	
	/*!
	* \brief Get the list of selected objects
	* 
	* \return The reference to the selected object list
	*/	  
  	list<SceneObject*>& getSelectedObjects();
  	
  	
	/*!
	* \brief Get the list of selected bounded vertices
	* 
	* \return The reference to the selected vertices list
	*/	   	
  	list<SelectedVertex>& getSelectedVertices();


    /*!
	* \brief Provides support for scaling in a 2D viewport
	* 
	* \param percentageX Factor for the X scaling (1.0 = no change)
	* \param percentageY Factor for the Y scaling (1.0 = no change)
	* \param percentageZ Factor for the Z scaling (1.0 = no change)
	*/     
	void scale(const double percentageX,const double percentageY,const double percentageZ);
	
	
	/*!
	* \brief Merges polygons to a single mesh
	*/
  	void mergePolysToMesh();
  
  	
	/*!
	* \brief Breaking down a mesh to the underlaying polygons
	*/  	
  	void fragmentMeshs();
  	
  	
	/*!
	* \brief Merges meshes to a single mesh
	*/  	  	
  	void mergeMeshes();
  	
  	
  	/*!
	* \brief Turns the indices order of the selected objects
	* 
	*/	  	
  	void changeBias();
  	
	/*!
	* \brief Rotates the object depending on a amount around a specific center
	* 
	* \param axis The rotation axis
	* \param a The amount of rotation in degrees
	*/	  	
  	void    rotate(const SceneAxis axis, const float a);
  	
  	
	/*!
	* \brief Mirrors the selected objects
	* 
	* \param axis The mirror axis
	*/	  	  	
  	void mirror(const SceneAxis axis);

	/*!
	* \brief Mirrors the selected objects by world coordinates
	* 
	* \param axis The mirror axis
	*/	  	  	
  	void mirrorWorld(const SceneAxis axis);

	/*!
	* \brief All selected vertices are getting the same position
	* 
	*/	  	  
	void arrangeTogether();
	
	
	/*!
	* \brief Aligns all selected vertices along a specified axis
	* 
	* \param axis The alignment axis
	*/	  	  
	void alignAxis(const SceneAxis axis);


  	/*!
	* \brief Calculates indices for this polygon
	* 
	* \param single True if the single mode should be used
	*/	
	void triangulateVerts(const bool single);


    /*!
	* \brief Set the material ID of this scene object
	* 
	* \param nID ID of the new material
	*/	
  	void setMaterial(const int nID);
  	
  	
    /*!
	* \brief Returns the bounding box of the selection objects
	* 
	* \return Assigned axis aligned bounding box
	*/	   	
  	Aabb getBoundary() 
  	{ 
  		return m_boundary; 
  	}

  	
  	
    /*!
	* \brief Recalculates the bounding box of the selection objects
	*/
  	void adjustSelectionBoundaries();


  	/*!
	* \brief Translate the texture coordinates
	* 
	* \param dtU Changes the U value of the offset coordinates
	* \param dtV Changes the V value of the offset coordinates
	* \param drU Changes the U value of the repeat coordinates
	* \param drV Changes the V value of the repeat coordinates  
	*/	  	
  	void transTextureCoords(double dtU, double dtV, double drU, double drV);
  	
  	
  	/*!
	* \brief Get the texture coordinates
	* 
	* \param pdtU The U value of the offset coordinates
	* \param pdtV The V value of the offset coordinates
	* \param pdrU The U value of the repeat coordinates
	* \param pdrV The V value of the repeat coordinates  
	*/	  	
  	bool getTextureTrans(double *pdtU, double *pdtV,double *pdrU, double *pdrV);
  	
    
signals:
    void meshAdded(QString);
    void meshRemoved(QString);
    
    void selectionChanged();
    
private:

	/*!
	* \brief Initializes the selectionbuffer
	*/		
  	void init();
  	
  	
	/*!
	* \brief Releases the selectionbuffer
	*/		  	
  	void release();
  	
  	
  	/*!
	* \brief Changes the current bounding box of selected objects
	* 
	* \param newBox The bounding box whose extremal points will be used
	*/	  	
  	void addBoundingBox(const Aabb &newBox);
  	
  	
  	/*!
	* \brief Triangulate all selected vertices to polygons in single mode
	*/	
  	void triangulateSingle();
  	
  	/*!
	* \brief Triangulate all selected vertices to polygons in continuous mode
	*/	  	
  	void triangulateContinuous();

private:
  	list<SelectedVertex>	m_vertices;  /*!< \brief List of selected bounded vertices */
  	list<SceneObject*> 		m_objects;  	/*!< \brief List of selected objects */
  	Aabb					m_boundary;/*!< \brief The current bounding box of this object  */
  	double       			m_fD;         /*!< \brief The bounding box oversizing delta */    
                                          

};

}

#endif /*SELECTIONBUFFER_H_*/
