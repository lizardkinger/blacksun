#include "../include/SearchForPlugins.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QCoreApplication>

#ifdef WIN32
#define FILE_ENDING "*.dll"
#endif
#ifndef WIN32
#define FILE_ENDING "*.so"
#endif

namespace BSPlgMgr
{

SearchForPlugins::SearchForPlugins()
{
	//Make connections
    connect(PlgMgr::getInstance(), SIGNAL(plgInstalled(UID)), this, SLOT(getInstalledUid(UID)));
    
    QStringList alreadyInstalled;
    QString searchPath;
    int options;
	QSettings settings;
	QByteArray path;
	
	int numberOfRegisteredPlugins = settings.beginReadArray("installedplugins");
	
	for (int i = 0; i < numberOfRegisteredPlugins; ++i)
    {
    	settings.setArrayIndex(i);
        
        path = settings.value("path").toByteArray();
        
        alreadyInstalled << QString(path);
    }
    settings.endArray();
    
    // init variables:
    if(settings.contains("PlgMgr/SearchPath"))
    	 searchPath = QString(
    	 	settings.value("PlgMgr/SearchPath").toByteArray());
    else
    {
    	QDir currentPath(QCoreApplication::applicationDirPath());
    	currentPath.cd("Plugins");
    	settings.setValue("PlgMgr/SearchPath", currentPath.path().toAscii());
    	searchPath = currentPath.path();
    }
    
    if(settings.contains("PlgMgr/Options"))
    	options = settings.value("PlgMgr/Options").toInt();
    else
    {
    	options = 2;
    	settings.setValue("PlgMgr/Options", 2);
    }
    
    if(options)
    {
	    QDir searchDir(searchPath);
	    QStringList fileResults = 
			searchDir.entryList(QStringList() << FILE_ENDING, QDir::Files);
		int resultFlag = 0;
		QString fileAndPaht;
		
		for(int i = 0; i < fileResults.size(); i++)
		{
			
			fileAndPaht = searchDir.absolutePath() + "/" + fileResults.at(i);
			
			
			resultFlag = PlgMgr::getInstance()->installPlg(fileAndPaht);
			
			if(resultFlag == BSPlgMgr::OK)
				if(options == 2)
					PlgMgr::getInstance()->loadPlugin(m_lastInstalledPlugin);
		}
    }
}

SearchForPlugins::~SearchForPlugins()
{}

void SearchForPlugins::getInstalledUid(UID uid)
{
	m_lastInstalledPlugin = uid;
}

}
