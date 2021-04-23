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
 *  File:       SceneManager.h
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

/** \file SceneManager.h
 * \brief Defines the SceneManager class.
 * 
 * \author Thomas Kuhndörfer.
 */

///////////////////////////////////////////////////////////
// Blacksun-Includes for Qt
///////////////////////////////////////////////////////////
#include <Qt>
#include <QPoint>
#include <QString>
#include <string> 
#include <QDataStream>

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "./SceneObject.h"
#include "./Scene.h"
#include "./SelectionBuffer.h"
#include "./MenuHandler.h"
#include "./SceneAction2D.h"

///////////////////////////////////////////////////////////
// for the Versions - Info
///////////////////////////////////////////////////////////
#include "./../../pluginsystem/include/BSBaseTypes.h"

using namespace std;

namespace BSScene
{

/*! \brief The scene manager
*
* A concrete class that manages the scene and provides data access for external components
*/
class SceneManager : public  QObject
{
	Q_OBJECT
	
private:
	static SceneManager* _instance;	 /*!< \brief Singleton instance */
	
public:
	static BSPlgMgr::Version version; /*!< \brief The Version */
	
	/*!
	* \brief Deconstructor
	* 
	*/		
	virtual ~SceneManager();
	
	
	/*!
	* \brief Returns the instance of the SceneManager to give access to it
	*
	* \return SceneManager* The instance of the SceneManager
	*/
	static 	SceneManager* getInstance();
	
	
	/*!
	* \brief Resets the state of the SceneManager to initialized. All scene objects are getting lost 
	* 
	*/	  	
	void	reset();
	
	
	/*!
	* \brief Deletes all materials by calling the MaterialManager 
	* 
	*/	  		
	void	resetMaterials();
	
	
	/*!
	* \brief Set the scene description
	* 
	* \param s The new description text
	*/	
    void    setDescription(const QString s);
    
    
	/*!
	* \brief Get the scene descriptio
	* 
	* \return The current description text
	*/	    
    QString getDescription() const;
    
    
	/*!
	* \brief Get the list of all mesh names
	* 
	* \return The mesh list
	*/		
    vector<QString> getSceneobjectNames(); 
      
      
	/*!
	* \brief Saves a scene into a file
	* 
	* \param name The filename for the blacksun file
	*/	      
   	bool    saveScene(const QString name);
   	
	/*!
	* \brief Loads a scene from a file
	* 
	* \param name The filename for the blacksun file
	* \param resetScene True if the current scene should be resetted before loading
	* \param keepMaterial True if the current materials should be hold
	*/	     	
    bool    loadScene(const QString name,const bool resetScene,const bool keepMaterial);
    
    
	/*!
	* \brief Adds a scene object to the scene
	* 
	* \param o The new scene object
	*/	    
   	SceneObject*    insertObject(SceneObject& o);
   	
	/*!
	* \brief Removes a scene object to the scene
	* 
	* \param o The scene object that become removed
	*/	       	
    void    removeObject(SceneObject* o);
    
    
    /*!
	* \brief Removes a selected vertices 
	* 
	* \param sv The selected vertex that should be removed
	*/	      
    void    removeSelectedVertex(SelectedVertex& sv);
    
    
	/*!
	* \brief Calls the blacksun renderer to render the scene
	* 
	* \param r Reference to the blacksun renderer
	*/	        
    void    renderScene(Renderer& r);
    
    /*!
	* \brief Get the underlaying scene 
	* 
	* \return The underlaying scene
	*/	    
    Scene&	getScene()	
    { 
    	return m_scene; 
    };
    
    /*!
	* \brief Set the current SceneSelectMode 
	* 
	* \param action The new selection mode
	*/	
    void 	setSelectionMode(const SceneSelectMode action) 
    {
    	m_selectionMode = action;
    }
    
    
    /*!
	* \brief Get the current SceneSelectMode 
	* 
	* \return The current selection mode
	*/	    
    SceneSelectMode getSelectionMode() const
    { 
    	return m_selectionMode; 
    }
    
    
    /*!
	* \brief Set the current SceneActionMode 
	* 
	* \param action The new action mode
	*/	    
    void 	setActionMode(const SceneActionMode action) 
    {
    	m_actionMode = action;
    }
    
    /*!
	* \brief Get the current SceneActionMode 
	* 
	* \return The current action mode
	*/	       
    SceneActionMode getActionMode() const
    { 
    	return m_actionMode; 
    }

    /*!
	* \brief Set the current SceneCreationMode 
	* 
	* \param action The new creation mode
	*/	        
    void 	setCreationMode(const SceneCreationMode action) 
    {
    	m_creationMode = action;
    }
    
    /*!
	* \brief Get the current SceneCreationMode 
	* 
	* \return The current creation mode
	*/	  
    SceneCreationMode getCreationMode() const
    {
    	return m_creationMode; 
    }
    
    /*!
	* \brief Performs an 2D action occured in an external component
	* 
	* \param sa Structure with details about the action
	*/	     
    void 	action2D(const SceneAction2D &sa);
    
    /*!
	* \brief Checks if the scene needs to be redrawn
	* 
	* \param redraw True if redraw is forced
	* \param justSBuffer True if just objects from the SelectionBuffer got checked
	*/	   
    void	checkForRedraw(const bool redraw = false,const bool justSBuffer = false);
    
    
    /*!
	* \brief Informs the SceneManager about scene changes
	* 
	*/
    void	notifyContentChanged();
    
    /*!
	* \brief Informs the SceneManager about scene changes
	* 
	* \param so Only objects of the type of this parameter has changed  
	*/    
    void 	notifyContentChanged(const SceneObject *so);
    
    
    /*!
	* \brief Informs the SceneManager about scene changes
	* 
	* \param sot Only objects of the type of this parameter has changed
	* \param justSBuffer True if just selected objects have changed  
	*/      
    void	notifyContentChanged(const SceneObjectType sot,const bool justSBuffer);
    

    /*!
	* \brief Get the SelectionBuffer 
	* 
	* \return Reference to the SelectionBuffer
	*/	 
    SelectionBuffer& getSelBuffer();
    
    
    /*!
	* \brief Get the MenuHandler 
	* 
	* \return Reference to the MenuHandler
	*/	 
    MenuHandler* getMenuHandler() 
    { 
    	return &m_menu;
    };
        
	        
    /*!
	* \brief Changes the title text of the main window
	* 
	* \param message The new title text
	*/
	void    changeTitle(const QString message);

    
private:
	/*!
	* \brief Private constructor
	* 
	*/	
	SceneManager();
	

	/*! \brief The guard for the SceneManager singleton 
	*
	* The guard for the SceneManager singleton arrange that the destructor
	* of the singleton will be called
	*/	
	class SceneManagerGuard
	{
	public:
	
		/*!
		* \brief The destructor
		*/
		virtual ~SceneManagerGuard() 
		{
			if(SceneManager::_instance != NULL)
				delete SceneManager::_instance;
		}
	};
	friend class SceneManagerGuard;
	
	
	/*!
	* \brief Initializes a scene
	*/		
	void 	init();
	
	
	/*!
	* \brief Releases a scene
	*/		
	void	release();


    /*!
	* \brief Support the interactive creation of polygons by the user
	* 
	* \param pos2D The clicked position as a creation information
	*/
    void	interactivePolyCreation(const Vector &pos2D, bool /*done*/, SceneAxis /*axis*/);
    
    /*!
	* \brief Supports the interactive creation of vertices by the user
	* 
	* \param pos2D The clicked position as a creation information
	*/    
    void	interactiveVertCreation(const Vector &pos2D, SceneAxis /*axis*/);
    
    
    /*!
	* \brief Provides support for selection in a 2D viewport
	* 
	* \param min Minimum point of the selection rectangle
	* \param max Maximum point of the selection rectangle
	* \param axis Axis of the view that will be the infinite component of the intersection test
	* \param selType Additional selection information
	*/   
    void	selection2D(const QPointF& min, const QPointF& max,const SceneAxis axis, const SceneAction2D_SelectionType selType);
    
    
    /*!
	* \brief Provides support for moving in a 2D viewport
	* 
	* \param source Source point of the selection rectangle
	* \param target Target point of the selection rectangle
	* \param axis Axis of the view that will be the infinite component of the intersection test
	*/       
    void	moveSelection(const QPointF& source,const  QPointF& target,const  SceneAxis axis);
    
    /*!
	* \brief Provides support for scaling in a 2D viewport
	* 
	* \param percentageX Factor for the X scaling (1.0 = no change)
	* \param percentageY Factor for the Y scaling (1.0 = no change)
	* \param percentageZ Factor for the Z scaling (1.0 = no change)
	*/         
    void 	scaleSelection(const double percentageX,const double percentageY,const double percentageZ);
    
    /*!
	* \brief Provides support for rotating in a 2D viewport
	* 
	* \param amount Rotate amount in degrees
	* \param axis Axis of the view that will be the infinite component of the intersection test
	*/         
    void	rotateSelection(const double amount,const  SceneAxis axis);
    	
    	
	/*!
	* \brief Calls the blacksun renderer to render the scene while creating a polygon by the user
	* 
	* \param r Reference to the blacksun renderer
	*/	            	
    void 	renderCreationState(Renderer& r);

	
private: 
	QString				m_description; /*!< \brief Description of the current scene */
	Scene				m_scene; /*!< \brief SceneManager handles the Scene object*/
	vector<Vertex> 		m_vertexbuffer; /*!< \brief Buffer for creating polygons */
	SceneSelectMode		m_selectionMode; /*!< \brief The curren selection mode */
	SceneActionMode 	m_actionMode; /*!< \brief The curren action mode */
	SceneCreationMode   m_creationMode; /*!< \brief The curren creation mode */
	SelectionBuffer		m_sbuffer; /*!< \brief SceneManager handles the SelectionBuffer object*/
	MenuHandler			m_menu; /*!< \brief SceneManager handles the MenuHandler object*/
	
	bool				m_redrawNeeded; /*!< \brief Status variable if the scene needs to be redrawn */
	QMutex 				m_insertMutex; /*!< \brief Lock mutex for synchronization while inserting objects */
	
	static const quint32 FILEID = 0xA0BFFF; /*!< \brief File ID for blacksun files, also called magic number */
	
signals:

	/*!
	* \brief Signal for redrawing the sene
	* 
	*/	   
	void 	sceneChanged();

	
	/*!
	* \brief Signal for an unknown mesh change has happen
	* 
	*/	   	
	void 	meshChange();
	

	/*!
	* \brief Signal for an inserted mesh 
	* 
	* \param name Name of the inserted mesh
	* 
	*/	   	
	void 	meshInserted(QString name);
	
	
	/*!
	* \brief Signal for an removed mesh 
	* 
	* \param name Name of the removed mesh
	* 
	*/	
	void 	meshRemoved( QString name);
	
	
	/*!
	* \brief Signal for changing the main window title
	* 
	* \param message Text of the new main window title
	* 
	*/	
	void 	changeWindowTitle(const QString& message);
};

}

#endif
