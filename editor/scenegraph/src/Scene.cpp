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
 *  File:       Scene.cpp
 *  Created:    06.12.2006
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
 #include "../include/Scene.h"

namespace BSScene
{

Scene::Scene()
{
	init();
}

Scene::~Scene()
{
	release();
}

void Scene::init()
{	
	m_sceneMeshes.clear();
	m_scenePolys.clear();
	m_sceneSingles.clear();
	m_sceneAllObjects.clear();
}

void Scene::release()
{
	m_sceneMeshes.clear();
	m_scenePolys.clear();
	m_sceneSingles.clear();
	m_sceneAllObjects.clear();
}

void Scene::reset()
{
	release();
	init();	
}

void Scene::save(QDataStream &out)
{
	// Serialize number of polys and meshes
	int numPolys = m_scenePolys.size();
	int numMeshes = m_sceneMeshes.size();
	int numSingles = m_sceneSingles.size();
	
	out << (int)numPolys;
	out << (int)numMeshes;
	out << (int)numSingles;
	
	//qDebug() << "(Save) Number of polys: " << numPolys;
	//qDebug() << "(Save) Number of meshes: " << numMeshes;
	
 	for(list<Polygon>::iterator it = m_scenePolys.begin();it != m_scenePolys.end(); ++it)
		it->save(out);

 	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
 		it->save(out);
 		
 	for(list<SceneVertex>::iterator it = m_sceneSingles.begin();it != m_sceneSingles.end(); ++it)
 		it->save(out); 		
}

void Scene::load(QDataStream &in)
{
	int numPolys;
	int numMeshes;
	int numSingles;
	
	in >> numPolys;
	in >> numMeshes;
	in >> numSingles; 
	
	//qDebug() << "(Load) Number of polys: " << numPolys;
	//qDebug() << "(Load) Number of meshes: " << numMeshes;
	
	for(int i = 1; i<=numPolys; i++)
	{
		Polygon p;
		p.load(in);
		this->addSceneObject(p,false);	
	} 
	
	for(int i = 1; i<=numMeshes; i++)
	{
		Mesh m;
		m.load(in);
		this->addSceneObject(m,false);	
	} 
	
	for(int i = 1; i<=numSingles; i++)
	{
		SceneVertex sv;
		sv.load(in);
		this->addSceneObject(sv,false);	
	} 
	
} 

vector<QString> Scene::getMeshNames()
{
	vector<QString> nameList;
	
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		nameList.push_back( it->getName() );
	
	return nameList;
}

void Scene::render(Renderer& r)
{
	for(list<Polygon>::iterator it = m_scenePolys.begin();it != m_scenePolys.end(); ++it)
	{
		if(!it->isSelected())
		{
			it->render(r);
		}
	}
			
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		if(!it->isSelected())
		{
			it->render(r);
		}
		
	for(list<SceneVertex>::iterator it = m_sceneSingles.begin();it != m_sceneSingles.end(); ++it)
		if(!it->isSelected())
		{
			it->render(r);
		}

}

void Scene::selectAll(const SceneSelectMode ssm, SelectionBuffer& sb)
{
	if (ssm==SELMODE_Polygon) 
	{
		for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
			if(!it->isHidden())
				it->selectAllPolys(sb);
	}

	buildTemporaryPointerList(ssm); 
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if(!(*it)->isHidden())
			sb.addSceneObject(*it,SCENETYPE_Select);	
	}
	
	
}

void Scene::selectInvert(const SceneSelectMode ssm, SelectionBuffer& sb)
{
	if (ssm==SELMODE_Polygon) 
	{
		for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
			if(!it->isHidden())
				it->selectInvert(sb);
	}
	
	if(ssm==SELMODE_Vertex)
		return;

	buildTemporaryPointerList(ssm);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if(!(*it)->isHidden())
			if(!(*it)->isSelected())
				sb.addSceneObject(*it,SCENETYPE_Select);
			else
				sb.deselectSceneObject(*it);		
	}
}

void Scene::selectIntersecting(const SceneSelectMode ssm, SelectionBuffer& sb,const Aabb& aabb,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	// 1. Special case: Vertices in polygons and meshes
   	switch(ssm)
   	{
   		case SELMODE_Vertex:
   		{
      		for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
         		if ( !it->isHidden() && !it->isFreezed()) 
         			it->getIntersectingVertices(aabb,sb,axis,selType);

      		for(list<Polygon>::iterator it = m_scenePolys.begin();it != m_scenePolys.end(); ++it)
         		if ( !it->isHidden() && !it->isFreezed() ) 
         			it->getIntersectingVertices(aabb,sb,axis,selType);
         			
      		for(list<SceneVertex>::iterator it = m_sceneSingles.begin();it != m_sceneSingles.end(); ++it)
         		if ( !it->isHidden() && !it->isFreezed() ) 
         			it->getIntersectingVertices(aabb,sb,axis,selType);
         			         		
   		}break;
   		
   		case SELMODE_Polygon:
   		{
      		for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
         		if ( !it->isHidden() && !it->isFreezed() ) 
         			it->getIntersectingPolygons(aabb,sb,axis,selType);
   		}break;	
   		
   		default:
   			break;
   	}

	buildTemporaryPointerList(ssm);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{ 
		if ( !(*it)->isHidden() && !(*it)->isFreezed() ) 
			if( (*it)->intersects(aabb,axis))
				sb.addSceneObject(*it,selType);
	}
}

void Scene::selectMeshByName(SelectionBuffer& sb,const QString &name)
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		if ( !it->isHidden() && !it->isFreezed()) 
		{
			if(it->getName().compare(name)==0)
			{
				SceneObject *so = &(*it);
				sb.addSceneObject(so,SCENETYPE_Select);
			}
		}
}

void Scene::renameMesh(const QString oldName,const QString newName)
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		{
			if(it->getName().compare(oldName)==0)
			{
				SceneObject *so = &(*it);
				Mesh* mesh = dynamic_cast<Mesh*>(so);
				mesh->setName(newName);
			}
		}
}


void Scene::recalcVertexNormals()
{
	for(list<Polygon>::iterator it = m_scenePolys.begin();it != m_scenePolys.end(); ++it)
		it->calcNormals();
		
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		it->calcNormals();
}


void Scene::hideNone()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		it->unHide();
		it->hideNoPolys();	
	}
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		(*it)->unHide();
	}
}

void Scene::hideSelected(const SceneSelectMode ssm)
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
		it->hideSelectedPolys();
	
	buildTemporaryPointerList(ssm);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if((*it)->isSelected())
			(*it)->hide();
	}
	
}

void Scene::hideAll()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		it->hide();
		it->hideNoPolys();	
	}
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		(*it)->hide();
	}
}

void Scene::hideInvert()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		if(it->isHidden())
		{
			it->hide();
			it->hideNoPolys();
		}
		else
			it->hideInvertPolys();
	}
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if((*it)->isHidden())
			(*it)->unHide();
		else
			(*it)->hide();
				
	}
}

void Scene::freezeNone()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		it->unFreeze();
		it->freezeNoPolys();	
	}	
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		(*it)->unFreeze();
	}
}

void Scene::freezeSelected(const SceneSelectMode ssm)
{

	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		if(it->isSelected())
		{
			it->freeze();
			it->freezeAllPolys();
		}	
	}	
	
	buildTemporaryPointerList(ssm);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if((*it)->isSelected())
			(*it)->freeze();
	}
}

void Scene::freezeAll()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		it->freeze();
		it->freezeAllPolys();	
	}
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		(*it)->freeze();
	}
}

void Scene::freezeInvert()
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		if(it->isFreezed())
		{
			it->freeze();
			it->freezeNoPolys();
		}
		else
			it->freezeInvertPolys();
	}
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if((*it)->isFreezed())
			(*it)->unFreeze();
		else
			(*it)->freeze();
				
	}
}

	


void Scene::buildTemporaryPointerList(const SceneSelectMode ssm)
{
	m_sceneAllObjects.clear();
	
	if(ssm == SELMODE_Polygon || ssm == SELMODE_All)
	{
		for(list<Polygon>::iterator it = m_scenePolys.begin(); 
										it != m_scenePolys.end(); ++it)
		{
			m_sceneAllObjects.push_back(&(*it));		
		}
	}	
	
	if(ssm == SELMODE_Mesh || ssm == SELMODE_All)
	{
		for(list<Mesh>::iterator it = m_sceneMeshes.begin(); 
										it != m_sceneMeshes.end(); ++it)
		{
			m_sceneAllObjects.push_back(&(*it));		
		}
	}	
	
	if(ssm == SELMODE_Vertex || ssm == SELMODE_All)
	{
		for(list<SceneVertex>::iterator it = m_sceneSingles.begin(); 
										it != m_sceneSingles.end(); ++it)
		{
			m_sceneAllObjects.push_back(&(*it));		
		}
	}			
}


SceneObject* Scene::addSceneObject(SceneObject& sObject,const bool newName)
{
	SceneObject *so = &sObject;
	Polygon *p;
	Mesh *m;
	SceneVertex *sv;
	QString name;
	
	switch ( sObject.getType() ) 
	{
		case SCENETYPE_Vertex:
		{
		    LOG_Ok(QString("Inserting Vertex.").toStdString());		
			sv = dynamic_cast<SceneVertex*>(so);
   			m_sceneSingles.push_back(*sv);
   			return &m_sceneSingles.back(); 
		}break;
		
    	case SCENETYPE_Polygon:	
    	{		
    		LOG_Ok(QString("Inserting polygon.").toStdString());		
			p = dynamic_cast<Polygon*>(so);
			m_scenePolys.push_back(*p);
         	return &m_scenePolys.back();
    	}break;
    		
      	case SCENETYPE_Mesh:
      	{
      		LOG_Ok(QString("Inserting mesh.").toStdString());	
      		m = dynamic_cast<Mesh*>(so);
      		
      		if(newName)
      		{
      			QString typname;
      			if(m->getName().length()==0)
      				typname = "Mesh";
      			else
      				typname = m->getName();
      			
      			QLocale loc;
      			name = typname + loc.toString(m_sceneMeshes.size() + 1) ;
      			m->setName( name );
      		}
			m_sceneMeshes.push_back(*m);
			
         	return &m_sceneMeshes.back();
      	}break;
      		
    	default: 
    		break;
	}
	
	return NULL;
}

void Scene::removeAllVertices( SelectionBuffer &sb)
{
	for(list<SceneVertex>::iterator it = m_sceneSingles.begin();it != m_sceneSingles.end(); ++it)
 	{
 		SceneObject *so = &(*it);
 		sb.deselectSceneObject(so); 
 	}
 		
	m_sceneSingles.clear();
}

void Scene::removeObject( SceneObject* sObject)
{
	Mesh *m;
	Polygon *p;
	SceneVertex *sv;
	
	switch ( sObject->getType() ) 
	{
		case SCENETYPE_Vertex:
			sv = dynamic_cast<SceneVertex*>(sObject);
			m_sceneSingles.remove(*sv);
			break;
			
		case SCENETYPE_Polygon:
			p = dynamic_cast<Polygon*>(sObject);
			m_scenePolys.remove(*p);
			break;
			
		case SCENETYPE_Mesh:
			m = dynamic_cast<Mesh*>(sObject);
			m_sceneMeshes.remove(*m);
			
			break;
			
		default: break;
	}
}

long Scene::countPolygons() const
{
	return m_scenePolys.size();
}

long Scene::countMeshes() const
{
	return m_sceneMeshes.size();
}

long Scene::countSingleVertices() const
{
	return m_sceneSingles.size();
}

long Scene::countBoundVertices(const bool real) const
{
	int count=0;
	
	for(list<Mesh>::const_iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		count += it->countVertices(real);		
	}
	
	for(list<Polygon>::const_iterator it = m_scenePolys.begin();it != m_scenePolys.end(); ++it)
	{
		if(real)
			count += it->countVertices();
		else
			count += it->countIndices();
	}
	
	return count;
}	

list<Mesh>* Scene::getSceneMeshes()		
{ 
	return &m_sceneMeshes;
}
	
list<Polygon>*	Scene::getScenePolys()
{ 
	return &m_scenePolys;
}

void Scene::getSceneAabb(Aabb& box)
{
	box.setMinimum(Vector(0,0,0));
	box.setMaximum(Vector(0,0,0));
	
	buildTemporaryPointerList(SELMODE_All);
   
	for(list<SceneObject*>::iterator it = m_sceneAllObjects.begin(); it != m_sceneAllObjects.end(); ++it)
	{
		if(!(*it)->isHidden() || !(*it)->isFreezed())
			box.merge( (*it)->getAabb() );
	}
}

void Scene::calcBoundingBoxByName(const QString name)
{
	for(list<Mesh>::iterator it = m_sceneMeshes.begin();it != m_sceneMeshes.end(); ++it)
	{
		if(it->getName().compare(name)==0)
			it->recalcBoundingBox();
	}
}

}
