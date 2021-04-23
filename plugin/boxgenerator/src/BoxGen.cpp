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
 *  File:       BoxGen.cpp
 *  Created:    20.12.2006
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/

#include "./../include/BoxGen.h"

#include <iostream>

using namespace BSScene;


BoxGen::BoxGen() : boxGenVersion(Version(1,0,0))
{
	createBoxButton = NULL;
}

BoxGen::~BoxGen()
{
}

// Functions, that implements from PlgInt:
UID BoxGen::getUID()
{
	return uid;
}

QString BoxGen::getName()
{
	return "BoxGenerator";
}
    
QString BoxGen::getAutor()
{
	return "Psirus";
}
    
QString BoxGen::getDescription()
{
	return "create a box\nversion 1.0";
}

Version BoxGen::getVersion()
{
	return boxGenVersion;
}

bool BoxGen::unloadPlg()
{
	
	if(createBoxButton != NULL)
	{
		delete createBoxButton;
		createBoxButton = NULL;
	}	
	
	return true;
}

bool BoxGen::loadPlg()
{
	LOG_Ok("Test");

	int nButtonSize = 32;
	int nIconSize = 24;
	
	createBoxButton = new QPushButton(QIcon(":/media/GenBox_32.png"), "");
	createBoxButton->setMaximumSize(nButtonSize, nButtonSize);
	createBoxButton->setMinimumSize(nButtonSize, nButtonSize);
	createBoxButton->setIconSize(QSize(nIconSize, nIconSize));
	createBoxButton->setToolTip("Box");	
	
	BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
    mw->getAvailableToolBoxItems().at(0)->addWidgetToGroupBox("commonObjectsGroupBox", createBoxButton);
    
    connect(createBoxButton, SIGNAL(clicked()), this, SLOT(createBox()));
	return true;
}

// <<singleton>>

static BoxGen* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new BoxGen();
	
	return static_cast<PlgInt*> (_instance);
}

void BoxGen::createBox()
{
		int nMaterialBox = Renderer::getInstance()->getMaterialManager()->createNewMaterial();
        Material* pMat = Renderer::getInstance()->getMaterialManager()->getMaterial(nMaterialBox);
        pMat->setColor(Color(1.0, 1.0, 1.0));
		
    	SceneManager *sm = SceneManager::getInstance();
	
		Mesh box;
		box.setName(tr("Box"));
		
		BSScene::Polygon front;
		
		vector<Vertex> vFront;
		vFront.push_back(Vertex(Vector(-0.5,	-0.5,	0.5),	Vector( 0.0,  0.0,  1.0),0.0,0.0));
		vFront.push_back(Vertex(Vector( 0.5,	-0.5,	0.5),	Vector( 0.0,  0.0,  1.0),1.0,0.0));
		vFront.push_back(Vertex(Vector( 0.5,	 0.5,	0.5),	Vector( 0.0,  0.0,  1.0),1.0,1.0));
		vFront.push_back(Vertex(Vector(-0.5,	 0.5,	0.5),	Vector( 0.0,  0.0,  1.0),0.0,1.0));
				
		vector<int> iFront;
		iFront.push_back(0);
		iFront.push_back(1);
		iFront.push_back(3);
		iFront.push_back(1);
		iFront.push_back(2);
		iFront.push_back(3);
		
		front.setVertices(vFront);
		front.setIndices(iFront);
		
		front.setMaterialID(nMaterialBox);
		//front.calcNormals();
		box.addPolygon(front);	
		
		
		BSScene::Polygon top;
		vector<Vertex> vTop;
		vTop.push_back(Vertex(Vector(-0.5,	0.5,	 0.5),	Vector( 0.0,  1.0,  0.0),0.0,1.0));
		vTop.push_back(Vertex(Vector( 0.5,	0.5,	 0.5),	Vector( 0.0,  1.0,  0.0),1.0,1.0));
		vTop.push_back(Vertex(Vector( 0.5,	0.5,	-0.5),	Vector( 0.0,  1.0,  0.0),1.0,0.0));
		vTop.push_back(Vertex(Vector(-0.5,	0.5,	-0.5),	Vector( 0.0,  1.0,  0.0),0.0,0.0));
		
		vector<int> iTop;
		iTop.push_back(0);
		iTop.push_back(1);
		iTop.push_back(3);
		iTop.push_back(1);
		iTop.push_back(2);
		iTop.push_back(3);
		
		top.setVertices(vTop);
		top.setIndices(iTop);
		//top.calcTextureCoordinates();
		top.setMaterialID(nMaterialBox);
		box.addPolygon(top);
		
		BSScene::Polygon left;
		vector<Vertex> vLeft;
		vLeft.push_back(Vertex(Vector(-0.5,	-0.5,	-0.5),	Vector( -1.0,  0.0,  0.0),0.0,0.0));
		vLeft.push_back(Vertex(Vector(-0.5,	-0.5,	 0.5),	Vector( -1.0,  0.0,  0.0),0.0,1.0));
		vLeft.push_back(Vertex(Vector(-0.5,	 0.5,	 0.5),	Vector( -1.0,  0.0,  0.0),1.0,1.0));
		vLeft.push_back(Vertex(Vector(-0.5,	 0.5,	-0.5),	Vector( -1.0,  0.0,  0.0),1.0,0.0));
		
		vector<int> iLeft;
		iLeft.push_back(0);
		iLeft.push_back(1);
		iLeft.push_back(3);
		iLeft.push_back(1);
		iLeft.push_back(2);
		iLeft.push_back(3);
		
		left.setVertices(vLeft);
		left.setIndices(iLeft);
		//left.calcTextureCoordinates();
		left.setMaterialID(nMaterialBox);
		box.addPolygon(left);
		
		
		BSScene::Polygon right;
		vector<Vertex> vRight;
		vRight.push_back(Vertex(Vector(0.5,	-0.5,	 0.5),	Vector( 1.0,  0.0,  0.0),0.0,1.0));
		vRight.push_back(Vertex(Vector(0.5,	-0.5,	-0.5),	Vector( 1.0,  0.0,  0.0),0.0,0.0));
		vRight.push_back(Vertex(Vector(0.5,	 0.5,	-0.5),	Vector( 1.0,  0.0,  0.0),1.0,0.0));
		vRight.push_back(Vertex(Vector(0.5,	 0.5,	 0.5),	Vector( 1.0,  0.0,  0.0),1.0,1.0));
		
		vector<int> iRight;
		iRight.push_back(0);
		iRight.push_back(1);
		iRight.push_back(3);
		iRight.push_back(1);
		iRight.push_back(2);
		iRight.push_back(3);
		
		right.setVertices(vRight);
		right.setIndices(iRight);
		//right.calcTextureCoordinates();
		right.setMaterialID(nMaterialBox);
		box.addPolygon(right);
		
		
		BSScene::Polygon back;
		vector<Vertex> vBack;
		vBack.push_back(Vertex(Vector( 0.5,	-0.5,	-0.5),	Vector( 0.0,  0.0,  -1.0),1.0,0.0));
		vBack.push_back(Vertex(Vector(-0.5,	-0.5,	-0.5),	Vector( 0.0,  0.0,  -1.0),0.0,0.0));
		vBack.push_back(Vertex(Vector(-0.5,	 0.5,	-0.5),	Vector( 0.0,  0.0,  -1.0),0.0,1.0));
		vBack.push_back(Vertex(Vector( 0.5,	 0.5,	-0.5),	Vector( 0.0,  0.0,  -1.0),1.0,1.0));
		
		vector<int> iBack;
		iBack.push_back(0);
		iBack.push_back(1);
		iBack.push_back(3);
		iBack.push_back(1);
		iBack.push_back(2);
		iBack.push_back(3);
		
		back.setVertices(vBack);
		back.setIndices(iBack);
		//back.calcTextureCoordinates();
		back.setMaterialID(nMaterialBox);
		box.addPolygon(back);
		
		
		BSScene::Polygon bottom;
		vector<Vertex> vBottom;
		vBottom.push_back(Vertex(Vector(-0.5,	-0.5,	 0.5),	Vector( 0.0,  -1.0,  0.0),0.0,1.0));
		vBottom.push_back(Vertex(Vector( 0.5,	-0.5,	 0.5),	Vector( 0.0,  -1.0,  0.0),1.0,1.0));
		vBottom.push_back(Vertex(Vector( 0.5,	-0.5,	-0.5),	Vector( 0.0,  -1.0,  0.0),1.0,0.0));
		vBottom.push_back(Vertex(Vector(-0.5,	-0.5,	-0.5),	Vector( 0.0,  -1.0,  0.0),0.0,0.0));
		
		vector<int> iBottom;
		iBottom.push_back(3);
		iBottom.push_back(1);
		iBottom.push_back(0);
		iBottom.push_back(1);
		iBottom.push_back(3);
		iBottom.push_back(2);
		
		bottom.setVertices(vBottom);
		bottom.setIndices(iBottom);
		//bottom.calcTextureCoordinates();
		bottom.setMaterialID(nMaterialBox);
		box.addPolygon(bottom);
		
		sm->insertObject(box); 
}
