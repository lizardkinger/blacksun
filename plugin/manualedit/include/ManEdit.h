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
 *  File:       ManEdit.h
 *  Created:    10.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MANEDIT_H_
#define MANEDIT_H_

#include <QtGui>

#include "./../../editor/renderer/include/Renderer.h"
#include <QGLWidget>
#include <SceneGraph.h>
#include "../ui_ManualEditDialog.h"

using namespace BSRenderer;

/*! \file
* 
*  \brief Describes the class ManEdit, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/

const int MANUALEDITCOLUMNS = 9;	/*!< \brief The number of columns*/
const int MANUALEDITPRECISSION = 6;	/*!< \brief The precission of the table widgets*/

/*! \brief The manual edit dialog
*
* The dialog for changing all vertex data manually
*/
class ManEdit : public QWidget
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	ManEdit(QWidget* parent = 0);
	
	/*! \brief Creates a table widget
	* 
	* Creates a table widget from all calculated table widget items 
	*/	
	void createTableFromScene();

	
private:
	/*! \brief Fills all edit items
	* 
	* Creates the table widget items for all selected vertices 
	*/
	void fillEditItems();

private slots:
	/*! \brief Slot for updating the scene
	*/
	void updateScene();
	
	/*! \brief Slot for updating the scene and quit
	*/	
	void updateSceneAndExit();
	
	/*! \brief Slot for quit
	*/
	void quitDialog();


private:
	
	/*!
	* \brief The edit item structure
	*/
	struct EditItem
	{
		BSRenderer::Vertex* pVert; /*!< \brief Pointer to the vertex*/
		BSScene::Mesh* pMesh; /*!< \brief Pointer to the mesh*/
		QTableWidgetItem items[MANUALEDITCOLUMNS]; /*!< \brief Widget items for all vertex components*/
	};

private:
	Ui_ManualEditDialog m_ui; 		/*!< \brief The UI-Object*/
	QList<EditItem> m_editItems; 	/*!< \brief List of all items for the table*/
	QStringList m_sHeader; 			/*!< \brief The horizontal header names*/
	QStringList m_sVHeader; 		/*!< \brief The vertical header names*/
};

#endif /*MANEDIT_H_*/
