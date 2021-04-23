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
 *  File:       TexCoordEditor.cpp
 *  Created:    22.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/TexCoordEditor.h"
#include "./../../../editor/pluginsystem/include/PlgMgr.h"
#include "./../../../plugin/materialeditor/include/MatEdit.h"

using namespace BSRenderer;
using namespace BSScene;

TexCoordEditor::TexCoordEditor(QWidget* parent)
 : QWidget(parent)
{
	//Initialize the UI
	m_ui.setupUi(this);	
	
	//Fill texture stage combo box
	for(int i=0; i<MAXTEXTURES; i++)
	{
		m_ui.textureStageComboBox->addItem(QString::number(i));	
	}

	//Fill 'Copy from' and 'Copy to' texture stage combo box
	m_ui.copyToComboBox->addItem("All");
	m_ui.copyToComboBox->addItem("All upper");
	m_ui.copyToComboBox->addItem("All lower");
	for(int i=0; i<MAXTEXTURES; i++)
	{
		m_ui.copyFromComboBox->addItem(QString::number(i));
		m_ui.copyToComboBox->addItem(QString::number(i));	
	}
	
	//Connect slots 
	connect(m_ui.OKButton, SIGNAL(clicked()), this , SLOT(updateSceneAndExit()));
	connect(m_ui.openMaterialEditor, SIGNAL(clicked()), this , SLOT(openMaterialEditor()));
	connect(m_ui.resetViewButton, SIGNAL(clicked()), m_ui.workarea , SLOT(resetCameras()));
	connect(m_ui.showGridCheckBox, SIGNAL(stateChanged(int)), this , SLOT(setShowGrid(int)));
	connect(m_ui.showRepetitionCheckBox, SIGNAL(stateChanged(int)), this , SLOT(setShowRepetition(int)));
	connect(m_ui.meshComboBox, SIGNAL(currentIndexChanged(int)), this , SLOT(selectMesh(int)));
	connect(m_ui.textureStageComboBox, SIGNAL(currentIndexChanged(int)), this , SLOT(selectTexStage(int)));
	connect(m_ui.copyButton, SIGNAL(clicked()), this , SLOT(copyTexCoord()));
	
	connect(m_ui.modeSelectButton, SIGNAL(clicked()), this , SLOT(setModeSelect()));
	connect(m_ui.modeMoveButton, SIGNAL(clicked()), this , SLOT(setModeMove()));
	connect(m_ui.modeScaleButton, SIGNAL(clicked()), this , SLOT(setModeScale()));
	connect(m_ui.modeRotateButton, SIGNAL(clicked()), this , SLOT(setModeRotate()));

	//Set correct start values for work area
	m_ui.workarea->setShowGrid(m_ui.showGridCheckBox->checkState() == Qt::Checked);
	m_ui.workarea->setShowRepeat(m_ui.showRepetitionCheckBox->checkState() == Qt::Checked);
	
	m_ui.modeSelectButton->setChecked(true);
	setModeSelect();
	
}

void TexCoordEditor::updateSceneAndExit()
{
	updateScene();
	quitDialog();
}

void TexCoordEditor::quitDialog()
{
	setVisible(false);
}

void TexCoordEditor::setShowGrid(int nState)
{
	m_ui.workarea->setShowGrid(nState == Qt::Checked);
}

void TexCoordEditor::setShowRepetition(int nState)
{
	m_ui.workarea->setShowRepeat(nState == Qt::Checked);
}

void TexCoordEditor::selectMesh(int index)
{	
	if(index >= 0 && index <= m_meshes.size())
		m_ui.workarea->setMesh(m_meshes[index].pMesh);
	else
		m_ui.workarea->setMesh(NULL);
		
	updateScene();
}

void TexCoordEditor::selectTexStage(int index)
{
	m_ui.workarea->setTextureStage(index);
	
	updateScene();
}

void TexCoordEditor::setModeSelect()
{
	m_ui.workarea->setMode(TexCoordWorkArea::TEM_Select);
}

void TexCoordEditor::setModeMove()
{
	m_ui.workarea->setMode(TexCoordWorkArea::TEM_Move);
}

void TexCoordEditor::setModeScale()
{
	m_ui.workarea->setMode(TexCoordWorkArea::TEM_Scale);
}

void TexCoordEditor::setModeRotate()
{
	m_ui.workarea->setMode(TexCoordWorkArea::TEM_Rotate);
}

void TexCoordEditor::updateScene()
{
	m_ui.workarea->updateMesh();
}

void TexCoordEditor::openMaterialEditor()
{
	//Try to get the material editor plugin
	MatEditInterface* pMaterialEditor = (MatEditInterface*)(BSPlgMgr::PlgMgr::getInstance()->getPlgInstance(MatEdit::uid,Version(1,0,0)));
	
	if(pMaterialEditor==NULL)
	{
		QMessageBox::critical(this, "Error", "Can't load material editor. Make sure, that the material editor (Version 1.0.0 or better) is installed and loaded.");
		return;
	}

	pMaterialEditor->execute();
}

void TexCoordEditor::copyTexCoord()
{
	//All texture stages should be copied
	if(m_ui.copyToComboBox->currentText() == QString("All"))
	{
		//Copy the texture stages
		for(int t=0; t<MAXTEXTURES; t++)
		{
			m_ui.workarea->copyTexCoords(m_ui.copyFromComboBox->currentIndex(), t);
		}
	}
	//All upper texture stages should be copied
	else if(m_ui.copyToComboBox->currentText() == QString("All upper"))
	{
		//Copy the texture stages
		for(int t=m_ui.copyFromComboBox->currentIndex()+1; t<MAXTEXTURES; t++)
		{
			m_ui.workarea->copyTexCoords(m_ui.copyFromComboBox->currentIndex(), t);
		}		
	}
	//All lower texture stages should be copied
	else if(m_ui.copyToComboBox->currentText() == QString("All lower"))
	{
		//Copy the texture stages
		for(int t=m_ui.copyFromComboBox->currentIndex()-1; t>=0; t--)
		{
			m_ui.workarea->copyTexCoords(m_ui.copyFromComboBox->currentIndex(), t);
		}			
	}
	//A single texture stage should be copied
	else
	{
		//Copy the texture stage
		m_ui.workarea->copyTexCoords(m_ui.copyFromComboBox->currentIndex(),
			m_ui.copyToComboBox->currentIndex()-3);
	}
}

void TexCoordEditor::fillFromScene()
{
	//TODO: Store last selected mesh and restore the selection after adding
	//all meshes, ...
	
	//Fill mesh combo box
	m_ui.meshComboBox->clear();
	m_meshes.clear();
	
	Scene &s = SceneManager::getInstance()->getScene();
	list<Mesh>*	 meshes = s.getSceneMeshes();
	 
	int i=0;
	for(list<Mesh>::iterator it = meshes->begin(); it != meshes->end(); ++it)
 	{	
		MeshItem newItem;
		newItem.sName = (*it).getName();
		newItem.pMesh = &(*it);
		newItem.nIndex = i;
		m_meshes.push_back(newItem);
		i++;
		
		m_ui.meshComboBox->addItem(newItem.sName);
 	}	
	
}
