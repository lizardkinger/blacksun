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
 *  File:       Mesh.cpp
 *  Created:    29.11.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
#include "../include/Mesh.h"
#include "../include/SceneUtils.h"

namespace BSScene
{

Mesh::Mesh()
{
	init();
}

Mesh::~Mesh()
{
	release();
}

Mesh::Mesh(const Mesh &copy) : SceneObject()
{
	init(copy);
}

Mesh&	Mesh::operator=(const Mesh& copy)
{
	if(this != &copy)
	{
		release();
		init(copy);
	}
	return *this;
}

bool	Mesh::operator==(const SceneObject &so) const
{
	if( this == &so )
		return true;
	else 
		return false;
}

void Mesh::init()
{
	m_type = SCENETYPE_Mesh;
	m_lockAutomaticBounding = false;	
	m_bHidden = false;
	m_bFreezed = false;
	m_bSelected = false;
	m_meshID.clear();
	
	m_polys.clear();
}

void Mesh::init(const Mesh& copy)
{
	init();
	
	for(vector<Polygon>::const_iterator it = copy.m_polys.begin(); it != copy.m_polys.end(); ++it)
		m_polys.push_back(*it);
		
	m_bHidden = copy.m_bHidden;
	m_bFreezed = copy.m_bFreezed;
	m_boundary = copy.m_boundary;
	m_meshID = copy.m_meshID;
	
	m_meshID = copy.m_meshID;
}

void Mesh::release()
{
	m_polys.clear();
}

void Mesh::setName(const QString n)
{
	m_meshID = n;
	
	//qDebug() << "Mesh.setName() " << m_meshID;
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->setAsPartOfMesh(m_meshID);
	}	
}
  	
QString Mesh::getName() const
{
	return m_meshID;
}

void Mesh::createFromPolygons(const vector<Polygon>& polys) 
{
   	release();
   	init();

   	m_lockAutomaticBounding = true;

   	for(vector<Polygon>::const_iterator it = polys.begin(); it != polys.end(); ++it)
   		addPolygon(*it);

   	calcBoundingBox();
   	m_lockAutomaticBounding = false;
}

void Mesh::addPolygon(const Polygon& poly)
{
	assert(poly.countVertices()>0);
	assert(poly.countIndices()>0);
	
	Polygon p(poly);
	p.changeBias();
	p.setAsPartOfMesh(m_meshID);
	
	m_polys.push_back(poly);
	
	if(!m_lockAutomaticBounding)
		calcBoundingBox();	
}

void Mesh::removePolygon(const Polygon* p)
{
	vector<Polygon>::iterator remove_it = m_polys.end();
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		if(p == &(*it))
			remove_it = it;		
	}
	
	assert(remove_it == m_polys.end());
	m_polys.erase(remove_it);

	calcBoundingBox();
}

void Mesh::changeBias()
{
   for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->changeBias();	
}

void Mesh::move(const Vector &v)
{
   	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->move(v);	
		
	calcBoundingBox();				  
}

bool Mesh::defaultIntersecting(const Aabb& aabb) const
{
	if( isHidden() || isFreezed())
		return false;
	
	if(!Intersection::AabbIntersects(m_boundary,aabb))
		return false;
	else
		return true;
}

bool Mesh::intersects(const  Aabb& aabb, const  SceneAxis axis)  
{
	if(!defaultIntersecting(aabb))
		return false;
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		if(it->isHidden() || it->isFreezed())
			continue;
		
		if(it->intersects(aabb,axis))
			return true;
	}
	
	return false;
}

void Mesh::getIntersectingPolygons(const Aabb& aabb,SelectionBuffer& sb,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	if(!defaultIntersecting(aabb))
		return;
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{	
		if(it->isHidden() || it->isFreezed())
			continue;
		
		if(it->intersects(aabb,axis))
		{
			SceneObject *s = &(*it);
			sb.addSceneObject(s,selType);
		}		
	}
}

    
void Mesh::getIntersectingVertices(const Aabb& aabb,SelectionBuffer& sb,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	if( isHidden() || isFreezed())
		return;
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->getIntersectingVertices(aabb,sb,axis,selType);	
		
}
    

void    Mesh::deSelect()
{
	m_bSelected = false;
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->deSelect();
	}	
}

void Mesh::select()    
{ 
	m_bSelected = true; 
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->select();
	}	 
}

void Mesh::render(Renderer& r)
{
	if( isHidden() )
		return;
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		if(!it->isHidden())
			it->render(r);	
	}	
}

void Mesh::selectAllPolys(SelectionBuffer& sb)
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{	
		if(!it->isHidden() && !it->isFreezed())
		{
			SceneObject *s = &(*it);
			
			sb.addSceneObject(s,SCENETYPE_Select);
		}		
	}
}

void Mesh::selectInvert(SelectionBuffer& sb)
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{	
		if(!it->isHidden() && !it->isFreezed())
		{
			if(it->isSelected())
			{
				SceneObject *s = &(*it);
				sb.addSceneObject(s,SCENETYPE_Select);
			}
			else
				it->deSelect();
		}		
	}
}

void Mesh::rotate(const SceneAxis axis,const Vector& center, const float amount)
{	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->rotate(axis,center,amount);
		
	calcBoundingBox();
}

void Mesh::mirror(const SceneAxis axis,const  Vector& center)
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->mirror(axis,center);
}

void Mesh::mirrorWorld(const SceneAxis axis)
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->mirrorWorld(axis);
}

void Mesh::hideNoPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->unHide();
}

void Mesh::hideInvertPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		if(it->isHidden())
			it->unHide();
		else
			it->hide();
	}		
}

void Mesh::hideSelectedPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		if(it->isSelected())
			it->unHide();
}

void Mesh::hideUnselectedPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		if(!it->isSelected())
			it->unHide();
}

void Mesh::freezeAllPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->freeze();
}

void Mesh::freezeNoPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->unFreeze();
}

void Mesh::freezeInvertPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		if(it->isFreezed())
			it->unFreeze();
		else
			it->freeze();
	}	
}

void Mesh::freezeSelectedPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		if(it->isSelected())
			it->freeze();
}

void Mesh::freezeUnselectedPolys()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		if(!it->isSelected())
			it->freeze();
}





void Mesh::recalcBoundingBox()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->calcBoundingBox();
	
	this->calcBoundingBox();	
}

void Mesh::calcBoundingBox()
{
/*
	qDebug() << "Mesh::calcBoundingBox";
	qDebug() << "\nBefore:";
	qDebug() << m_meshID;
	qDebug() << "AABB-Min: " << m_boundary.getMinimum().x << ", " << m_boundary.getMinimum().y << ", " << m_boundary.getMinimum().z;
	qDebug() << "AABB-Max: " << m_boundary.getMaximum().x << ", " << m_boundary.getMaximum().y << ", " << m_boundary.getMaximum().z;
	qDebug() << "AABB-Cen: " << m_boundary.getCentre().x << ", " << m_boundary.getCentre().y << ", " << m_boundary.getCentre().z;
*/
	Vector 	vecMax, vecMin, vecTemp;
	
	Aabb 	aabb = m_polys.at(0).getAabb();
	vecMax = vecMin = aabb.getCentre();
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		aabb = it->getAabb();
		
		vecTemp = aabb.getMaximum();
		SceneUtils::compareMinMax(vecTemp,vecMax,vecMin);
		
		vecTemp = aabb.getMinimum();
		SceneUtils::compareMinMax(vecTemp,vecMax,vecMin);
	}
	
	m_boundary.setCentre((vecMax + vecMin)/2.0) ;
	m_boundary.setMaximum(vecMax);
	m_boundary.setMinimum(vecMin);
	
/*	
	qDebug() << "Mesh::calcBoundingBox";
	qDebug() << "\nAfter:";
	qDebug() << m_meshID;
	qDebug() << this;
	qDebug() << "AABB-Min: " << m_boundary.getMinimum().x << ", " << m_boundary.getMinimum().y << ", " << m_boundary.getMinimum().z;
	qDebug() << "AABB-Max: " << m_boundary.getMaximum().x << ", " << m_boundary.getMaximum().y << ", " << m_boundary.getMaximum().z;
	qDebug() << "AABB-Cen: " << m_boundary.getCentre().x << ", " << m_boundary.getCentre().y << ", " << m_boundary.getCentre().z;
*/
}

Polygon* Mesh::getPolygon(const int i)
{
	int idx=0;
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it,++idx)
	{
		if(idx==i)
			return &(*it);	
	}
	return NULL;
}

void Mesh::calcNormals()
{
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->calcNormals();	
}

void Mesh::calcTextureCoords(const SceneAxis axis, const Aabb* pAabb)
{
   	const Aabb *local_aabb;

   	if (pAabb) 
   		local_aabb = pAabb;
   	else 
   		local_aabb = &m_boundary;
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->calcTextureCoords(axis,local_aabb);	
}

void Mesh::transTextureCoords(double dtU, double dtV, double drU, double drV)
{
   // brute force prevention of zero repeat
   	if ( (drU < 0.0001f) && (drU > -0.0001f)) 
   		drU = 0.0001f;
   	
   	if ( (drV < 0.0001f) && (drV > -0.0001f)) 
   		drV = 0.0001f;
   		
   for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
		it->transTextureCoords(dtU,dtV,drU,drV);
}

bool  Mesh::getTextureTrans(double *pdtU, double *pdtV,double *pdrU, double *pdrV)
{
	double dtU=0.0f, dtV=0.0f, drU=0.0f, drV=0.0f;
	
	bool firstSet = false;
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->getTextureTrans(&dtU,&dtV,&drU,&drV);
		
		if(firstSet)
			if ( (*pdtU != dtU) || (*pdtV != dtV) || (*pdrU != drU) || (*pdrV != drV) )
            	return false;
		
		*pdtU = dtU;  
		*pdtV = dtV;
	
      	*pdrU = drU;  
      	*pdrV = drV;
      	
      	firstSet = true;
	}
	
	return false;
	
}

void Mesh::save(QDataStream &out)
{
	out << (bool) m_bHidden;
	out << (bool) m_bFreezed;
	out << (bool) m_bSelected;
	out << (quint32) m_materialID;
	out << (quint32) m_type;
	
	out << (QString) m_meshID;
	
	quint32 numPolys = m_polys.size();
	out << (quint32) numPolys ;
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->save(out);
	}	
}

void Mesh::load(QDataStream &in)
{
	in >> m_bHidden;
	in >> m_bFreezed;
	in >> m_bSelected;
	in >> m_materialID;
	
	quint32 type;
	in >> type;
	m_type = (SceneObjectType)type;
	
	in >> m_meshID;
	
	quint32 numPolys;
	in >> numPolys;
	
	for(quint32 i=1; i<=numPolys;i++)
	{
		Polygon p;
		p.load(in);
		m_polys.push_back(p);
	}
	
	calcBoundingBox();
}

long Mesh::countPolygons() const
{
	return m_polys.size();
}

long Mesh::countVertices(const bool real) const
{
	long count = 0;
	
	for(vector<Polygon>::const_iterator it = m_polys.begin(); it != m_polys.end(); ++it)	
	{
		if(real)
			count += it->countVertices();
		else
			count += it->countIndices();
	}
	
	return count;
}

void Mesh::setMaterialID(int i)
{
	SceneObject::setMaterialID(i);
	
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)
	{
		it->setMaterialID(i);
	}
    SceneObject::setMaterialID(i);
}

void Mesh::scale(const double percentageX,const double percentageY,const double percentageZ)
{
	Aabb meshBoundary = this->getAabb();
		
	for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)	
	{
		it->scale(percentageX,percentageY,percentageZ);
	}	
	
	if(m_polys.size()>1)
	{
		Vector meshCenter = meshBoundary.getCentre();
		
		for(vector<Polygon>::iterator it = m_polys.begin(); it != m_polys.end(); ++it)	
		{
			Vector vecMoveToCenter;
			
			Aabb polBoundary = it->getAabb();
			Vector polCenter = polBoundary.getCentre();
			
			Vector vecToCenter = polCenter - meshCenter;

			vecMoveToCenter.x = vecToCenter.x * (1 - percentageX);
			vecMoveToCenter.y = vecToCenter.y * (1 - percentageX);
			vecMoveToCenter.z = vecToCenter.z * (1 - percentageX);
			vecMoveToCenter.negate();
			
			it->move(vecMoveToCenter);
		}	
	}
}

void Mesh::print() const
{
	cerr << "------------------[Mesh]---------------" << endl;
	
	int i = 1;
	for(vector<Polygon>::const_iterator it = m_polys.begin(); it != m_polys.end(); ++it, ++i)	
	{
		cerr << "++Nummer " << i << endl;
		it->print();
	}
	
	cerr << "------------------[/Mesh]---------------" << endl;
}

}
