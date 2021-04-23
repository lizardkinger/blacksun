/***************************************************************************
 *   Copyright (C) 2006 by Thomas Kuhndörfer
 *   tkuhndo@fh-landshut.de
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
 *  Module:     Scenegraph (BlackSun)
 *  File:       Polygon.cpp
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/

#include "./include/Polygon.h"
#include "./include/SelectionBuffer.h"
#include "./include/Scene.h"
#include "./include/SceneManager.h"
#include "./include/Mesh.h"
#include "./include/SceneUtils.h"  


namespace BSScene
{

Polygon::Polygon() : SceneObject()
{
	init();
}

Polygon::Polygon(const Polygon &copy) : SceneObject()
{
	init(copy);
}

Polygon&	Polygon::operator=(const Polygon& copy)
{
	if(this != &copy)
	{
		release();
		init(copy);
	}
	return *this;
}

Polygon::~Polygon()
{
	release();
}

bool Polygon::operator==(const SceneObject &so) const
{
	if( this == &so )
		return true;
	else 
		return false;
}

void Polygon::init() 
{
	m_type = SCENETYPE_Polygon;
	m_materialID = 0;
	m_parentID.clear();
	m_bHidden = false;
	m_bFreezed = false;
	m_bSelected = false; 
	
	m_textureOffset[0] = m_textureOffset[1] = 0.0;
	m_textureRepeat[0] = m_textureRepeat[1] = 1.0;
	
	m_vertices.clear();
	m_indices.clear();
}

void Polygon::init(const Polygon& copy)
{
	init();
	
	for(vector<Vertex>::const_iterator it = copy.m_vertices.begin(); it != copy.m_vertices.end(); ++it)
		m_vertices.push_back(*it);
		
    for(vector<int>::const_iterator it = copy.m_indices.begin(); it != copy.m_indices.end(); ++it)
		m_indices.push_back(*it);        

  	m_parentID = copy.m_parentID;
         
  	m_materialID = copy.m_materialID; 
  	
  	m_bHidden = copy.m_bHidden;
  	m_bFreezed = copy.m_bFreezed;
  	
  	m_boundary = copy.m_boundary;

  
  	m_textureOffset[0] = copy.m_textureOffset[0];
  	m_textureOffset[1] = copy.m_textureOffset[1];       
  	m_textureRepeat[0] = copy.m_textureRepeat[0];
  	m_textureRepeat[1] = copy.m_textureRepeat[1];  
   
}

void Polygon::release() 
{
	m_vertices.clear();
	m_indices.clear();
	m_parentID.clear();
}


void Polygon::setVertices(const vector<Vertex> &v)
{
	assert(v.size()>0);
	
	m_vertices.clear();
	
	for(vector<Vertex>::const_iterator it = v.begin(); it != v.end(); ++it)
		m_vertices.push_back(*it);
		
	forwardChanges();
		
	assert(m_vertices.size()>0);
}

void Polygon::setIndices(const vector<int> &v)
{
	assert(v.size()%3==0);
	
	m_indices.clear();
		
	for(vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
		m_indices.push_back(*it);
		
	assert(m_indices.size()%3==0);
}

void Polygon::triangulate( bool ccw)
{
	assert(m_vertices.size()==3);

	if(!ccw)
	{
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
	else
	{
		m_indices.push_back(2);
		m_indices.push_back(1);
		m_indices.push_back(0);
	}
}


void Polygon::changeBias()
{

	vector<int> reverse;

	for(vector<int>::reverse_iterator it = m_indices.rbegin(); it != m_indices.rend(); ++it)
		reverse.push_back(*it);
		
	setIndices(reverse);

}

void Polygon::render(Renderer& r)
{
	assert(m_vertices.size()>0);
	
	if(m_bHidden)
		return;
	  
	Matrix matIdent;
	matIdent.identity();
	
	if(!m_bSelected && !m_bFreezed)	
	{
		//qDebug() << "Render normal: " << m_materialID;
		r.render(PRIM_Triangle, m_vertices, m_indices, m_materialID, matIdent, m_boundary);
	}
	else if(m_bFreezed)
	{
		//qDebug() << "Render freeze: " << m_materialID;
		r.renderFreezed(PRIM_Triangle, m_vertices, m_indices, m_materialID, matIdent, m_boundary);
	}
	else
	{
		//qDebug() << "Render selected: " << m_materialID;
		r.renderSelection(PRIM_Triangle, m_vertices, m_indices, m_materialID, matIdent, m_boundary);
	}
	
	//r.renderAABB(m_boundary);

	//SceneUtils::showAabb(QString("Polygon::render"),m_boundary);
}


void Polygon::setAsPartOfMesh(const QString& pName)
{
	m_parentID = pName;
}


void Polygon::calcNormals()
{
	assert(m_vertices.size()==3);
	assert(m_indices.size()==3);
	
	Vector v1, v2, v3, vNormal;

	int idxV1 = m_indices.at(0);
	v1.set( m_vertices.at(idxV1).vPos.x,m_vertices.at(idxV1).vPos.y,m_vertices.at(idxV1).vPos.z);
	
	int idxV2 = m_indices.at(1);
	v2.set( m_vertices.at(idxV2).vPos.x,m_vertices.at(idxV2).vPos.y,m_vertices.at(idxV2).vPos.z);
	
	int idxV3 = m_indices.at(2);
	v3.set( m_vertices.at(idxV3).vPos.x,m_vertices.at(idxV3).vPos.y,m_vertices.at(idxV3).vPos.z);

   	vNormal.cross((v2-v1),(v3-v1));
   	vNormal.normalize();

	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		it->vNormal.x = vNormal.x;
		it->vNormal.y = vNormal.y;	
		it->vNormal.z = vNormal.z;		
	}
	
}


void Polygon::forwardChanges()
{
	calcBoundingBox();
 
   	if(isPartOfMesh())
   	{
   		assert(m_parentID.size()>0);
   		SceneManager::getInstance()->getScene().calcBoundingBoxByName(m_parentID);
   	}
}

void Polygon::calcBoundingBox()
{
	
	Vector vecMax, vecMin;

   	vecMax.x = vecMin.x = m_vertices.at(0).vPos.x;
   	vecMax.y = vecMin.y = m_vertices.at(0).vPos.y;
   	vecMax.z = vecMin.z = m_vertices.at(0).vPos.z;

	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
		SceneUtils::compareMinMax(*it,vecMax,vecMin);

	m_boundary.setMaximum(vecMax);
	m_boundary.setMinimum(vecMin);
	m_boundary.setCentre((vecMax + vecMin)/2.0) ;
	
	//this->print();
	//SceneUtils::showAabb(QString("Polygon::calcBoundingBox()"),m_boundary);
	
	
}

void Polygon::move(const Vector &v)
{
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		it->vPos.x += v.x;
		it->vPos.y += v.y;
		it->vPos.z += v.z;
	}

	m_boundary.setMaximum(m_boundary.getMaximum() + v);
	m_boundary.setMinimum(m_boundary.getMinimum() + v);
	m_boundary.setCentre((m_boundary.getMaximum() + m_boundary.getMinimum())/2.0) ;

	forwardChanges();
} 

void Polygon::rotate(SceneAxis axis,const Vector& position, float amount)
{
	Matrix matRotate;
	Vector temp, tempN;
	
	switch(axis)
	{
		case AXIS_x:	matRotate = Matrix::makeRotateX(BSMath::degToRad(amount)); 
		break;
		
		case AXIS_y:	matRotate = Matrix::makeRotateY(BSMath::degToRad(amount)); 
		break;
		
		case AXIS_z:	matRotate = Matrix::makeRotateZ(BSMath::degToRad(amount));
		break;
	}
	
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		// Rotate vertex position
		temp.x = it->vPos.x - position.x;
      	temp.y = it->vPos.y - position.y;
      	temp.z = it->vPos.z - position.z;

      	temp = matRotate * temp;

      	it->vPos.x = temp.x + position.x;
      	it->vPos.y = temp.y + position.y;
      	it->vPos.z = temp.z + position.z;
      	
      	// Rotate vertex normals
        it->vNormal = matRotate * it->vNormal;
	}
   	
	forwardChanges();
}


void Polygon::mirror(const SceneAxis axis,const Vector& position)
{
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		it->vPos.x -= position.x;
      	it->vPos.y -= position.y;
      	it->vPos.z -= position.z;

		switch(axis)
		{
			case AXIS_x:	it->vPos.x *= -1; 
							it->vNormal.x *= -1;
							break;
							
			case AXIS_y:	it->vPos.y *= -1;
							it->vNormal.y *= -1;			
							break;
							
			case AXIS_z:	it->vPos.z *= -1;	
							it->vNormal.z *= -1;
							break;							
		}

		it->vPos.x += position.x;
      	it->vPos.y += position.y;
      	it->vPos.z += position.z;
	}
   
   	this->changeBias();
	forwardChanges();
}

void Polygon::mirrorWorld(const SceneAxis axis)
{
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		switch(axis)
		{
			case AXIS_x:	it->vPos.z *= -1;	
							it->vNormal.z *= -1;								
							break;
							
			case AXIS_y:	it->vPos.x *= -1;
							it->vNormal.x *= -1;	
							break;
							
			case AXIS_z:	it->vPos.y *= -1;	
							it->vNormal.y *= -1;
							break;		
		}
	}
   
	this->changeBias();
	forwardChanges();
}

void Polygon::calcTextureCoords(const SceneAxis axis,const Aabb* pAabb)
{
   	const Aabb *local_aabb;

   	if (pAabb) 
   		local_aabb = pAabb;
   	else 
   		local_aabb = &m_boundary;
	
   m_textureOffset[0] = m_textureOffset[1] = 0.0f;
   m_textureRepeat[0] = m_textureRepeat[1] = 1.0f;

   Vector vecSize = local_aabb->getMaximum() - local_aabb->getMinimum();

	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		Vertex *pCurrent = &(*it);
		
		switch(axis)
		{
			case AXIS_x:	
				pCurrent->dU[0] = (pCurrent->vPos.z - local_aabb->getMinimum().z) / vecSize.z;
				pCurrent->dV[0] = (pCurrent->vPos.y - local_aabb->getMaximum().y) / vecSize.y;
				break;
						
			case AXIS_y:	
				pCurrent->dU[0] = (pCurrent->vPos.x - local_aabb->getMinimum().x) / vecSize.x;
				pCurrent->dV[0] = (pCurrent->vPos.z - local_aabb->getMaximum().z) / vecSize.z;
				break;	
						
			case AXIS_z:	
				pCurrent->dU[0] = (pCurrent->vPos.x - local_aabb->getMinimum().x) / vecSize.x;
				pCurrent->dV[0] = (pCurrent->vPos.y - local_aabb->getMaximum().y) / vecSize.y;
				break;	
		}
	}
}

void Polygon::transTextureCoords(double dtU, double dtV, double drU, double drV)
{
	if (drU == 0.0f) 
		drU = 0.00001;
		
   	if (drV == 0.0f) 
   		drV = 0.00001;

	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		Vertex *pCurrent = &(*it);
		
		      // undo old offset and repeat
		if (m_textureOffset[0] != 0.0)
		{   
			pCurrent->dU[0] -= m_textureOffset[0];
		}
		 
	    if (m_textureOffset[1] != 0.0)
	    {
	        pCurrent->dV[0] -= m_textureOffset[1];
	    }
	      
	    if ((m_textureRepeat[0] != 1.0) && (m_textureRepeat[0] != 0.0) ) 
	    {
	       pCurrent->dU[0] /= m_textureRepeat[0];
	    }
	      
	    if ((m_textureRepeat[1] != 1.0) &&  (m_textureRepeat[1] != 0.0) ) 
	    {
	       pCurrent->dV[0] /= m_textureRepeat[1];
	    }

	    // set to new values
	    pCurrent->dU[0] = pCurrent->dU[0] * drU + dtU;
	    pCurrent->dV[0] = pCurrent->dV[0] * drV + dtV;
	}
	
	m_textureOffset[0]=dtU;
	m_textureOffset[1]=dtV;
	
	m_textureRepeat[0]=drU;
	m_textureRepeat[1]=drV; 
}
  	
  	
void Polygon::getTextureTrans(double *pdtU, double *pdtV,double *pdrU, double *pdrV)
{
	if (pdtU) 
   		*pdtU = m_textureOffset[0];
   		
   	if (pdtV) 
   		*pdtV = m_textureOffset[1];
   		
   	if (pdrU) 
   		*pdrU = m_textureRepeat[0];
   		
   	if (pdrV) 
   		*pdrV = m_textureRepeat[1];
}


bool Polygon::intersects(const Aabb& boundary,const  SceneAxis axis) 
{
	//SceneUtils::showAabb(QString("Polygon::intersects"),m_boundary);
	//SceneUtils::showAabb(QString("Selektion"),boundary);
	
	if(!Intersection::AabbIntersects(m_boundary,boundary))
		return false;

	for(unsigned int i=0; i<m_indices.size(); i+=3)
	{
		Vertex &vert1 = m_vertices[ m_indices[i + 0] ];
		Vertex &vert2 = m_vertices[ m_indices[i + 1] ];
		Vertex &vert3 = m_vertices[ m_indices[i + 2] ];

		if(isVertexIntersecting(vert1,axis,boundary) ||
		   isVertexIntersecting(vert2,axis,boundary) ||
		   isVertexIntersecting(vert3,axis,boundary))
			return true;

		Vector vec1 = vert1.vPos;
		Vector vec2 = vert2.vPos;
		Vector vec3 = vert3.vPos;
		
		Plane<double> plane1_cw = SceneUtils::create2DPlane(vec1,vec2,axis);
		Plane<double> plane2_cw = SceneUtils::create2DPlane(vec2,vec3,axis);
		Plane<double> plane3_cw = SceneUtils::create2DPlane(vec3,vec1,axis);
		
		Plane<double> plane1_ccw = SceneUtils::create2DPlane(vec1,vec3,axis);
		Plane<double> plane2_ccw = SceneUtils::create2DPlane(vec3,vec2,axis);
		Plane<double> plane3_ccw = SceneUtils::create2DPlane(vec2,vec1,axis);

		if(isPlaneIntersecting(plane1_cw,plane2_cw,plane3_cw,boundary,axis) ||
		   isPlaneIntersecting(plane1_ccw,plane2_ccw,plane3_ccw,boundary,axis))
			return true;
	}

   return false;
}

bool Polygon::isPlaneIntersecting(const Plane<double> &planeA, const Plane<double> &planeB, const Plane<double> &planeC,
						  const  Aabb& aabb, const SceneAxis axis)
{
	Vector p1,p2,p3,p4;
	
	switch(axis)
	{
		case AXIS_x:
			p1 = Vector(0, aabb.getMinimum().x, aabb.getMinimum().z);
			p2 = Vector(0, aabb.getMaximum().x, aabb.getMinimum().z);
			p3 = Vector(0, aabb.getMinimum().x, aabb.getMaximum().z);
			p4 = Vector(0, aabb.getMaximum().x, aabb.getMaximum().z);	
			break;
		
		case AXIS_y:
			p1 = Vector(aabb.getMinimum().x, 0, aabb.getMinimum().z);
			p2 = Vector(aabb.getMinimum().x, 0, aabb.getMaximum().z);
			p3 = Vector(aabb.getMaximum().x, 0, aabb.getMinimum().z);
			p4 = Vector(aabb.getMaximum().x, 0, aabb.getMaximum().z);	
			break;
		
		case AXIS_z:
			p1 = Vector(aabb.getMinimum().x, aabb.getMinimum().y, 0);
			p2 = Vector(aabb.getMinimum().x, aabb.getMaximum().y, 0);
			p3 = Vector(aabb.getMaximum().x, aabb.getMinimum().y, 0);
			p4 = Vector(aabb.getMaximum().x, aabb.getMaximum().y, 0);	
			break;
	}
	
	
	// Front
	bool isPlaneA = 
		(planeA.classifyPoint( p1 ) == PPC_Back ) ||
		(planeA.classifyPoint( p2 ) == PPC_Back ) ||
		(planeA.classifyPoint( p3 ) == PPC_Back ) ||
		(planeA.classifyPoint( p4 ) == PPC_Back ) ;
	//qDebug() << "\nPlaneA: " << isPlaneA;
		
	bool isPlaneB = 
		(planeB.classifyPoint( p1 ) == PPC_Back ) ||
		(planeB.classifyPoint( p2 ) == PPC_Back ) ||
		(planeB.classifyPoint( p3 ) == PPC_Back ) ||
		(planeB.classifyPoint( p4 ) == PPC_Back ) ;
	//qDebug() << "PlaneB: " << isPlaneB;
		
	bool isPlaneC = 
		(planeC.classifyPoint( p1 ) == PPC_Back ) ||
		(planeC.classifyPoint( p2 ) == PPC_Back ) ||
		(planeC.classifyPoint( p3 ) == PPC_Back ) ||
		(planeC.classifyPoint( p4 ) == PPC_Back ) ;
	//qDebug() << "PlaneC: " << isPlaneC;
		
		
	if(isPlaneA && isPlaneB && isPlaneC)
		return true;
	else
		return false;
	
}

bool Polygon::isVertexIntersecting(const Vertex& vert, const SceneAxis axis, const Aabb& boundary)
{
	switch(axis)
	{
		case AXIS_x:	if(	(vert.vPos.y < boundary.getMaximum().y) && 
						(vert.vPos.y > boundary.getMinimum().y) && 
						(vert.vPos.z < boundary.getMaximum().z) && 
						(vert.vPos.z > boundary.getMinimum().z))
							return true; 			
	
					break;
		
		case AXIS_y:	if(	(vert.vPos.x < boundary.getMaximum().x) && 
						(vert.vPos.x > boundary.getMinimum().x) && 
						(vert.vPos.z < boundary.getMaximum().z) && 
						(vert.vPos.z > boundary.getMinimum().z))
							return true;
						
					break;

		case AXIS_z:	if(	(vert.vPos.y < boundary.getMaximum().y) && 
						(vert.vPos.y > boundary.getMinimum().y) && 
						(vert.vPos.x < boundary.getMaximum().x) && 
						(vert.vPos.x > boundary.getMinimum().x))
							return true;
						
					break;
	}
	return false;
}


void Polygon::getIntersectingVertices(const Aabb& boundary, SelectionBuffer& sb ,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		Vertex *pCurrent = &(*it);
		
		switch(axis)
		{
			case AXIS_x:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
							(pCurrent->vPos.y > boundary.getMinimum().y) && 
							(pCurrent->vPos.z < boundary.getMaximum().z) && 
							(pCurrent->vPos.z > boundary.getMinimum().z))
								sb.addSingleVertex(pCurrent,this); 			
		
						break;
			
			case AXIS_y:	if(	(pCurrent->vPos.x < boundary.getMaximum().x) && 
							(pCurrent->vPos.x > boundary.getMinimum().x) && 
							(pCurrent->vPos.z < boundary.getMaximum().z) && 
							(pCurrent->vPos.z > boundary.getMinimum().z))
								sb.addSingleVertex(pCurrent,this);
							
						break;

			case AXIS_z:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
							(pCurrent->vPos.y > boundary.getMinimum().y) && 
							(pCurrent->vPos.x < boundary.getMaximum().x) && 
							(pCurrent->vPos.x > boundary.getMinimum().x))
								sb.addSingleVertex(pCurrent,this);
							
						break;
		}
	}
}

void Polygon::save(QDataStream &out)
{
	//this->print();
	
	// Serialize number of vertices and indices
	quint32 numVerts = m_vertices.size();
	quint32 numInds = m_indices.size();
	
	out << (bool) m_bHidden;
	out << (bool) m_bFreezed;
	out << (bool) m_bSelected;
	out << (quint32) m_materialID;
	out << (quint32) m_type;
	
	out << (QString) m_parentID;

	out << m_textureOffset[0];
	out << m_textureOffset[1];
	out << m_textureRepeat[0];
	out << m_textureRepeat[1];
	
	
	out << (quint32) numVerts ;
	out << (quint32) numInds ;
	
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		Vector *p = &(it->vPos);
		out << p->x;
		out << p->y;
		out << p->z;
		
		Vector *n = &(it->vNormal);
		out << n->x;
		out << n->y;
		out << n->z;
		
		for(int i=0; i < BSRenderer::MAXTEXTURES; i++)
		{
			out << 	it->dU[i];
			out <<  it->dV[i];
		}	
	}
	
	for(vector<int>::iterator it = m_indices.begin(); it != m_indices.end(); ++it)
	{ 
		out << (quint32)*it;     
	}	
}

void Polygon::load(QDataStream &in)
{
	quint32 numVerts;
	quint32 numInds;
	
	in >> m_bHidden;
	in >> m_bFreezed;
	in >> m_bSelected;
	in >> m_materialID;
	
	quint32 type;
	in >> type;
	m_type = (SceneObjectType)type;

	in >> m_parentID;
	//qDebug() << "Polygon: Parent-Name: " << m_parentID;
	
	in >> m_textureOffset[0];
	in >> m_textureOffset[1];
	in >> m_textureRepeat[0];
	in >> m_textureRepeat[1];
	
	in >> numVerts;
	in >> numInds;
	
	for(quint32 i=1; i<=numVerts;i++)
	{
		Vector p;
		in >> p.x;
		in >> p.y;
		in >> p.z;
		
		Vector n;
		in >> n.x;
		in >> n.y;
		in >> n.z;

		Vertex v(p,n);

		for(int i=0; i < BSRenderer::MAXTEXTURES; i++)
		{
			double texU,texV;
			in >> texU;
			in >> texV;
			
			v.setTexCoord(i,texU,texV);
		}	
		
		m_vertices.push_back(v);
	}
	
	for(quint32 i=1; i<=numInds; i++)
	{
		quint32 ind;
		in >> ind;
		m_indices.push_back(ind);
	}
	
	calcBoundingBox();
}
  
  	
void Polygon::print() const
{
	cerr << "\n[Polygon] Verts: " << m_vertices.size() << " / Inds: " << m_indices.size() << endl;
	
	for(vector<Vertex>::const_iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		const Vector *p = &(it->vPos);
		cerr << "\t-[Vertex] x: " << p->x << " y: "<< p->y <<" z: "<< p->z << endl;
	}

	cerr << endl << "\t Indices: ";
	
	for(vector<int>::const_iterator it = m_indices.begin(); it != m_indices.end(); ++it)
	{
		cerr << *it << ",";       
	}

	cerr << endl << endl;
}

long	Polygon::countVertices() const
{
	return m_vertices.size();
}

long    Polygon::countIndices() const
{
	return m_indices.size();
}

void Polygon::scale(const double percentageX,const double percentageY,const double percentageZ)
{
	/*
	qDebug() << "percentageX: " << percentageX;
    qDebug() << "percentageY: " << percentageY;
    qDebug() << "percentageZ: " << percentageZ;*/
	
	Vector center = m_boundary.getCentre();
	
	for(vector<Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		Vertex *v = &(*it);
		
		Vector vecToCenter;
		vecToCenter.difference(center,v->vPos);
		
		vecToCenter.x *= 1 - percentageX;
		vecToCenter.y *= 1 - percentageY;
		vecToCenter.z *= 1 - percentageZ;
		
		v->vPos += vecToCenter;
	}
	
	forwardChanges();
}

void Polygon::removeVertex(Vertex* v)
{
	// Polygons with less than 3 indices after vertex removing are not allowed
	if(m_indices.size()==3)
		return;
	
	// Build up a list of the indices positions
	vector<int> indPosList;
	int vertexNumber;
	
	qDebug() << "[Polygon::removeVertex] " << "m_indices.size()" << m_indices.size();
	for(unsigned int i=0; i< m_indices.size(); i++)
	{
		Vertex &current = m_vertices[ m_indices[i] ];   
		
		if(v->vPos == current.vPos)
		{
			indPosList.push_back(i);
			vertexNumber = m_indices[i];
			qDebug() << "[Polygon::removeVertex] " << "vertexNumber" << vertexNumber;
		}  
	}	
	qDebug() << "[Polygon::removeVertex] " << "indPosList.size()" << indPosList.size();
	
	
	if(indPosList.size()<=0)
		return;

	// Remove the specified indices
	int correctFactor=0;
	for(unsigned int i=0; i < indPosList.size(); i++)
	{
		int pos = indPosList[i] - correctFactor;	
		
		m_indices.erase(m_indices.begin() + pos);
		
		correctFactor++;
	}
	
	
	return;
	
	
	if(m_indices.size() % 3 == 0)
		return;
	else
	{
		// Merge neighbours 
		//mergeNeighourIndices();
	}

	//Remove vertex physically:
    m_vertices.erase(m_vertices.begin() + vertexNumber);	
}

void Polygon::mergeNeighourIndices()
{
	int posA,posB;
	
	while(m_indices.size() % 3 != 0)
	{
		posA = m_indices[0];
		
		for(unsigned int i=1; i< m_indices.size(); i++)
		{
			
		}
	}
}



}


