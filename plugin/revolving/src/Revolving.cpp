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
 *  File:       Revolving.cpp
 *  Created:    05.04.2007
 *  Author:     Philipp Gruber (psirus)
 *
 **************************************************************************/

#include "./../include/Revolving.h"
#include <iostream>


static Revolving* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Revolving();

	return static_cast<PlgInt*>(_instance);
}

Revolving::Revolving() : version(Version(1, 0, 0))
{
	m_sName = "Revolving-Plugin";
	m_sDescription = "Rotates around axis";
	
	revolveButton = NULL;
}

Revolving::~Revolving()
{
}

bool Revolving::unloadPlg()
{
	if(revolveButton != NULL)
	{
		delete revolveButton;
		revolveButton = NULL;
	}
	
	return true;
}

bool Revolving::loadPlg()
{
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    
	int nButtonSize = 32;
	int nIconSize = 24;
	
	revolveButton = new QPushButton(QIcon(":/media/Revolving_32.png"), "");
	revolveButton->setMaximumSize(nButtonSize, nButtonSize);
	revolveButton->setMinimumSize(nButtonSize, nButtonSize);
	revolveButton->setIconSize(QSize(nIconSize, nIconSize));
	revolveButton->setToolTip("Revolving");
	revolveButton->setCheckable(true);
	revolveButton->setAutoExclusive(true);	
    
    QWidget* pluginSettings = new QWidget();
    pluginSettings->hide();
    ui.setupUi(pluginSettings);
    
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", revolveButton);
    BSGui::BSToolBoxItem* toolBox = mw->getAvailableToolBoxItems().at(0);
    BSGui::BSGroupBox* pPrev = toolBox->findChild<BSGui::BSGroupBox*>("commonObjectsGroupBox");
    toolBox->insertPluginWidget(pluginSettings, pPrev);
    
	connect(revolveButton, SIGNAL(toggled(bool)), pluginSettings, SLOT(setShown(bool)));
    connect(ui.insertButton, SIGNAL(clicked()), this, SLOT(execute()));
    
	return true;
}

void Revolving::execute()
{
	int nMaterialBox = Renderer::getInstance()->getMaterialManager()->createNewMaterial();
    Material* pMat = Renderer::getInstance()->getMaterialManager()->getMaterial(nMaterialBox);
    pMat->setColor(Color(1.0, 1.0, 1.0));
		
    SceneManager *sm = SceneManager::getInstance();
	
	BSScene::Polygon obj;
	Mesh rev;
	
	
	RotateAxis axis;			// Rotationsachse
	switch(ui.axis->currentIndex())
	{
		case 0:	axis = X;
				break;
		case 1:	axis = Y;
				break;
		case 2:	axis = Z;
				break;
	}	    
	bool closed = ui.closed->isChecked();			// Ersten und letzten Punkt verbinden
	int corners = ui.corners->value();				// Anzahl der Ecken
	double angle = ui.angle->value();			// Um wiviel Grad gedreht werden soll
	vector<Vertex> vertizes;		// Vertexliste
	vector<int> indizes;         	// Indexliste
	
	
	// vertexliste mit beispielpunkten füllen (Glas/Kelch)
	Matrix norm;
	switch(axis)
	{
		case X: norm = Matrix::makeRotateY(degToRad(-90.0));
				break;
				
		case Y: norm = Matrix::makeRotateZ(degToRad(-90.0));
				break;
				
		case Z: norm = Matrix::makeRotateX(degToRad(-90.0));
	 			break;
	 			
		default:LOG_Error("Error while creating ...");
	}
	// 1
	Vector vTmp;
	vTmp.x = 1.8-0.0;
	vTmp.y = 10.0-10.0;
	vTmp.z = 0.0;
	Vector nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(0.0,10.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(1.8,10.0,0.0),	nTmp, 0.0,0.0));
	
	// 2
	vTmp.x = 2.75-1.8;
	vTmp.y = 10.65-10.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(1.8,10.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(2.75,10.65,0.0),	nTmp, 0.0,0.0));
	
	// 3
	vTmp.x = 3.4-2.75;
	vTmp.y = 11.7-10.65;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(2.75,10.65,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(3.4,11.7,0.0),	nTmp, 0.0,0.0));
	
	// 4
	vTmp.x = 4.0-3.4;
	vTmp.y = 13.0-7.11;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(3.4,11.7,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.0,13.0,0.0),	nTmp, 0.0,0.0));
	
	// 5
	vTmp.x = 4.0-4.0;
	vTmp.y = 17.0-13.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.0,13.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.0,17.0,0.0),	nTmp, 0.0,0.0));
	
	// 6
	vTmp.x = 4.15-4.0;
	vTmp.y = 17.35-17.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.0,17.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.15,17.35,0.0),	nTmp, 0.0,0.0));
	
	// E6
	vTmp.x = 4.4-4.15;
	vTmp.y = 17.5-17.35;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.15,17.35,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.4,17.5,0.0),	nTmp, 0.0,0.0));
	
	// 7
	vTmp.x = 4.6-4.4;
	vTmp.y = 17.5-17.5;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.4,17.5,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.6,17.5,0.0),	nTmp, 0.0,0.0));
	
	// 8
	vTmp.x = 4.85-4.6;
	vTmp.y = 17.35-17.5;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.6,17.5,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.85,17.35,0.0),	nTmp, 0.0,0.0));
	
	// E5
	vTmp.x = 5.0-4.85;
	vTmp.y = 17.0-17.35;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.85,17.35,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(5.0,17.0,0.0),	nTmp, 0.0,0.0));
	
	// 9
	vTmp.x = 5.0-5.0;
	vTmp.y = 13.0-17.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(5.0,17.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(5.0,13.0,0.0),	nTmp, 0.0,0.0));
	
	// 10
	vTmp.x = 4.0-5.0;
	vTmp.y = 11.0-13.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(5.0,13.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.0,11.0,0.0),	nTmp, 0.0,0.0));
	
	// 11
	vTmp.x = 3.5-4.0;
	vTmp.y = 10.25-11.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.0,11.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(3.5,10.25,0.0),	nTmp, 0.0,0.0));
	
	// 12
	vTmp.x = 3.0-3.5;
	vTmp.y = 9.5-10.25;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(3.5,10.25,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(3.0,9.5,0.0),	nTmp, 0.0,0.0));
	
	// 13;
	vTmp.x = 1.1-3.0;
	vTmp.y = 9.0-9.5;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(3.0,9.5,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(1.1,9.0,0.0),	nTmp, 0.0,0.0));
	
	// 14
	vTmp.x = 0.65-1.1;
	vTmp.y = 9.0-9.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(1.1,9.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(0.65,9.0,0.0),	nTmp, 0.0,0.0));
	
	// E4
	vTmp.x = 0.5-0.65;
	vTmp.y = 8.85-9.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(0.65,9.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(0.5,8.85,0.0),	nTmp, 0.0,0.0));
	
	// 15
	vTmp.x = 0.5-0.5;
	vTmp.y = 1.15-8.85;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(0.5,8.85,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(0.5,1.15,0.0),	nTmp, 0.0,0.0));
	
	// E3
	vTmp.x = 0.65-0.5;
	vTmp.y = 1.0-1.15;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(0.5,1.15,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(0.65,1.0,0.0),	nTmp, 0.0,0.0));
	
	// 16
	vTmp.x = 4.0-0.65;
	vTmp.y = 1.0-1.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(0.65,1.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.0,1.0,0.0),	nTmp, 0.0,0.0));
	
	// 17
	vTmp.x = 4.35-4.0;
	vTmp.y = 0.85-1.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.0,1.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.35,0.85,0.0),	nTmp, 0.0,0.0));
	
	// E2
	vTmp.x = 4.5-4.35;
	vTmp.y = 0.6-0.85;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.35,0.85,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.5,0.6,0.0),	nTmp, 0.0,0.0));
	
	// 18
	vTmp.x = 4.5-4.5;
	vTmp.y = 0.4-0.6;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.5,0.6,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.5,0.4,0.0),	nTmp, 0.0,0.0));
	
	// 19
	vTmp.x = 4.35-4.5;
	vTmp.y = 0.15-0.4;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.5,0.4,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.35,0.15,0.0),	nTmp, 0.0,0.0));
	
	// E1
	vTmp.x = 4.0-4.35;
	vTmp.y = 0.0-0.15;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.35,0.15,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(4.0,0.0,0.0),	nTmp, 0.0,0.0));
	
	// 20
	vTmp.x = 0.0-4.0;
	vTmp.y = 0.0-0.0;
	vTmp.z = 0.0;
	nTmp = vTmp * norm;
	nTmp.normalize();
	vertizes.push_back(Vertex(Vector(4.0,0.0,0.0),	nTmp, 0.0,0.0));
	vertizes.push_back(Vertex(Vector(0.0,0.0,0.0),	nTmp, 0.0,0.0));
	
	// Winkel zwischen den Vektoren zu den Ecken
	double tmpAngle = -angle/static_cast<double>(corners);
	
	Matrix mRot;
	switch(axis)
	{
		case X: mRot = Matrix::makeRotateX(degToRad(tmpAngle));
				break;
				
		case Y: mRot = Matrix::makeRotateY(degToRad(tmpAngle));
				break;
				
		case Z: mRot = Matrix::makeRotateZ(degToRad(tmpAngle));
	 			break;
	 			
		default:LOG_Error("Error while Revolving");
	}
	

	int numberVertices = vertizes.size();
		
	// wenn ganze Umdrehung einmal rotieren weniger da letzte rotierung dann 
	// verbindung zwischen vorletzter rotation und anfang ist
	int rotations;
	if(angle == 360)	
	{
		rotations = corners-1;
	}
	else
	{
		rotations = corners;
	}
	
	for(int i=0; i<rotations; i++)
	{
		// drehen wenn nicht teil zwischen letzten und wieder anfang
		// alle zu drehenden punkte durchlaufen
		// vektor für normale
		//Vector tmp;
		for(int j=0; j<numberVertices; j++)
		{
			Vector vRot = vertizes[j+i*numberVertices].vPos * mRot;
			Vector vNorm = vertizes[j+i*numberVertices].vNormal * mRot;
			/*// normale
			tmp.x = vRot.x;
			tmp.y = 0.0;
			tmp.z = vRot.z;
			tmp.normalize();*/
			vertizes.push_back(Vertex(vRot,	vNorm, 0.0,0.0));
		}
					
		// indexliste
		for(int j=0; j<numberVertices-1; j++)
		{
			indizes.push_back(i*numberVertices +j);
			indizes.push_back(i*numberVertices+1 +j);
			indizes.push_back((i+1)*numberVertices +j);
					
			indizes.push_back(i*numberVertices+1 +j);
			indizes.push_back((i+1)*numberVertices+1 +j);
			indizes.push_back((i+1)*numberVertices +j);
		}
				
		if(closed)
		{
			indizes.push_back(i*numberVertices);
			indizes.push_back((i+1)*numberVertices);
			indizes.push_back(i*numberVertices +numberVertices-1);
					
			indizes.push_back((i+1)*numberVertices);
			indizes.push_back((i+1)*numberVertices +numberVertices-1);
			indizes.push_back(i*numberVertices +numberVertices-1);
		}
	}
	// indizes für letzten abschnitt
	//bei kompletter Umdrehung
	if(angle == 360)
	{
		for(int j=0; j<numberVertices-1; j++)
		{
			indizes.push_back((corners-1)*numberVertices +j);
			indizes.push_back((corners-1)*numberVertices+1 +j);
			indizes.push_back(j);
							
			indizes.push_back((corners-1)*numberVertices+1 +j);
			indizes.push_back(j+1);
			indizes.push_back(j);
		}
				
		if(closed)
		{
			indizes.push_back((corners-1)*numberVertices);
			indizes.push_back(0);
			indizes.push_back((corners-1)*numberVertices +numberVertices-1);
						
			indizes.push_back(0);
			indizes.push_back(numberVertices -1);
			indizes.push_back((corners-1)*numberVertices +numberVertices-1);
		}
	}
	
	
	obj.setVertices(vertizes);
	obj.setIndices(indizes);
		
	rev.addPolygon(obj);
		
	/*	cout << "KOORDINATEN:" << endl;
		for(int i=0; i<vertizes.size(); i++)
		{
			cout << i << ": \t" << "x: " << vertizes[i].vPos.x << "\t \t y: " << vertizes[i].vPos.y << "\t \t z: " << vertizes[i].vPos.z << endl;
		}
		
		cout << "INDIZES:" << endl;
		for(int i=0; i<indizes.size(); i++)
		{
			//if(indizes[i] > corners*40-2)
			cout << i << ": \t" << indizes[i] << endl;
		}*/
		
	sm->insertObject(rev);
}
