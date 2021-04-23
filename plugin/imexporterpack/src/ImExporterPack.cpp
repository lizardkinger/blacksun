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
 *  File:       ImExporterPack.cpp
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "./../include/ImExporterPack.h"
//#include <SceneGraph.h>
#include "./../../scenegraph/include/SceneGraph.h"
#include "./../../gui/include/mainwindow.h"

using namespace BSRenderer;

static ImExporterPack* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new ImExporterPack();

	return static_cast<PlgInt*>(_instance);
}

ImExporterPack::ImExporterPack() : version(Version(1, 0, 0))
{
	m_sName = "Im-/Exporter-Pack";
	m_sDescription = "Loads and saves a number of model formats.";
	m_sDescription += "<br><br><b>Supported file formats for import:</b>";
	
	m_importerMD2 = new ImporterMD2();
	m_importer3DS = new Importer3DS();
	m_importerOBJ = new ImporterOBJ();
	m_exporterOBJ = new ExporterOBJ();	
	
	m_sDescription += "<table>";
	m_sDescription += "<tr><td><b>" + QString(m_importerMD2->getFileExtension().c_str()) + 
		QString("</b></td><td><em>") + QString(m_importerMD2->getFileDescription().c_str()) + 
		QString("</em></td></tr>");
	m_sDescription += "<tr><td><b>" + QString(m_importer3DS->getFileExtension().c_str()) + 
		QString("</b></td><td><em>") + QString(m_importer3DS->getFileDescription().c_str()) + 
		QString("</em></td></tr>");
	m_sDescription += "<tr><td><b>" + QString(m_importerOBJ->getFileExtension().c_str()) + 
		QString("</b></td><td><em>") + QString(m_importerOBJ->getFileDescription().c_str()) + 
		QString("</em></td></tr>");
	m_sDescription += "</table>";		
	
	m_sDescription += "<br><b>Supported file formats for export:</b>";
	m_sDescription += "<table>";
	m_sDescription += "<tr><td><b>" + QString(m_exporterOBJ->getFileExtension().c_str()) + 
		QString("</b></td><td><em>") + QString(m_exporterOBJ->getFileDescription().c_str()) + 
		QString("</em></td></tr>");
	m_sDescription += "</table>";	
	
	m_pMenuImporter = NULL;
	m_pMenuExporter = NULL;										
}

ImExporterPack::~ImExporterPack()
{
}

bool ImExporterPack::unloadPlg()
{
	if(m_pMenuImporter != NULL)
	{
		delete m_pMenuImporter;
		m_pMenuImporter = NULL;	
	}
	
	if(m_pMenuExporter != NULL)
	{
		delete m_pMenuExporter;
		m_pMenuExporter = NULL;	
	}
	
	return true;
}

bool ImExporterPack::loadPlg()
{
	QAction* pImport = NULL;
	QAction* pExport = NULL;
	QMenu* pMenuFile = NULL;
	
	//Get the menu and actions where to add the new menu items
	getMenuActions(&pMenuFile, &pImport, &pExport);
	
	//Create importer menu
	m_pMenuImporter = new QMenu(pMenuFile);
	m_pMenuImporter->addAction(QString(m_importerMD2->getFileDescription().c_str()),
			this, SLOT(executeImporterMD2()));
	m_pMenuImporter->addAction(QString(m_importer3DS->getFileDescription().c_str()),
			this, SLOT(executeImporter3DS()));
	m_pMenuImporter->addAction(QString(m_importerOBJ->getFileDescription().c_str()),
			this, SLOT(executeImporterOBJ()));		

	//Create exporter menu
	m_pMenuExporter = new QMenu(pMenuFile);
	m_pMenuExporter->addAction(QString(m_exporterOBJ->getFileDescription().c_str()),
			this, SLOT(executeExporterOBJ()));

	//Set the new menus
	pImport->setMenu(m_pMenuImporter);
	pExport->setMenu(m_pMenuExporter);
	
	return true;
}

void ImExporterPack::getMenuActions(QMenu** pMenu, QAction** pImport, QAction** pExport)
{
	QList<QAction*> pActions = BSGui::MainWindow::getInstance()->getMenuBar()->actions();
	QAction* pM = NULL;

	//Walk through all menu-bar elements
	for(int i=0; i<pActions.size(); i++)
	{
		if(pActions[i]->text() == "&File")
		{
			pM = pActions[i];
			break;	
		}	
	}
	
	//Get the submenu-actions
	(*pMenu) = pM->menu();	
	pActions = (*pMenu)->actions();
	
	//Find the action of the import and export item
	for(int i=0; i<pActions.size(); i++)
	{
		if(pActions[i]->text() == "Import")
		{
			(*pImport) = pActions[i];	
		}
		
		if(pActions[i]->text() == "Export")
		{
			(*pExport) = pActions[i];	
		}				
	}
}

void ImExporterPack::executeImporterMD2()
{
	m_importerMD2->execute();	
}

void ImExporterPack::executeImporter3DS()
{
	if(!m_importer3DS->isVisible())
	{
		m_importer3DS->setVisible(true);
	}
}

void ImExporterPack::executeImporterOBJ()
{
	if(!m_importerOBJ->isVisible())
	{
		m_importerOBJ->setVisible(true);
	}
}

void ImExporterPack::executeExporterOBJ()
{
	m_exporterOBJ->execute();	
}
