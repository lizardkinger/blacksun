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
 *  File:       NBoxGen.cpp
 *  Created:    04.03.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
#include "./../include/NBoxGen.h"
#include "./../../gui/include/mainwindow.h"
#include "./../../../editor/math/include/Vector3.h"
#include <SceneGraph.h>
#include <iostream>


//### ...
/*
#include <SceneManager.h>
#include <Mesh.h>
#include <SelectionBuffer.h>
*/

using namespace BSScene;

NBoxGen::NBoxGen() : nBoxGenVersion(Version(1,0,0))
{
	createnBoxButton = NULL;
}

NBoxGen::~NBoxGen()
{
}

// Functions, that implements from PlgInt:
UID NBoxGen::getUID()
{
	return uid;
}

QString NBoxGen::getName()
{
	return "nBoxGenerator";
}
    
QString NBoxGen::getAutor()
{
	return "Psirus";
}
    
QString NBoxGen::getDescription()
{
	return "create a nBox\nversion 1.0";
}

Version NBoxGen::getVersion()
{
	return nBoxGenVersion;
}

bool NBoxGen::unloadPlg()
{
	if(createnBoxButton != NULL)
	{
		delete createnBoxButton;
		createnBoxButton = NULL;
	}
	
	return true;
}

bool NBoxGen::loadPlg()
{
    BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
	LOG_Ok("nBox-Test");
    
	int nButtonSize = 32;
	int nIconSize = 24;
	
	createnBoxButton = new QPushButton(QIcon(":/media/GenNBox_32.png"), "");
	createnBoxButton->setMaximumSize(nButtonSize, nButtonSize);
	createnBoxButton->setMinimumSize(nButtonSize, nButtonSize);
	createnBoxButton->setIconSize(QSize(nIconSize, nIconSize));
	createnBoxButton->setToolTip("nBox");
	createnBoxButton->setCheckable(true);
	createnBoxButton->setAutoExclusive(true);
    
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", createnBoxButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(createnBoxButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(createBox()));
    
	return true;
}

// <<singleton>>

static NBoxGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new NBoxGen();
	
	return static_cast<PlgInt*> (_instance);
}

void NBoxGen::createBox()
{
	int nMaterialBox = Renderer::getInstance()->getMaterialManager()->createNewMaterial();
    Material* pMat = Renderer::getInstance()->getMaterialManager()->getMaterial(nMaterialBox);
    pMat->setColor(Color(1.0, 1.0, 1.0));
        
	sharedVertices = ui.sharedVerticesBox->isChecked();
	centered = ui.centeredBox->isChecked();
	parts = ui.partsBox->value();
    corners = ui.cornersBox->value();
	radius = ui.radiusBox->value();
	height = ui.heightBox->value();
	
	SelectionBuffer sb;
	SceneManager *sm = SceneManager::getInstance();
	Mesh nBox;
	nBox.setName("nBox");
	BSScene::Polygon bottom;
	BSScene::Polygon top;
	BSScene::Polygon mantle;
	vector<Vertex> vBottom;
	vector<Vertex> vTop;
	vector<Vertex> vMantle;
	vector<int> iBottom;
	vector<int> iTop;
	vector<int> iMantle;
	
	
	// Koordinaten der Ecken Berechnen
	
	// Winkel zwischen den Vektoren zu den Ecken
	double angle = -360.0/static_cast<double>(corners);

	// BODEN
	Vector* bottomList = new Vector[corners+1];
	// Ursprung (Zentrum) damit dieser auch in der Vertexliste vorhanden ist
	bottomList[0].set(0.0,-height/2 ,0.0);
	// Vektor vom Ursprung zur ersten Ecke von dem ausgegangen wird
	bottomList[1].set(radius,-height/2 ,0.0);
	for(int i=2; i<corners+1; i++)
	{
		// Vorgänger-Vektor um Winkel rotieren
		Matrix mRot = Matrix::makeRotateY(degToRad(angle));
		Vector vRot = bottomList[i-1] * mRot;
		
		// Werte in Liste schreiben
		bottomList[i].set(vRot.x,vRot.y,vRot.z);
	}
		
	// Vertexliste für den Boden füllen
	for(int i=0; i<corners+1; i++)
	{	
		double dU = (bottomList[i].x / radius + 1.0) * 0.5;
		double dV = (bottomList[i].z / radius + 1.0) * 0.5;
		vBottom.push_back(Vertex(Vector(bottomList[i].x, bottomList[i].y, bottomList[i].z),	Vector( 0.0,  -1.0,  0.0),dU, dV));
	}
		
	// Indexliste für Boden füllen
	// Von der Mitte aus gehend
	if(centered)
	{	
		int ctmp = 0;
		while(ctmp < corners-1)
		{
			iBottom.push_back(0);
			iBottom.push_back(ctmp+2);
			iBottom.push_back(ctmp+1);
			
			ctmp++;
		}
		iBottom.push_back(0);
		iBottom.push_back(1);
		iBottom.push_back(corners);
	
	}
	// Von Ecke aus gehend
	else
	{		
		for(int i=0; i<corners-2; i++)
		{
			iBottom.push_back(1);
			iBottom.push_back(corners-i);
			iBottom.push_back(corners-i-1);
		}
	}
	
	// Werte setzten
	bottom.setVertices(vBottom);
	bottom.setIndices(iBottom);
	bottom.setMaterialID(nMaterialBox);
	nBox.addPolygon(bottom);
		
	// DECKEL
	Vector* topList = new Vector[corners+1];
		
	// Liste füllen (selben Werte wie bottom nur geänderte y-komponente)
	for(int i=0; i<corners+1; i++)
	{
		topList[i].set(bottomList[i].x, height/2, bottomList[i].z);
	}
		
	// Vertexliste für den Deckel füllen
	for(int i=0; i<corners+1; i++)
	{
		double dU = (topList[i].x / radius + 1.0) * 0.5;
		double dV = (topList[i].z / radius + 1.0) * 0.5;		
		vTop.push_back(Vertex(Vector(topList[i].x, topList[i].y, topList[i].z),	Vector( 0.0,  1.0,  0.0),dU,dV));
	}
		
	// Von Mitte aus gehend
	if(centered)
	{
		// Indexliste füllen
		int ctmp = 0;
		while(ctmp < corners-1)
		{
			iTop.push_back(0);
			iTop.push_back(ctmp+1);
			iTop.push_back(ctmp+2);
			
			ctmp++;
		}
		iTop.push_back(0);
		iTop.push_back(corners);
		iTop.push_back(1);
	}
	// Von Ecke aus gehend
	else
	{		
		for(int i=0; i<corners-2; i++)
		{
			iTop.push_back(1);
			iTop.push_back(i+2);
			iTop.push_back(i+3);
		}
	}

	// Werte setzen
	top.setVertices(vTop);
	top.setIndices(iTop);
	top.setMaterialID(nMaterialBox);
	nBox.addPolygon(top);
	
	// MANTEL
	// Zweig für geteilte Vertices
	if(sharedVertices)
	{
		Vector tmp;
		double height = topList[1].y - bottomList[1].y;
		double distance = height/parts;
		
		// Vektorliste für Mantel füllen
		for(int i=1; i<=corners; i++)
		{	
			// Normale berechen
			tmp.x = bottomList[i].x;
			tmp.y = 0.0;
			tmp.z = bottomList[i].z;
			if(tmp != Vector(0.0, 0.0, 0.0))
				tmp.normalize();
			
			vMantle.push_back(Vertex(bottomList[i], tmp, 1.0/corners*i,0.0));	
			for(int j=1; j<parts; j++)
			{	
				Vector tmp1;
				tmp1.x = bottomList[i].x;
				tmp1.y = bottomList[i].y+j*distance;
				tmp1.z = bottomList[i].z;
				vMantle.push_back(Vertex(tmp1, tmp, 1.0/corners*i,1.0/parts*j));
			}
			vMantle.push_back(Vertex(topList[i], tmp, 1.0/corners*i,1.0));
		}
		
		// Indexliste für Mantel füllen
		int a = 0;
		int b = parts+1;
		int c = 1;
		
		for(int i=0; i<corners; i++)
		{
			if(i == corners-1)
			{
				b = 0;
			}
			
			for(int j=0; j<parts; j++)
			{
				iMantle.push_back(a);
				iMantle.push_back(b);
				iMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			
			a++;
			b++;
			c++;
		}
		
		a = parts+1;
		b = parts+2;
		c = 1;
		
		for(int i=0; i<corners; i++)
		{
			if(i == corners-1)
			{
				a = 0;
				b = 1;
			}
			
			for(int j=0; j<parts; j++)
			{
				iMantle.push_back(a);
				iMantle.push_back(b);
				iMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			
			a++;
			b++;
			c++;
		}
	}
	// Zweig für doppelte Vertices
	else
	{
		Vector norm;
		double height = topList[1].y - bottomList[1].y;
		double distance = height/parts;
		
		for(int i=1; i<=corners; i++)
		{			
			// Normale berechnen
			Vector tmp1;
			Vector tmp2;
			
			// Zwei Seitenvektoren für´s Kreizprodukt berechnen
			tmp1.x = bottomList[i].x - topList[i].x;
			tmp1.y = bottomList[i].y - topList[i].y;
			tmp1.z = bottomList[i].z - topList[i].z;
			
			if(i == corners)
			{
				tmp2.x = topList[1].x - topList[i].x;
				tmp2.y = topList[1].y - topList[i].y;
				tmp2.z = topList[1].z - topList[i].z;
			}
			else
			{
				tmp2.x = topList[i+1].x - topList[i].x;
				tmp2.y = topList[i+1].y - topList[i].y;
				tmp2.z = topList[i+1].z - topList[i].z;
			}
			
			Vector norm;
			norm.cross(tmp1, tmp2);
			norm.normalize();
			
			// Vektorliste füllen
			vMantle.push_back(Vertex(bottomList[i], norm, 1.0/corners*(i-1),0.0));	
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				tmp.x = bottomList[i].x;
				tmp.y = bottomList[i].y+j*distance;
				tmp.z = bottomList[i].z;
				vMantle.push_back(Vertex(tmp, norm, 1.0/corners*(i-1),1.0/parts*j));
			}
			vMantle.push_back(Vertex(topList[i], norm, 1.0/corners*(i-1),1.0));
			
			if(i == corners)
			{
				vMantle.push_back(Vertex(bottomList[1], norm, 1.0,0.0));
			}
			else
			{
				vMantle.push_back(Vertex(bottomList[i+1], norm, 1.0/corners*i,0.0));	
			}
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				if(i == corners)
				{
					tmp.x = bottomList[1].x;
					tmp.y = bottomList[1].y+j*distance;
					tmp.z = bottomList[1].z;
					
					vMantle.push_back(Vertex(tmp, norm, 1.0,1.0/parts*j));
				}
				else
				{
					tmp.x = bottomList[i+1].x;
					tmp.y = bottomList[i+1].y+j*distance;
					tmp.z = bottomList[i+1].z;
					vMantle.push_back(Vertex(tmp, norm, 1.0/corners*i,1.0/parts*j));
				}				
			}
			if(i == corners)
			{
				vMantle.push_back(Vertex(topList[1], norm, 1.0,1.0));
			}
			else
			{
				vMantle.push_back(Vertex(topList[i+1], norm, 1.0/corners*i,1.0));
			}
		}
			
		// Indexliste für Mantel füllen 
		int a = 0;
		int b = parts+1;
		int c = 1;
	
		for(int i=0; i<corners; i++)
		{
			for(int j=0; j<parts; j++)
			{
				iMantle.push_back(a);
				iMantle.push_back(b);
				iMantle.push_back(c);
			
				a++;
				b++;
				c++;
			}
			a = a+2+parts;
			b = b+2+parts;
			c = c+2+parts;
		}
		
		a = parts+1;
		b = parts+2;
		c = 1;
		
		for(int i=0; i<corners; i++)
		{	
			for(int j=0; j<parts; j++)
			{
				iMantle.push_back(a);
				iMantle.push_back(b);
				iMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			a = a+2+parts;
			b = b+2+parts;
			c = c+2+parts;
		}
	}
	
	mantle.setVertices(vMantle);
	mantle.setIndices(iMantle);
	mantle.setMaterialID(nMaterialBox);
	nBox.addPolygon(mantle);
	
	// Dynamische Arrays löschen
	delete [] bottomList;
	delete [] topList;	
	
	sm->insertObject(nBox);
	
	LOG_Ok("nBox added");
}
