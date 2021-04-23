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
 *  File:       ManEdit.cpp
 *  Created:    10.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/ManualEdit.h"

using namespace BSRenderer;
using namespace BSScene;

ManEdit::ManEdit(QWidget* parent)
 : QWidget(parent)
{
	//Initialize the UI
	m_ui.setupUi(this);
	
	//Set up table
	m_sHeader << "Mesh-Name";
	m_sHeader << "X";
	m_sHeader << "Y";
	m_sHeader << "Z";
	m_sHeader << "nX";
	m_sHeader << "nY";
	m_sHeader << "nZ";
	m_sHeader << "U";
	m_sHeader << "V"; 
	
	//QTableWidgetItem* pPrototyp = new QTableWidgetItem("0.0");
	//m_ui.table->setItemPrototype(pPrototyp);
	
	m_ui.table->setColumnCount(MANUALEDITCOLUMNS);
	m_ui.table->setHorizontalHeaderLabels(m_sHeader);
	m_ui.table->setRowCount(10);
	
	for(int i=0; i<m_ui.table->rowCount(); i++)
		m_ui.table->setRowHeight(i, 16);
	for(int i=0; i<m_ui.table->columnCount(); i++)
		m_ui.table->setColumnWidth(i, 66);//m_ui.table->width()/10);
	m_ui.table->setColumnWidth(0, 80);
	
	m_ui.table->setItem(2, 2, new QTableWidgetItem("Test"));
	
	//Connect slots
	connect(m_ui.OKButton, SIGNAL(clicked()), this , SLOT(updateSceneAndExit()));
	connect(m_ui.applyButton, SIGNAL(clicked()), this , SLOT(updateScene()));
	connect(m_ui.cancelButton, SIGNAL(clicked()), this , SLOT(quitDialog()));
}


void ManEdit::updateScene()
{
	SceneManager *sm = SceneManager::getInstance();
	
	//Update all vertices
	for(int i=0; i<m_editItems.size(); i++)
	{
		m_editItems[i].pVert->vPos.x 	= m_editItems[i].items[1].text().toDouble();
		m_editItems[i].pVert->vPos.y 	= m_editItems[i].items[2].text().toDouble();
		m_editItems[i].pVert->vPos.z 	= m_editItems[i].items[3].text().toDouble();
		m_editItems[i].pVert->vNormal.x = m_editItems[i].items[4].text().toDouble();
		m_editItems[i].pVert->vNormal.y = m_editItems[i].items[5].text().toDouble();
		m_editItems[i].pVert->vNormal.z = m_editItems[i].items[6].text().toDouble();
		m_editItems[i].pVert->dU[0] 	= m_editItems[i].items[7].text().toDouble();
		m_editItems[i].pVert->dV[0]	 	= m_editItems[i].items[8].text().toDouble();
		
		if(m_editItems[i].pMesh != NULL)
			sm->getScene().calcBoundingBoxByName(m_editItems[i].pMesh->getName());
	}

	sm->checkForRedraw(true);
}

void ManEdit::updateSceneAndExit()
{
	updateScene();
	setVisible(false);
}

void ManEdit::quitDialog()
{
	setVisible(false);
}

void ManEdit::createTableFromScene()
{	
	//Fill the items form the scene
	fillEditItems();
	
	//Clear the table cells
	m_ui.table->clear();
	
	//Reset the table properties
	m_ui.table->setColumnCount(MANUALEDITCOLUMNS);
	m_ui.table->setRowCount(m_editItems.size());
	m_ui.table->setHorizontalHeaderLabels(m_sHeader);
	m_ui.table->setVerticalHeaderLabels(m_sVHeader);
	
	//Set the cells
	for(int i=0; i<m_editItems.size(); i++)
	{
		for(int c=0; c<MANUALEDITCOLUMNS; c++)
		{
			m_ui.table->setItem(i, c, &(m_editItems[i].items[c]));
			m_ui.table->setRowHeight(i, 16);			
		}	
	}
}

void ManEdit::fillEditItems()
{
	m_editItems.clear();
	m_sVHeader.clear();
	
 	SelectionBuffer &sb = SceneManager::getInstance()->getSelBuffer();
	list<SceneObject*>& objects = sb.getSelectedObjects();
	list<SelectedVertex>& selVerts = sb.getSelectedVertices();
	
	for(list<SceneObject*>::iterator it = objects.begin();it != objects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);

			vector<BSScene::Polygon>* polys = mesh->getPolys();
 		 
			//Walk through all polygons
			for(vector<BSScene::Polygon>::iterator itPolys = polys->begin(); itPolys != polys->end(); ++itPolys)
			{			
				vector<Vertex>*	verts = itPolys->getVertices();
				
				//Walk through all vertices
				for(unsigned int v=0; v<verts->size(); v++)
				{
					EditItem newItem;
					newItem.pMesh = mesh;
					newItem.pVert = &((*verts)[v]);
					
					//Mesh-Name should be only enabled, but not
					//selectable and editable
					newItem.items[0].setFlags(Qt::ItemIsEnabled);
					
					//Fill items with correct data
					newItem.items[0].setText(newItem.pMesh->getName());
					newItem.items[1].setText(QString::number(newItem.pVert->vPos.x,    'f', MANUALEDITPRECISSION));
					newItem.items[2].setText(QString::number(newItem.pVert->vPos.y,    'f', MANUALEDITPRECISSION));
					newItem.items[3].setText(QString::number(newItem.pVert->vPos.z,    'f', MANUALEDITPRECISSION));
					newItem.items[4].setText(QString::number(newItem.pVert->vNormal.x, 'f', MANUALEDITPRECISSION));
					newItem.items[5].setText(QString::number(newItem.pVert->vNormal.y, 'f', MANUALEDITPRECISSION));
					newItem.items[6].setText(QString::number(newItem.pVert->vNormal.z, 'f', MANUALEDITPRECISSION));
					newItem.items[7].setText(QString::number(newItem.pVert->dU[0],     'f', MANUALEDITPRECISSION));
					newItem.items[8].setText(QString::number(newItem.pVert->dV[0],     'f', MANUALEDITPRECISSION));
					
					m_sVHeader << QString::number(v);
					m_editItems.push_back(newItem);						
				}
			}
		}
   	}
   	
   	//Walk through all selected vertices
   	int nV = 0;
	for(list<SelectedVertex>::iterator it = selVerts.begin(); it != selVerts.end(); ++it) 
	{
		SelectedVertex *sVert = &(*it);
		
		if(sVert != NULL)
		{
			EditItem newItem;
			newItem.pMesh = NULL;
			newItem.pVert = sVert->getVertex();
			
			//Mesh-Name should be only enabled, but not
			//selectable and editable
			newItem.items[0].setFlags(Qt::ItemIsEnabled);
			
			//Fill items with correct data
			newItem.items[0].setText("");
			newItem.items[1].setText(QString::number(newItem.pVert->vPos.x,    'f', MANUALEDITPRECISSION));
			newItem.items[2].setText(QString::number(newItem.pVert->vPos.y,    'f', MANUALEDITPRECISSION));
			newItem.items[3].setText(QString::number(newItem.pVert->vPos.z,    'f', MANUALEDITPRECISSION));
			newItem.items[4].setText(QString::number(newItem.pVert->vNormal.x, 'f', MANUALEDITPRECISSION));
			newItem.items[5].setText(QString::number(newItem.pVert->vNormal.y, 'f', MANUALEDITPRECISSION));
			newItem.items[6].setText(QString::number(newItem.pVert->vNormal.z, 'f', MANUALEDITPRECISSION));
			newItem.items[7].setText(QString::number(newItem.pVert->dU[0],     'f', MANUALEDITPRECISSION));
			newItem.items[8].setText(QString::number(newItem.pVert->dV[0],     'f', MANUALEDITPRECISSION));
			
			m_sVHeader << QString::number(nV);
			m_editItems.push_back(newItem);	
			nV++;
		}
	}
}
