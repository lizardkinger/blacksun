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
 *  File:       TubeGen.cpp
 *  Created:    05.03.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
#include "./../include/TubeGen.h"
#include "./../../../editor/math/include/Vector3.h"
#include "./../../gui/include/mainwindow.h"
#include <SceneGraph.h>
#include <iostream>

//### ...
/*
#include <SceneManager.h>
#include <Mesh.h>
#include <SelectionBuffer.h>
*/

using namespace BSScene;

TubeGen::TubeGen() : tubeGenVersion(Version(1,0,0))
{
	createTubeButton = NULL;
}

TubeGen::~TubeGen()
{
}

// Functions, that implements from PlgInt:
UID TubeGen::getUID()
{
	return uid;
}

QString TubeGen::getName()
{
	return "tubeGenerator";
}
    
QString TubeGen::getAutor()
{
	return "Psirus";
}
    
QString TubeGen::getDescription()
{
	return "create a Tube\nversion 1.0";
}

Version TubeGen::getVersion()
{
	return tubeGenVersion;
}

bool TubeGen::unloadPlg()
{
	if(createTubeButton != NULL)
	{
		delete createTubeButton;
		createTubeButton = NULL;
	}
	
	return true;
}

bool TubeGen::loadPlg()
{
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    
	int nButtonSize = 32;
	int nIconSize = 24;
	
	QPushButton* createTubeButton = new QPushButton(QIcon(":/media/GenTube_32.png"), "");
	createTubeButton->setMaximumSize(nButtonSize, nButtonSize);
	createTubeButton->setMinimumSize(nButtonSize, nButtonSize);
	createTubeButton->setIconSize(QSize(nIconSize, nIconSize));
	createTubeButton->setToolTip("Tube");
	createTubeButton->setCheckable(true);
	createTubeButton->setAutoExclusive(true);	
    
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", createTubeButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(createTubeButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(createTube()));
    
	return true;
}

// <<singleton>>

static TubeGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new TubeGen();
	
	return static_cast<PlgInt*> (_instance);
}

void TubeGen::createTube()
{
	int nMaterialTube = Renderer::getInstance()->getMaterialManager()->createNewMaterial();
    Material* pMat = Renderer::getInstance()->getMaterialManager()->getMaterial(nMaterialTube);
    pMat->setColor(Color(1.0, 1.0, 1.0));
        
	sharedVertices = ui.sharedVertices->isChecked();
	corners = ui.corners->value();
	parts = ui.parts->value();
	oRadius = ui.oRadius->value();
	iRadius = ui.iRadius->value();
	height = ui.height->value();
	
	if(iRadius >= oRadius)
	{
		LOG_Error("Tube Generator: incorrect Parameters");
		// Fenster mit Fehler
		return;
	}
	
	SelectionBuffer sb;
	SceneManager *sm = SceneManager::getInstance();
	Mesh tube;
	tube.setName("Tube");
	BSScene::Polygon bottom;
	BSScene::Polygon top;
	BSScene::Polygon innerMantle;
	BSScene::Polygon outerMantle;
	vector<Vertex> vBottom;
	vector<Vertex> vTop;
	vector<Vertex> vInnerMantle;
	vector<Vertex> vOuterMantle;
	vector<int> iBottom;
	vector<int> iTop;
	vector<int> iInnerMantle;
	vector<int> iOuterMantle;
	
	
	// Koordinaten der Ecken Berechnen
	
	// Winkel zwischen den Vektoren zu den Ecken
	double angle = -360.0/static_cast<double>(corners);

	// BODEN
	Vector* outerBottomList = new Vector[corners+1];
	Vector* innerBottomList = new Vector[corners+1];
	// Ursprung (Zentrum) damit dieser auch in der Vertexliste vorhanden ist
	innerBottomList[0].set(0.0,-height/2 ,0.0);
	outerBottomList[0].set(0.0,-height/2 ,0.0);
	// Vektor vom Ursprung zur ersten Ecke von dem ausgegangen wird
	innerBottomList[1].set(iRadius,-height/2 ,0.0);
	outerBottomList[1].set(oRadius,-height/2 ,0.0);
	for(int i=2; i<corners+1; i++)
	{
		// Koordinaten innen
		// Vorgänger-Vektor um Winkel rotieren
		Matrix mRot = Matrix::makeRotateY(degToRad(angle));
		Vector vRot = innerBottomList[i-1] * mRot;
		
		// Werte in Liste schreiben
		innerBottomList[i].set(vRot.x,vRot.y,vRot.z);
		
		// Koordinaten außen
		vRot = outerBottomList[i-1] * mRot;
		outerBottomList[i].set(vRot.x,vRot.y,vRot.z);
	}
		
	// Vertexliste für den Boden füllen
	for(int i=0; i<corners+1; i++)
	{	
		double dU = (outerBottomList[i].x / oRadius + 1.0) * 0.5;
		double dV = (outerBottomList[i].z / oRadius + 1.0) * 0.5;					 
		vBottom.push_back(Vertex(Vector(outerBottomList[i].x, outerBottomList[i].y, outerBottomList[i].z),	Vector( 0.0,  -1.0,  0.0),dU,dV));
	}
	for(int i=0; i<corners+1; i++)
	{	
		double dU = (innerBottomList[i].x / iRadius + 1.0) * 0.5;
		double dV = (innerBottomList[i].z / iRadius + 1.0) * 0.5;					 
		vBottom.push_back(Vertex(Vector(innerBottomList[i].x, innerBottomList[i].y, innerBottomList[i].z),	Vector( 0.0,  -1.0,  0.0),dU,dV));
	}
		
	// Indexliste für Boden füllen
	int a = 1;
	int b = corners+2;
	int c = 2;
	
	for(int i=0; i<corners; i++)
	{
		if(i == corners-1)
		{
			c = 1;
		}
		
		iBottom.push_back(a);
		iBottom.push_back(b);
		iBottom.push_back(c);
		
		a++;
		b++;
		c++;
	}
	
	a = 2;
	b = corners+2;
	c = corners+3;
	
	for(int i=0; i<corners; i++)
	{
		if(i == corners-1)
		{
			a = 1;
			b = 2*corners+1;
			c = corners+2;
		}
		
		iBottom.push_back(a);
		iBottom.push_back(b);
		iBottom.push_back(c);
		
		a++;
		b++;
		c++;
	}
	
	// Werte setzten
	bottom.setVertices(vBottom);
	bottom.setIndices(iBottom);
	bottom.setMaterialID(nMaterialTube);
	tube.addPolygon(bottom);
		
	// DECKEL
	Vector* innerTopList = new Vector[corners+1];
	Vector* outerTopList = new Vector[corners+1];
		
	// Liste füllen (selben Werte wie bottom nur geänderte y-komponente)
	for(int i=0; i<corners+1; i++)
	{
		innerTopList[i].set(innerBottomList[i].x, height/2, innerBottomList[i].z);
		outerTopList[i].set(outerBottomList[i].x, height/2, outerBottomList[i].z);
	}
		
	// Vertexliste für den Deckel füllen
	for(int i=0; i<corners+1; i++)
	{
		double dU = (outerTopList[i].x / oRadius + 1.0) * 0.5;
		double dV = (outerTopList[i].z / oRadius + 1.0) * 0.5;	
		vTop.push_back(Vertex(Vector(outerTopList[i].x, outerTopList[i].y, outerTopList[i].z),	Vector( 0.0,  1.0,  0.0),dU,dV));
	}
	for(int i=0; i<corners+1; i++)
	{
		double dU = (innerTopList[i].x / iRadius + 1.0) * 0.5;
		double dV = (innerTopList[i].z / iRadius + 1.0) * 0.5;	
		vTop.push_back(Vertex(Vector(innerTopList[i].x, innerTopList[i].y, innerTopList[i].z),	Vector( 0.0,  1.0,  0.0),dU,dV));
	}
		
	a = 1;
	b = 2;
	c = corners+2;
	
	for(int i=0; i<corners; i++)
	{
		if(i == corners-1)
		{
			a = corners;
			b = 1;
			c = 2*corners+1;
		}
		
		iTop.push_back(a);
		iTop.push_back(b);
		iTop.push_back(c);
		
		a++;
		b++;
		c++;
	}
	
	a = 2;
	b = corners+3;
	c = corners+2;
	
	for(int i=0; i<corners; i++)
	{
		if(i == corners-1)
		{
			a = 1;
			b = corners+2;
			c = 2*corners+1;
		}
		
		iTop.push_back(a);
		iTop.push_back(b);
		iTop.push_back(c);
		
		a++;
		b++;
		c++;
	}

	// Werte setzen
	top.setVertices(vTop);
	top.setIndices(iTop);
	top.setMaterialID(nMaterialTube);
	tube.addPolygon(top);
	
	// MANTEL
	// äußerer Mantel
	// Zweig für geteilte Vertices
	if(sharedVertices)
	{
		Vector tmp;
		double height = outerTopList[1].y - outerBottomList[1].y;
		double distance = height/parts;
		
		// Vektorliste für Mantel füllen
		for(int i=1; i<=corners; i++)
		{	
			// Normale berechen
			tmp.x = outerBottomList[i].x;
			tmp.y = 0.0;
			tmp.z = outerBottomList[i].z;
			
			if(tmp != Vector(0.0, 0.0, 0.0))
				tmp.normalize();
		
			vOuterMantle.push_back(Vertex(outerBottomList[i], tmp, 1.0/corners*i,0.0));	
			for(int j=1; j<parts; j++)
			{	
				Vector tmp1;
				tmp1.x = outerBottomList[i].x;
				tmp1.y = outerBottomList[i].y+j*distance;
				tmp1.z = outerBottomList[i].z;
				vOuterMantle.push_back(Vertex(tmp1, tmp, 1.0/corners*i,1.0/parts*j));
			}
			vOuterMantle.push_back(Vertex(outerTopList[i], tmp, 1.0/corners*i,1.0));
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
				iOuterMantle.push_back(a);
				iOuterMantle.push_back(b);
				iOuterMantle.push_back(c);
				
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
				iOuterMantle.push_back(a);
				iOuterMantle.push_back(b);
				iOuterMantle.push_back(c);
				
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
		double height = outerTopList[1].y - outerBottomList[1].y;
		double distance = height/parts;
		
		for(int i=1; i<=corners; i++)
		{			
			// Normale berechnen
			Vector tmp1;
			Vector tmp2;
			
			// Zwei Seitenvektoren für´s Kreizprodukt berechnen
			tmp1.x = outerBottomList[i].x - outerTopList[i].x;
			tmp1.y = outerBottomList[i].y - outerTopList[i].y;
			tmp1.z = outerBottomList[i].z - outerTopList[i].z;
			
			if(i == corners)
			{
				tmp2.x = outerTopList[1].x - outerTopList[i].x;
				tmp2.y = outerTopList[1].y - outerTopList[i].y;
				tmp2.z = outerTopList[1].z - outerTopList[i].z;
			}
			else
			{
				tmp2.x = outerTopList[i+1].x - outerTopList[i].x;
				tmp2.y = outerTopList[i+1].y - outerTopList[i].y;
				tmp2.z = outerTopList[i+1].z - outerTopList[i].z;
			}
			
			Vector norm;
			norm.cross(tmp1, tmp2);
			norm.normalize();
			
			// Vektorliste füllen
			vOuterMantle.push_back(Vertex(outerBottomList[i], norm, 1.0/corners*(i-1),0.0));	
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				tmp.x = outerBottomList[i].x;
				tmp.y = outerBottomList[i].y+j*distance;
				tmp.z = outerBottomList[i].z;
				vOuterMantle.push_back(Vertex(tmp, norm, 1.0/corners*(i-1),1.0/parts*j));
			}
			vOuterMantle.push_back(Vertex(outerTopList[i], norm, 1.0/corners*(i-1),1.0));
			
			if(i == corners)
			{
				vOuterMantle.push_back(Vertex(outerBottomList[1], norm, 1.0,0.0));
			}
			else
			{
				vOuterMantle.push_back(Vertex(outerBottomList[i+1], norm, 1.0/corners*i,0.0));	
			}
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				if(i == corners)
				{
					tmp.x = outerBottomList[1].x;
					tmp.y = outerBottomList[1].y+j*distance;
					tmp.z = outerBottomList[1].z;
					
					vOuterMantle.push_back(Vertex(tmp, norm, 1.0,1.0/parts*j));
				}
				else
				{
					tmp.x = outerBottomList[i+1].x;
					tmp.y = outerBottomList[i+1].y+j*distance;
					tmp.z = outerBottomList[i+1].z;
					
					vOuterMantle.push_back(Vertex(tmp, norm, 1.0/corners*i,1.0/parts*j));
				}
				
			}
			if(i == corners)
			{
				vOuterMantle.push_back(Vertex(outerTopList[1], norm, 1.0,1.0));
			}
			else
			{
				vOuterMantle.push_back(Vertex(outerTopList[i+1], norm, 1.0/corners*i,1.0));
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
				iOuterMantle.push_back(a);
				iOuterMantle.push_back(b);
				iOuterMantle.push_back(c);
			
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
				iOuterMantle.push_back(a);
				iOuterMantle.push_back(b);
				iOuterMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			a = a+2+parts;
			b = b+2+parts;
			c = c+2+parts;
		}
	}
	
	// innerer Mantel
	// Zweig für geteilte Vertices
	if(sharedVertices)
	{
		Vector tmp;
		double height = innerTopList[1].y - innerBottomList[1].y;
		double distance = height/parts;
		
		// Vektorliste für Mantel füllen
		for(int i=1; i<=corners; i++)
		{	
			// Normale berechen
			tmp.x = -innerBottomList[i].x;
			tmp.y = 0.0;
			tmp.z = -innerBottomList[i].z;
			tmp.normalize();
		
			vInnerMantle.push_back(Vertex(innerBottomList[i], tmp, 1.0/corners*i,0.0));
			
			Vector tmp1;	
			for(int j=1; j<parts; j++)
			{	
				tmp1.x = innerBottomList[i].x;
				tmp1.y = innerBottomList[i].y+j*distance;
				tmp1.z = innerBottomList[i].z;
				vInnerMantle.push_back(Vertex(tmp1, tmp, 1.0/corners*i,1.0/parts*j));
			}
			vInnerMantle.push_back(Vertex(innerTopList[i], tmp, 1.0/corners*i,1.0));
		}
		
		// Indexliste für Mantel füllen
		int a = 0;
		int b = 1;
		int c = parts+1;
		
		for(int i=0; i<corners; i++)
		{
			if(i == corners-1)
			{
				c = 0;
			}
			
			for(int j=0; j<parts; j++)
			{
				iInnerMantle.push_back(a);
				iInnerMantle.push_back(b);
				iInnerMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			
			a++;
			b++;
			c++;
		}
		
		a = parts+1;
		b = 1;
		c = parts+2;
		
		for(int i=0; i<corners; i++)
		{
			if(i == corners-1)
			{
				a = 0;
				c = 1;
			}
			
			for(int j=0; j<parts; j++)
			{
				iInnerMantle.push_back(a);
				iInnerMantle.push_back(b);
				iInnerMantle.push_back(c);
				
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
		double height = innerTopList[1].y - innerBottomList[1].y;
		double distance = height/parts;
		
		for(int i=1; i<=corners; i++)
		{			
			// Normale berechnen
			Vector tmp1;
			Vector tmp2;
			
			// Zwei Seitenvektoren für´s Kreizprodukt berechnen
			tmp1.x = innerBottomList[i].x - innerTopList[i].x;
			tmp1.y = innerBottomList[i].y - innerTopList[i].y;
			tmp1.z = innerBottomList[i].z - innerTopList[i].z;
			
			if(i == corners)
			{
				tmp2.x = innerTopList[1].x - innerTopList[i].x;
				tmp2.y = innerTopList[1].y - innerTopList[i].y;
				tmp2.z = innerTopList[1].z - innerTopList[i].z;
			}
			else
			{
				tmp2.x = innerTopList[i+1].x - innerTopList[i].x;
				tmp2.y = innerTopList[i+1].y - innerTopList[i].y;
				tmp2.z = innerTopList[i+1].z - innerTopList[i].z;
			}
			
			Vector norm;
			norm.cross(tmp1, tmp2);
			norm.negate();
			norm.normalize();
			
			// Vektorliste füllen
			vInnerMantle.push_back(Vertex(innerBottomList[i], norm, 1.0/corners*(i-1),0.0));	
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				tmp.x = innerBottomList[i].x;
				tmp.y = innerBottomList[i].y+j*distance;
				tmp.z = innerBottomList[i].z;
				vInnerMantle.push_back(Vertex(tmp, norm, 1.0/corners*(i-1),1.0/parts*j));
			}
			vInnerMantle.push_back(Vertex(innerTopList[i], norm, 1.0/corners*(i-1),1.0));
			
			if(i == corners)
			{
				vInnerMantle.push_back(Vertex(innerBottomList[1], norm, 1.0,0.0));
			}
			else
			{
				vInnerMantle.push_back(Vertex(innerBottomList[i+1], norm, 1.0/corners*i,0.0));	
			}
			for(int j=1; j<parts; j++)
			{	
				Vector tmp;
				if(i == corners)
				{
					tmp.x = innerBottomList[1].x;
					tmp.y = innerBottomList[1].y+j*distance;
					tmp.z = innerBottomList[1].z;
					
					vInnerMantle.push_back(Vertex(tmp, norm, 1.0,1.0/parts*j));
				}
				else
				{
					tmp.x = innerBottomList[i+1].x;
					tmp.y = innerBottomList[i+1].y+j*distance;
					tmp.z = innerBottomList[i+1].z;
					
					vInnerMantle.push_back(Vertex(tmp, norm, 1.0/corners*i,1.0/parts*j));
				}
				
			}
			if(i == corners)
			{
				vInnerMantle.push_back(Vertex(innerTopList[1], norm, 1.0,1.0));
			}
			else
			{
				vInnerMantle.push_back(Vertex(innerTopList[i+1], norm, 1.0/corners*i,1.0));
			}
		}
			
		// Indexliste für Mantel füllen 
		int a = 0;
		int b = 1;
		int c = parts+1;
	
		for(int i=0; i<corners; i++)
		{
			for(int j=0; j<parts; j++)
			{
				iInnerMantle.push_back(a);
				iInnerMantle.push_back(b);
				iInnerMantle.push_back(c);
			
				a++;
				b++;
				c++;
			}
			a = a+2+parts;
			b = b+2+parts;
			c = c+2+parts;
		}
		
		a = parts+1;
		b = 1;
		c = parts+2;
		
		for(int i=0; i<corners; i++)
		{	
			for(int j=0; j<parts; j++)
			{
				iInnerMantle.push_back(a);
				iInnerMantle.push_back(b);
				iInnerMantle.push_back(c);
				
				a++;
				b++;
				c++;
			}
			a = a+2+parts;
			b = b+2+parts;
			c = c+2+parts;
		}
	}
	
	outerMantle.setVertices(vOuterMantle);
	outerMantle.setIndices(iOuterMantle);
	outerMantle.setMaterialID(nMaterialTube);
	tube.addPolygon(outerMantle);
	
	innerMantle.setVertices(vInnerMantle);
	innerMantle.setIndices(iInnerMantle);
	innerMantle.setMaterialID(nMaterialTube);
	tube.addPolygon(innerMantle);
	
	// Dynamische Arrays löschen
	delete [] innerBottomList;
	delete [] outerBottomList;
	delete [] innerTopList;	
	delete [] outerTopList;
	
	sm->insertObject(tube);
	
	
	LOG_Ok("Tube added");
}
