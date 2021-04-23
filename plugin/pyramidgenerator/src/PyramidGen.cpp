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
 *  File:       PyramidGen.cpp
 *  Created:    04.03.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
#include "./../include/PyramidGen.h"
#include "./../../../editor/math/include/Vector3.h"
#include "./../../gui/include/mainwindow.h"
#include <math.h>
#include <SceneGraph.h>
#include <iostream>


PyramidGen::PyramidGen() : pyramidGenVersion(Version(1,0,0))
{
	createPyramidButton = NULL;
}

PyramidGen::~PyramidGen()
{
}

// Functions, that implements from PlgInt:
UID PyramidGen::getUID()
{
	return uid;
}

QString PyramidGen::getName()
{
	return "PyramidGenerator";
}
    
QString PyramidGen::getAutor()
{
	return "Psirus";
}
    
QString PyramidGen::getDescription()
{
	return "create a Pyramid\nversion 1.0";
}

Version PyramidGen::getVersion()
{
	return pyramidGenVersion;
}

bool PyramidGen::unloadPlg()
{
	if(createPyramidButton != NULL)
	{
		delete createPyramidButton;
		createPyramidButton = NULL;
	}
	
	return true;
}

bool PyramidGen::loadPlg()
{
	LOG_Ok("Pyramid-Test");
    
	int nButtonSize = 32;
	int nIconSize = 24;
	
	createPyramidButton = new QPushButton(QIcon(":/media/GenPyramid_32.png"), "");
	createPyramidButton->setMaximumSize(nButtonSize, nButtonSize);
	createPyramidButton->setMinimumSize(nButtonSize, nButtonSize);
	createPyramidButton->setIconSize(QSize(nIconSize, nIconSize));
	createPyramidButton->setToolTip("Pyramid");
	createPyramidButton->setCheckable(true);
	createPyramidButton->setAutoExclusive(true);
    
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", createPyramidButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(createPyramidButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(createPyramid()));
    
	return true;
}

// <<singleton>>

static PyramidGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new PyramidGen();
	
	return static_cast<PlgInt*> (_instance);
}

void PyramidGen::createPyramid()
{
	int nMaterialPyramid = Renderer::getInstance()->getMaterialManager()->createNewMaterial();
    Material* pMat = Renderer::getInstance()->getMaterialManager()->getMaterial(nMaterialPyramid);
    pMat->setColor(Color(1.0, 1.0, 1.0));
        
	sharedVertices = ui.sharedVerticesPyramid->isChecked();
	centered = ui.centeredPyramid->isChecked();
	corners = ui.cornersPyramid->value();
	radius = ui.radiusPyramid->value();
	height = ui.heightPyramid->value();
	
	SelectionBuffer sb;
	SceneManager *sm = SceneManager::getInstance();
	Mesh pyramid;
	pyramid.setName("Pyramid");
	BSScene::Polygon bottom;
	BSScene::Polygon mantle;
	vector<Vertex> vBottom;
	vector<Vertex> vMantle;
	vector<int> iBottom;
	vector<int> iMantle;
	
	
	// Koordinaten der Ecken Berechnen
	
	// Winkel zwischen den Vektoren zu den Ecken
	double angle = -360.0/static_cast<double>(corners);
	double sideWidth = sqrt(2*radius*radius);

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
	bottom.setMaterialID(nMaterialPyramid);
	pyramid.addPolygon(bottom);
		
	
	
	// MANTEL		
	// Zweig für geteilte Vertices
	if(sharedVertices)
	{
		Vector tmp;
		Vector norm;
		
		//Spitze in Liste einfügen
		tmp.x = 0.0;
		tmp.y = height/2;
		tmp.z = 0.0;
		
		norm.x = 0.0;
		norm.y = 1.0;
		norm.z = 0.0;
		vMantle.push_back(Vertex(tmp, norm, 0.5,1.0));
		
		// Vektorliste für Mantel füllen
		for(int i=1; i<=corners; i++)
		{	
			// Normale berechen
			Vector tmp1;
			Vector tmp2;
			
			// Winkel zwischen Ebene und Normale berechnen
			tmp1.x = bottomList[i].x;
			tmp1.y = bottomList[i].y - height/2;
			tmp1.z = bottomList[i].z;
			
			tmp2.x = 0.0;
			tmp2.y = bottomList[0].y - height/2;
			tmp2.z = 0.0;
			
			double angle = tmp1.angleWith(tmp2);
			
			// Vektor drehen
			Matrix mRot = Matrix::makeRotateX(angle);
			norm = bottomList[i] * mRot;
			if(norm != Vector(0.0, 0.0, 0.0))
				norm.normalize();
			
			tmp.x = bottomList[i].x;
			tmp.y = bottomList[i].y;
			tmp.z = bottomList[i].z;
			
			// Vektor der Vektorliste hinzufügen
			vMantle.push_back(Vertex(tmp, norm, 1/corners*i,0.0));
		}
		
		// Indexliste für Mantel füllen
		for(int i=0; i<corners; i++)
		{
			if(i == corners-1)
			{
				iMantle.push_back(corners);
				iMantle.push_back(1);
				iMantle.push_back(0);
			}
			else
			{
				iMantle.push_back(i+1);
				iMantle.push_back(i+2);
				iMantle.push_back(0);
			}
			
			
		}
		
	}
	// Zweig für doppelte Vertices
	else
	{	
		// Spitze
		Vector tmp;
		tmp.x = 0.0;
		tmp.y = height/2;
		tmp.z = 0.0;
		
		for(int i=1; i<=corners; i++)
		{			
			// Normale berechnen
			Vector tmp1;
			Vector tmp2;
			
			// Zwei Seitenvektoren für´s Kreizprodukt berechnen
			tmp1.x = 0.0 - bottomList[i].x;
			tmp1.y = height/2 - bottomList[i].y;
			tmp1.z = 0.0 - bottomList[i].z;
			
			if(i == corners)
			{
				tmp2.x = bottomList[1].x - bottomList[i].x;
				tmp2.y = bottomList[1].y - bottomList[i].y;
				tmp2.z = bottomList[1].z - bottomList[i].z;
			}
			else
			{
				tmp2.x = bottomList[i+1].x - bottomList[i].x;
				tmp2.y = bottomList[i+1].y - bottomList[i].y;
				tmp2.z = bottomList[i+1].z - bottomList[i].z;
			}
			
			Vector norm;
			norm.cross(tmp1, tmp2);
			norm.negate();
			norm.normalize();
			
			// Vektorliste füllen
			vMantle.push_back(Vertex(bottomList[i], norm, 1.0/corners*(i-1),0.0));
			
			if(i == corners)
			{
				vMantle.push_back(Vertex(bottomList[1], norm, 1.0/corners*i,0.0));
			}
			else
			{
				vMantle.push_back(Vertex(bottomList[i+1], norm, 1.0/corners*i,0.0));
			}
			vMantle.push_back(Vertex(tmp, norm, 1/corners*i-0.5*sideWidth,1.0));
		}
			
		// Indexliste für Mantel füllen 
		int count = 0;
		for(int i=0; i<corners; i++)
		{
			iMantle.push_back(count);
			count++;
			iMantle.push_back(count);
			count++;
			iMantle.push_back(count);
			count++;
		}
	}
	
	mantle.setVertices(vMantle);
	mantle.setIndices(iMantle);
	mantle.setMaterialID(nMaterialPyramid);
	pyramid.addPolygon(mantle);
	
	// Dynamische Arrays löschen
	delete [] bottomList;
	
	sm->insertObject(pyramid);
	
	LOG_Ok("Pyramid added");
}
