/***************************************************************************
 *   Copyright (C) 2006 by Blacksun project team
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
 *  Module:     Blacksun (BlackSun)
 *  File:       main.cpp
 *  Created:    01.10.2006
 *  Author:     Blacksun project team
 *
 **************************************************************************/

#include <QtGui>
#include <QUiLoader>
#include <QApplication>
#include <QTime>

#include "./../../core/include/Log.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../gui/include/mainwindow.h"
#include "./../../gui/include/SplashScreen.h"
#include "./../../pluginsystem/include/PlgMgr.h"

void wait(int waitTime, const QApplication &app);

int main(int argc, char** argv)
{
	Q_INIT_RESOURCE(app);
	
    QApplication app(argc, argv);

    //Setup organisation name for QSettings
    QCoreApplication::setOrganizationName("FH-Landshut");
    QCoreApplication::setApplicationName("Blacksun");
    app.setWindowIcon(QIcon(":/splash"));
	
    QPixmap pixmap(":/splash");
    
	BSGui::SplashScreen *splash = new BSGui::SplashScreen(pixmap);
	splash->show();
	app.processEvents();
	
	// register components:
	splash->showMessage("Loading core",Qt::AlignRight, Qt::red);
	//BSCore::Log::getInstance();
/*	wait(2000, app);

	splash->showMessage("Loading plugin system",Qt::AlignRight, Qt::red);
	//BSPlgMgr::PlgMgr::getInstance();
	wait(800, app);
	
	splash->showMessage("Loading scene management",Qt::AlignRight, Qt::red);
	//BSScene::SceneManager::getInstance();
	wait(800, app);
	
	splash->showMessage("Loading rendering engine",Qt::AlignRight, Qt::red);
	// not implement yet. . . BSRenderer::Renderer::createInstance();
	wait(800, app);*/

	PlgMgr::getInstance()->restoreSavedStatus();

    BSGui::MainWindow* mw = BSGui::MainWindow::getInstance();
	splash->finish(mw);
    mw->show();

    QUiLoader loader;
    //qDebug() << loader.availableWidgets();
    loader.addPluginPath("asdf");
	
	delete splash;
	return app.exec();
}
