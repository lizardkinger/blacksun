/***************************************************************************
 *   Copyright (C) 2006-07 by Stefan Zeltner
 *   szeltne@fh-landshut.de
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
 *  File:       Updater.h
 *  Created:    14.04.2007
 *  Author:     Stefan Zeltner
 *
 **************************************************************************/

#include "./../include/Updater.h"
#include <mainwindow.h>
#include <SceneManager.h>
#include <Log.h>
#include <UR_Manager.h>
#include <Renderer.h>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <QList>

static Updater* _instance = 0;

extern "C" DECLSPEC PlgInt* createInstance()
{
	if(_instance == 0)
		_instance = new Updater();

	return static_cast<PlgInt*>(_instance);
}

Updater::Updater() : fileName(QCoreApplication::applicationDirPath () + "/UpdaterApp") {}

Updater::~Updater(){}

bool Updater::loadPlg()
{
	QList<QAction*> mwActions = BSGui::MainWindow::getInstance()->getMenuBar()->actions();
	
	for(int i=0; i < mwActions.size(); i++)
	{
		if(mwActions[i]->text() == "&Help")
		{
			updaterAction = mwActions[i];
			break;	
		}	
	}
	
	mwActions = updaterAction->menu()->actions();
	
	for(int i=0; i < mwActions.size(); i++)
		if(mwActions[i]->text() == "Check for new version")
		{
			updaterAction = mwActions[i];
			break;
		}
	
	connect(updaterAction, SIGNAL(triggered()), this, SLOT(startUpdate()));
	
	return true;
}

bool Updater::unloadPlg()
{
	disconnect(updaterAction, SIGNAL(triggered()), this, SLOT(startUpdate()));
	return true;
}


bool Updater::uninstallPlg()
{
	disconnect(updaterAction, SIGNAL(triggered()), this, SLOT(startUpdate()));
	return true;
}

void Updater::closePlg()
{}

void Updater::startUpdate()
{
	if(QFile::exists (fileName) || QFile::exists (fileName + ".exe"))
	{
		QStringList arg;
		QList<UID> plugins = BSPlgMgr::PlgMgr::getInstance()->getAvailablePlugins();
		
		BSPlgMgr::Version* core;
		if(BSCore::Log::version < BSCore::UR_Manager::version)
			core = &BSCore::UR_Manager::version;
		else
			core = &BSCore::Log::version;
		
		arg << "-" << "Core" << core->toString()
		    << "-" << "Gui" << BSGui::MainWindow::version.toString()
		    << "-" << "PluginSystem" <<  BSPlgMgr::PlgMgr::version.toString()
		    << "-" << "Renderer" <<  BSRenderer::Renderer::version.toString()
		    << "-" << "Scenegraph" << BSScene::SceneManager::version.toString();
		
		for(int i = 0; i < plugins.size(); i++)
		{
			arg << QString().setNum(plugins[i])
				<< BSPlgMgr::PlgMgr::getInstance()->getName(plugins[i])
				<< BSPlgMgr::PlgMgr::getInstance()->getVersion(plugins[i]).toString();
		}
		
		if(!QProcess::startDetached (fileName, arg))
			QMessageBox::critical(NULL,"Error", "Can't start UpdaterApp");
	}
	else
		QMessageBox::critical(NULL,"Error", "UpdaterApp not found");
}
