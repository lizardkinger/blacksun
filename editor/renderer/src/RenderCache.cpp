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
 *  File:       RenderCache.cpp
 *  Created:    04.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#include "../include/RenderCache.h"

namespace BSRenderer
{
	
RenderCache* RenderCache::m_instance = NULL;

RenderCache* RenderCache::getInstance()
{
	static RenderCacheWaechter w;

	if(m_instance == NULL)
	{
		m_instance = new RenderCache();
	}
	
	return m_instance;
}	

RenderCache::RenderCache()
: m_nMaxVertsPerCache(9000)
{
	
}

void RenderCache::render(PolygonMode polymode, PrimitiveType type, vector<Vertex>& verts, 
			int nMaterialID, const Matrix& mat, bool bSwapFaceOrder)
{
	//Nothing to render
	if(verts.size() == 0)
		return;
		
	//Check for data correctness
	switch(type)
	{
		case PRIM_Triangle:
		{
			//There have to be pairs of three indices
			if((verts.size() < 3) || ((verts.size()%3) != 0))
			{
				//TODO: Log-Meldung
				return;	
			} 		
		}break;
		case PRIM_Line:
		{
			//There have to be pairs of three indices
			if((verts.size() < 2) || ((verts.size()%2) != 0))
			{
				//TODO: Log-Meldung
				return;
			}
		}break;
		default:
		{
			//Points have no limits, they can be rendered separately
		}break;
	}	
	
	//Find the cache object to add the vertices in
	CacheObject* curObject = NULL;
	
	//Try to find a free cache object
	for(vector<CacheObject*>::iterator it = m_cacheObjects.begin(); it != m_cacheObjects.end(); ++it)
	{
		CacheObject* temp = *it;
		
		if(temp != NULL)
			continue;
			
		//Does the cache object saves the right informations?
		if((temp->getPrimitiveType() == type) && (temp->getMaterial() == nMaterialID) && 
			(temp->getMatrix() == mat) && (temp->getNumVerts() < m_nMaxVertsPerCache) &&
			(temp->getPolygonMode() == polymode) && (temp->getSwapFaceOrder() == bSwapFaceOrder))
		{
			//A free cache object was found
			curObject = temp;
			break;	
		}	
	}
	
	//No cache object was found, so we have to create a new one
	if(curObject == NULL)
	{
		curObject = new CacheObject(polymode, type, nMaterialID, mat, bSwapFaceOrder);
		m_cacheObjects.push_back(curObject);
	}
	
	//Add all vertices in the cache
	for(vector<Vertex>::iterator it = verts.begin(); it != verts.end(); ++it)
	{
		//There is no more free space in the cache, we have to create a new one
		if(curObject->getNumVerts() >= m_nMaxVertsPerCache)
		{
			curObject = new CacheObject(polymode, type, nMaterialID, mat, bSwapFaceOrder);
			m_cacheObjects.push_back(curObject);	
		}
		
		curObject->addVertex(*it);
	}
}

void RenderCache::render(PolygonMode polymode, PrimitiveType type, vector<Vertex>& verts, 
			vector<int>& indices, int nMaterialID, const Matrix& mat, bool bSwapFaceOrder)
{
	//Nothing to render
	if(indices.size() == 0)
		return;
		
	//Check for data correctness
	switch(type)
	{
		case PRIM_Triangle:
		{
			//There have to be pairs of three indices
			if((indices.size() < 3) || ((indices.size()%3) != 0))
			{
				//TODO: Log-Meldung
				return;	
			} 		
		}break;
		case PRIM_Line:
		{
			//There have to be pairs of three indices
			if((indices.size() < 2) || ((indices.size()%2) != 0))
			{
				//TODO: Log-Meldung
				return;
			}
		}break;
		default:
		{
			//Points have no limits, they can be rendered separately
		}break;
	}	
	
	//Find the cache object to add the vertices in
	CacheObject* curObject = NULL;
	
	//Try to find a free cache object
	for(vector<CacheObject*>::iterator it = m_cacheObjects.begin(); it != m_cacheObjects.end(); ++it)
	{
		CacheObject* temp = *it;
		
		if(temp != NULL)
			continue;
			
		//Does the cache object saves the right informations?
		if((temp->getPrimitiveType() == type) && (temp->getMaterial() == nMaterialID) && 
			(temp->getMatrix() == mat) && (temp->getNumVerts() < m_nMaxVertsPerCache) &&
			(temp->getPolygonMode() == polymode) && (temp->getSwapFaceOrder() == bSwapFaceOrder))
		{
			//A free cache object was found
			curObject = temp;
			break;	
		}	
	}
	
	//No cache object was found, so we have to create a new one
	if(curObject == NULL)
	{
		curObject = new CacheObject(polymode, type, nMaterialID, mat, bSwapFaceOrder);
		m_cacheObjects.push_back(curObject);
	}
	
	//Add all vertices in the cache
	for(vector<int>::iterator it = indices.begin(); it != indices.end(); ++it)
	{
		//There is no more free space in the cache, we have to create a new one
		if(curObject->getNumVerts() >= m_nMaxVertsPerCache)
		{
			curObject = new CacheObject(polymode, type, nMaterialID, mat, bSwapFaceOrder);
			m_cacheObjects.push_back(curObject);	
		}
		
		curObject->addVertex(verts[*it]);
	}
}
						
int RenderCache::flush()
{
	int nRenderedVerts = 0;
	
	//TODO: Sort cache-bjects
	
	//Render all none-alpha objects first
	for(vector<CacheObject*>::iterator it = m_cacheObjects.begin(); it != m_cacheObjects.end(); ++it)
	{
		//Is it none-alpha?
		Material* pMat = MaterialManager::getInstance()->getMaterial((*it)->getMaterial());
		if(pMat != NULL)
		{
			if(pMat->hasAlpha())
			{
				continue;
			}
		}	
			
		nRenderedVerts += (*it)->getVerts().size();
		renderCacheObject(*it);
		
		//We don't need the data anymore
		//delete (*it);
	}

	//Render all other (alpha) objects and delete the caches
	for(vector<CacheObject*>::iterator it = m_cacheObjects.begin(); it != m_cacheObjects.end(); ++it)
	{
		//Is it none-alpha?
		Material* pMat = MaterialManager::getInstance()->getMaterial((*it)->getMaterial());
		if(pMat != NULL)
		{
			if(pMat->hasAlpha()==false)
			{
				//We don't need the data anymore
				delete (*it);
								
				continue;
			}
		}
				
		nRenderedVerts += (*it)->getVerts().size();
		renderCacheObject(*it);
		
		//We don't need the data anymore
		delete (*it);
	}
	
	m_cacheObjects.clear();
	
	return nRenderedVerts;
}

void RenderCache::renderCacheObject(CacheObject* pObject)
{	
	//Save for better performance
	vector<Vertex>& verts = pObject->getVerts();
	int nNumVerts = verts.size();
	
	//Deactive all textur stages
	for(int i=0; i<MAXTEXTURES; i++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+i);
		glDisable(GL_TEXTURE_2D);
	}	
	
	//Set the material
	Material* pMat = MaterialManager::getInstance()->getMaterial(pObject->getMaterial());
	pMat->set();
	
	//Get the number of valid textures and the valid textur stages
	int nNumValidTextures = pMat->getNumValidTextures();
	int* nValidTextures = new int[nNumValidTextures];
	pMat->getValidTextureStages(nValidTextures);
	
	//Set polygon-mode and lightning
	if(pObject->getPolygonMode()==POLYMODE_Fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	else if(pObject->getPolygonMode()==POLYMODE_Wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDisable(GL_LIGHTING);
	}
	
	//Is texturing really needed?
	Renderer* r = Renderer::getInstance();
	if((r->getFillMode() != FILL_Textured) && (r->getFillMode() != FILL_WireframeOverlay))
	{
		nNumValidTextures = 0;
		//Deactive all textur stages
		for(int i=0; i<MAXTEXTURES; i++)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB+i);
			glDisable(GL_TEXTURE_2D);
		}
	}
	
	//Enable face-culling?
	if((r->getFillMode() == FILL_Wireframe) ||
		(r->getFillMode() == FILL_Point) ||
		(pMat->hasAlpha()==false && r->getSettings()->getShowBackfaces()))
	{
		glDisable(GL_CULL_FACE);	
	}
	else
	{
		glEnable(GL_CULL_FACE);	
	}
		
	glPushMatrix();
	
	//Set the matrix
	//pObject->getMatrix().transpose();
	glMultMatrixd(pObject->getMatrix());
	
	//Calc start pass (alpha needs 2 passes, otherwise 1)
	UINT nPassStart = 0;
	if(pMat->hasAlpha()==false)
	{
		nPassStart = 1;
	}
	
	for(UINT nPass = nPassStart; nPass < 2; nPass++)
	{
		//Pass 0 = CW (needed for alpha)
		//Pass 1 = CCW
		if(nPass==0)
		{
			if(pObject->getSwapFaceOrder())
			{
				glFrontFace(GL_CCW);
			}
			else
			{
				glFrontFace(GL_CW);
			}
		}
		else
		{
			if(pObject->getSwapFaceOrder())
			{
				glFrontFace(GL_CW);
			}
			else
			{
				glFrontFace(GL_CCW);
			}
		}
				
		//Start rendering
		if(pObject->getPrimitiveType()==PRIM_Triangle)
			glBegin(GL_TRIANGLES);
		else if(pObject->getPrimitiveType()==PRIM_Line)
			glBegin(GL_LINES);
		else 
			glBegin(GL_POINTS);
		
		//Render all vertices
		int t = 0;
		glColor4dv(pMat->getColor());
		for(int v=0; v < nNumVerts; v++)
		{
			//TODO: add color, if needed (GroupColoring, ...)
			for (t = 0; t < nNumValidTextures; t++)
			{
				glMultiTexCoord2dARB(GL_TEXTURE0_ARB+t, verts[v].dU[nValidTextures[t]], 
					verts[v].dV[nValidTextures[t]]); //Texture-Coordinate
			}
			glNormal3dv(verts[v].vNormal); //Normal-vector
			glVertex3dv(verts[v].vPos); //Position
		}
		
		//End rendering
		glEnd();
	}
	glPopMatrix();
	
	delete[] nValidTextures;
}
	
}
