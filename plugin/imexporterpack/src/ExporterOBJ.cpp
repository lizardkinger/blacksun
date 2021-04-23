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
 *  File:       ExporterOBJ.cpp
 *  Created:    09.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/ExporterOBJ.h"
#include "../include/ExporterOBJWriter.h"
#include <QtGui> 

using namespace BSRenderer;
using namespace BSScene;

bool ExporterOBJ::execute()
{
	QString sFilter = getFileDescription().c_str();
	sFilter += " (*.";
	sFilter += getFileExtension().c_str();
	sFilter += ")";
	
	QString sFileName = QFileDialog::getSaveFileName(NULL,
		"Export model file", QDir::currentPath(),
		sFilter);

	//QFileDialog aborted?
	if(sFileName == "")
		return true;

	//Get model from scenegraph
	vector< vector<Vertex> > verts;
	vector< vector<int> > indices;
	vector<Material*> materials;
	
	Scene &s = SceneManager::getInstance()->getScene();
	list<Mesh>*	 meshes = s.getSceneMeshes();
	 
	for(list<Mesh>::iterator itMesh=meshes->begin(); itMesh!=meshes->end(); ++itMesh)
 	{
 			vector<BSScene::Polygon>* polys = itMesh->getPolys();
 		 
			//Walk through all polygons
			for(unsigned int i=0; i<polys->size(); i++)
			{	
				BSScene::Polygon* p = &((*polys)[i]);
				
				verts.push_back(*(p->getVertices()));
				indices.push_back(*(p->getIndices()));
				materials.push_back(MaterialManager::getInstance()->getMaterial(p->getMaterialID()));
			}
 	}	

	ExporterOBJWriter writer;
	if(writer.execute(sFileName.toStdString(), verts, indices, materials)==false)
		return false;

	stringstream msg;
	msg << "Model saved: \"" << sFileName.toStdString() << "\"";
	LOG_Ok(msg.str());
		
	return true;
}
