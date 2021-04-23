#include "./../include/PlgMgr.h"
#include "./../../core/include/Log.h"
#include "./../include/SearchForPlugins.h"
#include <QSettings>
#include <QVariant>
#include <QFile>

#include <QtDebug>

namespace BSPlgMgr
{

PlgMgr* PlgMgr::m_instance = 0;

//Set version
BSPlgMgr::Version PlgMgr::version(1,0,0);

PlgMgr::PlgMgr() {}

PlgMgr::~PlgMgr()
{
	QList<UID> allUIDs = m_plgList.keys();
	
	int listEnd = allUIDs.size();
	
	QSettings settings;
	
	settings.beginWriteArray("installedplugins");
	settings.remove("");
	
	for(int i = 0; i < listEnd; i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("path", m_plgList[allUIDs.at(i)]->getFileName());
		settings.setValue("state", m_plgList[allUIDs.at(i)]->getState());
    	
    	m_plgList[allUIDs.at(i)]->getInstance()->closePlg();
    	
    	delete m_plgList[allUIDs.at(i)];
	}
    settings.endArray();
}

void PlgMgr::restoreSavedStatus()
{
	QSettings settings;
	QByteArray path;
	BSPlugin* loadingPlg;
	bool state;
	int numberOfRegisteredPlugins = settings.beginReadArray("installedplugins");
	
	qDebug() << " Number of registered plugins: " << numberOfRegisteredPlugins;
	
    for (int i = 0; i < numberOfRegisteredPlugins; ++i)
    {
    	settings.setArrayIndex(i);
        
        path = settings.value("path").toByteArray();
        state = settings.value("state").toBool();
        
        if(!QFile::exists(QString(path)))
		{
			LOG_Error(QString("Error: File " + QString(path)
			+ " does not exist!\nPlease reinstall the plugin").toStdString());
		}
        else
        {
        	loadingPlg = new BSPlugin(path);
			
			if(loadingPlg->getInstance() == NULL)
			{
				LOG_Error(QString("Error: Can't load " + QString(path)
	    			+ "\nPlease reinstall the plugin").toStdString());
	    		
	    		delete loadingPlg;
			}
			else
			{
				if(loadingPlg->getInstance()->installPlg())
				{
					if(state)
					{
					LOG_Ok(QString("Load Plugin "
						+ loadingPlg->getInstance()->getName() + " from file "
						+ loadingPlg->getFileName()).toStdString());
					
						if(loadingPlg->getInstance()->loadPlg())
						{
							LOG_Ok(QString("Plugin "
								+ loadingPlg->getInstance()->getName()
								+ " switch to state \"load\" ").toStdString());
								
							loadingPlg->setState(true);
						}
						else
							LOG_Error(QString("Error: Plugin " 
	    						+ loadingPlg->getInstance()->getName()
	    						+ " can't switch to state \"load\" ")
	    						.toStdString());
					}
				
				m_plgList.insert(loadingPlg->getUID(), loadingPlg);
				}
				else
				{
					LOG_Error(QString("Error: Can't initialise "
	    				+ loadingPlg->getInstance()->getName()).toStdString());
	    			
	    			delete loadingPlg;
				}
			}
        }
    }
    settings.endArray();
    
    // open the autosearch dialog for plugins
	SearchForPlugins();
}

int PlgMgr::installPlg(QString fileName)
{
	emit setStatusBar("Install Plugin " + fileName);
	
	if(!QFile::exists(fileName))
	{
		LOG_Error(
			QString("File " + fileName + "not exists").toStdString());
		return BSPlgMgr::FILE_NOT_FOUND;
	}
	
	if(!onlyAscii(fileName))
		LOG_Warning(
			QString("File " + fileName + "includes non-ASCII symbols!\nThe Plugin may work not correct").toStdString());
	
	QByteArray path = fileName.toAscii();
	
	BSPlugin* loadingPlg = new BSPlugin(path);
	
	if(loadingPlg->getInstance() == NULL)
	{
		LOG_Error(QString("Can't create an instance from file "
			+ fileName).toStdString());
		return BSPlgMgr::NOT_A_VALID_PLUGIN;
	}
	
	if(m_plgList.contains(loadingPlg->getUID()))
	{
		LOG_Warning(QString("Plugin " + loadingPlg->getInstance()->getName()
			+ " is already installed").toStdString());
		return ALREADY_INSTALLED;
	}
	
	if(loadingPlg->getInstance()->installPlg())
	{
	    m_plgList.insert(loadingPlg->getUID(), loadingPlg);
		
		emit plgInstalled(loadingPlg->getUID());
		
		LOG_Ok(QString("Install " + loadingPlg->getInstance()->getName()
			+ " from "+ fileName).toStdString());
		
		return BSPlgMgr::OK;
	}
	LOG_Error(QString("Can't initialise " + loadingPlg->getInstance()->getName()
		+ "from" + fileName).toStdString());
	
	return BSPlgMgr::PLG_SPECIFIC_ERROR;
}

int PlgMgr::uninstallPlg(UID uid)
{
	emit setStatusBar("Uninstall Plugin " + QString::number(uid));
	
	if(!m_plgList.contains(uid))
	{
		LOG_Error(QString("Plugin with uid " + QString::number(uid)
			+ " not installed").toStdString());
		return NOT_INSTALLED;
	}
	
	if(m_plgList[uid]->getInstance()->uninstallPlg())
	{
		LOG_Ok(QString("Uninstall "
			+ m_plgList[uid]->getInstance()->getName()).toStdString());
		
		BSPlugin* temp_Plg = m_plgList[uid];
		
		m_plgList.remove(uid);
		
		emit plgRemoved(temp_Plg->getUID());
		
		delete temp_Plg;
		
		return BSPlgMgr::OK;
	}
	LOG_Error(QString("Uninstall faild! Can't uninstall "
		+  m_plgList[uid]->getInstance()->getName()).toStdString());
	
	return BSPlgMgr::PLG_SPECIFIC_ERROR;
}

int PlgMgr::getInformation(UID uid, const Version& version)
{
	int iFlag = 0x0000;
	
	if(!m_plgList.contains(uid))
		return iFlag;
	iFlag += IS_INSTALLED;
	
	if(!m_plgList[uid]->getState())
		iFlag += IS_LOADED;
	
	if(version > m_plgList[uid]->getInstance()->getVersion())
		iFlag += VERSION_ERROR;
	
	return iFlag;
}

PlgInt* PlgMgr::getPlgInstance(UID uid, const Version& version)
{
	errorFlag = 0x0000;
	
	
	if(!m_plgList.contains(uid))
	{
		errorFlag = NOT_INSTALLED;
		return NULL;
	}
	
	if(version < m_plgList[uid]->getInstance()->getVersion())
	{
		errorFlag = VERSION_DISPUTE;
		return NULL;
	}
	
	errorFlag = BSPlgMgr::OK;
	
	return m_plgList[uid]->getInstance();
}

int PlgMgr::getErrorFlag()
{
	return errorFlag;
}

int PlgMgr::clearErrorFlag()
{
	int temp = errorFlag;
	
	errorFlag = 0x0000;
	
	return temp;
}

QString PlgMgr::getAboutData(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		errorFlag = NOT_INSTALLED;
		return "";
	}
	
	return m_plgList[uid]->getInstance()->getDescription();
}

QString PlgMgr::getName(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		errorFlag = NOT_INSTALLED;
		return "";
	}
	
	return m_plgList[uid]->getInstance()->getName();
}

Version PlgMgr::getVersion(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		errorFlag = NOT_INSTALLED;
		return Version(0,0,0);
	}
	
	return m_plgList[uid]->getInstance()->getVersion();
}

bool PlgMgr::getStatus(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		errorFlag = NOT_INSTALLED;
		return false;
	}
	
	return m_plgList[uid]->getState();
}

QList<UID> PlgMgr::getAvailablePlugins()
{
	return m_plgList.keys();
}

int PlgMgr::loadPlugin(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		LOG_Error(QString("A plugin with the UID "
			+ QString::number(uid) + " is not installed!").toStdString());
		return NOT_INSTALLED;
	}
	
	if(m_plgList[uid]->getState())
	{
		LOG_Warning(QString("Plugin "+ m_plgList[uid]->getInstance()->getName()
			+ " is already running!").toStdString());
		return ALREADY_RUNNING;
	}
	
	if(m_plgList[uid]->getInstance()->loadPlg())
	{
		m_plgList[uid]->setState(true);
		
		LOG_Ok(QString("Load the plugin "
			+ m_plgList[uid]->getInstance()->getName()
			+ " successful!").toStdString());
		
		emit stateChanged(uid);
		
		return BSPlgMgr::OK;
	}
	
	LOG_Error(QString("Loading the plugin "
		+ m_plgList[uid]->getInstance()->getName()
		+ " failed!").toStdString());
	
	return BSPlgMgr::PLG_SPECIFIC_ERROR;
}

int PlgMgr::unloadPlugin(UID uid)
{
	if(!m_plgList.contains(uid))
	{
		LOG_Error(QString("A plugin with the UID "
			+ QString::number(uid) + " is not installed!").toStdString());
		return NOT_INSTALLED;
	}
	
	if(!m_plgList[uid]->getState())
	{
		LOG_Warning(QString("Plugin "+ m_plgList[uid]->getInstance()->getName()
			+ " is already stopped!").toStdString());
		return ALREADY_STOPPED;
	}
	
	if(m_plgList[uid]->getInstance()->unloadPlg())
	{
		m_plgList[uid]->setState(false);
		
		LOG_Ok(QString("Unload the plugin "
			+ m_plgList[uid]->getInstance()->getName()
			+ " successful!").toStdString());
		
		emit stateChanged(uid);
		
		return BSPlgMgr::OK;
	}
	
	LOG_Error(QString("Unloading the plugin "
		+ m_plgList[uid]->getInstance()->getName()
		+ " failed!").toStdString());
	
	return BSPlgMgr::PLG_SPECIFIC_ERROR;
}

PlgMgr* PlgMgr::getInstance()
{
	static Guardian g;
	
	if(m_instance == 0)
		m_instance = new PlgMgr;

	return m_instance;
}

bool PlgMgr::onlyAscii(QString str)
{
	int length = str.length();
	
	for(int i = 0; i < length; i++)
	{
		if(str.at(i) > 127)
			return false;
	}
	
	return true;
}

}
