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
 *  Module:     Plugin (BlackSun)
 *  File:       TexCoordEditor.h
 *  Created:    22.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXCOORDEDITOR_H_
#define TEXCOORDEDITOR_H_

#include <QtGui>

#include "./../../editor/renderer/include/Renderer.h"
#include <QGLWidget>
#include <SceneGraph.h>
#include "../ui_TexCoordEditorDialog.h"

/*! \file
* 
*  \brief Describes the class TexCoordEditor, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The texture coordinate editor dialog
*
* The dialog which allows the user to manipulate the texture coordinates of a mesh
*/
class TexCoordEditor : public QWidget
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	TexCoordEditor(QWidget* parent = 0);
	
	/*! \brief Fills the widets with informations of the scene
	*/	
	void fillFromScene();
	
private:
	/*! \brief Update scene 
	*
	* Updates the texture coorindates of the manipulated scene mesh(es)
	*/
	void updateScene();	
	
private slots:
	/*! \brief Slot for updating the scene and quit
	*/	
	void updateSceneAndExit();
	
	/*! \brief Slot for quit
	*/	
	void quitDialog();	
	
	/*! \brief Slot for showing the grid
	*
	* \param nState The check box state
	*/		
	void setShowGrid(int nState);
	
	/*! \brief Slot for showing the texture repetition
	*
	* \param nState The check box state
	*/	
	void setShowRepetition(int nState);
	
	/*! \brief Slot for clicking the 'Open materal editor'-Button
	*/	
	void openMaterialEditor();
	
	/*! \brief Slot for changing the mesh
	* 
	* \param index The index of the combo box item
	*/	
	void selectMesh(int index);
	
	/*! \brief Slot for changing the texture stage
	* 
	* \param index The index of the combo box item 
	*/
	void selectTexStage(int index);
	
	/*! \brief Slot for copying the texture coordinates
	*/	
	void copyTexCoord();
	
	/*! \brief Slot for set the selection mode
	*/	
	void setModeSelect();
	
	/*! \brief Slot for set the move mode
	*/	
	void setModeMove();
	
	/*! \brief Slot for set the scale mode
	*/	
	void setModeScale();
	
	/*! \brief Slot for set the rotation mode
	*/	
	void setModeRotate();
	
private:
	/*!
	* \brief Mesh item structure
	*/
	struct MeshItem
	{
		QString sName; 	/*!< \brief Mesh name*/
		Mesh* pMesh;	/*!< \brief Pointer to the mesh*/
		int nIndex;		/*!< \brief Index of the mesh*/
	};

private:
	Ui_TexCoordEditorDialog m_ui; 	/*!< \brief The UI-Object*/
	QList<MeshItem> m_meshes;		/*!< \brief The scene meshes*/
};

#endif /*TEXCOORDEDITOR_H_*/
