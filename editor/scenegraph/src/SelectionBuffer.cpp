#include "../include/SelectionBuffer.h"
#include "../include/SceneManager.h"

namespace BSScene
{

SelectionBuffer::SelectionBuffer()
{
	init();
}

SelectionBuffer::~SelectionBuffer()
{
	release();
}

void SelectionBuffer::init()
{
	m_vertices.clear();
	m_objects.clear();
	
	m_boundary.setMinimum(Vector(0,0,0));
	m_boundary.setMaximum(Vector(0,0,0));
	m_boundary.setCentre(Vector(0,0,0));
	
	m_fD = 0.05;
}

void SelectionBuffer::release()
{
   	for(list<SelectedVertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
		(*it).set(NULL,NULL);

   	m_vertices.clear();		
	
    list<Mesh*> signalsToEmit;
    
	for(list<SceneObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
   		(*it)->deSelect();
        Mesh* mesh = dynamic_cast<Mesh*>(*it);
        if(mesh != NULL)
        {
            signalsToEmit.push_back(mesh);
        }
    }
    
    m_objects.clear();
    
    for(list<Mesh*>::iterator it = signalsToEmit.begin() ; it != signalsToEmit.end(); ++it)
    {
        emit meshRemoved((*it)->getName());
    }
    
    signalsToEmit.clear();
    
    emit selectionChanged();
}

void SelectionBuffer::reset()
{
	release();
	init();
}

void SelectionBuffer::triangulateVerts(const bool single)
{
	if(single)
		triangulateSingle();
	else
		triangulateContinuous(); 	
}	

void SelectionBuffer::triangulateSingle()
{
	SceneVertex *vert=NULL, *vert0=NULL,*vert1=NULL,*vert2=NULL;
	vector<Polygon> polys;
	int i=0;
	
	// Loop through all SceneObjects 
   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it, ++i) 
   	{  
		// Only save SOs of type Vertex
      	switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Vertex:
            	vert = dynamic_cast<SceneVertex*>(*it);
	            break;
	        
	        default: break;
      	}
      	
      	// Assign the tempory pointers
      	switch(i)
      	{
      		case 0: vert0 = vert; break;	      			
      		case 1: vert1 = vert; break;	      			
      		case 2: vert2 = vert; break;	
      	}
      	
      	// After 3 collected vertices build an polygon out of it
      	if(i==2)
      	{
      		Polygon p;
      		
      		vector<Vertex> v;
      		v.push_back(vert0->getVertex());
      		v.push_back(vert1->getVertex());
      		v.push_back(vert2->getVertex());
      		
      		p.setVertices(v);
      		p.triangulate();
      		p.calcNormals();
      
      		polys.push_back(p);

      		i = -1;		
      	} 
   	}
   	
 	for(vector<Polygon>::iterator it = polys.begin();it != polys.end(); ++it)
   	{
   		SceneManager::getInstance()->insertObject(*it);	
   	}
}

void SelectionBuffer::triangulateContinuous()
{
	
}

void SelectionBuffer::deselectSceneObject(SceneObject* so)
{
	list<SceneObject*>::iterator remove_it 
		= find(m_objects.begin(),m_objects.end(),so);

	//assert(remove_it == m_objects.end());
	if(remove_it == m_objects.end()) return;
	
	(*remove_it)->deSelect();
    Mesh* mesh = dynamic_cast<Mesh*>(*remove_it);
    m_objects.erase(remove_it);
    if(mesh != NULL)
    {
        emit meshRemoved(mesh->getName());
    }
    
    emit selectionChanged();
}

void SelectionBuffer::move(const Vector& v)
{
	bool boundChanges = false;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		(*it)->move(v);
		boundChanges = true;
	}
		
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		it->getVertex()->vPos.x += v.x;
		it->getVertex()->vPos.y += v.y;
		it->getVertex()->vPos.z += v.z; 
		
		Polygon *p = it->getPolygon();
		p->forwardChanges();
		
		boundChanges = true;
	}
	
	if(boundChanges)
	{
		m_boundary.setMaximum(m_boundary.getMaximum()+v);
		m_boundary.setMinimum(m_boundary.getMinimum()+v);
		m_boundary.setCentre((m_boundary.getMaximum() + m_boundary.getMinimum())/2.0) ;
	}
	
	SceneManager *sm = SceneManager::getInstance();
	sm->checkForRedraw(true);
}

void SelectionBuffer::deleteSelectedObjects()
{
	SceneManager *sm = SceneManager::getInstance();
	
	while(m_objects.size()>0)
	{
		SceneObject* temp = m_objects.back();
		sm->removeObject(temp);
	}
	
	while(m_vertices.size()>0)
	{
		sm->removeSelectedVertex(m_vertices.back());
	}
	
	reset();
}

void SelectionBuffer::copySelectedObjects()
{
	Polygon  *poly;
   	Mesh *mesh;
   	Mesh newMesh;
   	Polygon newPoly;
   	
	list<Polygon> createPoly;
	list<Mesh> createMesh;

   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
		(*it)->deSelect();
		
      	switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Polygon:
            	poly = dynamic_cast<Polygon*>(*it);
            	poly->print();
				createPoly.push_back(*poly);
	            break;
            
         	case SCENETYPE_Mesh:
            	mesh = dynamic_cast<Mesh*>(*it);
            	newMesh  = *mesh;
            	createMesh.push_back(newMesh);
            	break;
            
         	default: 
         		break;
		} 
	}

	Vector vecBeside(1.0f,0.0f,0.0f);
	
   	for(list<Polygon>::iterator it = createPoly.begin();it != createPoly.end(); ++it) 
   	{  
	   it->move(vecBeside);
	   SceneManager::getInstance()->insertObject(*it);
   	}
   	
   	for(list<Mesh>::iterator it = createMesh.begin();it != createMesh.end(); ++it) 
   	{  
	   it->move(vecBeside);
	   SceneManager::getInstance()->insertObject(*it);
   	}
}

void SelectionBuffer::render(Renderer& r)
{
	bool sthVisible = false;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		assert(*it != NULL);
		(*it)->render(r);
		
		if(!(*it)->isHidden())
			sthVisible = true;
	}
	
	if(m_vertices.size()>0)
		renderVertices(r);
	
	if((sthVisible && m_objects.size()>0 ) || m_vertices.size()> 0)
	{
		r.renderAABB(m_boundary);
		
		// render AABB centre
		vector<Vertex>	tempList;
		Vertex v(m_boundary.getCentre());
		
		tempList.push_back(v);
		Aabb aabb;
		Matrix matIdent;
		matIdent.identity();
		//r.renderSelection(PRIM_Point,tempList,matIdent,0,aabb);
	}
}

void SelectionBuffer::renderVertices(Renderer& r)
{
	vector<Vertex>	tempList;
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		tempList.push_back(*(it->getVertex()));
	} 
	
	Aabb aabb;
	Matrix matIdent;
	matIdent.identity();
	
	r.renderSelection(PRIM_Point,tempList,matIdent,0,aabb);
	

}


void SelectionBuffer::addSceneObject(SceneObject* so,const SceneAction2D_SelectionType selType)
{
	assert(so!=NULL);	
	if(selType == SCENETYPE_RemoveFromSelection)
	{
		if(!so->isSelected())
			return;
			
		deselectSceneObject(so);
		
		if(so->getType() == SCENETYPE_Mesh)
		{
			Mesh* mesh = dynamic_cast<Mesh*>(so);
	        emit meshRemoved(mesh->getName());
	    }
		
		return;
	}
	else
	{
		if(so->isSelected())
			return;
		
		m_objects.push_back(so);	
	
		so->select();
    
		if(so->getType() == SCENETYPE_Mesh)
		{
			Mesh* mesh = dynamic_cast<Mesh*>(so);
	        emit meshAdded(mesh->getName());
	    }
	
		addBoundingBox(so->getAabb());
	}
	
    emit selectionChanged();
}

void SelectionBuffer::adjustSelectionBoundaries()
{
	m_boundary.setMinimum(Vector(0,0,0));
	m_boundary.setMaximum(Vector(0,0,0));
	m_boundary.setCentre(Vector(0,0,0));	
	
	bool selObject = false;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		addBoundingBox((*it)->getAabb());
		selObject = true;
	}
	
	/*
	double distance;
	if(selObject)
		distance = 0.0;
	else
		distance = m_fD;
	
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		Vertex *pV = it->getVertex();
		
		Aabb aabb;
		aabb.setMaximum(Vector(pV->vPos.x, pV->vPos.y, pV->vPos.z));
   		aabb.setMinimum(Vector(pV->vPos.x, pV->vPos.y, pV->vPos.z));
   		aabb.setCentre(Vector(pV->vPos.x, pV->vPos.y, pV->vPos.z));

   		addBoundingBox(aabb);
	}*/

}


void SelectionBuffer::addSingleVertex(Vertex* v,Polygon* p)
{
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		if(it->getVertex() == v)
			return;
	}
	
	SelectedVertex sv(v,p);
	m_vertices.push_back(sv);
	
	Aabb aabb;
	Vertex *pV = sv.getVertex();
	
	aabb.setMaximum(Vector(pV->vPos.x+m_fD, pV->vPos.y+m_fD, pV->vPos.z+m_fD));
   	aabb.setMinimum(Vector(pV->vPos.x-m_fD, pV->vPos.y-m_fD, pV->vPos.z-m_fD));
   	aabb.setCentre(Vector(pV->vPos.x, pV->vPos.y, pV->vPos.z));

   	addBoundingBox(aabb);
}

void SelectionBuffer::addBoundingBox(const Aabb &newBox)
{
	if ( m_objects.size() == 1 || m_vertices.size() == 1) 
  	{
      	m_boundary = newBox;
      	
      	Vector newMin = m_boundary.getMinimum();
      	newMin.x -= m_fD;
      	newMin.y -= m_fD;
      	m_boundary.setMinimum(  newMin );
      	
      	Vector newMax = m_boundary.getMaximum();
      	newMax.x += m_fD;
      	newMax.y += m_fD;
      	m_boundary.setMaximum(  newMax );
      	
      	m_boundary.setCentre((m_boundary.getMaximum() + m_boundary.getMinimum())/2.0) ;
  	}
  	else 
   	{
      	if ((newBox.getMinimum().x-m_fD) < m_boundary.getMinimum().x)
      	{
      		Vector minX =  m_boundary.getMinimum();
      		minX.x = newBox.getMinimum().x - m_fD;
         	m_boundary.setMinimum(minX);
      	}  
      		
      	if ((newBox.getMinimum().y-m_fD) < m_boundary.getMinimum().y)
      	{
      		Vector minY =  m_boundary.getMinimum();
      		minY.y = newBox.getMinimum().y - m_fD;
         	m_boundary.setMinimum(minY);
      	}  	
         	
      	if ((newBox.getMinimum().z-m_fD) < m_boundary.getMinimum().z)
      	{
      		Vector minZ =  m_boundary.getMinimum();
      		minZ.z = newBox.getMinimum().z - m_fD;
         	m_boundary.setMinimum(minZ);
      	}  	

      	if ((newBox.getMaximum().x+m_fD) > m_boundary.getMaximum().x)
      	{
      		Vector maxX =  m_boundary.getMaximum();
      		maxX.x = newBox.getMaximum().x + m_fD;
         	m_boundary.setMaximum(maxX);
      	}  
      		
      	if ((newBox.getMaximum().y+m_fD) > m_boundary.getMaximum().y)
      	{
      		Vector maxY =  m_boundary.getMaximum();
      		maxY.y = newBox.getMaximum().y + m_fD;
         	m_boundary.setMaximum(maxY);
      	}  	
         	
      	if ((newBox.getMaximum().z+m_fD) > m_boundary.getMaximum().z)
      	{
      		Vector maxZ =  m_boundary.getMaximum();
      		maxZ.z = newBox.getMaximum().z + m_fD;
         	m_boundary.setMaximum(maxZ);
      	}  
      	m_boundary.setCentre((m_boundary.getMaximum() + m_boundary.getMinimum())/2.0) ;
	}
}


void SelectionBuffer::mergePolysToMesh()
{
	Polygon *p;
	Mesh newMesh;
	list<Polygon*> removePoly;
	bool      bAsked=false;
   	bool      bUseEm=false;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
      	if( (*it)->getType() != SCENETYPE_Polygon) 
        	continue;
        	
        p = dynamic_cast<Polygon*>(*it);
        
        if(p->isPartOfMesh())
        {
	    	if (bAsked && !bUseEm) 
	    		continue;
	        
	        if (!bAsked) 
	        {
	        	QMessageBox msgBox;
 				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
 				msgBox.setText("At least one selected polygon is part of a mesh. \nMerge them also?");
 				
	        	if(msgBox.exec() == QMessageBox::Yes)
	        		bUseEm=true;
	        	else
	        		bUseEm=false;
	        		   
	            bAsked=true;
	     	}
	     	
	     	if(bUseEm==true)
	     		newMesh.addPolygon(*p);
        }
        else
        {
        	newMesh.addPolygon(*p);
        }
        
        removePoly.push_back(p);
   	}
   	
    for(list<Polygon*>::iterator it = removePoly.begin();it != removePoly.end(); ++it) 
	   SceneManager::getInstance()->removeObject(*it);
	   
	newMesh.setName("merged");
	SceneManager::getInstance()->insertObject(newMesh);
}

void SelectionBuffer::mergeMeshes()
{
	Mesh *m;
	Mesh bigMesh;
	list<Mesh*> removeMesh;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
      	if( (*it)->getType() != SCENETYPE_Mesh) 
        	continue;
        	
        m = dynamic_cast<Mesh*>(*it);
        removeMesh.push_back(m);
        
        vector<Polygon>*	polys =   m->getPolys();
        for(vector<Polygon>::iterator itPolys = polys->begin(); itPolys != polys->end(); ++itPolys)
			bigMesh.addPolygon(*itPolys); 
   	}
   	
   	for(list<Mesh*>::iterator it = removeMesh.begin();it != removeMesh.end(); ++it) 
	   SceneManager::getInstance()->removeObject(*it);
	   
	SceneManager::getInstance()->insertObject(bigMesh);
}

void SelectionBuffer::fragmentMeshs()
{
	Mesh *m;
	list<Mesh*> removeMesh;
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
      	if( (*it)->getType() != SCENETYPE_Mesh) 
        	continue;
        	
        m = dynamic_cast<Mesh*>(*it);
        removeMesh.push_back(m);
        
        vector<Polygon>*	polys =   m->getPolys();
        for(vector<Polygon>::iterator itPolys = polys->begin(); itPolys != polys->end(); ++itPolys)
		{
			Polygon pCopy = *itPolys;
			pCopy.setAsPartOfMesh("");
			SceneManager::getInstance()->insertObject(pCopy);
		} 
   	}
   	
    for(list<Mesh*>::iterator it = removeMesh.begin();it != removeMesh.end(); ++it) 
	   SceneManager::getInstance()->removeObject(*it);
}

void SelectionBuffer::changeBias()
{
	Polygon *poly;
	Mesh *mesh;
	
   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
      	switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Polygon:
            	poly = dynamic_cast<Polygon*>(*it);
				poly->changeBias();
	            break;
            
         	case SCENETYPE_Mesh:
            	mesh = dynamic_cast<Mesh*>(*it);
            	mesh->changeBias();
            	break;
            
         	default: 
         		break;
		} 
	}   	
}

void SelectionBuffer::rotate(const SceneAxis axis, const float a)
{
	Vector vc = m_boundary.getCentre();
	
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		(*it)->rotate(axis, vc, a);
		addBoundingBox( (*it)->getAabb());
	}
	
	if(m_vertices.size()==0)
		return;
	
	Matrix matRotate;
	Vector temp, tempN;
	
	switch(axis)
	{
		case AXIS_x:	matRotate = Matrix::makeRotateX(BSMath::degToRad(a)); 
		break;
		
		case AXIS_y:	matRotate = Matrix::makeRotateY(BSMath::degToRad(a)); 
		break;
		
		case AXIS_z:	matRotate = Matrix::makeRotateZ(BSMath::degToRad(a));
		break;
	}
	
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		// Rotate vertex position
		temp.x = it->getVertex()->vPos.x - vc.x;
      	temp.y = it->getVertex()->vPos.y - vc.y;
      	temp.z = it->getVertex()->vPos.z - vc.z;

      	temp = matRotate * temp;

      	it->getVertex()->vPos.x = temp.x + vc.x;
      	it->getVertex()->vPos.y = temp.y + vc.y;
      	it->getVertex()->vPos.z = temp.z + vc.z;
      	
      	// Rotate vertex normals
        it->getVertex()->vNormal = matRotate * it->getVertex()->vNormal;
		
		Polygon *p = it->getPolygon();
		p->forwardChanges();
	}	
}

void SelectionBuffer::mirror(const SceneAxis axis)
{
	Vector vc = m_boundary.getCentre();

	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		(*it)->mirror(axis, vc);
		addBoundingBox( (*it)->getAabb());
	}
}

void SelectionBuffer::mirrorWorld(const SceneAxis axis)
{
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
	{
		(*it)->mirrorWorld(axis);
		addBoundingBox( (*it)->getAabb());
	}
}

void SelectionBuffer::setMaterial(const int nID)
{
	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it)
		(*it)->setMaterialID(nID);
		
	SceneManager *sm = SceneManager::getInstance();
	sm->checkForRedraw(true);
}

list<SelectedVertex>& SelectionBuffer::getSelectedVertices()
{
	return m_vertices;
}

list<SceneObject*>& SelectionBuffer::getSelectedObjects()
{
	return m_objects;
}

void SelectionBuffer::transTextureCoords(double dtU, double dtV, double drU, double drV)
{
	Polygon *poly;
	Mesh    *mesh;
	
   	if ( (drU < 0.0001f) && (drU > -0.0001f)) 
   		drU = 0.0001f;
   	
   	if ( (drV < 0.0001f) && (drV > -0.0001f)) 
   		drV = 0.0001f;	
   		
   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
      	switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Polygon:
            	poly = dynamic_cast<Polygon*>(*it);
				poly->transTextureCoords(dtU,dtV,drU,drV);
	            break;
            
         	case SCENETYPE_Mesh:
            	mesh = dynamic_cast<Mesh*>(*it);
            	mesh->transTextureCoords(dtU,dtV,drU,drV);
            	break;
            
         	default: 
         		break;
		} 
	}   		
}

bool SelectionBuffer::getTextureTrans(double *pdtU, double *pdtV,double *pdrU, double *pdrV)
{
	Polygon *poly;
	Mesh    *mesh;
	double dtU=0.0f, dtV=0.0f, drU=0.0f, drV=0.0f;
	
	bool firstSet = false;
	
   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
		(*it)->deSelect();
		
      	switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Polygon:
            	poly = dynamic_cast<Polygon*>(*it);
				poly->getTextureTrans(&dtU,&dtV,&drU,&drV);
	            break;
            
         	case SCENETYPE_Mesh:
            	mesh = dynamic_cast<Mesh*>(*it);
            	mesh->getTextureTrans(&dtU,&dtV,&drU,&drV);
            	break;
            
         	default: 
         		continue;;
		} 
		
		if(firstSet)
			if ( (*pdtU != dtU) || (*pdtV != dtV) || (*pdrU != drU) || (*pdrV != drV) )
            	return false;
		
		*pdtU = dtU;  
		*pdtV = dtV;
	
      	*pdrU = drU;  
      	*pdrV = drV;
      	
      	firstSet = true;
	}   	
	
	return true;
}

void SelectionBuffer::scale(const double percentageX,const double percentageY,const double percentageZ)
{
   	for(list<SceneObject*>::iterator it = m_objects.begin();it != m_objects.end(); ++it) 
   	{  
   		switch ( (*it)->getType() ) 
      	{
        	case SCENETYPE_Polygon:
        	case SCENETYPE_Mesh:
   				(*it)->scale(percentageX,percentageY,percentageZ);
   				addBoundingBox( (*it)->getAabb());
   				break;
   			
   			default: break;
      	}
      	
   	}
}

void SelectionBuffer::arrangeTogether()
{
	if(m_vertices.size()==0)
		return;

	SelectedVertex v = m_vertices.front();
	Vertex *pV =  v.getVertex();
	
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		it->getVertex()->vPos.x = pV->vPos.x;
		it->getVertex()->vPos.y = pV->vPos.y;
		it->getVertex()->vPos.z = pV->vPos.z; 
		
		Polygon *p = it->getPolygon();
		p->forwardChanges();
	}
	
	m_boundary.setMaximum(Vector(pV->vPos.x+m_fD, pV->vPos.y+m_fD, pV->vPos.z+m_fD));
   	m_boundary.setMinimum(Vector(pV->vPos.x-m_fD, pV->vPos.y-m_fD, pV->vPos.z-m_fD));
   	m_boundary.setCentre(Vector(pV->vPos.x, pV->vPos.y, pV->vPos.z));
	
	SceneManager *sm = SceneManager::getInstance();
	sm->checkForRedraw(true);
}

void SelectionBuffer::alignAxis(const SceneAxis axis)
{
	if(m_vertices.size()==0)
		return;

	SelectedVertex v = m_vertices.front();
	Vertex *pV =  v.getVertex();
	
	for(list<SelectedVertex>::iterator it = m_vertices.begin();it != m_vertices.end(); ++it)
	{
		
		switch(axis)
		{
			case AXIS_x: it->getVertex()->vPos.x = pV->vPos.x;
				break;
			
			case AXIS_y: it->getVertex()->vPos.y = pV->vPos.y;
				break;
			
			case AXIS_z: it->getVertex()->vPos.z = pV->vPos.z; 
				break;
		}		
			
		Polygon *p = it->getPolygon();
		p->forwardChanges();
	}
	
	m_boundary.setMinimum(Vector(0,0,0));
	m_boundary.setMaximum(Vector(0,0,0));
	m_boundary.setCentre(Vector(0,0,0));
	
	adjustSelectionBoundaries();
	
	SceneManager *sm = SceneManager::getInstance();
	sm->checkForRedraw(true);	
	
	

}

}
