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
 *  File:       Subdivision.cpp
 *  Created:    05.05.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/Subdivision.h"
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSScene;
using namespace BSRenderer;

static Subdivision* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Subdivision();

	return static_cast<PlgInt*>(_instance);
}

Subdivision::Subdivision() : version(Version(1, 0, 0))
{
	m_sName = "Subdivision";
	m_sDescription = "Subdivides a mesh via the 'Subdivision loop'-algorithm";

	m_pActionSubdivide = NULL;
}

Subdivision::~Subdivision()
{
}

bool Subdivision::unloadPlg()
{
	if(m_pActionSubdivide != NULL)
	{
		delete m_pActionSubdivide;
		m_pActionSubdivide = NULL;
	}
			
	return true;
}

bool Subdivision::loadPlg()
{
	QMenu* pMenu = NULL;
	
	//Get the menu where to add the new menu items
	getMenu(&pMenu);

	m_pActionSubdivide = pMenu->addAction("Subdivide", this, SLOT(execute()));
    
	return true;
}

void Subdivision::getMenu(QMenu** pMenu)
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

void Subdivision::execute()
{
 	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	
	//Copy original mesh
	vector<Vertex> vertsOriginal;
	vector<int> indicesOriginal;
	int nIndexBias = 0;

	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(unsigned int p=0; p<polys->size(); p++)
			{
				vector<int>* copyIndices = (*polys)[p].getIndices();
				vector<Vertex>* copyVertices = (*polys)[p].getVertices();
				
				nIndexBias = vertsOriginal.size();
				
				for(unsigned int v=0; v<copyVertices->size(); v++)
				{
					vertsOriginal.push_back((*copyVertices)[v]);
				}
				
				for(unsigned int i=0; i<copyIndices->size(); i++)
				{
					indicesOriginal.push_back((*copyIndices)[i]+nIndexBias);	
				}						
					
			}
			
		}
   	}
   	
   	//No verts selected
   	if(indicesOriginal.size() == 0)	
   		return;
	
	//Build up table for beta
	for(int b=0; b<NUMBETATABLE; b++)
		m_betaTable[b] = calcBeta(b, true);
		
	m_progressDialog = new QProgressDialog("Subdivision in progress.", "Cancel", 0, vertsOriginal.size()/3, BSGui::MainWindow::getInstance());
	m_progressDialog->setWindowModality(Qt::WindowModal);
	m_progressDialog->setMinimumDuration(1000);
	m_progressDialog->setValue(0);
	m_progressDialog->setCancelButton(NULL);
	qApp->processEvents();
	
	//Subdivide all polygons
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(unsigned int p=0; p<polys->size(); p++)
			{				
				subdividePolygon(&((*polys)[p]), vertsOriginal, indicesOriginal);
			}
		}
   	}

   	delete m_progressDialog;
   	m_progressDialog = NULL;
   	
   	SceneManager::getInstance()->notifyContentChanged(BSScene::SCENETYPE_Mesh,true);
}

void Subdivision::subdividePolygon(BSScene::Polygon* p, const vector<BSRenderer::Vertex>& vertsOriginal,
		const vector<int> indicesOriginal)
{
	if(p==NULL)
		return;
	
	vector<int> indices;
	vector<Vertex> verts;
	
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	
	Vertex newVerts[12];
	Vertex vert1, vert2, vert3;
	Vertex vert12, vert23, vert13;
	Vector vNorm;
		
	for(unsigned int i=0; i<copyIndices.size(); i+=3)
	{
		
		/*                 v3
		 *                /  \                                            
		 *               /    \                           
		 *              /      \                         
		 *             /        \                        
		 *            /          \                       
		 *       v13 /            \ v23      
		 *          / ************ \               
		 *         / *           *  \              
		 *        /   *         *    \             
		 *       /     *       *      \             
		 *      /       *     *        \            
		 *     /         *   *          \
		 *    /           * *            \
		 *   ------------------------------
		 *  v1            v12             v2
		 *
		 * Every Face needs three new vertices because of their
		 * individual normal vectors */     
		
		vert1 = copyVertices[copyIndices[i+0]];
		vert2 = copyVertices[copyIndices[i+1]];
		vert3 = copyVertices[copyIndices[i+2]];
		
		//Create the new vertex v12, v23 and v13
		vert12.vPos    = (vert1.vPos    + vert2.vPos)    * 0.5;
		vert12.vNormal = (vert1.vNormal + vert2.vNormal) * 0.5;
		
		vert23.vPos    = (vert2.vPos    + vert3.vPos)    * 0.5;
		vert23.vNormal = (vert2.vNormal + vert3.vNormal) * 0.5;
		
		vert13.vPos    = (vert1.vPos    + vert3.vPos)    * 0.5;
		vert13.vNormal = (vert1.vNormal + vert3.vNormal) * 0.5;
		
		for(int t=0; t<MAXTEXTURES; t++)
		{
			vert12.dU[t] = (vert1.dU[t] + vert2.dU[t]) * 0.5;
			vert12.dV[t] = (vert1.dV[t] + vert2.dV[t]) * 0.5;
			
			vert23.dU[t] = (vert2.dU[t] + vert3.dU[t]) * 0.5;
			vert23.dV[t] = (vert2.dV[t] + vert3.dV[t]) * 0.5;
			
			vert13.dU[t] = (vert1.dU[t] + vert3.dU[t]) * 0.5;
			vert13.dV[t] = (vert1.dV[t] + vert3.dV[t]) * 0.5;										
		}
		
	
		vector<Vector> neighbours;
		double dAlpha = 0.0;

		//Calculate translation of v1
		neighbours.clear();
		getNeighbours(vert1.vPos, neighbours, vertsOriginal, indicesOriginal);
		dAlpha = calcAlpha(neighbours.size(), calcBeta(neighbours.size()));
		//vert1.vPos = (dAlpha * vert1.vPos + calcVectorSum(neighbours, vert1.vPos)) / (dAlpha + neighbours.size());
		vert1.vPos = vert1.vPos * (1.0 - neighbours.size()*calcBeta(neighbours.size())) + calcBeta(neighbours.size())*calcVectorSum(neighbours, vert1.vPos);

		//Calculate translation of v2
		neighbours.clear();
		getNeighbours(vert2.vPos, neighbours, vertsOriginal, indicesOriginal);
		dAlpha = calcAlpha(neighbours.size(), calcBeta(neighbours.size()));
		//vert2.vPos = (dAlpha * vert2.vPos + calcVectorSum(neighbours, vert2.vPos)) / (dAlpha + neighbours.size());
		vert2.vPos = vert2.vPos * (1.0 - neighbours.size()*calcBeta(neighbours.size())) + calcBeta(neighbours.size())*calcVectorSum(neighbours, vert2.vPos);	

		//Calculate translation of v3
		neighbours.clear();
		getNeighbours(vert3.vPos, neighbours, vertsOriginal, indicesOriginal);
		dAlpha = calcAlpha(neighbours.size(), calcBeta(neighbours.size()));
		//vert3.vPos = (dAlpha * vert3.vPos + calcVectorSum(neighbours, vert3.vPos)) / (dAlpha + neighbours.size());
		vert3.vPos = vert3.vPos * (1.0 - neighbours.size()*calcBeta(neighbours.size())) + calcBeta(neighbours.size())*calcVectorSum(neighbours, vert3.vPos);
		
		Vector vNorm;
		
		//Create face (v1, v12, v13)
		newVerts[ 0] = vert1;
		newVerts[ 1] = vert12;
		newVerts[ 2] = vert13;
		vNorm = (newVerts[1].vPos - newVerts[0].vPos) ^ (newVerts[2].vPos - newVerts[0].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		newVerts[0].vNormal = newVerts[1].vNormal = newVerts[2].vNormal = vNorm;

		//Create face (v2, v23, v12)
		newVerts[ 3] = vert2;
		newVerts[ 4] = vert23;
		newVerts[ 5] = vert12;		
		vNorm = (newVerts[4].vPos - newVerts[3].vPos) ^ (newVerts[5].vPos - newVerts[3].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		newVerts[3].vNormal = newVerts[4].vNormal = newVerts[5].vNormal = vNorm;		

		//Create face (v3, v13, v23)
		newVerts[ 6] = vert3;
		newVerts[ 7] = vert13;
		newVerts[ 8] = vert23;
		vNorm = (newVerts[7].vPos - newVerts[6].vPos) ^ (newVerts[8].vPos - newVerts[6].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		newVerts[6].vNormal = newVerts[7].vNormal = newVerts[8].vNormal = vNorm;		

		//Create face (v12, v23, v13)
		newVerts[ 9] = vert12;
		newVerts[10] = vert23;
		newVerts[11] = vert13;
		vNorm = (newVerts[10].vPos - newVerts[9].vPos) ^ (newVerts[11].vPos - newVerts[9].vPos);
		if(vNorm != Vector(0.0, 0.0, 0.0))
			vNorm.normalize();
		newVerts[9].vNormal = newVerts[10].vNormal = newVerts[11].vNormal = vNorm;		
		
		//Add new indices
		int nCurIndex = indices.size();
		for(int ind=0; ind<12; ind++)
		{
			indices.push_back(nCurIndex + ind);	
		}
		
		//Add new vertices
		for(int vert=0; vert<12; vert++)
		{
			verts.push_back(newVerts[vert]);	
		}
		
		QString sText = "Subdivision in progress:\nFace " + QString::number(m_progressDialog->value()+1) + " of " + QString::number(vertsOriginal.size()/3);
		m_progressDialog->setLabelText(sText);
		m_progressDialog->layout();
		m_progressDialog->setValue(m_progressDialog->value()+1);
		qApp->processEvents();
	}

	p->setIndices(indices);
	p->setVertices(verts);	

}

void Subdivision::getNeighbours(Vector vPos, vector<Vector>& posNeighbours, const vector<BSRenderer::Vertex>& vertsOriginal,
	const vector<int> indicesOriginal)
{
	//Walk through all faces
	for(unsigned int i=0; i<indicesOriginal.size(); i+=3)
	{
		//First face point is equal to vPos
		if(isVectorNearTo(vertsOriginal[indicesOriginal[i+0]].vPos, vPos, EPSILON))
		{
			//The two other points are neighbours
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+1]].vPos);
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+2]].vPos);
		}
		//Second face point is equal to vPos
		else if(isVectorNearTo(vertsOriginal[indicesOriginal[i+1]].vPos, vPos, EPSILON))
		{
			//The two other points are neighbours
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+0]].vPos);
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+2]].vPos);			
		}
		//Third face point is equal to vPos
		else if(isVectorNearTo(vertsOriginal[indicesOriginal[i+2]].vPos, vPos, EPSILON))
		{
			//The two other points are neighbours
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+0]].vPos);
			addVector(posNeighbours, vertsOriginal[indicesOriginal[i+1]].vPos);			
		} 
	}	
}

void Subdivision::addVector(vector<Vector>& v, const Vector& vec) const
{
	for(unsigned int i=0; i<v.size(); i++)
	{
		if(v[i] == vec)
		{
			return;
		}
	}
	
	v.push_back(vec);
}

bool Subdivision::isVectorNearTo(const Vector& v1, const Vector& v2, double dEpsilon) const
{
	return (isNearTo(v1.x, v2.x, dEpsilon) && isNearTo(v1.y, v2.y, dEpsilon) && isNearTo(v1.z, v2.z, dEpsilon));
}

Vector Subdivision::calcVectorSum(const vector<Vector>& v, const Vector vRelationPos) const
{
	Vector vSum(0.0, 0.0, 0.0);

	for(unsigned int i=0; i<v.size(); i++)
	{
		vSum += v[i];// - vRelationPos;
	}	
	
	return vSum;	
}

double Subdivision::calcBeta(int n, bool bIgnoreTable) const
{
	//n = 3;
	if(n <= 0)
		return 0.0;
	else if((bIgnoreTable == false) && (n < NUMBETATABLE))
		return m_betaTable[n];
	
	
	/*if(n>3)
		return 0.375/n;
	else
		return 0.1875;*/
	
	
	//double dTemp = 3.0 + 2.0 * cos(2.0 * PI / static_cast<double>(n));
	//return 1.25 - dTemp * dTemp * 0.03125;
	
	//return (5.0/4.0 - (3+2*cos(2*PI / n)) * (3+2*cos(2*PI / n)) / 32);
	
	double dTemp = 0.375 + 0.25 * cos(2.0 * PI / static_cast<double>(n));
	return (0.625 - dTemp * dTemp) / static_cast<double>(n);
	
}

double Subdivision::calcAlpha(int n, double dBeta) const
{
	return (static_cast<double>(n) * (1.0 - dBeta)) / dBeta;
}
