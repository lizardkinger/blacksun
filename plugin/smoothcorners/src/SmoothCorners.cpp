/***************************************************************************
 *   Copyright (C) 2006-07 by Philipp Gruber
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
 *  File:       SmoothCorners.cpp
 *  Created:    05.05.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/

#include "./../include/SmoothCorners.h"
#include <iostream>

using namespace std;

#include <iostream>

using namespace BSScene;

// Struktur die zum finden mehrfacher vertizes dient
/*struct pos
{
	int poly;
	int ind;
}*/

struct cVertex
{
	Vertex vert;
	vector<Vector> normal;
	//Vector normal;
	//int anz;

};

SmoothCorners::SmoothCorners() : SmoothCornersVersion(Version(1,0,0))
{
	smoothCornersButton = NULL;
}

SmoothCorners::~SmoothCorners()
{
}

// Functions, that implements from PlgInt:
UID SmoothCorners::getUID()
{
	return uid;
	vector<int> list;
}

QString SmoothCorners::getName()
{
	return "SmoothCorners-Plugin";
}
    
QString SmoothCorners::getAutor()
{
	return "Psirus";
}
    
QString SmoothCorners::getDescription()
{
	return "smoothes corners\nversion 1.0";
}

Version SmoothCorners::getVersion()
{
	return SmoothCornersVersion;
}

bool SmoothCorners::unloadPlg()
{
	if(smoothCornersButton != NULL)
	{
		delete smoothCornersButton;
		smoothCornersButton = NULL;	
	}
	
	return true;
}

bool SmoothCorners::loadPlg()
{
	int nButtonSize = 32;
	int nIconSize = 24;
	
	smoothCornersButton = new QPushButton(QIcon(":/media/SmoothCorners_32.png"), "");
	smoothCornersButton->setMaximumSize(nButtonSize, nButtonSize);
	smoothCornersButton->setMinimumSize(nButtonSize, nButtonSize);
	smoothCornersButton->setIconSize(QSize(nIconSize, nIconSize));
	smoothCornersButton->setToolTip("Smooth Corners");
	
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("modificatorGroupBox", smoothCornersButton);
    
	connect(smoothCornersButton, SIGNAL(clicked()), this, SLOT(execute()));
	return true;
}

// <<singleton>>

static SmoothCorners* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new SmoothCorners();
	
	return static_cast<PlgInt*> (_instance);
}

void SmoothCorners::execute()
{
	
	
	//cout << "start" << endl;
	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();

	vector<BSScene::Polygon>* polys;
	
	//Copy original mesh
	vector<Vertex> vertsOriginal;
	vector<int> indicesOriginal;
	
	vector<cVertex> searchList;

	// Liste aller mehrfachen Vertices zusammenstellen
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			polys = mesh->getPolys();
 		 
			//Walk through all polygons
			cout << "polygongroesse: "<< polys->size() << endl;
			for(unsigned int p=0; p<polys->size(); p++)
			{
				//cout << "Polygon: " << p << endl;
				
				
				BSScene::Polygon* poly = &((*polys)[p]);
				vector<Vertex> copyVertices = *(poly->getVertices());
				cout << "goesse vertexliste: " << copyVertices.size()<<endl;
				int temp = 0;
				cVertex tmp;
				if(searchList.size() == 0)
				{
					tmp.vert = copyVertices[0];
					//tmp.normal = copyVertices[0].vNormal;
					tmp.normal.push_back(copyVertices[0].vNormal);
					//tmp.anz = 1;
					searchList.push_back(tmp);
					temp = 1;
				}
			//	cout << "suchenliste: " << copyVertices.size() << endl;
				for(unsigned int i=temp; i<copyVertices.size(); i++)
				{
				//	cout << "suchen: " << i << endl;
					cout << "copyVertices: " << i << endl;
					bool check = false;
					for(unsigned int j=0; j<searchList.size(); j++)
					{
					//	cout << "searchList: " << j << endl;;
						//cout << "vergleichen: " << j << endl;
						//if(searchList[j].vert.vPos == copyVertices[i].vPos)
						if(isVectorNearTo(copyVertices[i].vPos, searchList[j].vert.vPos, EPSILON))
						{
							//cout << "gefunden" << endl;
							//cout << "near anfang" << endl;
							//searchList[j].anz = searchList[j].anz+1;
							//searchList[j].normal = (searchList[j].normal+copyVertices[i].vNormal)/2;
							searchList[j].normal.push_back(copyVertices[i].vNormal);
							check = true;
							break;
							//cout << "gefunden: " << endl;
						}
					}
					if(!check)
					{
						//cout << "neu" << endl;
						tmp.vert = copyVertices[i];
						//tmp.normal.push_back(copyVertices[i].vNormal);
						tmp.normal.clear();
						tmp.normal.push_back(copyVertices[i].vNormal);
						//tmp.anz = 1;
						searchList.push_back(tmp);
					}
				}
			}
			//cout << "ferdisch" << endl;
		}
   	}
   //	cout << "listengroesse: " << searchList.size() << endl;
   
   
 
   
   
   	// Neue Normalen eintragen
   	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			polys = mesh->getPolys();
			
			// fr jedes polygonarray des meshs
			for(unsigned int p=0; p<polys->size(); p++)
			{
				BSScene::Polygon* poly = &((*polys)[p]);
				vector<Vertex> copyVertices = *(poly->getVertices());
				vector<int> copyIndices = *(poly->getIndices());
				
				for(unsigned int i=0; i<copyVertices.size(); i++)
				{
					for(unsigned int j=0; j<searchList.size(); j++)
					{
						//cout << "anz norm: " << searchList[j].normal.size() << endl;
						//if(copyVertices[i].vPos == searchList[j].vert.vPos)
						if(isVectorNearTo(copyVertices[i].vPos, searchList[j].vert.vPos, EPSILON))
						{
							
							if(searchList[j].normal.size() > 2)
							{
							//	cout << "ok: " << j << endl;
								Vector tNorm;
								tNorm.x = 0.0;
   								tNorm.y = 0.0;
   								tNorm.z = 0.0;
								for(unsigned int n=0; n<searchList[j].normal.size(); n++)
								{
									tNorm = tNorm + searchList[j].normal[n];
								}
								tNorm = tNorm/searchList[j].normal.size();
								tNorm.normalize();
								copyVertices[i].vNormal = tNorm;/*searchList[j].normal;*/
							}
							else
							{
							//	cout << "geht ned !" << endl;
							}
						}
					}
				}
				poly->setIndices(copyIndices);
				poly->setVertices(copyVertices);	
				SceneManager::getInstance()->checkForRedraw(true);
			}
		}
   	}
   	
  
}

bool SmoothCorners::isVectorNearTo(const Vector& v1, const Vector& v2, double dEpsilon) const
{
	//cout << "nearto anfang" << endl;
	return (isNearTo(v1.x, v2.x, dEpsilon) && isNearTo(v1.y, v2.y, dEpsilon) && isNearTo(v1.z, v2.z, dEpsilon));
	//cout << v2.x << endl;
//	cout << v2.y << endl;
	//cout << v2.z << endl;
//	cout << v1.x << endl;
	//cout << v1.y << endl;
	//cout << v1.z << endl;
/*	if(v2.x > v1.x-0.1 && v2.x < v1.x+0.1)
	{
		cout << "if 1" << endl;
		if(v2.y > v1.y-0.1 && v2.y < v1.y+0.1)
		{
			cout << "if 2" << endl;
			if(v2.z > v1.z-0.1 && v2.z < v1.z+0.1)
			{
				cout << "if 3" << endl;
				return true;
			}
		}
	}
	else
	{
		cout << "else" << endl;
		return false;
	}*/
//	cout << "nearto ende" << endl;
}
