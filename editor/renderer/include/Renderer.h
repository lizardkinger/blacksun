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
 *  File:       Renderer.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererTypes.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "RendererSettings.h"
#include "Camera.h"
#include "CameraArcball.h"
#include "CameraOrthogonal.h"

///////////////////////////////////////////////////////////
// for the Versions - Info
///////////////////////////////////////////////////////////
#include "./../../pluginsystem/include/BSBaseTypes.h"

/*! \file
* 
*  \brief Describes the class Renderer, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

class RendererSettings;	

/*! \brief The renderer
*
* The renderer renders all data from the scene, caluclates additional data
* and transmit the result to the render cache. 
*/
class Renderer
{
public:
	/*!
	 * \brief The Version
	 */
	static BSPlgMgr::Version version;
	
	/*!
	* \brief Returns the instance of the renderer to give access to it
	*
	* \return Renderer* The instance of the renderer
	*/
	static Renderer* getInstance();

	/*!
	* \brief Initializes the renderer
	*
	* \return bool Returns true if initialization was successful
	*/	
	bool init();

	/*!
	* \brief Clears the szene
	*
	* Clears the scene and the background with the color set via the
	* renderer settings
	*/		
	void clear();
	
	/*!
	* \brief Returns informations about OpenGl
	*
	* \return OGLInfo The informations
	*/		
	OGLInfo getInfo() const
	{
		return m_info;
	}
	
	/*!
	* \brief Set the dimension of the viewport
	* 
	* \param nWidth The viewport width
	* \param nHeight The viewport height
	*/
	void setViewport(int nWidth, int nHeight);
	
	/*!
	* \brief Returns if the renderer is ready
	*
	* \return bool Returns true if the renderer is ready, otherwise false
	*/	
	bool isReady() const
	{
		return m_bReady;	
	}

	/*!
	* \brief Returns the last OpenGl error
	*
	* \return GLenum The last error code of OpenGl
	*/		
	GLenum getError() const
	{
		return glGetError();	
	}
	
	/*!
	* \brief Returns the string of the last OpenGl error
	*
	* \return string The string of the last error code of OpenGl
	*/	
	string getErrorString() const
	{
		return string((char *)(gluErrorString(glGetError())));	
	}	

	/*!
	* \brief Render data 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/		
	void render(PrimitiveType type, vector<Vertex>& verts, int nMaterialID,
				const Matrix& mat, const Aabb& aabb);
				
	/*!
	* \brief Render indexed data 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/		
	void render(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
				 int nMaterialID, const Matrix& mat, const Aabb& aabb);

	/*!
	* \brief Render data as selection 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/					
	void renderSelection(PrimitiveType type, vector<Vertex>& verts, const Matrix& mat,
				int nMaterialID, const Aabb& aabb);
	
	/*!
	* \brief Render indexed data as selection 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/
	void renderSelection(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
				int nMaterialID, const Matrix& mat, const Aabb& aabb);
		
	/*!
	* \brief Render data freezed 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/			
	void renderFreezed(PrimitiveType type, vector<Vertex>& verts, const Matrix& mat,
				int nMaterialID, const Aabb& aabb);
	
	/*!
	* \brief Render indexed data freezed 
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	*/	
	void renderFreezed(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
				int nMaterialID, const Matrix& mat, const Aabb& aabb);


	/*!
	* \brief Render a box that represents an AABB 
	*
	* \param aabb The AABB
	*/
	void renderAABB(const Aabb& aabb);	

	/*!
	* \brief Render the grid
	*
	* Renders a grid depending on the camera settings and the viewmode
	*/	
	void renderGrid();
	
	/*!
	* \brief Force to render all data now
	*
	* \return int Number of rendered vertices
	*/
	int flush();
	
	/*!
	* \brief Return the perspective camera
	*
	* \return Camera* Pointer to the perspective camera
	*/	
	Camera* getCameraPerspective()
	{
		return m_pCamPerspective;	
	}
	
	/*!
	* \brief Set the perspective camera
	*
	* \param pCam Pointer to a perspective camera
	*/
	void setCameraPerspective(CameraArcball* pCam)
	{
		m_pCamPerspective = pCam;	
	}

	/*!
	* \brief Return the orthogonal camera
	*
	* \return Camera* Pointer to the orthogonal camera
	*/	
	Camera* getCameraOrthogonal()
	{
		return m_pCamOrthogonal;	
	}

	/*!
	* \brief Set the orthogonal camera
	*
	* \param pCam Pointer to a orthogonal camera
	*/	
	void setCameraOrthogonal(CameraOrthogonal* pCam)
	{
		m_pCamOrthogonal = pCam;	
	}
	
	//Light-Functions
	//...

	/*!
	* \brief Set the viewmode
	*
	* \param vm The viewmode
	*/	
	void setViewMode(ViewMode vm)
	{
		m_viewmode = vm;	
	}
	
	/*!
	* \brief Return the viewmode
	*
	* \return ViewMode The viewmode
	*/
	ViewMode getViewMode() const
	{
		return m_viewmode;	
	}

	/*!
	* \brief Set the fillmode
	*
	* \param fm The fillmode
	*/		
	void setFillMode(FillMode fm)
	{
		m_fillmode = fm;	
	}

	/*!
	* \brief Return the fillmode
	*
	* \return FillMode The fillmode
	*/	
	FillMode getFillMode() const
	{
		return m_fillmode;
	}

	/*!
	* \brief Enable the ZBuffer
	*
	* \param bEnable true, if the ZBuffer should be enabled, otherwise false
	*/	
	void enableZBuffer(bool bEnable = true) const
	{
		if(bEnable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	/*!
	* \brief Return if the ZBuffer is enabled
	*
	* \return bool true, if the ZBuffer is enabled, otherwise false
	*/		
	bool hasZBuffer() const
	{
		return glIsEnabled(GL_DEPTH_TEST);	
	}

	/*!
	* \brief Enable a specific OpenGl-option
	*
	* \param cap The OpenGl-option that should be enabled
	*/	
	void enableGLOption(GLenum cap) const
	{
		glEnable(cap);	
	}

	/*!
	* \brief Disable a specific OpenGl-option
	*
	* \param cap The OpenGl-option that should be disabled
	*/		
	void disableGLOption(GLenum cap) const
	{
		glDisable(cap);	
	}

	/*!
	* \brief Return if a specific OpenGl-option is enabled
	*
	* \param cap The OpenGl-option that should be checked
	* \return bool True, if the specific OpenGl-option is enabled, otherwise false
	*/		
	bool hasGLOption(GLenum cap) const
	{
		return glIsEnabled(cap);
	}

	/*!
	* \brief Set the near clipping plane
	* 
	* \param dNearPlane The near clipping plane
	*/	
	void setNearPlane(double dNearPlane);
	
	/*!
	* \brief Return the near clipping plane
	* 
	* \return double The near clipping plane
	*/	
	double getNearPlane() const
	{
		return m_dNearPlane;	
	}

	/*!
	* \brief Set the far clipping plane
	* 
	* \param dFarPlane The far clipping plane
	*/	
	void setFarPlane(double dFarPlane);

	/*!
	* \brief Return the far clipping plane
	* 
	* \return double The far clipping plane
	*/	
	double getFarPlane() const
	{
		return m_dFarPlane;	
	}
	
	/*!
	* \brief Set the field of view
	* 
	* \param dFOV The field of view
	*/		
	void setFOV(double dFOV);
	
	/*!
	* \brief Returns the field of view
	* 
	* \return double The field of view
	*/		
	double getFOV() const
	{
		return m_dFOV;
	}	

	/*!
	* \brief Return a pointer to the render settings
	* 
	* \return RendererSettings* If the renderer was initialized successfully, the
	*   pointer to the renderer settings is returned, otherwise NULL.
	*/	
	RendererSettings* getSettings();

	/*!
	* \brief Return a pointer to the material manager
	* 
	* \return MaterialManager* Pointer to the material manager
	*/	
	MaterialManager* getMaterialManager() const
	{
		return MaterialManager::getInstance();
	}
	
	/*!
	* \brief Return a pointer to the texture manager
	* 
	* \return TextureManager* Pointer to the texture manager
	*/	
	TextureManager* getTextureManager() const
	{
		return TextureManager::getInstance();
	}	

private:
	/*!
	* \brief The standard constructor
	*/
	Renderer();
	
	/*!
	* \brief The destructor
	*/
	virtual ~Renderer();
	
	/*!
	* \brief Render indexed data in a specific format
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	* \param bSelected Render the object as a selected object
	* \param bFreezed Render the object as a freezed object
	*/	
	void render(PrimitiveType type, vector<Vertex>& verts, vector<int>& indices, 
				 int nMaterialID, const Matrix& mat, const Aabb& aabb, 
				 bool bSelected, bool bFreezed);

	/*!
	* \brief Render data in a specific format
	*
	* \param type The primitive type
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	* \param aabb The AABB of the rendered object
	* \param bSelected Render the object as a selected object
	* \param bFreezed Render the object as a freezed object
	*/				 
	void render(PrimitiveType type, vector<Vertex>& verts, 
				 int nMaterialID, const Matrix& mat, const Aabb& aabb, 
				 bool bSelected, bool bFreezed);
	
	/*!
	* \brief Render normals
	*
	* \param verts The vertices that should be rendered
	* \param indices The indices of the data that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	*/	
	void renderNormals(vector<Vertex>& verts, vector<int>& indices,
				int nMaterialID, const Matrix& mat);

	/*!
	* \brief Render normals
	*
	* \param verts The vertices that should be rendered
	* \param nMaterialID The material
	* \param mat The matrix for the object
	*/					
	void renderNormals(vector<Vertex>& verts, int nMaterialID, 
				const Matrix& mat);				  

	/*!
	* \brief Render 2D-grid
	*
	* Renders a two-dimensional grid depending on the camera settings
	*/		
	void render2DGrid();
	
	/*!
	* \brief Render 3D-grid
	*
	* Renders a third-dimensional grid depending on the camera settings
	*/		
	void render3DGrid();

	/*! \brief The guard for the renderer singleton 
	*
	* The guard for the renderer singleton arrange that the destructor
	* of the singleton will be called
	*/	
	class RendererWaechter 
	{
	public:
		/*!
		* \brief The destructor
		*/
		virtual ~RendererWaechter() 
		{
			if(Renderer::m_instance != NULL)
				delete Renderer::m_instance;
		}
	};
	friend class RendererWaechter;
	
private:
	bool m_bReady; /*!< \brief Is the renderer ready? */
	OGLInfo m_info /*!< \brief Informations about OpenGL */;
	ViewMode m_viewmode; /*!< \brief The viewmode */
	FillMode m_fillmode; /*!< \brief The fillmode */
	double m_dNearPlane; /*!< \brief The near clipping plane */
	double m_dFarPlane; /*!< \brief The far clipping plane */
	double m_dFOV; /*!< \brief The field of view */
	RendererSettings* m_pSettings; /*!< \brief The renderer settings */
	
	CameraArcball* m_pCamPerspective; /*!< \brief The perspective camera */
	CameraOrthogonal* m_pCamOrthogonal; /*!< \brief The orthogonal camera */
	
	static Renderer* m_instance; /*!< \brief The instance of the renderer singleton*/
};


}

#endif /*RENDERER_H*/
