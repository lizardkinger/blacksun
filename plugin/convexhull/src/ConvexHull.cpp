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
 *  File:       ConvexHull.cpp
 *  Created:    14.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/ConvexHull.h"
//#include <SceneGraph.h>
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"
#include <set>

using namespace BSRenderer;

static ConvexHull* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new ConvexHull();

	return static_cast<PlgInt*>(_instance);
}

ConvexHull::ConvexHull() : version(Version(1, 0, 0))
{
	m_sName = "ConvexHull";
	m_sDescription = "Creates a triangulated convex hull from a ";
	m_sDescription += "number of selected vertices. The convex hull ";
	m_sDescription += "will be created as a new mesh.";
	
	button = NULL;
}

ConvexHull::~ConvexHull()
{
}

bool ConvexHull::unloadPlg()
{
	if(button != NULL)
	{
		delete button;
		button = NULL;
	}
			
	return true;
}

bool ConvexHull::loadPlg()
{	
	int nButtonSize = 32;
	int nIconSize = 24;
	
	button = new QPushButton(QIcon(":/media/ConvexHull_32.png"), "");
	button->setMaximumSize(nButtonSize, nButtonSize);
	button->setMinimumSize(nButtonSize, nButtonSize);
	button->setIconSize(QSize(nIconSize, nIconSize));
	button->setToolTip("Create convex hull");	
	
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", button);
    
    connect(button, SIGNAL(clicked()), this, SLOT(execute()));
	return true;
}

void ConvexHull::execute()
{
	//Set vor all points, without dublicates
	set<Vector> tempPoints;
	
	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	list<SelectedVertex>& selVerts = sb.getSelectedVertices();
	
	//Walk through all selected objects
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(vector<BSScene::Polygon>::iterator itPolys = polys->begin(); itPolys != polys->end(); ++itPolys)
			{			
				vector<Vertex>*	verts = itPolys->getVertices();
				
				//Walk through all vertices
				for(unsigned int v=0; v<verts->size(); v++)
				{
					tempPoints.insert((*verts)[v].vPos);						
				}
			}
		}
   	}
   	
   	//Walk through all selected vertices
	for(list<SelectedVertex>::iterator it = selVerts.begin(); it != selVerts.end(); ++it) 
	{
		SelectedVertex *sVert = &(*it);
		
		if(sVert != NULL)
		{
			tempPoints.insert(sVert->getVertex()->vPos);	
		}
	}   	
	
	
	vector<Vector> points;
	
	//Insert all points in the point-list
	for(set<Vector>::iterator it = tempPoints.begin(); it != tempPoints.end(); ++it)
	{
		points.push_back(*it);
	}
	
	tempPoints.clear();
	
	//There aren't enough points
	if(points.size() < 3)
		return;	

	Mesh m;
	m.setName(tr("ConvexHull"));
	BSScene::Polygon p;
	vector<int> indices; 
	vector<Vertex> verts;
	
	//Create the convex hull
	createConvexHull(points, indices, verts);
	
	//No indices -> no mesh
	if(indices.size() == 0)
		return;	
	
	//Add vertices and indices to the mesh
	p.setVertices(verts);
	p.setIndices(indices);
	m.addPolygon(p);
	
	//Add new mesh to the scene
	SceneManager::getInstance()->insertObject(m);	
}

void ConvexHull::createConvexHull(const vector<Vector>& points, vector<int>& indices, 
		vector<Vertex>& verts)
{
	//There are no vertices, so do nothing
	if(points.size() < 3)
		return;
		
	//Its a face, so create the face
	if(points.size() == 3)
	{
		verts.push_back(Vertex(points[0]));
		verts.push_back(Vertex(points[1]));
		verts.push_back(Vertex(points[2]));
		
		indices.push_back(indices.size());
		indices.push_back(indices.size());
		indices.push_back(indices.size());	
	}
		
	stack<Edge> edges;
	
	//Find the two first points to create the first edge
	Vector vBottom = findBottomPoint(points);
	Vector vEdgePoint = findFirstEdgePoint(points, vBottom);
	
	//Add the edge from Bottom to EdgePoint and vice versa
	edges.push(Edge(vBottom, vEdgePoint));
	edges.push(Edge(vEdgePoint, vBottom));
	
	//Set the variable for the current edge
	Edge e = Edge(vBottom, vEdgePoint);
	
	//Find new edges until there are are no more edges to check
	while(!edges.empty())
	{		
		//Get the next edge and delete it from stack
		e = edges.top();
		edges.pop();	
		
		//Find the new point for creating a new edge and face
		Vector newEdgePoint = findEdgePoint(points, e);
		
		//Add new face
		if(addFace(indices, verts, e.vStart, e.vEnd, newEdgePoint))
		{
			//Put new edges to the stack and avoid adding one edges two times
			putOnStack(edges, Edge(e.vStart, newEdgePoint));
			putOnStack(edges, Edge(newEdgePoint, e.vEnd));
		}
	}
}

Vector ConvexHull::findBottomPoint(const vector<Vector>& points)
{
	//Take first point as bottom point
	Vector vBottom = points[0];
	
	for(unsigned int p=1; p<points.size(); p++)
	{
		//Is the current point bottom?
		if(points[p].y < vBottom.y)
			vBottom = points[p];
	}
	
	return vBottom;
}

Plane<double> ConvexHull::create2DPlane(const Vector& v1, const Vector& v2)
{
	//Calculte the normal vector
	Vector norm = (v2 - v1) ^ Vector(0.0, 0.0, 1.0);
	if(norm != Vector(0.0, 0.0, 0.0))
		norm.normalize();
		
	return Plane<double>(norm,  norm * v1);	
}

Plane<double> ConvexHull::createPlane(const Vector& v1, const Vector& v2,
	const Vector& v3)
{
	//Calculte the normal vector
	Vector norm = (v2 - v1) ^ (v3 - v1);
	if(norm != Vector(0.0, 0.0, 0.0))
		norm.normalize();
		
	return Plane<double>(norm,  norm * v1);	
}

Vector ConvexHull::findFirstEdgePoint(const vector<Vector>& points, 
	const Vector& vBottom)
{
	Vector vEdgePoint = points[0];
	int nFirstIndex = 0;
	
	//Check if the two points are the same.
	if(points[0] == vBottom)
	{
		//Choose the next point
		nFirstIndex = 1;
		vEdgePoint = points[1];
	}
	
	//Create a plane
	Plane<double> plane = create2DPlane(vBottom, vEdgePoint);
	
	//Walk through all points
	for(unsigned int p=nFirstIndex; p<points.size(); p++)
	{
		//Check if the point is in front of the plane
		if((points[p] != vBottom) && (plane.classifyPoint(points[p]) == PPC_Front))
		{
			//New edge point found
			vEdgePoint = points[p];
			
			//Create a new plane with the new edge point 
			plane = create2DPlane(vBottom, vEdgePoint);
		}	
	}
	
	return vEdgePoint;	
}

void ConvexHull::putOnStack(stack<Edge>& stack, const Edge& e)
{
	//Convert the stack to a list
	list<Edge> edges;
	while(stack.size() != 0)
	{
		edges.push_front(stack.top());
		stack.pop();	
	}
	
	//Walk through all edges and find the edge e
	list<Edge>::iterator it;
	for(it = edges.begin(); it != edges.end(); it++)
	{
		if((*it) == e)
			break;
	}
	
	//Delete the edge from the list, if possible
	if(it != edges.end())
	{
		edges.erase(it);	
	}
	
	//Build the stack
	for(it = edges.begin(); it != edges.end(); it++)
	{
		stack.push(*it);
	}
	
	//No edge deleted, so push it in the stack
	if(it == edges.end())
	{
		stack.push(e);
	}
}

Vector ConvexHull::findEdgePoint(const vector<Vector>& points, const Edge& e)
{
	int nIndex = 0;
	
	//Find start index
	for(nIndex = 0; points[nIndex] == e.vStart || points[nIndex] == e.vEnd; nIndex++)
		;
	
	Vector vEdgePoint = points[nIndex];
	
	//Create a plane
	Plane<double> plane = createPlane(e.vStart, e.vEnd, vEdgePoint);
	
	//Walk through all points
	for(unsigned int p=nIndex+1; p<points.size(); p++)
	{
		//Check if the point is in front of the plane
		if((points[p] != e.vStart) && (points[p] != e.vEnd) &&
			(plane.classifyPoint(points[p]) == PPC_Front))
		{			
			//New edge point found
			vEdgePoint = points[p]; 
			
			//Create a new plane with the new edge point
			plane = createPlane(e.vStart, e.vEnd, vEdgePoint);
		}	
	}
	
	return vEdgePoint;
}

bool ConvexHull::addFace(vector<int>& indices, vector<Vertex>& verts, const Vector& v1,
	const Vector& v2, const Vector& v3)
{
	//Check if the face allready exists
	for(unsigned int f=0; f<(verts.size()/3); f++)
	{
		//Check all combinations of v1, v2 and v3
		if((verts[f*3+0].vPos == v1 || verts[f*3+0].vPos == v2 || verts[f*3+0].vPos == v3) && 
		   (verts[f*3+1].vPos == v1 || verts[f*3+1].vPos == v2 || verts[f*3+1].vPos == v3) && 
		   (verts[f*3+2].vPos == v1 || verts[f*3+2].vPos == v2 || verts[f*3+2].vPos == v3))
			return false;

	}
	
	//Calculate normal
	Vector vNormal = (v2 - v1) ^ (v3 - v1);
	if(vNormal != Vector(0.0, 0.0, 0.0))
		vNormal.normalize();
	
	//Create vertices
	verts.push_back(Vertex(v1, vNormal));
	verts.push_back(Vertex(v2, vNormal));
	verts.push_back(Vertex(v3, vNormal));
	
	//Create indices
	indices.push_back(indices.size());
	indices.push_back(indices.size());
	indices.push_back(indices.size());
	
	return true;
}
