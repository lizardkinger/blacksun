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
 *  File:       SplitFace.cpp
 *  Created:    15.05.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/

#include "./../include/SplitFace.h"
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static SplitFace* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new SplitFace();

	return static_cast<PlgInt*>(_instance);
}

SplitFace::SplitFace() : version(Version(1, 0, 0))
{
	m_sName = "Split face";
	m_sDescription = "Splits each selected faces into 2, 3, 4, 5 or 6 subfaces";
	
	m_pSplitFace = NULL;
}

SplitFace::~SplitFace()
{

}

bool SplitFace::unloadPlg()
{
	if(m_pSplitFace != NULL)
	{
		delete m_pSplitFace;
		m_pSplitFace = NULL;
	}
	
	return true;		
}	

bool SplitFace::loadPlg()
{
	m_pSplitFace = NULL;
	QMenu* pMenu = NULL;
	
	//Get the menu and actions where to add the new menu items
	getMenu(&pMenu);

	m_pSplitFace = pMenu->addAction("Split face ...");
	
	//Create sub menu
	QMenu* pSubMenu = new QMenu(pMenu);
	pSubMenu->addAction("... into 2 subfaces", this, SLOT(execute2Subfaces()));
	pSubMenu->addAction("... into 3 subfaces", this, SLOT(execute3Subfaces()));
	pSubMenu->addAction("... into 4 subfaces", this, SLOT(execute4Subfaces()));
	pSubMenu->addAction("... into 5 subfaces", this, SLOT(execute5Subfaces()));
	pSubMenu->addAction("... into 6 subfaces", this, SLOT(execute6Subfaces()));
	
	m_pSplitFace->setMenu(pSubMenu);
	
	return true;
}

void SplitFace::execute(int nNumSplits)
{	
 	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(unsigned int p=0; p<polys->size(); p++)
			{			
				splitFace(&((*polys)[p]), nNumSplits);	
			}
			
			SceneManager::getInstance()->notifyContentChanged(mesh);
		}
   	}
}

void SplitFace::getMenu(QMenu** pMenu)
{
	QList<QAction*> pActions = BSGui::MainWindow::getInstance()->getMenuBar()->actions();

	//Walk through all menu-bar elements
	for(int i=0; i<pActions.size(); i++)
	{
		if(pActions[i]->text() == "Modify")
		{
			(*pMenu) = pActions[i]->menu();
			break;	
		}	
	}
}

void SplitFace::splitFace(BSScene::Polygon* p, int nNumSplits)
{
	if(p==NULL)
		return;
	
	vector<int> indices;
	vector<Vertex> verts;
	
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	
	for(unsigned int i=0; i<copyIndices.size(); i+=3)
	{
		switch(nNumSplits)
		{
			case 2:
			{
				splitFaceTo2(copyVertices[copyIndices[i+0]],
							copyVertices[copyIndices[i+1]],
							copyVertices[copyIndices[i+2]], indices, verts);
			}break;
			case 3:
			{
				splitFaceTo3(copyVertices[copyIndices[i+0]],
							copyVertices[copyIndices[i+1]],
							copyVertices[copyIndices[i+2]], indices, verts);			
			}break;
			case 4:
			{
				splitFaceTo4(copyVertices[copyIndices[i+0]],
							copyVertices[copyIndices[i+1]],
							copyVertices[copyIndices[i+2]], indices, verts);
			}break;
			case 5:
			{
				splitFaceTo5(copyVertices[copyIndices[i+0]],
							copyVertices[copyIndices[i+1]],
							copyVertices[copyIndices[i+2]], indices, verts);
			}break;
			default:
			{
				splitFaceTo6(copyVertices[copyIndices[i+0]],
							copyVertices[copyIndices[i+1]],
							copyVertices[copyIndices[i+2]], indices, verts);
			};	
		}
	}

	p->setIndices(indices);
	p->setVertices(verts);	
}

void SplitFace::splitFaceTo2(const Vertex& v1, const Vertex& v2,
		const Vertex& v3, vector<int>& indRes, vector<Vertex>& vertsRes)
{
	/*                 V2
	 *                /  \                                            
	 *               / *  \                           
	 *              /  *   \                         
	 *             /   *    \                        
	 *            /    *     \                       
	 *           /     *      \      
	 *          /      *       \               
	 *         /       *        \             
	 *        /        *         \             
	 *       /         *          \             
	 *      /          *           \            
	 *     /           *            \
	 *    ----------------------------
	 *  V0             V3             V1
	 *
	 */
	 
 	int nBias = vertsRes.size();	 
	 
	 vertsRes.push_back(v1);
	 vertsRes.push_back(v2);
	 vertsRes.push_back(v3);
	 vertsRes.push_back(averageVertex(v1, v2));
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(3+nBias);
	 indRes.push_back(2+nBias);	
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(1+nBias);
	 indRes.push_back(2+nBias); 	
}

void SplitFace::splitFaceTo3(const Vertex& v1, const Vertex& v2,
		const Vertex& v3, vector<int>& indRes, vector<Vertex>& vertsRes)
{
	/*                 V2
	 *                /  \                                            
	 *               / *  \                           
	 *              /  *   \                         
	 *             /   *    \                        
	 *            /    *     \                       
	 *           /     *      \      
	 *          / F1   *   F3  \               
	 *         /       X        \             
	 *        /     *     *      \             
	 *       /    *    V3   *     \             
	 *      /   *             *    \            
	 *     /  *        F2       *   \
	 *    ----------------------------
	 *  V0                            V1
	 *
	 */
	 
 	int nBias = vertsRes.size();	 
	 
	 vertsRes.push_back(v1);
	 vertsRes.push_back(v2);
	 vertsRes.push_back(v3);
	 vertsRes.push_back(averageVertex(v1, v2, v3));
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(1+nBias);
	 indRes.push_back(3+nBias);	
	 
	 indRes.push_back(1+nBias);
	 indRes.push_back(2+nBias);
	 indRes.push_back(3+nBias);	
	  
	 indRes.push_back(2+nBias);
	 indRes.push_back(0+nBias);
	 indRes.push_back(3+nBias); 
}

void SplitFace::splitFaceTo4(const Vertex& v1, const Vertex& v2,
		const Vertex& v3, vector<int>& indRes, vector<Vertex>& vertsRes)
{
	/*                 V2
	 *                /  \                                            
	 *               / *  \                           
	 *              /  *   \                         
	 *             /   *    \                        
	 *            /    *     \                       
	 *           /     *      \      
	 *       v5 /      *       \  v4              
	 *         / *     *      * \             
	 *        /   *    *     *   \             
	 *       /     *   *    *     \             
	 *      /       *  *   *       \            
	 *     /         * *  *         \
	 *    ----------------------------
	 *  V0             V3             V1
	 *
	 */
	 
 	int nBias = vertsRes.size();	 
	 
	 vertsRes.push_back(v1);
	 vertsRes.push_back(v2);
	 vertsRes.push_back(v3);
	 vertsRes.push_back(averageVertex(v1, v2));
	 vertsRes.push_back(averageVertex(v2, v3));
	 vertsRes.push_back(averageVertex(v1, v3));
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(3+nBias);
	 indRes.push_back(5+nBias);	
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(1+nBias);
	 indRes.push_back(4+nBias);
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(4+nBias);
	 indRes.push_back(2+nBias);	
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(2+nBias);
	 indRes.push_back(5+nBias);	 
}

void SplitFace::splitFaceTo5(const Vertex& v1, const Vertex& v2,
		const Vertex& v3, vector<int>& indRes, vector<Vertex>& vertsRes)
{
	/*                 v2
	 *                /  \                                            
	 *               / *  \                           
	 *              /  *   \                         
	 *             /   *    \                        
	 *            /    *     \                       
	 *        v4 /     *      \ v3      
	 *          / *    *     * \               
	 *         /   *   *   *    \              
	 *        /      * * *       \             
	 *       /       * * *        \             
	 *      /     *    v5   *      \            
	 *     /   *               *    \
	 *    /  *                   *   \
	 *   ------------------------------
	 *  v0                            v1
	 *
	 */
	 
 	int nBias = vertsRes.size();	 
	 
	 vertsRes.push_back(v1);
	 vertsRes.push_back(v2);
	 vertsRes.push_back(v3);
	 vertsRes.push_back(averageVertex(v2, v3));
	 vertsRes.push_back(averageVertex(v1, v3));
	 vertsRes.push_back(averageVertex(v1, v2, v3));
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(1+nBias);
	 indRes.push_back(5+nBias);	
	 
	 indRes.push_back(1+nBias);
	 indRes.push_back(3+nBias);
	 indRes.push_back(5+nBias);
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(2+nBias);
	 indRes.push_back(5+nBias);	
	 
	 indRes.push_back(2+nBias);
	 indRes.push_back(4+nBias);
	 indRes.push_back(5+nBias);	
	 
	 indRes.push_back(4+nBias);
	 indRes.push_back(0+nBias);
	 indRes.push_back(5+nBias);		
}

void SplitFace::splitFaceTo6(const Vertex& v1, const Vertex& v2,
		const Vertex& v3, vector<int>& indRes, vector<Vertex>& vertsRes)
{
	/*                 v2
	 *                /  \                                            
	 *               / *  \                           
	 *              /  *   \                         
	 *             /   *    \                        
	 *            /    *     \                       
	 *        v5 /     *      \ v4      
	 *          / *    *     * \               
	 *         /   *   *   *    \              
	 *        /      * * *       \             
	 *       /       * * *        \             
	 *      /     *    * v6 *      \            
	 *     /   *       *       *    \
	 *    /  *         *         *   \
	 *   ------------------------------
	 *  v0            v3             v1
	 *
	 */
	 
 	int nBias = vertsRes.size();	 
	 
	 vertsRes.push_back(v1);
	 vertsRes.push_back(v2);
	 vertsRes.push_back(v3);
	 vertsRes.push_back(averageVertex(v1, v2));
	 vertsRes.push_back(averageVertex(v2, v3));
	 vertsRes.push_back(averageVertex(v1, v3));
	 vertsRes.push_back(averageVertex(v1, v2, v3));
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(3+nBias);
	 indRes.push_back(6+nBias);	
	 
	 indRes.push_back(3+nBias);
	 indRes.push_back(1+nBias);
	 indRes.push_back(6+nBias);
	 
	 indRes.push_back(1+nBias);
	 indRes.push_back(4+nBias);
	 indRes.push_back(6+nBias);	
	 
	 indRes.push_back(0+nBias);
	 indRes.push_back(6+nBias);
	 indRes.push_back(5+nBias);	
	 
	 indRes.push_back(6+nBias);
	 indRes.push_back(4+nBias);
	 indRes.push_back(2+nBias);	
	 
	 indRes.push_back(6+nBias);
	 indRes.push_back(2+nBias);
	 indRes.push_back(5+nBias);		 	
}

Vertex SplitFace::averageVertex(const Vertex& v1, const Vertex& v2) const
{
	Vertex vert;
	
	vert.vPos = (v1.vPos + v2.vPos) * 0.5;
	vert.vNormal = (v1.vNormal + v2.vNormal) * 0.5;
	if(vert.vNormal != Vector(0.0, 0.0, 0.0))
		vert.vNormal.normalize();
	
	for(int t=0; t<MAXTEXTURES; t++)
	{
		vert.dU[t] = (v1.dU[t] + v2.dU[t]) * 0.5;
		vert.dV[t] = (v1.dV[t] + v2.dV[t]) * 0.5;				
	}			
	
	return vert;
}

Vertex SplitFace::averageVertex(const Vertex& v1, const Vertex& v2,
	const Vertex& v3) const
{
	Vertex vert;
	
	vert.vPos = (v1.vPos + v2.vPos + v3.vPos) / 3.0;
	vert.vNormal = (v1.vNormal + v2.vNormal + v3.vNormal) / 3.0;
	if(vert.vNormal != Vector(0.0, 0.0, 0.0))
		vert.vNormal.normalize();
	
	for(int t=0; t<MAXTEXTURES; t++)
	{
		vert.dU[t] = (v1.dU[t] + v2.dU[t] + v3.dU[t]) / 3.0;
		vert.dV[t] = (v1.dV[t] + v2.dV[t] + v3.dV[t]) / 3.0;				
	}			
	
	return vert;
}
