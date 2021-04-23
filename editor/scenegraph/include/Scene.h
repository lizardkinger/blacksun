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
 *  File:       Scene.h
 *  Created:    06.12.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
 
#ifndef SCENE_H_
#define SCENE_H_

/** \file Scene.h
 * \brief Defines the scene class.
 * 
 * \author Thomas Kuhndörfer.
 */


///////////////////////////////////////////////////////////
// Blacksun-Includes for STL
///////////////////////////////////////////////////////////
#include <list> 

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./SceneObject.h"
#include "./Polygon.h"
#include "./Mesh.h"
#include "./SceneAction2D.h"

namespace BSScene
{

///////////////////////////////////////////////////////////
// Forward declarations
///////////////////////////////////////////////////////////	
class SelectionBuffer;


/*! \brief The scene object 
*
* A concrete class that holding polygons and meshes 
*/
class Scene
{
public:

	/*!
	* \brief Constructor
	* 
	*/		
	Scene();
	
	/*!
	* \brief Deconstructor
	* 
	*/			
	virtual ~Scene();
	
	
	/*!
	* \brief Resets the state of the scene to initialized. All scene objects are getting lost 
	* 
	*/	  	
	void reset();
	
	
	/*!
	* \brief Saves a scene into a file
	* 
	* \param out The stream the data will be written to
	*/	
	void save(QDataStream &out);
	
	
	/*!
	* \brief Loads a scene from a file
	* 
	* \param in The stream the data will read from
	*/		
  	void    load(QDataStream &in);


	/*!
	* \brief Calls the Renderer to draw this object
	* 
	* \param r The rendering device
	*/	  	
	void    render(Renderer& r);


	/*!
	* \brief Adds an scene object to the scene
	* 
	* \param sObject The new scene object
	* \param newName True if an automatic name should be generated
	*/	  		    
	SceneObject*  addSceneObject(SceneObject& sObject, const bool newName = true); 
    
    
	/*!
	* \brief Removes a specific scene object
	* 
	* \param sObject Pointer to the scene object that will be removed
	*/	    
    void 	removeObject(SceneObject *sObject);
    
	/*!
	* \brief Removes all single vertices
	* 
	* \param sb Reference to the SelectionBuffer to remove items
	*/	        
    void 	removeAllVertices( SelectionBuffer &sb);

  	/*!
	* \brief Selects all scene objects of the current selection mode
	* 
	* \param ssm Determines the current selection mode
	* \param sb The SelectionBuffer that gets informed about the selections
	*/	        
    void 	selectAll(const SceneSelectMode ssm, SelectionBuffer& sb);
    
  	/*!
	* \brief Invert selects all scene objects of the current selection mode
	* 
	* \param ssm Determines the current selection mode
	* \param sb The SelectionBuffer that gets informed about the selections
	*/	       
    void 	selectInvert(const SceneSelectMode ssm, SelectionBuffer& sb);
    
  	/*!
	* \brief Selects all intersecting scene objects of the current selection mode
	* 
	* \param ssm Determines the current selection mode
	* \param sb The SelectionBuffer that gets informed about the selections
	* \param aabb The AABB for bounding tests
	* \param axis	Axis of the view that will be the infinite component of the intersection test
	* \param selType Additional selection information	 
	*/	       
    void 	selectIntersecting(const SceneSelectMode ssm, SelectionBuffer& sb,const Aabb& aabb,const SceneAxis axis, const SceneAction2D_SelectionType selType);
    
  	/*!
	* \brief Selects a mesh by a certain name
	* 
	* \param sb The SelectionBuffer that gets informed about the selections
	* \param name Name of the mesh that should be selected
	*/	           
	void    selectMeshByName(SelectionBuffer& sb,const QString &name);

  	/*!
	* \brief Renames a single mesh in the scene
	* 
	* \param oldName The mesh identifier for the mesh that becomes renamed
	* \param newName The new name of the mesh
	*/	       
	void    renameMesh(const QString oldName,const QString newName);

    /*!
	* \brief Calculates the Normals of all scene objects
	* 
	*/	
    void 	recalcVertexNormals();

    /*!
	* \brief Sets the status of all scene objects to unhidden
	* 
	*/	        
    void 	hideNone();
    
    /*!
	* \brief Sets the status to hidden
	* 
	* \param ssm Determines the current selection mode
	*/	            
    void 	hideSelected(const SceneSelectMode ssm);
    
    /*!
	* \brief Sets the status of all scene objects to hidden
	* 
	*/	            
    void 	hideAll();
    
    /*!
	* \brief Invert the hidden status of all scene objects
	* 
	*/	            
    void 	hideInvert();
    
  	/*!
	* \brief Sets the status of all scene objects to unfreezed
	* 
	*/	       
    void 	freezeNone();
    
    /*!
	* \brief Sets the status to freezed
	* 
	* \param ssm Determines the current selection mode
	*/	             
    void 	freezeSelected(const SceneSelectMode ssm);
    
  	/*!
	* \brief Sets the status of all scene objects to freezed
	* 
	*/	       
    void 	freezeAll();
    
  	/*!
	* \brief Invert the freezed status of all scene objects
	* 
	*/	       
    void 	freezeInvert();

    /*!
	* \brief Get the polygon count
	* 
	* \return The polygon count
	*/	 
	long 	countPolygons() const;
	
    /*!
	* \brief Get the bound vertices count
	* 
	* \param real True: Only real vertices will be counted 
	* 
	* \return The vertices count
	*/	 	
	long 	countBoundVertices(const bool real) const;

    /*!
	* \brief Get the meshes count
	* 
	* \return The mesh count
	*/	 
	long 	countMeshes() const;
	
	     
	/*!
	* \brief Get the single vertices count
	* 
	* \return The single vertices count
	*/	 
	long 	countSingleVertices() const;
	
	
	/*!
	* \brief Get the names list of all scene meshes
	* 
	* \return The mesh names list
	*/	 	
	vector<QString> getMeshNames();
	
	/*!
	* \brief Get the list of all scene meshes
	* 
	* \return Reference to the mesh list
	*/		
	list<Mesh>*	getSceneMeshes();
	
	/*!
	* \brief Get the list of all scene polygons
	* 
	* \return Reference to the polygon list
	*/			
	list<Polygon>* getScenePolys();
	
    /*!
	* \brief Calculates the bounding box of a specific mesh
	* 
	* \param name Name of the specific mesh 
	*/	
	void calcBoundingBoxByName(const QString name);
	
	/*!
	* \brief Get the AABB of the whole scene
	* 
	* \param box AABB with the scene AABB
	*/	
	void getSceneAabb(Aabb& box);
    
private:

	/*!
	* \brief Initializes a scene
	*/	
	void 	init();
	
	/*!
	* \brief Releases a scene
	*/	
	void 	release();
	
    /*!
	* \brief Generates a list of all scene objects for easy iterating
	* 
	* \param ssm Determines the current selection mode
	*/	   	
	void	buildTemporaryPointerList(const SceneSelectMode ssm);

private:
	list<Mesh>			m_sceneMeshes; /*!< \brief Array of all meshes of this scene */
	list<Polygon>		m_scenePolys;  /*!< \brief Array of all polygons of this scene */
	list<SceneVertex>	m_sceneSingles; /*!< \brief Array of all single vertices of this scene */
	
	list<SceneObject*>	m_sceneAllObjects; /*!< \brief Temporary array of all scene objects of this scene */
};

}

#endif /*SCENE_H_*/
