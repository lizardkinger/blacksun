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
 *  File:       SceneObject.hh
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

/** \file SceneObject.h
 * \brief This is the base class for all drawable scene objects.
 * 
 * \author Thomas Kuhndörfer.
 */

///////////////////////////////////////////////////////////
// Includes for Qt
/////////////////////////////////////////////////////////// 
#include <QtGui>
#include <QFile>
#include <QDataStream>

///////////////////////////////////////////////////////////
// Blacksun-Includes for renderer-lib
///////////////////////////////////////////////////////////
#include "./../../renderer/include/Renderer.h"

///////////////////////////////////////////////////////////
// Blacksun-Includes for core-lib
///////////////////////////////////////////////////////////
#include "./../../core/include/Types.h"
#include "./../../core/include/Log.h"

///////////////////////////////////////////////////////////
// Blacksun-Includes for math-lib
///////////////////////////////////////////////////////////
#include "./../../math/include/Vector3.h"
#include "./../../math/include/Matrix4x4.h"
#include "./../../math/include/AABB.h"
#include "./../../math/include/Ray.h"
#include "./../../math/include/ColorRGB.h"
#include "./../../math/include/Intersection.h"


using namespace std;
using namespace BSMath;
using namespace BSRenderer;


/*! \brief The scene management component
*
* The scene management component of the editor. 
* It provides all classes to handle for inserting and manipulating scene objects.
* It also communicates directly with the GUI and the renderer component.
*/
namespace BSScene
{ 

///////////////////////////////////////////////////////////
// Enumerations
//////////////////////////////////////////////////////////

/*!
* \brief Defines the type of a scene object
*/	
enum SceneObjectType 	
{ 	
	SCENETYPE_All, 		/*!< Generic value */
	SCENETYPE_Polygon,  /*!< Identifies a polygon */
	SCENETYPE_Mesh,	 	/*!< Identifies a mesh */
	SCENETYPE_Vertex	/*!< Identifies a single vertex */
};

/*!
* \brief Decides what space axis is currently meant
*/	
enum SceneAxis 			
{ 	
	AXIS_x,	/*!<  Specifies the X axis */
	AXIS_y,	/*!<  Specifies the Y axis */ 	
	AXIS_z 	/*!<  Specifies the Z axis */
};

/*!
* \brief Decides what scene view is currently meant
*/	
enum SceneView 			
{
	SCENEVIEW_Front, 	/*!< Specifies the front view */
	SCENEVIEW_Back, 	/*!< Specifies the back view */
	SCENEVIEW_Left , 	/*!< Specifies the left view */
	SCENEVIEW_Right, 	/*!< Specifies the right view */
	SCENEVIEW_Bottom , 	/*!< Specifies the bottom view */
	SCENEVIEW_Top		/*!< Specifies the top view */
};

/*!
* \brief Decides what selection mode is currently chosen
*/	
enum SceneSelectMode	
{
	SELMODE_All, 		/*!< Generic specification */
	SELMODE_Polygon, 	/*!< Specifies polygons as selection mode */
	SELMODE_Mesh, 		/*!< Specifies meshes as selection mode  */
	SELMODE_Vertex 		/*!< Specifies vertices as selection mode  */
};

/*!
* \brief Decides what creation mode is currently chosen
*/	
enum SceneCreationMode	
{
	CREATEMODE_None, 	/*!< Nothing will be created */
	CREATEMODE_Polygon, /*!< Polygons will be created */	
	CREATEMODE_Vertex 	/*!< Vertices will be created */
};

/*!
* \brief Decides what action mode is currently chosen
*/	
enum SceneActionMode   
{
	SCENEACTION_None, 		/*!< No action mode is chosen */
	SCENEACTION_Selection, 	/*!< Selection of objects */
	SCENEACTION_Move, 		/*!< Moving of objects */
	SCENEACTION_Rotate, 	/*!< Rotating objects */	
	SCENEACTION_Scale, 		/*!< Scaling objects */
	SCENEACTION_Creation 	/*!< Creating objects by the user */
};

/*! \brief The scene object 
*
* An abstract interface for all scene objects that can be drawn.
* Every new scene class must derive from this class 
*/
class SceneObject
{
public:

	/*!
	* \brief Constructor
	*/	
	SceneObject() {};
	
	/*!
	* \brief Destructor
	*/	
	virtual ~SceneObject() {} ;
	
	/*!
	* \brief Initializes a sceneobject
	*/	
	virtual void init()=0;
	
	/*!
	* \brief Releases a sceneobject
	*/	
	virtual void release()=0;
	
	/*!
	* \brief Saves a sceneobject into a file
	* 
	* \param qd The stream the data will be written to
	*/	
	virtual void save(QDataStream &qd)=0;
	
	/*!
	* \brief Loads a sceneobject from a file
	* 
	* \param qd The stream the data will read from
	*/		
	virtual void load(QDataStream &qd)=0;
	
	/*!
	* \brief Calls the Renderer to draw this object
	* 
	* \param r The rendering device
	*/	
	virtual void render(Renderer &r)=0;
	
	/*!
	* \brief Moves the object depending on a vector
	* 
	* \param v Translation vector for the scene object
	*/	
	virtual void move(const Vector &v)=0; 
	
	/*!
	* \brief Rotates the object depending on a amount around a specific center
	* 
	* \param axis The rotation axis
	* \param center The center point for the rotation
	* \param amount The amount of rotation in degrees
	*/	
	virtual void rotate(const SceneAxis axis,const Vector& center, const float amount)=0;
	
	/*!
	* \brief Mirror the object
	* 
	* \param axis The mirror axis
	* \param center The center point for the mirroring
	*/	
	virtual void mirror(const SceneAxis axis,const Vector& center)=0;
	
	/*!
	* \brief Mirror the object by world axis
	* 
	* \param axis The mirror axis
	*/	
	virtual void mirrorWorld(const SceneAxis axis)=0;	
	
	/*!
	* \brief Scales an object freely on each direction 
	* 
	* \param percentageX The value of scaling the X components
	* \param percentageY The value of scaling the Y components
	* \param percentageZ The value of scaling the Z components
	*/	
	virtual void scale(const double percentageX,const double percentageY,const double percentageZ)=0;
    
    /*!
	* \brief Hit test for selection in 2D view
	* 
	* \param boundary	Box that was generated by an selection area
	* \param axis	Axis of the view that will be the infinite component of the intersection test
	*/	
    virtual bool intersects(const Aabb& boundary,const SceneAxis axis)   =0;

	/*!
	* \brief Resets the state of the scene object to initialized. All changes are getting lost 
	* 
	*/	    
    virtual void reset()
    {
    	release(); 
    	init(); 
    }

	/*!
	* \brief Tests if this scene object is of a particular type  
	* 
	* \param sco A instance of a SceneObjectType
	* 
	* \return True if this object is kind of the parameter type
	*/	   
	bool isKindOf(SceneObjectType sco) const
	{ 
		return (m_type==sco); 
	}
	
	/*!
	* \brief Return the type of this scene object
	* 
	* \return Type of the scene object
	*/	   
    SceneObjectType	getType() const      
    {  
    	return m_type;       
    }
    
    
    /*!
	* \brief Returns the bounding box for this scene object
	* 
	* \return Assigned axis aligned bounding box
	*/	 
    Aabb&	getAabb() 	
    { 
    	return m_boundary;       
    }

	/*!
	* \brief Marks this scene object als not selected
	*
	*/	
    virtual void    deSelect()
	{
		m_bSelected = false;    
	}
    
    /*!
	* \brief Marks this scene object als selected
	*
	*/	
    virtual void    select()
	{
		m_bSelected = true;    
	}
    
    /*!
	* \brief Returns the selection state of this scene object
	* 
	* \return The selection state
	*/	
    bool    isSelected()   const
    { 
    	return m_bSelected;  
    }

    /*!
	* \brief Marks this scene object als hidden
	*
	*/	
    void	hide()         
    { 
    	m_bHidden = true; 
    	m_bSelected = false;
    }
    
    /*!
	* \brief Marks this scene object als not visible
	*
	*/	
    void	unHide()       
    { 
    	m_bHidden = false;   
    }
    
    /*!
	* \brief Returns the visible state of this scene object
	* 
	* \return The visible state
	*/	
    bool isHidden()     const
    { 
    	return m_bHidden;    
    }
    
        /*!
	* \brief Marks this scene object als freezed
	*
	*/	
    void	freeze()         
    { 
    	m_bFreezed = true; 
    	m_bSelected = false;
    }
    
    /*!
	* \brief Marks this scene object als not freezed
	*
	*/	
    void	unFreeze()       
    { 
    	m_bFreezed = false;   
    }
    
    /*!
	* \brief Returns the freeze state of this scene object
	* 
	*/	
    bool isFreezed()     const
    { 
    	return m_bFreezed;
    }
    
    /*!
	* \brief Set the material ID of this scene object
	* 
	* \param i ID of the new material
	*/	
    virtual void   setMaterialID(const int i) 	
    { 
    	m_materialID = i; 
    }
    
    /*!
	* \brief Returns the material ID of this scene object
	* 
	* \return The material ID
	*/	
  	int    getMaterialID() const    		
  	{ 
  		return m_materialID;   
  	}
	
	/*!
	* \brief Compares two scene objects
	* 
	* \return Return true if it's the same objects
	*/	
	virtual bool operator==(const SceneObject &) const
	{ 
		return false; 
	};

	
protected:
	bool m_bHidden; /*!< \brief Indicates if this object is hidden */
	bool m_bFreezed; /*!< \brief Indicates if this object is freezed */
	bool m_bSelected; /*!< \brief Indicates if this object is selected */
	int  m_materialID; /*!< \brief The current material ID of this object */
	
	SceneObjectType m_type; /*!< \brief The type of this object */
	
	Aabb m_boundary; /*!< \brief The current bounding box of this object */
	
};

}

#endif /*SCENEOBJECT_H_*/
