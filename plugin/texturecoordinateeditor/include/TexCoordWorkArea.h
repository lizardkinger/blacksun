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
 *  File:       TexCoordWorkArea.h
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXCOORDWORKAREA_H_
#define TEXCOORDWORKAREA_H_

#include <QtGui>
#include "./../../editor/renderer/include/Renderer.h" 
#include "./../../editor/gui/include/bsglwidget.h"
#include "./../../editor/scenegraph/include/Mesh.h" 
#include <QtOpenGL/QGLWidget>
#include <QTime>
using namespace BSRenderer;

/*! \file
* 
*  \brief Describes the class TexCoordWorkArea, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The opengl widget shows the texture coordinates
*
* This widgets shows the texture coordinates of a mesh
*/
class TexCoordWorkArea : public BSGui::BSGLWidget
{
       Q_OBJECT
     
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	TexCoordWorkArea(QWidget *parent = 0);
		
	/*! \brief Resets the views 
	*
	* Resets the camera of the current viemode
	*/		
	void resetView() 
	{
		if(m_vm == VIEWMODE_Perspective)
			m_pCamPerspective->reset();
		else
			m_pCamOrthogonal->reset();	
	}

	/*! \brief Sets the mesh that should be displayed 
	*
	* \param pMesh The mesh that should be displayed
	*/		
	void setMesh(BSScene::Mesh* pMesh);
	
	/*! \brief Sets the texture stage that should be displayed 
	*
	* \param nTexStage The texture stage that should be displayed
	*/		
	void setTextureStage(int nTexStage);

	/*! \brief Updates the mesh 
	*
	* Updates the mesh of the scene with the manipulated coordinates
	*/	
	void updateMesh();
	
	/*!
	* \brief The texture edit manipulation mode
	*/	
	enum TexEditMode
	{
		TEM_Select,	/*!< Selection-Mode*/
		TEM_Move,	/*!< Move-Mode*/
		TEM_Scale,	/*!< Scale-Mode*/
		TEM_Rotate	/*!< Rotation-Mode*/
	};

	/*! \brief Sets the manipulation mode 
	*
	* \param mode The manipulation mode
	*/		
	void setMode(TexEditMode mode)
	{
		m_mode = mode;	
	}
	
	/*! \brief Copies the texture coordinates 
	*
	* \param nStageFrom The stage get the coordinates from
	* \param nStageTo The stage where to copy the coordinates
	*/		
	void copyTexCoords(int nStageFrom, int nStageTo);
	      
protected:
	/*! \brief Initializes the OpenGl-Widget
	*/
	void initializeGL();
	
	/*! \brief Paints the 3D-scene
	*/	
	void paintGL();
	
	/*! \brief Resizes the widget 
	*
	* \param width The new width of the widget
	* \param height The new height of the widget
	*/	
	void resizeGL(int width, int height);
	
	/*! \brief An event if a mouse-button was pressed
	*
	* \param event The mouse-event
	*/
    void mousePressEvent(QMouseEvent *event);
    
   	/*! \brief An event if the mouse was moved
	*
	* \param event The mouse-event
	*/
	void mouseMoveEvent(QMouseEvent *event);

	/*! \brief An event if a mouse-button was released
	*
	* \param event The mouse-event
	*/	
	void mouseReleaseEvent(QMouseEvent *event);
	
	/*! \brief An event if the mouse-wheel was moved
	*
	* \param event The mouse-wheel-event
	*/
	void wheelEvent(QWheelEvent *event);

	/*! \brief Converts the mesh to display it
	*
	* Converts the mesh texture coordinates to display it
	*/	
	void convertMesh();

	/*! \brief Creates the repetition mesh
	*
	* Creates the mesh to display the texture repetition
	*/		
	void createRepeatMesh();
		
protected slots:
	/*! \brief Slot for updating the widget
	*/
	void update();

	/*! \brief Slot for resetting the view/cameras
	*/	
	void resetCameras();

public slots:
	/*! \brief Slot for showing the grid
	*/		
	void setShowGrid(bool b)
	{
		m_bShowGrid = b;
		update();	
	}

	/*! \brief Slot for showing the texture repetition
	*/
	void setShowRepeat(bool b)
	{
		m_bShowRepeat = b;
		update();	
	}
	
private:
	/*!
	* \brief Texture coordinate vertex structure
	*/
	struct TexCoordVertex
	{
		/*! \brief Constructor
		* 
		* \param vertMesh Pointer to the original mesh vertex
		* \param vertWorkArea Pointer to the vertex in the work area 
		*/
		TexCoordVertex(Vertex* vertMesh, Vertex* vertWorkArea)
		 : pVertMesh(vertMesh), pVertWorkArea(vertWorkArea) { }
		 
		Vertex* pVertMesh;		/*!< \brief Pointer to the original mesh vertex*/
		Vertex* pVertWorkArea;	/*!< \brief Pointer to the vertex in the work area*/
	};
	
private:
	ViewMode m_vm; 					/*!< \brief The current viewmode*/
	Renderer* r; 					/*!< \brief Pointer to the renderer*/
	vector<Vertex> m_vertsMesh;		/*!< \brief Vertices of the converted mesh*/
    vector<int> m_indicesMesh;		/*!< \brief Indices of the converted mesh*/
    vector<TexCoordVertex> m_vertAllocations;		/*!< \brief The allocation of mesh vertices and preview vertices*/
    vector<TexCoordVertex*> m_vertsMeshSelection;	/*!< \brief The allocation for the selected vertices*/
    
    vector<Vertex> m_vertsTexQuad;			/*!< \brief The quad to display the texture*/
    vector<Vertex> m_vertsTexQuadRepeat;	/*!< \brief Vertices for the repetition quad*/
    vector<int> m_indicesTexQuadRepeat;		/*!< \brief Indices for the repetition quad*/

    Matrix m_mTransform; 			/*!< \brief The trasnformation matrix*/
    Aabb m_aabb; 					/*!< \brief The bounding box of the model*/
	bool m_bShowGrid;				/*!< \brief Render the grid?*/
	
	int m_nLastPosX; /*!< \brief Last X-coordinate of the mouse*/
	int m_nLastPosY; /*!< \brief Last Y-coordinate of the mouse*/ 
	
	CameraArcball* m_pCamPerspective; 	/*!< \brief The perspective camera*/
	CameraOrthogonal* m_pCamOrthogonal; /*!< \brief The orthogonal camera*/

	BSScene::Mesh* m_pMesh;	/*!< \brief The manipulated mesh*/
	int m_nTexStage;		/*!< \brief The manipulated texture stage*/
	TexEditMode m_mode;		/*!< \brief The manipulation mode*/
	bool m_bShowRepeat;		/*!< \brief Display the texture repetition mesh?*/
	
	QPoint m_posRubberBandOrigin;		/*!< \brief The rubber band origin*/
	QPoint m_posRubberBandDestination;	/*!< \brief The rubber band destination*/
	bool m_bShowRubberBand;				/*!< \brief Display the rubber band?*/
}; 

#endif /*TEXCOORDWORKAREA_H_*/
