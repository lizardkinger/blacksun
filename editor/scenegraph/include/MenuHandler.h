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
 *  File:       MenuHandler.h
 *  Created:    02.04.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#ifndef MENUHANDLER_H_
#define MENUHANDLER_H_

/** \file MenuHandler.h
 * \brief Defines the menuhandler class.
 * 
 * \author Thomas Kuhndörfer.
 */
 
///////////////////////////////////////////////////////////
// Blacksun-Includes for Qt
///////////////////////////////////////////////////////////
#include <QObject> 

///////////////////////////////////////////////////////////
// Blacksun-Includes for scenegraph-lib
///////////////////////////////////////////////////////////
#include "../include/SceneObject.h"
#include "../include/Mesh.h"
#include "../include/Polygon.h"

namespace BSScene
{



/*! \brief The MenuHandler object 
*
* A concrete class that has actions for menu bindings the scenegraph is responsible for
*/	
class MenuHandler : public  QObject
{
	Q_OBJECT

public:

	/*!
	* \brief Constructor
	* 
	*/	
	MenuHandler() ;
	
	/*!
	* \brief Deconstructor
	* 
	*/	
	virtual ~MenuHandler();
	
private:

	/*!
	* \brief Extracts just the pure filename of a given filename 
	* 
	* \param fname The full path
	*/	
	QString convertFilename(const QString &fname);
	
	/*!
	* \brief Saves the current state of the SceneManager
	* 
	*/		
	void saveSMState();
	
	/*!
	* \brief Loads the current state of the SceneManager
	* 
	*/		
	void loadSMState();
	
	/*!
	* \brief Checks if the AABB is an empty one
	* 
	* \param aabb The AABB that will be checked
	* 
	* \return True if the AABB is empty
	*/		
	bool isEmptyAabb(const Aabb &aabb);
	
private:
	QString m_file_savename; /*!< \brief Saved filename for load/save */
	SceneSelectMode m_save_ssm; /*!< \brief Saved SceneSelectMode for load/save */
	SceneActionMode m_save_sam; /*!< \brief Saved SceneActionMode for load/save */
	SceneCreationMode m_save_scm; /*!< \brief Saved SceneCreationMode for load/save */
	
	vector<Mesh> m_savedMeshes; /*!< \brief Array of all meshes for copy/paste */
	vector<Polygon> m_savedPolys;  /*!< \brief Array of all polygons for copy/paste */

public slots:

	/*!
	* \brief Slot for menu binding File->New
	* 
	*/	
	void file_new();
	
	/*!
	* \brief Slot for menu binding File->Open
	* 
	*/		
	void file_open();
	
	/*!
	* \brief Slot for menu binding File->Merge
	* 
	*/		
	void file_merge();
	
	/*!
	* \brief Slot for menu binding File->Save
	* 
	*/		
	void file_save();
	
	/*!
	* \brief Slot for menu binding File->Save as
	* 
	* \param resave Determines if the current saved file should get a new name
	* 
	*/		
	void file_save_as(bool resave = false);

	/*!
	* \brief Slot for menu binding File->Comment
	* 
	*/	
	void file_comment();
	
	/*!
	* \brief Slot for menu binding Edit->Cut
	* 
	*/	
	void edit_cut();

	/*!
	* \brief Slot for menu binding Edit->Copy
	* 
	*/	
	void edit_copy();

	/*!
	* \brief Slot for menu binding Edit->Paste
	* 
	*/	
	void edit_paste();

	/*!
	* \brief Slot for menu bindingEdit->Delete
	* 
	*/	
	void edit_delete();
	
	/*!
	* \brief Slot for menu binding Edit->Clone
	* 
	*/		
	void edit_clone();
	
	/*!
	* \brief Slot for menu binding Edit->Mirror->X
	* 
	*/		
	void edit_mirror_x();
	
	/*!
	* \brief Slot for menu binding Edit->Mirror->Y
	* 
	*/		
	void edit_mirror_y();
	
	/*!
	* \brief Slot for menu binding Edit->Mirror->Z
	* 
	*/		
	void edit_mirror_z();
	
	
		/*!
	* \brief Slot for menu binding Edit->Mirror by world->X
	* 
	*/		
	void edit_mirror_world_x();
	
	/*!
	* \brief Slot for menu binding Edit->Mirror by world->Y
	* 
	*/		
	void edit_mirror_world_y();
	
	/*!
	* \brief Slot for menu binding Edit->Mirror by world->Z
	* 
	*/		
	void edit_mirror_world_z();
	
	/*!
	* \brief Slot for menu binding Edit->Make mesh from polygons
	* 
	*/		
	void edit_meshFromPolygons();
	
	/*!
	* \brief Slot for menu binding Edit->Extract polygons from mesh
	* 
	*/		
	void edit_extractPolygonsFromMesh();
	
	/*!
	* \brief Slot for menu binding Edit->Merge meshes to single mesh
	* 
	*/		
	void edit_mergeMeshes();


	/*!
	* \brief Slot for menu binding Selection->Select all
	* 
	*/	
	void selection_select_all();

	/*!
	* \brief Slot for menu binding Selection->Select none
	* 
	*/	
	void selection_select_none();
	
	/*!
	* \brief Slot for menu binding Selection->Select invert
	* 
	*/		
	void selection_select_invert();

	/*!
	* \brief Slot for menu binding Selection->Select intersecting
	* 
	*/	
	void selection_select_intersecting();
	

	/*!
	* \brief Slot for menu binding Selection->Hide selection
	* 
	*/	
	void selection_hide_selection();
	
		/*!
	* \brief Slot for menu binding Selection->Hide all
	* 
	*/	
	void selection_hide_all();
	
	/*!
	* \brief Slot for menu binding Selection->Hide none
	* 
	*/		
	void selection_hide_none();

	/*!
	* \brief Slot for menu binding Selection->Hide invert
	* 
	*/	
	void selection_hide_invert();	


	/*!
	* \brief Slot for menu binding Selection->Freeze selection
	* 
	*/		
	void selection_freeze_selection();
	
	/*!
	* \brief Slot for menu binding Selection->Freeze all
	* 
	*/		
	void selection_freeze_all();

	/*!
	* \brief Slot for menu binding Selection->Freeze none
	* 
	*/	
	void selection_freeze_none();

	/*!
	* \brief Slot for menu binding Selection->Freeze invert
	* 
	*/	
	void selection_freeze_invert();	

	/*!
	* \brief Slot for menu binding Selection->Zoom to selection
	* 
	*/		
	void selection_zoomTo();

	/*!
	* \brief Slot for menu binding Selection->Zoom to all
	* 
	*/	
	void selection_zoomAll();
	
	/*!
	* \brief Slot for menu binding Selection->Arrange together
	* 
	*/	
	void selection_ArrangeTogether();
	
	/*!
	* \brief Slot for menu binding Selection->Align vertices->X axis
	* 
	*/	
	void selection_AlignXAxis();


	/*!
	* \brief Slot for menu binding Selection->Align vertices->Y axis
	* 
	*/	
	void selection_AlignYAxis();
	
	/*!
	* \brief Slot for menu binding Selection->Align vertices->Z axis
	* 
	*/	
	void selection_AlignZAxis();	
	
	
	/*!
	* \brief Slot for menu binding Modify->Triangulate
	* 
	*/	
	void modify_triangulate();
	/*!
	* \brief Slot for menu binding Modify->Remove unbound vertices
	* 
	*/	
	void modify_removeUnboundVerts();
	
	/*!
	* \brief Slot for menu binding Modify->Turn vertex order
	* 
	*/		
	void modify_turnVertexOrder();
	
	/*!
	* \brief Slot for menu binding Material->Texture offset/repeat editor
	* 
	*/	
	void material_coordinates();

	/*!
	* \brief Slot for menu binding Material->Remove material bindings
	* 
	*/	
	void material_remove();

	/*!
	* \brief Slot for menu binding View->Statistics
	* 
	*/		
	void view_statistics();
	
	
signals:

	/*!
	* \brief Signal for automatic zooming
	* 
	*/	
	 void zoomToSelection(Aabb boundary);	
};


}


#endif /*MENUHANDLER_H_*/
