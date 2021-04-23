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
 *  File:       SceneUtils.cpp
 *  Created:    10.05.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
#include "./../include/SceneUtils.h" 

namespace BSScene
{ 
 	Plane<double> SceneUtils::create2DPlane(const Vector& v1, const Vector& v2,const  SceneAxis axis)
	{
		Vector perpend;
		
		switch(axis)
		{
			case AXIS_x: 
				perpend = Vector(1.0, 0.0, 0.0);
				break;
			
			case AXIS_y:	
				perpend = Vector(0.0, 1.0, 0.0);
				break;
	
			case AXIS_z:	
				perpend = Vector(0.0, 0.0, 1.0);
				break;
		}
		
		//Calculate the normal vector
		Vector norm = (v2 - v1) ^ perpend;
		
		if(norm != Vector(0.0, 0.0, 0.0))
			norm.normalize();
			
		Plane<double> p(norm,  norm * v1);
		
		Vector vNorm;
		double dist;
		//p.get(vNorm,dist);
		
		/*
		showVec(QString("Polygon::create2DPlane()"),vNorm,true);
		qDebug() << "vNorm.length(): " << vNorm.length();
		
		if(vNorm.length()<=0)
		{
			showVec(QString("v1"),v1);
			showVec(QString("v2"),v2);
			showVec(QString("perpend"),perpend);
		}
		*/
		//assert(vNorm.length()>0);
		 
		return p;	
	}
	
	
	void SceneUtils::showAabb(const QString &text,const Aabb& a,const  bool newline)
	{
		if(newline)
			qDebug() << ""; 
			
		qDebug() << text;
		qDebug() << "AABB-Max: " << a.getMaximum().x << ", " << a.getMaximum().y << ", " << a.getMaximum().z;
		qDebug() << "AABB-Min: " << a.getMinimum().x << ", " << a.getMinimum().y << ", " << a.getMinimum().z;
		qDebug() << "AABB-Center: " << a.getCentre().x << ", " << a.getCentre().y << ", " << a.getCentre().z;
	}
	
	
	void SceneUtils::showVec(const QString &text, const Vector& v,const bool newline)
	{
		if(newline)
			qDebug() << "\n[" << text << "] X: " << v.x << "Y: " << v.y<< "Z: " << v.z;
		else
			qDebug() << "[" << text << "] X: " << v.x << "Y: " << v.y<< "Z: " << v.z;
	}
	
	
	void SceneUtils::compareMinMax(const Vertex &comp,Vector &vecMax,Vector &vecMin)
	{
		compareMinMax(comp.vPos, vecMax,vecMin);
	}
	
	
	void SceneUtils::compareMinMax(const Vector &comp,Vector &vecMax,Vector &vecMin)
	{
		if(comp.x > vecMax.x)
			vecMax.x = comp.x;
		else if(comp.x < vecMin.x)
			vecMin.x = comp.x;	
			
		if(comp.y > vecMax.y)
			vecMax.y = comp.y;
		else if(comp.y < vecMin.y)
			vecMin.y = comp.y;
			
		if(comp.z > vecMax.z)
			vecMax.z = comp.z;
		else if(comp.z < vecMin.z)
			vecMin.z = comp.z;	
	}
	
	
	void SceneUtils::getMinMax(Vector &vecMin,Vector &vecMax)
	{
		if(vecMax.x < vecMin.x)
		{
			double x = vecMin.x;
			vecMin.x = vecMax.x;
			vecMax.x = x;
		}
	
		if(vecMax.y < vecMin.y)
		{
			double y = vecMin.y;
			vecMin.y = vecMax.y;
			vecMax.y = y;
		}
				
		if(vecMax.z < vecMin.z)
		{
			double z = vecMin.z;
			vecMin.z = vecMax.z;
			vecMax.z = z;
		}
	}			
}
