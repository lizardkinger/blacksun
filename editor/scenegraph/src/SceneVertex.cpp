#include "../include/SceneVertex.h"
#include "../include/SelectionBuffer.h"
#include <vector>

namespace BSScene
{

SceneVertex::SceneVertex() : SceneObject()
{
	init();
}

SceneVertex::SceneVertex(const SceneVertex &copy) : SceneObject()
{
	init(copy);
}

SceneVertex&	SceneVertex::operator=(const SceneVertex& copy)
{
	if(this != &copy)
	{
		release();
		init(copy);
	}
	return *this;
}

SceneVertex::~SceneVertex()
{
	release();
}

bool SceneVertex::operator==(const SceneObject &so)
{
	if( this == &so )
		return true;
	else 
		return false;
}

void SceneVertex::init() 
{
	m_type = SCENETYPE_Vertex;
	m_bHidden = false;
	m_bFreezed = false;
	m_bSelected = false; 

}

void SceneVertex::init(const SceneVertex& copy)
{
	init();
	m_vertex = copy.m_vertex;
	m_bHidden = copy.m_bHidden;
	m_bFreezed = copy.m_bFreezed;
	calcBoundingBox();
}

void SceneVertex::release() 
{

}


void SceneVertex::render(Renderer& r)
{	
	if(m_bHidden)
		return;
	  
	Matrix matIdent;
	matIdent.identity();

	vector<Vertex> tempList;
	tempList.push_back(m_vertex);

	if(!m_bSelected)	
		r.render(PRIM_Point, tempList, 0, matIdent, m_boundary);
	else if(isFreezed())
		r.renderFreezed(PRIM_Point, tempList, matIdent, 0, m_boundary);
	else
		r.renderSelection(PRIM_Point, tempList, matIdent, 0, m_boundary);

}


void SceneVertex::move(const Vector &v)
{
	m_vertex.vPos.x += v.x;
	m_vertex.vPos.y += v.y;
	m_vertex.vPos.z += v.z;
	
	calcBoundingBox();
} 

void SceneVertex::rotate(const SceneAxis axis,const Vector& center, const float amount)
{
	calcBoundingBox();
}


void SceneVertex::mirror(const SceneAxis axis,const  Vector& center)
{
	calcBoundingBox();
}

void SceneVertex::mirrorWorld(const SceneAxis axis)
{
	calcBoundingBox();
}

bool SceneVertex::intersects(const Aabb& boundary ,const SceneAxis axis)
{
	Vertex *pCurrent = &m_vertex;
	
	switch(axis)
	{
		case AXIS_x:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
						(pCurrent->vPos.y > boundary.getMinimum().y) && 
						(pCurrent->vPos.z < boundary.getMaximum().z) && 
						(pCurrent->vPos.z > boundary.getMinimum().z))
							return true; 			
	
					break;
		
		case AXIS_y:	if(	(pCurrent->vPos.x < boundary.getMaximum().x) && 
						(pCurrent->vPos.x > boundary.getMinimum().x) && 
						(pCurrent->vPos.z < boundary.getMaximum().z) && 
						(pCurrent->vPos.z > boundary.getMinimum().z))
							return true;
						
					break;

		case AXIS_z:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
						(pCurrent->vPos.y > boundary.getMinimum().y) && 
						(pCurrent->vPos.x < boundary.getMaximum().x) && 
						(pCurrent->vPos.x > boundary.getMinimum().x))
							return true;
						
					break;
	}

	return false;
}

void  SceneVertex::getIntersectingVertices(const Aabb& boundary, SelectionBuffer& sb ,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	Vertex *pCurrent = &m_vertex;
	
	switch(axis)
	{
		case AXIS_x:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
						(pCurrent->vPos.y > boundary.getMinimum().y) && 
						(pCurrent->vPos.z < boundary.getMaximum().z) && 
						(pCurrent->vPos.z > boundary.getMinimum().z))
							sb.addSceneObject(this, selType);		
	
					break;
		
		case AXIS_y:	if(	(pCurrent->vPos.x < boundary.getMaximum().x) && 
						(pCurrent->vPos.x > boundary.getMinimum().x) && 
						(pCurrent->vPos.z < boundary.getMaximum().z) && 
						(pCurrent->vPos.z > boundary.getMinimum().z))
							sb.addSceneObject(this, selType);
						
					break;

		case AXIS_z:	if(	(pCurrent->vPos.y < boundary.getMaximum().y) && 
						(pCurrent->vPos.y > boundary.getMinimum().y) && 
						(pCurrent->vPos.x < boundary.getMaximum().x) && 
						(pCurrent->vPos.x > boundary.getMinimum().x))
							sb.addSceneObject(this, selType);
						
					break;
	}
}

void SceneVertex::calcBoundingBox()
{	
	m_boundary.setMinimum( m_vertex.vPos );
	m_boundary.setMaximum( m_vertex.vPos );
	m_boundary.setCentre( m_vertex.vPos );
}

void SceneVertex::save(QDataStream &out)
{
	out << (bool) m_bHidden;
	out << (bool) m_bFreezed;
	out << (bool) m_bSelected;
	out << (quint32) m_type;
	
	out << m_vertex.vPos.x;
	out << m_vertex.vPos.y;
	out << m_vertex.vPos.z;
	
	out << m_vertex.vPos.x;
	out << m_vertex.vPos.y;
	out << m_vertex.vPos.z;
	
	for(int i=0; i < BSRenderer::MAXTEXTURES; i++)
	{
		out << 	m_vertex.dU[i];
		out <<  m_vertex.dV[i];
	}	
}

void SceneVertex::load(QDataStream &in)
{
	in >> m_bHidden;
	in >> m_bFreezed;	
	in >> m_bSelected;
	
	quint32 type;
	in >> type;
	m_type = (SceneObjectType)type;
	
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
	
	setVertex(v);
}

}


