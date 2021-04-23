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
 *  File:       RendererSettings.h
 *  Created:    06.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef RENDERERSETTINGS_H_
#define RENDERERSETTINGS_H_

#include "RendererTypes.h"
#include "Renderer.h"

/*! \file
* 
*  \brief Describes the class RendererSettings, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

class Renderer;


/*! \brief Combines all renderer settings
*
* All important settings that belongs to the renderer are stored here. It combines
* all classes and transmits the settings to the correct classes.
*/
class RendererSettings
{
public:
	/*!
	* \brief The standard constructor
	*/
	RendererSettings()
	{
	}
	
	/*!
	* \brief Set all OpenGl-Settings
	*/	
	void setGlOptions();
	
	/*!
	* \brief Set the line width
	* 
	* \param dLineWidth The line width
	*/	
	void setLineWidth(double dLineWidth)
	{
		m_dLineWidth = dLineWidth;
		glLineWidth(static_cast<float>(dLineWidth));
	}

	/*!
	* \brief Return the line width
	* 
	* \return double The line width
	*/		
	double getLineWidth() const
	{
		return m_dLineWidth;	
	}
	
	/*!
	* \brief Set the point size
	* 
	* \param dPointSize The point size
	*/		
	void setPointSize(double dPointSize)
	{
		m_dPointSize = dPointSize;
		glPointSize(static_cast<float>(dPointSize));
	}

	/*!
	* \brief Return the point size
	* 
	* \return double The point size
	*/			
	double getPointSize() const
	{
		return m_dPointSize;	
	}
	
	/*!
	* \brief Enable/Disable the smoothing of lines
	* 
	* \param bEnable true if to enable smoothing, otherwise false
	*/		
	void setLineSmoothing(bool bEnable)
	{
		m_bLineSmoothing = bEnable;
		
		if(bEnable)
			glEnable(GL_LINE_SMOOTH);		
		else
			glDisable(GL_LINE_SMOOTH);
	}

	/*!
	* \brief Return the smoothing of lines
	* 
	* \return bool true if smoothing is enabled, otherwise false
	*/			
	bool getLineSmoothing() const
	{
		return m_bLineSmoothing;	
	}	

	/*!
	* \brief Enable/Disable the smoothing of points
	* 
	* \param bEnable true to enable smoothing, otherwise false
	*/			
	void setPointSmoothing(bool bEnable)
	{
		m_bPointSmoothing = bEnable;
		
		if(bEnable)
			glEnable(GL_POINT_SMOOTH);		
		else
			glDisable(GL_POINT_SMOOTH);
	}

	/*!
	* \brief Return the smoothing of points
	* 
	* \return bool true if smoothing is enabled, otherwise false
	*/				
	bool getPointSmoothing() const
	{
		return m_bPointSmoothing;	
	}	

	/*!
	* \brief Enable/Disable the smoothing of polygones
	* 
	* \param bEnable true to enable smoothing, otherwise false
	*/			
	void setPolygonSmoothing(bool bEnable)
	{
		m_bPolygonSmoothing = bEnable;
		
		if(bEnable)
			glEnable(GL_POLYGON_SMOOTH);		
		else
			glDisable(GL_POLYGON_SMOOTH);
	}

	/*!
	* \brief Return the smoothing of polygons
	* 
	* \return bool true smoothing is enabled, otherwise false
	*/				
	bool getPolygonSmoothing() const
	{
		return m_bPolygonSmoothing;	
	}			
	
	/*!
	* \brief Set how the scene should be mirrored
	* 
	* \param rm How the scene should be mirrored
	*/				
	void setRealtimeMirror(RealtimeMirror rm)
	{
		m_realtimeMirror = rm;	
	}

	/*!
	* \brief Return how the scene should be mirrored
	* 
	* \return RealtimeMirror How the scene should be mirrored
	*/					
	RealtimeMirror getRealtimeMirror() const
	{
		return m_realtimeMirror;	
	}

	/*!
	* \brief Enable/Disable the coloring by group
	* 
	* \param bEnable true to enable group coloring, otherwise false
	*/	
	void setColoringByGroup(bool bEnable)
	{
		m_bColorByGroups = bEnable;
	}

	/*!
	* \brief Return the coloring by group
	* 
	* \return bool true if coloring by group is enabled, otherwise false
	*/		
	bool getColoringByGroup() const
	{
		return m_bColorByGroups;	
	}	

	/*!
	* \brief Set the wireframe color
	* 
	* \param col The wireframe color
	*/	
	void setWireframeColor(const Color& col)
	{
		MaterialManager::getInstance()->getWireframeMaterial()->setColor(col);
	}
	
	/*!
	* \brief Return the wireframe color
	* 
	* \return Color The wireframe color
	*/		
	Color getWireframeColor() const
	{
		return MaterialManager::getInstance()->getWireframeMaterial()->getColor();
	}

	/*!
	* \brief Set the wireframe-selection color
	* 
	* \param col The wireframe-selection color
	*/	
	void setWireframeSelectionColor(const Color& col)
	{
		MaterialManager::getInstance()->getWireframeSelectionMaterial()->setColor(col);
	}

	/*!
	* \brief Return the wireframe-selection color
	* 
	* \return Color The wireframe-selection color
	*/			
	Color getWireframeSelectionColor() const
	{
		return MaterialManager::getInstance()->getWireframeSelectionMaterial()->getColor();
	}

	/*!
	* \brief Set the point color
	* 
	* \param col The point color
	*/		
	void setPointColor(const Color& col)
	{
		MaterialManager::getInstance()->getPointMaterial()->setColor(col);
	}

	/*!
	* \brief Return the point color
	* 
	* \return Color The point color
	*/			
	Color getPointColor() const
	{
		return MaterialManager::getInstance()->getPointMaterial()->getColor();
	}

	/*!
	* \brief Set the point-selection color
	* 
	* \param col The point-selection color
	*/		
	void setPointSelectionColor(const Color& col)
	{
		MaterialManager::getInstance()->getPointSelectionMaterial()->setColor(col);
	}

	/*!
	* \brief Return the point-selection color
	* 
	* \return Color The point-selection color
	*/			
	Color getPointSelectionColor() const
	{
		return MaterialManager::getInstance()->getPointSelectionMaterial()->getColor();
	}	

	/*!
	* \brief Set the freeze color
	* 
	* \param col The freeze color
	*/		
	void setFreezeColor(const Color& col)
	{
		MaterialManager::getInstance()->getFreezeMaterial()->setColor(col);
	}
	
	/*!
	* \brief Return the freeze color
	* 
	* \return Color The freeze color
	*/		
	Color getFreezeColor() const
	{
		return MaterialManager::getInstance()->getFreezeMaterial()->getColor();
	}	

	/*!
	* \brief Enable/Disable rendering of the normals
	* 
	* \param bShow true to render normals, otherwise false
	*/	
	void setShowNormals(bool bShow)
	{
		m_bShowNormals = bShow;	
	}

	/*!
	* \brief Return the rendering of the normals
	* 
	* \return bool true if normal rendering is enabled, otherwise false
	*/		
	bool getShowNormals() const
	{
		return m_bShowNormals;	
	}	

	/*!
	* \brief Set the scaling of the rendered normals (length of the lines)
	* 
	* \param dScaling The scaling factor
	*/	
	void setNormalScaling(double dScaling)
	{
		m_dNormalScaling = dScaling;	
	}

	/*!
	* \brief Return the scaling of the rendered normals (length of the lines)
	* 
	* \return double The scaling factor
	*/		
	double getNormalScaling() const
	{
		return m_dNormalScaling;	
	}			

	/*!
	* \brief Set the normal color
	* 
	* \param col The normal color
	*/		
	void setNormalColor(const Color& col)
	{
		MaterialManager::getInstance()->getNormalMaterial()->setColor(col);
	}
	
	/*!
	* \brief Return the normal color
	* 
	* \return Color The normal color
	*/		
	Color getNormalColor() const
	{
		return MaterialManager::getInstance()->getNormalMaterial()->getColor();
	}	
	
	/*!
	* \brief Enable/Disable frustum culling
	* 
	* \param bEnable true to enable frustum culling, otherwise false
	*/
	void enableFrustumCulling(bool bEnable)
	{
		m_bFrustumCulling = bEnable;
	}
	
	/*!
	* \brief Return if frustum culling is enabled
	* 
	* \return bool true if normal rendering is enabled, otherwise false
	*/		
	bool getEnableFrustumCulling() const
	{
		return m_bFrustumCulling;
	}	

	/*!
	* \brief Set the clear color for orthogonal view
	* 
	* \param col The clear color
	*/
	void setClearColorOrtho(const Color& col)
	{
		m_cClearOrtho = col;
	}

	/*!
	* \brief Return the clear color for orthogonal view
	* 
	* \return Color The clear color
	*/		
	Color getClearColorOrtho() const
	{
		return m_cClearOrtho;	
	}

	/*!
	* \brief Set the clear color for perspective view
	* 
	* \param col The clear color
	*/
	void setClearColorPerspective(const Color& col)
	{
		m_cClearPerspective = col;
	}
	
	/*!
	* \brief Return the clear color for perspective view
	* 
	* \return Color The clear color
	*/		
	Color getClearColorPerspective() const
	{
		return m_cClearPerspective;	
	}

	/*!
	* \brief Enable/Disable rendering the selection AABB
	* 
	* \param bEnable true to render the selection AABB, otherwise false
	*/	
	void setShowSelectionAABB(bool bEnable)
	{
		m_bShowSelectionAABB = bEnable;	
	}

	/*!
	* \brief Return if rendering the selection AABB is enabled
	* 
	* \return bool true if rendering the selection AABB is enabled, otherwise false
	*/		
	bool getShowSelectionAABB() const
	{
		return m_bShowSelectionAABB;
	}

	/*!
	* \brief Set the selection-AABB color
	* 
	* \param col The selection-AABB color
	*/	
	void setSelectionAABBColor(const Color& col)
	{
		MaterialManager::getInstance()->getSelectionAABBMaterial()->setColor(col);
	}

	/*!
	* \brief Return the selection-AABB color
	* 
	* \return Color The selection-AABB color
	*/
	Color getSelectionAABBColor() const
	{
		return MaterialManager::getInstance()->getSelectionAABBMaterial()->getColor();
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
	double getNearPlane() const;
	
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
	double getFarPlane() const;

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
	double getFOV() const;
	
	/*!
	* \brief Set the mouse wheel zoom speed
	* 
	* \param dMouseWheelSpeed The mouse wheel speed
	*/		
	void setMouseWheelSpeed(double dMouseWheelSpeed)
	{
		m_dMouseWheelSpeed = dMouseWheelSpeed;	
	}
	
	/*!
	* \brief Returns the mouse wheel zoom speed
	* 
	* \return double The mouse wheel speed
	*/		
	double getMouseWheelSpeed() const
	{
		return m_dMouseWheelSpeed;	
	}

	/*!
	* \brief Set the scaling of the selection-AABB
	* 
	* \param dScaling The scaling factor
	*/	
	void setSelectionAABBScaling(double dScaling)
	{
		m_dSelectionAABBScaling = dScaling;	
	}

	/*!
	* \brief Return the scaling of the selection-AABB
	* 
	* \return double The scaling factor
	*/		
	double getSelectionAABBScaling() const
	{
		return m_dSelectionAABBScaling;	
	}	

	/*!
	* \brief Set the scaling of the wireframe overlay lines
	* 
	* \param dScaling The scaling factor
	*/
	void setWireframeOverlayScaling(double dScaling)
	{
		m_dWireframeOverlayScaling = dScaling;	
	}

	/*!
	* \brief Return the scaling of the wireframe overlay lines
	* 
	* \return double The scaling factor
	*/	
	double getWireframeOverlayScaling() const
	{
		return m_dWireframeOverlayScaling;	
	}

	/*!
	* \brief Set the major grid line color
	* 
	* \param col The major grid line color
	*/		
	void setGridMajorLineColor(const Color& col)
	{
		MaterialManager::getInstance()->getGridMajorLineMaterial()->setColor(col);
	}

	/*!
	* \brief Return the major grid line color
	* 
	* \return Color The major grid line color
	*/			
	Color getGridMajorLineColor() const
	{
		return MaterialManager::getInstance()->getGridMajorLineMaterial()->getColor();
	}	

	/*!
	* \brief Set the minor grid line color
	* 
	* \param col The minor grid line color
	*/		
	void setGridMinorLineColor(const Color& col)
	{
		MaterialManager::getInstance()->getGridMinorLineMaterial()->setColor(col);
	}

	/*!
	* \brief Return the minor grid line color
	* 
	* \return Color The minor grid line color
	*/	
	Color getGridMinorLineColor() const
	{
		return MaterialManager::getInstance()->getGridMinorLineMaterial()->getColor();
	}
	
	/*!
	* \brief Set the grid size (partitions per decade)
	* 
	* \param nGridSize The grid size
	*/		
	void setGridSize(int nGridSize)
	{
		m_nGridSize = nGridSize;	
	}
	
	/*!
	* \brief Return the grid size (partitions per decade)
	* 
	* \return int The grid size
	*/		
	int getGridSize() const
	{
		return m_nGridSize;
	}
	
	/*!
	* \brief Enable/Disable rendering the backfaces
	* 
	* \param bEnable true to render the backfaces, otherwise false
	*/	
	void setShowBackfaces(bool bEnable)
	{
		m_bShowBackfaces = bEnable;	
	}

	/*!
	* \brief Return if rendering the backfaces is enabled
	* 
	* \return bool true if rendering the backfaces is enabled, otherwise false
	*/		
	bool getShowBackfaces() const
	{
		return m_bShowBackfaces;
	}	
	
private:
	RealtimeMirror m_realtimeMirror; /*!< \brief How to mirror the scene */
	double m_dLineWidth; /*!< \brief Line width */
	double m_dPointSize; /*!< \brief Point size */
	double m_dNormalScaling; /*!< \brief Size of the rendered normal lines */
	bool   m_bLineSmoothing; /*!< \brief Is line smoothing activated? */
	bool   m_bPointSmoothing; /*!< \brief Is point smoothing activated? */
	bool   m_bPolygonSmoothing; /*!< \brief Is polygon smoothing activated? */
	bool   m_bColorByGroups; /*!< \brief Is group coloring activated? */
	bool   m_bShowNormals; /*!< \brief Rendering the normals? */
	bool   m_bFrustumCulling; /*!< \brief Is frustum culling activated? */
	bool   m_bShowSelectionAABB; /*!< \brief Render an AABB around the selection? */
	Color  m_cClearOrtho; /*!< \brief Color for the orthogonal view */
	Color  m_cClearPerspective; /*!< \brief Color for the perspective view */
	double m_dSelectionAABBScaling; /*!< \brief Scaling for the AABB around the selection */
	double m_dWireframeOverlayScaling; /*!< \brief Scaling for the wireframe overlay lines */
	int m_nGridSize; /*!< \brief Grid size */
	double m_dMouseWheelSpeed; /*!< \brief Mouse wheel zoom speed */
	bool m_bShowBackfaces; /*!< \brief Render the backfaces? */

};
	
}

#endif /*RENDERERSETTINGS_H_*/
