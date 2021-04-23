/***************************************************************************
 *   Copyright (C) 2006-07 by Gruber Philipp
 *   pgruber@fh-landshut.de
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
 *  File:       Explode.cpp
 *  Created:    06.04.2007
 *  Author:     Philipp Gruber (psirus)
 *
 **************************************************************************/

#include "./../include/Explode.h"
#include <stdlib.h>
#include <iostream>


static Explode* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Explode();

	return static_cast<PlgInt*>(_instance);
}

Explode::Explode() : version(Version(1, 0, 0))
{
	m_sName = "Explode-Plugin";
	m_sDescription = "asdf";
	
	explodeButton = NULL;
}

Explode::~Explode()
{
}

bool Explode::unloadPlg()
{
	if(explodeButton)
	{
		delete explodeButton;
		explodeButton = NULL;
	}
	
	return true;
}

bool Explode::loadPlg()
{
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
	LOG_Ok("Explode-Test");
    
	int nButtonSize = 32;
	int nIconSize = 24;
	
	explodeButton = new QPushButton(QIcon(":/media/Explode_32.png"), "");
	explodeButton->setMaximumSize(nButtonSize, nButtonSize);
	explodeButton->setMinimumSize(nButtonSize, nButtonSize);
	explodeButton->setIconSize(QSize(nIconSize, nIconSize));
	explodeButton->setToolTip("Explode");
	explodeButton->setCheckable(true);
	explodeButton->setAutoExclusive(true);	
	
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", explodeButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("modificatorGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(explodeButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
    
	return true;
}

void Explode::execute()
{
	double distance = ui.distance->value();
	double diffusion = ui.diffusion->value();
	bool center = ui.center->isChecked();
	bool recalcNormals = ui.recalcnorm->isChecked();
	
	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& sObjects = sb.getSelectedObjects();
	
	
	//für jedes selectierte Objekt
	for(list<SceneObject*>::iterator it = sObjects.begin();it != sObjects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
			
			// für jedes polygonarray des mesh´s
			for(unsigned int i=0; i<polys->size(); i++)
			{
				if(center)
				{
					makeCenterExplosion(&((*polys)[i]), distance, recalcNormals, diffusion);
				}
				else
				{
					makeNormalExplosion(&((*polys)[i]), distance, recalcNormals, diffusion);
				}
			}
			
			SceneManager::getInstance()->notifyContentChanged(mesh->getType(),true);
		}
	}
}

void Explode::makeNormalExplosion(BSScene::Polygon* p, double distance, bool recalcNormals, double diffusion)
{
	vector<int> indices;
	vector<Vertex> vertices;
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	int count = copyVertices.size();
	/*cout << "Vertexliste:" << endl;
	for(int i=0; i<copyVertices.size(); i++)
	{
		cout << i << ": x: "<<copyVertices[i].vPos.x << " y: "<< copyVertices[i].vPos.y<<" z: "<<copyVertices[i].vPos.z << endl;
	}*/
	
	// prüfen ob vertices shared durch Verhältnis vertices : intizes
	//if(copyVertices.size() != copyIndices.size())
	//{
		cout << "Vertices spalten" << endl;
		// dann sind vertizes shared und müssen "aufgebrochen" werden
		vector<int> tmp;
						
		// doppelte indizes suchen
	/*	for(int j=0; j<copyIndices.size(); j++)
		{
			for(unsigned int k=0; k<tmp.size(); k++)
			{
				// wenn doppelter index gefunden
				if(copyIndices[j] == tmp[k])
				{
					// vertices kopieren und index ändern/anpassen
					copyVertices.push_back(copyVertices[copyIndices[j]]);
					cout << "kopierter vertex: "<< j << endl;
					cout << "x: "<<copyVertices[copyIndices[j]].vPos.x << " y: "<< copyVertices[copyIndices[j]].vPos.y<<" z: "<<copyVertices[copyIndices[j]].vPos.z << endl;
					copyIndices[j] = count;
					count++;
				}
			}
			tmp.push_back(copyIndices[j]);
		}*/
		for(int j=0; j<copyIndices.size(); j++)
		{
			vertices.push_back(copyVertices[copyIndices[j]]);
			indices.push_back(j);
		}
//	}
	
	cout << "Vertexlistengrösse: " << copyVertices.size() << endl;
	cout << "Indexlistengrösse: " << copyIndices.size() << endl;
	
	
	/*cout << "Vertexliste nacher:" << endl;
	for(int i=0; i<copyVertices.size(); i++)
	{
		cout << i << ": x: "<<copyVertices[i].vPos.x << " y: "<< copyVertices[i].vPos.y<<" z: "<<copyVertices[i].vPos.z << endl;
	}*/
	// Normalen neu berechnen
	if(recalcNormals)
	{
		Vector temp;
		for(int j=0; j</*copyI*/indices.size(); j++)
		{
			// alle 3 vertizes die normale neu berechnen
			if(j%3 == 0)
			{
				Vector t1 = /*copyV*/vertices[/*copyI*/indices[j+1]].vPos - /*copyV*/vertices[/*copyI*/indices[j]].vPos;
				Vector t2 = /*copyV*/vertices[/*copyI*/indices[j+2]].vPos - /*copyV*/vertices[/*copyI*/indices[j]].vPos;
				cout << "t1: "<< t1.x << " | " << t1.y << " | " << t1.z << endl;
				cout << "t2: "<< t2.x << " | " << t2.y << " | " << t2.z << endl;
				temp.cross(t1,t2);
				cout << "berechnung:" << endl;
				cout << copyVertices[copyIndices[j+1]].vPos.x << " - " << copyVertices[copyIndices[j]].vPos.x << endl;
				cout << copyVertices[copyIndices[j+1]].vPos.y << " - " << copyVertices[copyIndices[j]].vPos.y << endl;
				cout << copyVertices[copyIndices[j+1]].vPos.z << " - " << copyVertices[copyIndices[j]].vPos.z << endl;
				cout << copyVertices[copyIndices[j+2]].vPos.x << " - " << copyVertices[copyIndices[j]].vPos.x << endl;
				cout << copyVertices[copyIndices[j+2]].vPos.y << " - " << copyVertices[copyIndices[j]].vPos.y << endl;
				cout << copyVertices[copyIndices[j+2]].vPos.z << " - " << copyVertices[copyIndices[j]].vPos.z << endl;
				cout << "neue Normale:" << endl;
				cout << "x: "<<temp.x << "  y: "<<temp.y << "  z: "<<temp.z << endl;
				
				if(temp.length() != 0.0)
				{
					temp.normalize();
				}
			}
			
			/*copyV*/vertices[/*copyI*/indices[j]].vNormal = temp;
		}
	}
	
	// vertices verschieben
	
	double tmpDistance;
	double random;
	srand(time(NULL));
	for(int j=0; j</*copyV*/vertices.size(); j++)
	{
		if(j%3 == 0)
		{
			random = (double)rand()/(double)RAND_MAX;
			tmpDistance = distance + diffusion * (random+1) - diffusion;
		}
		/*copyV*/vertices[j].vPos = /*copyV*/vertices[j].vPos + /*copyV*/vertices[j].vNormal * tmpDistance;
	}
	
	cout << "Vertexlistengrösse: " << copyVertices.size() << endl;
	cout << "Indexlistengrösse: " << copyIndices.size() << endl;
	
	cout << "neue Vertexlistengrösse: " << vertices.size() << endl;
	cout << "neue Indexlistengrösse: " << indices.size() << endl;
	
/*	for(int i=0; i<copyIndices.size(); i++)
	{
		cout << i << ": " << copyIndices[i] << endl;
	}*/
	/*cout << "Normalen:" << endl;
	for(int i=0; i<copyVertices.size(); i++)
	{
		cout << i << ": x: "<<copyVertices[i].vNormal.x << " y: "<< copyVertices[i].vNormal.y<<" z: "<<copyVertices[i].vNormal.z << endl;
	}*/
	
	cout << endl;
	
	p->setIndices(/*copyI*/indices);
	p->setVertices(/*copyV*/vertices);	
}

void Explode::makeCenterExplosion(BSScene::Polygon* p, double distance, bool recalcNormals, double diffusion)
{
	SceneManager *sm = SceneManager::getInstance();
	Aabb aabb = sm->getSelBuffer().getBoundary();
	Vector center = aabb.getCentre();
	
	vector<int> indices;
	vector<Vertex> vertices;
	vector<int> copyIndices = *(p->getIndices());
	vector<Vertex> copyVertices = *(p->getVertices());
	int count = copyVertices.size();
	vector<int> tmp;
						
	for(int j=0; j<copyIndices.size(); j++)
	{
		vertices.push_back(copyVertices[copyIndices[j]]);
		indices.push_back(j);
	}
	
	// Normalen neu berechnen
	if(recalcNormals)
	{
		Vector temp;
		for(int j=0; j</*copyI*/indices.size(); j++)
		{
			// alle 3 vertizes die normale neu berechnen
			if(j%3 == 0)
			{
				Vector t1 = vertices[indices[j+1]].vPos - vertices[indices[j]].vPos;
				Vector t2 = vertices[indices[j+2]].vPos - vertices[indices[j]].vPos;
				temp.cross(t1,t2);
				
				if(temp.length() != 0.0)
				{
					temp.normalize();
				}
			}
			
			vertices[indices[j]].vNormal = temp;
		}
	}
	
	// vertices verschieben
	
	double tmpDistance;
	double random;
	srand(time(NULL));
	Vector move;
	Vector centroid;
	cout << "vektor move" << endl;
	cout << center.x << " | " << center.y << " | " << center.z << endl;
	for(int j=0; j<vertices.size(); j++)
	{
		if(j%3 == 0)
		{
			// Schwerpunkt der dreiecke
			centroid.x = (vertices[j].vPos.x+vertices[j+1].vPos.x+vertices[j+2].vPos.x)/3;
			centroid.y = (vertices[j].vPos.y+vertices[j+1].vPos.y+vertices[j+2].vPos.y)/3;
			centroid.z = (vertices[j].vPos.z+vertices[j+1].vPos.z+vertices[j+2].vPos.z)/3;
			move = centroid-center;
			move.normalize();
			//cout << move.x << " | " << move.y << " | " << move.z << endl;
			random = (double)rand()/(double)RAND_MAX;
			tmpDistance = distance + diffusion * (random+1) - diffusion;
		}
		vertices[j].vPos = vertices[j].vPos + move * distance;
	}
	
	p->setIndices(indices);
	p->setVertices(vertices);	
}
