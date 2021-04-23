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
 *  Module:     Renderer (BlackSun)
 *  File:       Renderer.cpp
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include <QtCore>
#include "../include/Renderer.h"
#include "../include/RendererSettings.h"
#include "../include/RenderCache.h"


namespace BSRenderer
{

Renderer* Renderer::m_instance = NULL;

//Set version
BSPlgMgr::Version Renderer::version(1,0,0);

Renderer* Renderer::getInstance()
{
	static RendererWaechter w;

	if(m_instance == NULL)
	{
		m_instance = new Renderer();
	}
	
	return m_instance;
}

Renderer::Renderer()
: m_bReady(false), m_dNearPlane(0.1), m_dFarPlane(10000.0), m_dFOV(45.0),
  m_pSettings(NULL), m_pCamPerspective(NULL), m_pCamOrthogonal(NULL)
{
}

Renderer::~Renderer()
{
	if(m_pSettings!=NULL)
	{
		delete m_pSettings;
		m_pSettings = NULL;
	}
}

bool Renderer::init()
{	
	setViewport(400, 300); 		 //Initialize the viewport
	
	//Set up all important render options
	glShadeModel(GL_SMOOTH); 	 //Enable Smooth Shading
	glClearDepth(1.0);			 // Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);		 // The Type Of Depth Testing To Do	
	glFrontFace(GL_CCW);		 //Counter clock wise order of face indices

	//Error found, so exit, because other gl-functions could be crash
    if(glGetError()!=GL_NO_ERROR)
    {
		return m_bReady;
    }
		
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice Perspective Calculations
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); 	   // Nice Mipmap Calculations
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 		   // Nice line smoothing Calculations
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 		   // Nice point smoothing Calculations
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); 		   // Nice polygon smoothing Calculations

	glEnable(GL_DEPTH_TEST); 	 //Enable depth-buffer (Z-Buffer)
	glEnable(GL_CULL_FACE); 	 //Back faces will not be rendered
	glEnable(GL_TEXTURE_2D);	 //Enable texturing
	glEnable(GL_COLOR_MATERIAL); //Enable materials
	glEnable(GL_LIGHTING);		 //Enable lightning
		
	glEnable(GL_BLEND); //Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	//Renderer-Settings
	if(m_pSettings == NULL)
	{
		m_pSettings = new RendererSettings();
	
		m_pSettings->setLineWidth(1.0); 			 //Line Width
		m_pSettings->setPointSize(5.0); 			 //Size of a point
		m_pSettings->setLineSmoothing(true); 		 //Render the lines smooth
		m_pSettings->setPointSmoothing(true);		 //Render the points smooth
		m_pSettings->setPolygonSmoothing(false);	 //Render the polygons not smooth
		m_pSettings->setRealtimeMirror(MIRROR_None); //Do not mirror the scene
		m_pSettings->setColoringByGroup(false); 	 //No coloring by group
		m_pSettings->setShowNormals(false); 		 //Do not show the normals
		m_pSettings->setNormalScaling(0.5);			 //Scaling of the shown normals
		m_pSettings->enableFrustumCulling(true); 	 //Enable frustum culling
		m_pSettings->setShowSelectionAABB(true); 	 //Show a AABB around the selection
		m_pSettings->setNearPlane(0.1); 			 //The near-plane for clipping
		m_pSettings->setFarPlane(10000.0);			 //The far-plane for clipping
		m_pSettings->setFOV(45.0);					 //The field of view
		m_pSettings->setSelectionAABBScaling(1.05);  //Scaling of the AABB in relation to the selected object	
		m_pSettings->setWireframeOverlayScaling(1.00); //Scaling for the wireframe-overlay mode
		m_pSettings->setGridSize(5); 				 //The grid size
		m_pSettings->setMouseWheelSpeed(0.05); 		 //The mouse wheel zoom speed
		m_pSettings->setShowBackfaces(false);		 //Render backfaces
	
		//Color-Settings
		m_pSettings->setWireframeColor(Color(1.0, 1.0, 1.0)); 		   //Color of wireframe lines and faces
		m_pSettings->setWireframeSelectionColor(Color(1.0, 0.0, 0.0)); //Color of selected wireframe lines and faces
		m_pSettings->setPointColor(Color(1.0, 1.0, 0.0)); 			   //Color of wireframe points
		m_pSettings->setPointSelectionColor(Color(1.0, 0.5, 0.0)); 	   //Color of selected wireframe points 
		m_pSettings->setFreezeColor(Color(0.55, 0.6, 1.0)); 		   //Color of freezed objects
		m_pSettings->setNormalColor(Color(1.0, 0.0, 1.0));  		   //Color of the normals
		m_pSettings->setClearColorOrtho(Color(0.8, 0.8, 0.8)); 		   //Clear color for orthogonal viewport
		m_pSettings->setClearColorPerspective(Color(0.2, 0.5, 0.7));   //Clear color for perspective viewport
		m_pSettings->setSelectionAABBColor(Color(0.0, 1.0, 0.0)); 	   //Color of the the selection AABB
		m_pSettings->setGridMinorLineColor(Color(0.7, 0.7, 0.7));	   //Color of the minor grid lines
		m_pSettings->setGridMajorLineColor(Color(0.5, 0.5, 0.0));	   //Color of the major grid lines
		
	}
	else
	{
		m_pSettings->setGlOptions();	
	}
	
	//Rendercache-Settings
	RenderCache::getInstance()->setMaxVertsPerCache(9000);
	
	//Viewport-Renderer-Settings
	m_viewmode = VIEWMODE_Perspective;
	m_fillmode = FILL_Textured;
		
	if(m_pCamPerspective==NULL)
	{
		m_pCamPerspective = new CameraArcball(400, 300);
	}
	
	if(m_pCamOrthogonal==NULL)
	{
		m_pCamOrthogonal = new CameraOrthogonal(400, 300, ORTHO_Front);
	}
	
	//Fill the info-struct
	m_info.sVendor 		= string((char *)(glGetString(GL_VENDOR)));
	m_info.sRenderer 	= string((char *)(glGetString(GL_RENDERER)));
	m_info.sVersion 	= string((char *)(glGetString(GL_VERSION)));
	m_info.sExtensions 	= string((char *)(glGetString(GL_EXTENSIONS)));
	
	//Set the light
	float LightAmbient[] =  { 0.0f, 0.0f, 0.0f, 1.0f };
	float LightDiffuse[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
	float LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	float LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
		
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
        	
	return m_bReady;
}

void Renderer::clear()
{
	if(m_viewmode == VIEWMODE_Orthogonal)
	{
		glClearColor(m_pSettings->getClearColorOrtho().r,
					m_pSettings->getClearColorOrtho().g,
					m_pSettings->getClearColorOrtho().b,
					m_pSettings->getClearColorOrtho().a);
	}
	else
	{
		glClearColor(m_pSettings->getClearColorPerspective().r,
					m_pSettings->getClearColorPerspective().g,
					m_pSettings->getClearColorPerspective().b,
					m_pSettings->getClearColorPerspective().a);
		
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
}

void Renderer::render(PrimitiveType type, vector<Vertex>& verts, int nMaterialID,
			const Matrix& mat, const Aabb& aabb)
{
	render(type, verts, nMaterialID, mat, aabb, false, false);
}

void Renderer::render(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
			 int nMaterialID, const Matrix& mat, const Aabb& aabb)
{
	render(type, verts, indices, nMaterialID, mat, aabb, false, false);
}
			
void Renderer::renderSelection(PrimitiveType type, vector<Vertex>& verts, const Matrix& mat,
			int nMaterialID, const Aabb& aabb)
{
	render(type, verts, nMaterialID, mat, aabb, true, false);
}
			
void Renderer::renderSelection(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
			int nMaterialID, const Matrix& mat, const Aabb& aabb)
{
	render(type, verts, indices, nMaterialID, mat, aabb, true, false);
}

void Renderer::renderFreezed(PrimitiveType type, vector<Vertex>& verts, const Matrix& mat,
			int nMaterialID, const Aabb& aabb)
{
	render(type, verts, nMaterialID, mat, aabb, false, true);
}
			
void Renderer::renderFreezed(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
			int nMaterialID, const Matrix& mat, const Aabb& aabb)
{
	render(type, verts, indices, nMaterialID, mat, aabb, false, true);
}

void Renderer::renderAABB(const Aabb& aabb)
{
	//Selection aabb deactivated?
	if(getSettings()->getShowSelectionAABB() == false)
		return;
		
	vector<Vertex> verts;
	vector<int> indices;
	double dScale = getSettings()->getSelectionAABBScaling();	
	
	Aabb copy = aabb;
	Vector vCentre = copy.getCentre();
	copy.setCentre(Vector(0.0, 0.0, 0.0));
	copy.setMinimum(aabb.getMinimum()*dScale);
	copy.setMaximum(aabb.getMaximum()*dScale);
	copy.setCentre(vCentre);
	
	Vector vMin = copy.getMinimum();
	Vector vMax = copy.getMaximum();
	
	//Add the 8 vertices for a box
	verts.push_back(Vertex(Vector(vMin.x, vMin.y, vMin.z)));
	verts.push_back(Vertex(Vector(vMax.x, vMin.y, vMin.z)));
	verts.push_back(Vertex(Vector(vMax.x, vMax.y, vMin.z)));
	verts.push_back(Vertex(Vector(vMin.x, vMax.y, vMin.z)));
	
	verts.push_back(Vertex(Vector(vMin.x, vMin.y, vMax.z)));
	verts.push_back(Vertex(Vector(vMax.x, vMin.y, vMax.z)));
	verts.push_back(Vertex(Vector(vMax.x, vMax.y, vMax.z)));
	verts.push_back(Vertex(Vector(vMin.x, vMax.y, vMax.z)));
	
	//Add the indices for the 12 lines to create the box of lines	
	indices.push_back(0);  indices.push_back(1);
	indices.push_back(1);  indices.push_back(2);
	indices.push_back(2);  indices.push_back(3);
	indices.push_back(0);  indices.push_back(3);
	
	indices.push_back(5);  indices.push_back(6);
	indices.push_back(6);  indices.push_back(7);
	indices.push_back(7);  indices.push_back(4);
	indices.push_back(4);  indices.push_back(5);
	
	indices.push_back(7);  indices.push_back(3);
	indices.push_back(4);  indices.push_back(0);
	indices.push_back(5);  indices.push_back(1);
	indices.push_back(6);  indices.push_back(2);
	
	//Render with selection aabb color
	RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, verts, indices, 
		getMaterialManager()->getSelectionAABBMaterialID(), Matrix(), false);
	
}

void Renderer::renderGrid()
{
	if(m_viewmode == VIEWMODE_Perspective)
	{
		render3DGrid();
	}
	else
	{
		render2DGrid();
	}
}
						
int Renderer::flush()
{
	int nNum = 0;
	
	if(m_viewmode == VIEWMODE_Perspective)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(m_pCamPerspective->getProjectionMatrix().getTranspose());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(m_pCamPerspective->getViewMatrix().getTranspose());
	}
	else //VIEWMODE_Orthogonal
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(m_pCamOrthogonal->getProjectionMatrix().getTranspose());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(m_pCamOrthogonal->getViewMatrix().getTranspose());
	}
	
	glPushMatrix();
    
	nNum = RenderCache::getInstance()->flush();
	glPopMatrix();
	
	return nNum;
}

void Renderer::setNearPlane(double dNearPlane)
{
	m_dNearPlane = dNearPlane;
}

void Renderer::setFarPlane(double dFarPlane)
{
	m_dFarPlane = dFarPlane;
	
}
	
void Renderer::setFOV(double dFOV)
{
	m_dFOV = dFOV;
}

RendererSettings* Renderer::getSettings()
{
	return m_pSettings;	
}

void Renderer::render(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
			int nMaterialID, const Matrix& mat, const Aabb& /*aabb*/, bool bSelected, 
			bool bFreezed)
{
	RenderCache* rc = RenderCache::getInstance();
	
	//Frustum Culling
	if(m_pSettings->getEnableFrustumCulling())
	{
		//TODO: Frustum-Culling	
	}
	
	//Save the correct materials
	int nMaterialPoint = -1;
	int nMaterialWire  = -1;
	MaterialManager* matMgr = MaterialManager::getInstance();
	if(bSelected) //Use the selection-materials
	{
		nMaterialPoint = matMgr->getPointSelectionMaterialID();
		nMaterialWire  = matMgr->getWireframeSelectionMaterialID();
	}
	else if(bFreezed) //Use the freezed-materials
	{
		nMaterialPoint = matMgr->getFreezeMaterialID();
		nMaterialWire  = matMgr->getFreezeMaterialID();		
	}
	else //Use normal materials
	{
		nMaterialPoint = matMgr->getPointMaterialID();
		nMaterialWire  = matMgr->getWireframeMaterialID();		
	}
	
	//Only generate a mirror-matrix if necessary
	Matrix* mMirror = NULL;
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		mMirror = new Matrix(mat);
		
		if(m_pSettings->getRealtimeMirror()==MIRROR_XY)
		{
			//Mirror z-coordinate of basis-vectors
			mMirror->_13 *= (-1.0);
			mMirror->_23 *= (-1.0);
			mMirror->_33 *= (-1.0);
			mMirror->_43 *= (-1.0);
		}
		else if(m_pSettings->getRealtimeMirror()==MIRROR_YZ)
		{
			//Mirror x-coordinate of basis-vectors
			mMirror->_11 *= (-1.0);
			mMirror->_21 *= (-1.0);
			mMirror->_31 *= (-1.0);
			mMirror->_41 *= (-1.0);
		}
		else if(m_pSettings->getRealtimeMirror()==MIRROR_XZ)
		{
			//Mirror y-coordinate of basis-vectors
			mMirror->_12 *= (-1.0);
			mMirror->_22 *= (-1.0);
			mMirror->_32 *= (-1.0);
			mMirror->_42 *= (-1.0);
		}	
	}
		
	
	//According to the fill mode, render with the right options
	if(m_fillmode == FILL_Textured || m_fillmode == FILL_Solid)
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it once
			rc->render(POLYMODE_Fill, PRIM_Triangle, verts, indices, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Triangle, verts, indices, nMaterialID, *mMirror, true);
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it once
			rc->render(POLYMODE_Fill, PRIM_Line, verts, indices, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Line, verts, indices, nMaterialID, *mMirror, true);
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Fill, PRIM_Point, verts, indices, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialID, *mMirror, true);
			}			
		}
	}
	else if(m_fillmode == FILL_Wireframe)
	{	
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it wireframe and as points
			rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, indices, nMaterialWire, mat, false);
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, indices, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, *mMirror, true);				
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it wireframe and as points
			rc->render(POLYMODE_Wireframe, PRIM_Line, verts, indices, nMaterialWire, mat, false);
			rc->render(POLYMODE_Point, PRIM_Line, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, indices, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, *mMirror, true);				
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, *mMirror, true);				
			}
		}
	}
	else if(m_fillmode == FILL_WireframeOverlay)
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it normal, wireframe and than as points
			rc->render(POLYMODE_Fill, PRIM_Triangle, verts, indices, nMaterialID, mat, false);
			//Matrix matScaled(mat);
			//matScaled.scale(m_pSettings->getWireframeOverlayScaling());
			Matrix matScaled(mat);
			matScaled.scale(m_pSettings->getWireframeOverlayScaling());
			matScaled.transpose();
			rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, indices, nMaterialWire, matScaled, false);
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, matScaled, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Triangle, verts, indices, nMaterialID, *mMirror, true);
				mMirror->scale(m_pSettings->getWireframeOverlayScaling());
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, indices, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, *mMirror, true);				
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it normal and as points
			rc->render(POLYMODE_Fill, PRIM_Line, verts, indices, nMaterialID, mat, false);
			rc->render(POLYMODE_Point, PRIM_Line, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Line, verts, indices, nMaterialID, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Line, verts, indices, nMaterialPoint, *mMirror, true);
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, *mMirror, true);				
			}		
		}
	}
	else //FILL_Point
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, indices, nMaterialPoint, *mMirror, true);				
			}		

		}
		else if(type == PRIM_Line)
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Line, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Line, verts, indices, nMaterialPoint, *mMirror, true);				
			}	
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, indices, nMaterialPoint, *mMirror, true);				
			}				
		}
	}
	 
	//Render the normals if needed
	if(m_pSettings->getShowNormals())
		renderNormals(verts, indices, matMgr->getNormalMaterialID(), mat);
	
	//Delete the mirror-matrix	
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		delete mMirror;
	}
}

void Renderer::render(PrimitiveType type, vector<Vertex>& verts, int nMaterialID, 
			const Matrix& mat, const Aabb& /*aabb*/, bool bSelected, bool bFreezed)
{
	RenderCache* rc = RenderCache::getInstance();
	
	//Frustum Culling
	if(m_pSettings->getEnableFrustumCulling())
	{
		//TODO: Frustum-Culling	
		//Add a value to the min and max-vector to include the normals (if needed) too
	}
	
	//Save the correct materials
	int nMaterialPoint = -1;
	int nMaterialWire  = -1;
	MaterialManager* matMgr = MaterialManager::getInstance();
	if(bSelected) //Use the selection-materials
	{
		nMaterialPoint = matMgr->getPointSelectionMaterialID();
		nMaterialWire  = matMgr->getWireframeSelectionMaterialID();
	}
	else if(bFreezed) //Use the freezed-materials
	{
		nMaterialPoint = matMgr->getFreezeMaterialID();
		nMaterialWire  = matMgr->getFreezeMaterialID();		
	}
	else //Use normal materials
	{
		nMaterialPoint = matMgr->getPointMaterialID();
		nMaterialWire  = matMgr->getWireframeMaterialID();		
	}
	
	//Only generate a mirror-matrix if necessary
	Matrix* mMirror = NULL;
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		mMirror = new Matrix(mat);
		
		if(m_pSettings->getRealtimeMirror()==MIRROR_XY)
		{
			//Mirror z-coordinate of basis-vectors
			mMirror->_13 *= (-1.0);
			mMirror->_23 *= (-1.0);
			mMirror->_33 *= (-1.0);
			mMirror->_43 *= (-1.0);
		}
		else if(m_pSettings->getRealtimeMirror()==MIRROR_YZ)
		{
			//Mirror x-coordinate of basis-vectors
			mMirror->_11 *= (-1.0);
			mMirror->_21 *= (-1.0);
			mMirror->_31 *= (-1.0);
			mMirror->_41 *= (-1.0);
		} 
		else if(m_pSettings->getRealtimeMirror()==MIRROR_XZ)
		{
			//Mirror y-coordinate of basis-vectors
			mMirror->_12 *= (-1.0);
			mMirror->_22 *= (-1.0);
			mMirror->_32 *= (-1.0);
			mMirror->_42 *= (-1.0);
		}		
	}
		
	
	//According to the fill mode, render with the right options
	if(m_fillmode == FILL_Textured || m_fillmode == FILL_Solid)
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it once
			rc->render(POLYMODE_Fill, PRIM_Triangle, verts, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Triangle, verts, nMaterialID, *mMirror, true);
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it once
			rc->render(POLYMODE_Wireframe, PRIM_Line, verts, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Wireframe, PRIM_Line, verts, nMaterialID, *mMirror, true);
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Fill, PRIM_Point, verts, nMaterialID, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialID, *mMirror, true);
			}			
		}
	}
	else if(m_fillmode == FILL_Wireframe)
	{	
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it wireframe and as points
			rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, nMaterialWire, mat, false);
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, *mMirror, true);				
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it wireframe and as points
			rc->render(POLYMODE_Wireframe, PRIM_Line, verts, nMaterialWire, mat, false);
			rc->render(POLYMODE_Point, PRIM_Line, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, *mMirror, true);				
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, *mMirror, true);				
			}		
		}
	}
	else if(m_fillmode == FILL_WireframeOverlay)
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it normal, wireframe and than as points
			rc->render(POLYMODE_Fill, PRIM_Triangle, verts, nMaterialID, mat, false);
			Matrix matScaled(mat);
			matScaled.scale(m_pSettings->getWireframeOverlayScaling());
			rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, nMaterialWire, matScaled, false);
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, matScaled, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Triangle, verts, nMaterialID, *mMirror, true);
				mMirror->scale(m_pSettings->getWireframeOverlayScaling());
				rc->render(POLYMODE_Wireframe, PRIM_Triangle, verts, nMaterialWire, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, *mMirror, true);				
			}
		}
		else if(type == PRIM_Line)
		{
			//Render it normal and as points
			rc->render(POLYMODE_Fill, PRIM_Line, verts, nMaterialID, mat, false);
			rc->render(POLYMODE_Point, PRIM_Line, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Fill, PRIM_Line, verts, nMaterialID, *mMirror, true);
				rc->render(POLYMODE_Point, PRIM_Line, verts, nMaterialPoint, *mMirror, true);
			}
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, *mMirror, true);				
			}		
		}
	}
	else //FILL_Point
	{
		//According to the primitive type, render with the right options
		if(type == PRIM_Triangle)
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Triangle, verts, nMaterialPoint, *mMirror, true);				
			}		

		}
		else if(type == PRIM_Line)
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Line, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Line, verts, nMaterialPoint, *mMirror, true);				
			}	
		}
		else //PRIM_Point
		{
			//Render it once as a point
			rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, mat, false);
			
			if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
			{
				rc->render(POLYMODE_Point, PRIM_Point, verts, nMaterialPoint, *mMirror, true);				
			}				
		}
	}
	
	//Render the normals if needed
	if(m_pSettings->getShowNormals())
		renderNormals(verts, matMgr->getNormalMaterialID(), mat);
		
	//Delete the mirror-matrix	
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		delete mMirror;
	}
}

void Renderer::renderNormals(vector<Vertex>& verts, vector<int>& indices,
			int nMaterialID, const Matrix& mat)
{
	vector<Vertex> vertsLines;
	
	int nIndices = indices.size();
	Vector vNormal, vPos;
	double dScaling = m_pSettings->getNormalScaling();
	
	//Walk through all indices
	for(int i=0; i<nIndices; i++)
	{
		vNormal = verts[indices[i]].vNormal;
		vPos = verts[indices[i]].vPos;
		//Just add a normal-line if there is a correct normal-vector
		if(vNormal != Vector(0.0, 0.0, 0.0))
		{
			//Normlize the vector
			vNormal.normalize();
			
			//Add the start and end point of the line
			vertsLines.push_back(Vertex(vPos));
			vertsLines.push_back(Vertex(vPos + vNormal * dScaling));
		}	
	}
	
	//Only generate a mirror-matrix if necessary
	Matrix* mMirror = NULL;
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		mMirror = new Matrix(mat);
		
		if(m_pSettings->getRealtimeMirror()==MIRROR_XY)
		{
			//Mirror z-coordinate of basis-vectors
			mMirror->_13 *= (-1.0);
			mMirror->_23 *= (-1.0);
			mMirror->_33 *= (-1.0);
			mMirror->_43 *= (-1.0);
		}
		else if(m_pSettings->getRealtimeMirror()==MIRROR_YZ)
		{
			//Mirror x-coordinate of basis-vectors
			mMirror->_11 *= (-1.0);
			mMirror->_21 *= (-1.0);
			mMirror->_31 *= (-1.0);
			mMirror->_41 *= (-1.0);
		} 
		else if(m_pSettings->getRealtimeMirror()==MIRROR_XZ)
		{
			//Mirror y-coordinate of basis-vectors
			mMirror->_12 *= (-1.0);
			mMirror->_22 *= (-1.0);
			mMirror->_32 *= (-1.0);
			mMirror->_42 *= (-1.0);
		}		
	}	
		
	RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, vertsLines, nMaterialID, mat, false);
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, vertsLines, nMaterialID, *mMirror, true);				
	}

	//Delete the mirror-matrix	
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		delete mMirror;
	}
}
				
void Renderer::renderNormals(vector<Vertex>& verts, int nMaterialID, 
			const Matrix& mat)
{
	vector<Vertex> vertsLines;
	
	int nVerts = verts.size();
	Vector vNormal, vPos;
	double dScaling = m_pSettings->getNormalScaling();
	
	//Walk through all indices
	for(int i=0; i<nVerts; i++)
	{
		vNormal = verts[i].vNormal;
		vPos = verts[i].vPos;
		//Just add a normal-line if there is a correct normal-vector
		if(vNormal != Vector(0.0, 0.0, 0.0))
		{
			//Normlize the vector
			vNormal.normalize();
			
			//Add the start and end point of the line
			vertsLines.push_back(Vertex(vPos));
			vertsLines.push_back(Vertex(vPos + vNormal * dScaling));
		}	
	}
	
	//Only generate a mirror-matrix if necessary
	Matrix* mMirror = NULL;
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		mMirror = new Matrix(mat);
		
		if(m_pSettings->getRealtimeMirror()==MIRROR_XY)
		{
			//Mirror z-coordinate of basis-vectors
			mMirror->_13 *= (-1.0);
			mMirror->_23 *= (-1.0);
			mMirror->_33 *= (-1.0);
			mMirror->_43 *= (-1.0);
		}
		else if(m_pSettings->getRealtimeMirror()==MIRROR_YZ)
		{
			//Mirror x-coordinate of basis-vectors
			mMirror->_11 *= (-1.0);
			mMirror->_21 *= (-1.0);
			mMirror->_31 *= (-1.0);
			mMirror->_41 *= (-1.0);
		} 
		else if(m_pSettings->getRealtimeMirror()==MIRROR_XZ)
		{
			//Mirror y-coordinate of basis-vectors
			mMirror->_12 *= (-1.0);
			mMirror->_22 *= (-1.0);
			mMirror->_32 *= (-1.0);
			mMirror->_42 *= (-1.0);
		}	
	}	
		
	RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, vertsLines, nMaterialID, mat, false);
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, vertsLines, nMaterialID, *mMirror, true);				
	}
	
	//Delete the mirror-matrix	
	if(m_pSettings->getRealtimeMirror()!=MIRROR_None)
	{
		delete mMirror;
	}	
}
	
void Renderer::render2DGrid()
{
	//Disable ZBuffer
	glDisable(GL_DEPTH_TEST);
		
	vector<Vertex> verts;
	
	//Get the limits and viewport
	double dL, dT, dR, dB;
	m_pCamOrthogonal->getLimits(dL, dT, dR, dB);
	int nWidth, nHeight;
	m_pCamOrthogonal->getViewport(nWidth, nHeight);
	
	//Set the type of the camera
	OrthogonalType ot = m_pCamOrthogonal->getOrthogonalType();
	m_pCamOrthogonal->setOrthogonalType(ORTHO_Front);
	
	//Calculate distance between two (major) grid lines
	double dDist = calcDecade(std::max(dB - dT, dR - dL));
	
	//Calculate the start value of the lines		
	double dStartX = int(dL / dDist - 1.0) * dDist;
	double dStartY = int(dB / dDist - 1.0) * dDist;	
	
	for(int i=0; i<2; i++)
	{
		verts.clear();
		
		if(i==0)
		{			
			//Distance for minor lines
			dDist /= m_pSettings->getGridSize();
		}
		else
		{
			//Distance for mayor lines
			dDist *= m_pSettings->getGridSize();
		}
		
		//Vertical lines
		for(double x = dStartX; x <= dR; x += dDist)
		{
			verts.push_back(Vertex(Vector(x, dT + 1.0, 0.0)));
			verts.push_back(Vertex(Vector(x, dB - 1.0, 0.0)));
		}
		
		//Horizontal lines
		for(double y = dStartY; y <= dT; y += dDist)
		{
			verts.push_back(Vertex(Vector(dL - 1.0, y, 0.0)));
			verts.push_back(Vertex(Vector(dR + 1.0, y, 0.0)));
		}
				
		if(i==0)
		{
			//Render with minor line material
			RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, verts, 
				getMaterialManager()->getGridMinorLineMaterialID(), Matrix(), false);
		}
		else
		{
			//Render with major line materal
			RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, verts, 
				getMaterialManager()->getGridMajorLineMaterialID(), Matrix(), false);
		}
	}
	
	//Force renderer to render now		
	flush();
	
	//Enable ZBuffer
	glEnable(GL_DEPTH_TEST);
	
	//Reset the type of the camera
	m_pCamOrthogonal->setOrthogonalType(ot);
}

void Renderer::render3DGrid()
{
	vector<Vertex> verts;
	
	//Calculate distance from origin
	double dLen = m_pCamPerspective->getPosition().length();
	
	//Calculate distance between two (major) grid lines
	double dDist = calcDecade(dLen);
	
	for(int i=0; i<2; i++)
	{
		verts.clear();

		if(i==0)
		{
			//Distance for minor lines
			dDist /= m_pSettings->getGridSize();
		}
		else
		{
			//Distance for mayor lines
			dDist *= m_pSettings->getGridSize();
		}		

		//Vertical lines
		for(double x = 0; x <= dLen; x += dDist)
		{
			verts.push_back(Vertex(Vector( x, 0.0,  dLen)));
			verts.push_back(Vertex(Vector( x, 0.0, -dLen)));
			verts.push_back(Vertex(Vector(-x, 0.0,  dLen)));
			verts.push_back(Vertex(Vector(-x, 0.0, -dLen)));
		}
		
		//Horizontal lines
		for(double y = 0.0; y <= dLen; y += dDist)
		{
			verts.push_back(Vertex(Vector(-dLen, 0.0,  y)));
			verts.push_back(Vertex(Vector( dLen, 0.0,  y)));
			verts.push_back(Vertex(Vector(-dLen, 0.0, -y)));
			verts.push_back(Vertex(Vector( dLen, 0.0, -y)));
		}
		
		if(i==0)
		{
			//Render with minor line material
			RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, verts, 
				getMaterialManager()->getGridMinorLineMaterialID(), Matrix(), false);
		}
		else
		{
			//Render with major line materal
			RenderCache::getInstance()->render(POLYMODE_Wireframe, PRIM_Line, verts, 
				getMaterialManager()->getGridMajorLineMaterialID(), Matrix(), false);
		}
	}	
}	
	
}
