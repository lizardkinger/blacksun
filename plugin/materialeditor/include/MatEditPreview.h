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
 *  File:       MatEditPreview.h
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATEDITPREVIEW_H_
#define MATEDITPREVIEW_H_

#include <QtGui>
#include "./../../editor/renderer/include/Renderer.h" 
#include "./../../editor/gui/include/bsglwidget.h"  
#include <QtOpenGL/QGLWidget>
#include <QTime>
using namespace BSRenderer;

/*! \file
* 
*  \brief Describes the class MatEditPreview, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The opengl widget to preview the material
*
* This widgets previews the a material on different primitive objects. The camera
* is an arcball-camera.
*/
class MatEditPreview : public BSGui::BSGLWidget
{
       Q_OBJECT
     
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	* \param fm The fillmode
	* \param pnMat Pointer to the material ID
	*/
	MatEditPreview(QWidget *parent = 0, FillMode fm = FILL_Textured, int* pnMat = 0);

	/*! \brief Destructor
	*/	
	virtual ~MatEditPreview()
	{
		delete m_timer;	
	}
		
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
	      
protected:
	/*! \brief Initializes the OpenGl-Widget
	*/
	void initializeGL();
	
	/*! \brief Paints the 3D-scene
	*/	
	void paintGL();

	/*! \brief An event to show the context menu
	*/	
	void contextMenuEvent(QContextMenuEvent *event);
	
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
	
	/*! \brief An event if the mouse-wheel was moved
	*
	* \param event The mouse-wheel-event
	*/
	void wheelEvent(QWheelEvent *event);

	/*! \brief Creates a box
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dWidth The width of the box
	* \param dHeight The height of the box
	* \param dDepth The depth of the box
	*/		
	void createBox(vector<int>& indices, vector<Vertex>& verts,
		double dWidth, double dHeight, double dDepth);
	
	/*! \brief Creates a sphere
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dRadius The radius of the sphere
	* \param nRings The numer of ring
	* \param nSegments The number of segments per ring
	*/	
	void createSphere(vector<int>& indices, vector<Vertex>& verts,
		double dRadius, int nRings, int nSegments);
		
	/*! \brief Creates a torus
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dRadiusOuter The outer radius of the torus
	* \param dRadiusInner The inner radius of the torus
	* \param nRings The numer of ring
	* \param nSegments The number of segments per ring
	* \param dStart The start angle in degree
	* \param dEnd The end angle in degree
	*/			
	void createTorus(vector<int>& indices, vector<Vertex>& verts,
		double dRadiusOuter, double dRadiusInner, int nRings, int nSegments,
		double dStart = 0.0, double dEnd = 360.0);

	/*! \brief Creates a helix
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dRadiusOuterStart The starting outer radius of the helix
	* \param dRadiusInnerStart The ending inner radius of the helix
	* \param dRadiusOuterEnd The starting outer radius of the helix
	* \param dRadiusInnerEnd The ending inner radius of the helix
	* \param dHeight The height of the helix 
	* \param nRings The numer of ring
	* \param nSegments The number of segments per ring
	* \param dStart The start angle in degree
	* \param dEnd The end angle in degree
	*/					
	void createHelix(vector<int>& indices, vector<Vertex>& verts,
		double dRadiusOuterStart, double dRadiusInnerStart,
		double dRadiusOuterEnd, double dRadiusInnerEnd, double dHeight,
		int nRings, int nSegments, double dStart, double dEnd);

	/*! \brief Creates a grid
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dWidth Width of the grid
	* \param dHeight Height of the grid
	* \param nSegmentsWidth Number of segments on X-Axis
	* \param nSegmentsHeight Number of segments on Z-Axis
	*/			
	void createGrid(vector<int>& indices, vector<Vertex>& verts,
		double dWidth, double dHeight, //Width = X, Height = Z
		int nSegmentsWidth, int nSegmentsHeight);
		
	/*! \brief Creates a geosphere
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dRadius The radius of the geopshere
	* \param nDepth The depth of the geosphere (depth of subdevision)
	*/			
	void createGeoSphere(vector<int>& indices, vector<Vertex>& verts, 
		double dRadius, int nDepth);
		
	/*! \brief Subdivides a face
	*
	* \param v1 First vertex of the face
	* \param v2 Second vertex of the face
	* \param v3 Third vertex of the face
	* \param nDepth The subdivision-depth
	* \param indices References to an array, where the result indices should be stored
	* \param verts Reference to an array, where the result vertices should be stored
	* \param dRadius The radius of the original model
	*/			
	void subdivideGeoSphere(Vector& v1, Vector& v2, Vector& v3, int nDepth,
		vector<int>& indices, vector<Vertex>& verts, double dRadius);
		
	/*! \brief Calculates the U-texture-coordinate of a specific point on a sphere
	*
	* \param v The point on a sphere
	* \param dRadius The radius of the sphere
	* \return double The U-texture-coordinate
	*/			
	double calcSphereMapU(Vector& v, double dRadius);
	
	/*! \brief Calculates the V-texture-coordinate of a specific point on a sphere
	*
	* \param v The point on a sphere
	* \param dRadius The radius of the sphere
	* \return double The V-texture-coordinate
	*/				
	double calcSphereMapV(Vector& v, double dRadius);
		
protected slots:
	/*! \brief Slot for updating the widget
	*/
	void update();

	/*! \brief Slot for rendering a box for preview
	*/	
	void setRenderBox(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 0;
	}

	/*! \brief Slot for rendering a sphere for preview
	*/		
	void setRenderSphere(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 1;
	}

	/*! \brief Slot for rendering a torus for preview
	*/		
	void setRenderTorus(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 2;
	}

	/*! \brief Slot for rendering a helix for preview
	*/		
	void setRenderHelix(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 3;
	}

	/*! \brief Slot for rendering a grid for preview
	*/		
	void setRenderGrid(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 4;
	}

	/*! \brief Slot for rendering a geosphere for preview
	*/		
	void setRenderGeosphere(bool b)
	{
		if(b)
			m_nPrimitiveTyp = 5;
	}

	/*! \brief Slot for resetting the view/cameras
	*/	
	void resetCameras()
	{	
		m_pCamPerspective->reset();
		m_pCamOrthogonal->reset();
	}	

	/*! \brief Slot for showing the grid
	*/		
	void setShowGrid(bool b)
	{
		m_bShowGrid = b;	
	}
	
private:
	ViewMode m_vm; 					/*!< \brief The current viewmode*/
	FillMode m_fm; 					/*!< \brief The current fillmode*/
	Renderer* r; 					/*!< \brief Pointer to the renderer*/
	int* m_pnMaterial; 				/*!< \brief Pointer to the material id*/
	QTime m_time; 					/*!< \brief Timer, to update the window in a specific periode*/
	vector<Vertex> 	m_vertsBox; 	/*!< \brief Array with the vertices for a box*/
    vector<int> 	m_indicesBox; 	/*!< \brief Array with the indices for a box*/
    vector<Vertex> 	m_vertsSphere; 	/*!< \brief Array with the vertices for a sphere*/
    vector<int> 	m_indicesSphere;/*!< \brief Array with the indices for a sphere*/
	vector<Vertex> 	m_vertsTorus; 	/*!< \brief Array with the vertices for a torus*/
    vector<int> 	m_indicesTorus; /*!< \brief Array with the indices for a torus*/
	vector<Vertex> 	m_vertsHelix; 	/*!< \brief Array with the vertices for a helix*/
    vector<int> 	m_indicesHelix; /*!< \brief Array with the indices for a helix*/
	vector<Vertex> 	m_vertsGrid; 	/*!< \brief Array with the vertices for a grid*/
    vector<int> 	m_indicesGrid; 	/*!< \brief Array with the indices for a grid*/
    vector<Vertex> m_vertsGeoSphere;/*!< \brief Array with the vertices for a geosphere*/
    vector<int> m_indicesGeoSphere; /*!< \brief Array with the indices for a geosphere*/
    Matrix m_mTransform; 			/*!< \brief The trasnformation matrix*/
    Aabb m_aabb; 					/*!< \brief The bounding box of the model*/
	int m_nPrimitiveTyp;			/*!< \brief The preview primitive*/
	bool m_bShowGrid;				/*!< \brief Render the grid?*/
	
	int m_nLastPosX; /*!< \brief Last X-coordinate of the mouse*/
	int m_nLastPosY; /*!< \brief Last Y-coordinate of the mouse*/ 
	
	CameraArcball* m_pCamPerspective; 	/*!< \brief The perspective camera*/
	CameraOrthogonal* m_pCamOrthogonal; /*!< \brief The orthogonal camera*/

	QMenu* m_contextMenu; 				/*!< \brief The context menu*/
	QActionGroup* m_actionGroupPrimitive; /*!< \brief Group for the preview primitives*/
	QAction* m_actionRenderBox; 		/*!< \brief Action for rendering a box*/
	QAction* m_actionRenderSphere; 		/*!< \brief Action for rendering a sphere*/
	QAction* m_actionRenderTorus; 		/*!< \brief Action for rendering a torus*/
	QAction* m_actionRenderHelix; 		/*!< \brief Action for rendering a helix*/
	QAction* m_actionRenderGrid; 		/*!< \brief Action for rendering a grid*/
	QAction* m_actionRenderGeosphere; 	/*!< \brief Action for rendering a geosphere*/
	QAction* m_actionSeperator; 		/*!< \brief Action for a seperator*/
	QAction* m_actionResetView;	 		/*!< \brief Action for resetting the view*/
	QAction* m_actionShowGrid;	 		/*!< \brief Action for rendering the grid*/
	
	QTimer* m_timer; /*!< \brief Timer*/
}; 

#endif /*MATEDITPREVIEW_H_*/
