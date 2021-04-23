#ifdef WIN32
#define URL	"url_win"
#endif

#ifndef WIN32 // for Linux
#define URL	"url_unix"
#endif

#include "./../include/UpdateDialog.h"
#include "./../include/Modul.h"
#include "./../include/PluginModul.h"
#include <QStringList>
#include <QCoreApplication>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QUrl>
#include <QFileInfo>
#include <QDomElement>
#include <QDomNodeList>
#include <QFile>
#include <QDateTime>
#include <QDir>

UpdateDialog::UpdateDialog(QWidget* parent)
    : QDialog(parent), m_activeDownload(false), version(1,0,0)
{
    m_ui.setupUi(this);
    
    http = new QHttp(this);
    
    infoFileBuf = new QBuffer(this);
    
    m_ui.show_updates->setRowCount(0);
    m_ui.show_updates->setColumnCount(8);
    
    QStringList headerLabels;
    
    headerLabels
                 << "Local UID" 
                 << "Local Module"
                 << "Local Version"
                 << ""
                 << "Remote UID"
                 << "Remote Module"
                 << "Remote Version"
                 << "Update?";
    
    m_ui.show_updates->setHorizontalHeaderLabels(headerLabels);
    
    QStringList args = QCoreApplication::arguments();
    args << "-" << "UpdateApplication" << version.toString();
    
    m_ui.show_updates->setRowCount(args.size() / 3);
    
    QString strUid, strName, strVersion;
    
    for(int row, column, j, i = 1; i < args.size();)
    {
	    row = (i-1) / 3;
	    
    	for(j = 0; j < 3; j++, i++)
    	{
	    	column = j % 3;
	    	
	    	QTableWidgetItem *item = new QTableWidgetItem(args[i]);
	    	m_ui.show_updates->setItem(row, column, item);
	    	
	    	switch(j)
	    	{
	    	case 0:		strUid = args[i]; break;
	    	case 1:		strName = args[i]; break;
	    	case 2:		strVersion = args[i]; break;
	    	}
    	}
    	
    	if(strUid == "-")
    		oldComponents.append(new Modul
     			(BSPlgMgr::Version(strVersion), strName, ""));
     	else
    		oldComponents.append(new PluginModul
     			(BSPlgMgr::Version(strVersion), strName, "", strUid.toUInt()));
    	
	}
	
	m_ui.updateButton->setEnabled(false);
	
	m_ui.curFile->setText("");
	m_ui.transfered->setText("0 B/ 0 B");
	m_ui.fileCount->setText("File 0 / 0");
	m_ui.progressBar->setValue(0);
	
	m_ui.pathToInfoFile->setText("http://people.fh-landshut.de/~szeltne/InfoFile.xml");
	
	// connections
	connect(m_ui.startChancelButton, SIGNAL(clicked()),
		this, SLOT(slotClickStart()));
	connect(this, SIGNAL(startClicked()),
		this, SLOT(update()));
	connect(this, SIGNAL(cancelClicked()),
		this, SLOT(cancel()));
	connect(m_ui.updateButton, SIGNAL(clicked()),
		this, SLOT(slotUpdateClicked()));
	connect(http, SIGNAL(requestStarted(int)),
		this, SLOT(_requestStarted(int)));
	connect(http, SIGNAL(requestFinished(int, bool)),
		this, SLOT(_requestFinished(int, bool)));
	connect(http, SIGNAL(stateChanged(int)),
		this, SLOT(_stateChanged(int)));
	connect(http, SIGNAL(dataSendProgress(int, int)),
		this, SLOT(showProgress(int, int)));
	connect(http, SIGNAL(dataReadProgress(int, int)),
		this, SLOT(showProgress(int, int)));
	connect(http, SIGNAL(responseHeaderReceived(QHttpResponseHeader)),
		this, SLOT(_responseHeaderReceived(QHttpResponseHeader)));
	connect(http, SIGNAL(done(bool)),
		this, SLOT(_done(bool)));
	
	addLog("Init successful");
}

UpdateDialog::~UpdateDialog()
{}

void UpdateDialog::addLog(const QString& msg)
{	
	m_ui.listWidget->addItem (msg);
}

void UpdateDialog::showProgress(int value, int max)
{
	if(value <= max)
	{
		m_ui.progressBar->setMaximum(max);
		
		m_ui.progressBar->setValue(value);
		
		QString msg = QString::number(value) + " B / " 
			+ QString::number(max) + " B";
		
		m_ui.transfered->setText(msg);
	}
}

void UpdateDialog::cancel()
{
	http->abort();
	m_ui.pathToInfoFile->setEnabled(true);
	m_ui.startChancelButton->setText("&Start");
	m_activeDownload = false;
}

void UpdateDialog::slotClickStart()
{
	m_ui.pathToInfoFile->setEnabled(false);
	m_ui.updateButton->setEnabled(false);
	
	if(m_activeDownload)
		emit cancelClicked();
	else
	{
		m_activeDownload = true;
		emit startClicked();
	}
}

void UpdateDialog::_requestStarted(int id )
{
	addLog("Request with id " + QString().setNum(id) +  "startet");
}

void UpdateDialog::_stateChanged(int state)
{
	switch(state)
	{
	case 0: addLog("Switch to State: Unconnected"); return;
	case 1: addLog("Switch to State: HostLookup"); return;
	case 2: addLog("Switch to State: Connecting"); return;
	case 3: addLog("Switch to State: Sending"); return;
	case 4: addLog("Switch to State: Reading"); return;
	case 5: addLog("Switch to State: Connected"); return;
	case 6: addLog("Switch to State: Closing");return;
	}
}

void UpdateDialog::_responseHeaderReceived(const QHttpResponseHeader& resp)
{
	addLog(" Http-Responseheader received: Status:"
		+ QString().setNum(resp.statusCode()) +  " : "
		+ resp.reasonPhrase());
}

void UpdateDialog::_requestFinished(int id, bool error)
{
	addLog("Request with id " + QString().setNum(id) +  "finished with "
		+ (error? "Error" : "no Error"));
}

void UpdateDialog::_done(bool error)
{
	m_activeDownload = false;
	
	m_ui.pathToInfoFile->setEnabled(true);
	m_ui.startChancelButton->setText("Start");
	
	addLog(QString("Download finished with") + (error? "no Error" : "Error"));
	
	if(error)
		return;
	
	if(infoFileBuf->size() == 0)
		return;
	
	if(m_ui.updateButton->isEnabled())
		saveFile();
	else
		parseXml();
}

void UpdateDialog::parseXml()
{
	{
		QString errorStr;
		int errorLine;
		int errorColumn;
		
		// Seeks to the start of input for random-access devices
		infoFileBuf->reset();
		
		if(!xmlInfoFile.setContent(infoFileBuf, true,
			&errorStr, &errorLine, &errorColumn))
		{
			QMessageBox::critical(this, "XML - Parser", "Parse error at line "
				+ QString().setNum(errorLine) + ", column "
				+ QString().setNum(errorColumn) + ":\n" + errorStr);
			
			return; 
		}
	}
	
	QDomElement root = xmlInfoFile.documentElement();
	
	if(root.tagName() != "BlackSunInfoFile")
	{
		QMessageBox::critical(this, "XML - Parser",
         	"The file is not an valid file.");
         
         return;
     }
     
     QDomNodeList xmlCoreComponents = root.firstChildElement("coreComponents").childNodes();
     QDomElement xmlVersion;
     QDomElement xmlUrl;
     
     for(int i = 0; i < xmlCoreComponents.size(); i++)
     {
     	xmlVersion = xmlCoreComponents.at(i).firstChildElement("version");
     	if(xmlVersion.isNull())
     	{
     		QMessageBox::critical(this, "XML - Parser",
         		"The file is not an valid file.\n"
         		+ xmlCoreComponents.at(i).toElement().tagName()
         		+ " - No tag version found!");
         	
         	continue;
     	}
     	
     	xmlUrl = xmlVersion.firstChildElement(URL);
     	if(xmlUrl.isNull())
     	{
     		QMessageBox::critical(this, "XML - Parser",
         		"The file is not an valid file.\n"
         		+ xmlCoreComponents.at(i).toElement().tagName()
         		+ " - No tag " + URL + " found!");
         	
         	continue;
     	}
     	
     	newComponents.insert(xmlCoreComponents.at(i).toElement().tagName(), new Modul
     				(BSPlgMgr::Version(xmlVersion.attributeNode("value").value()),
     				 xmlCoreComponents.at(i).toElement().tagName(),
     				 xmlUrl.text()));
     }
     
     QDomNodeList xmlPlugins = root.firstChildElement("plugins").childNodes();
     
     for(int i = 0; i < xmlPlugins.size(); i++)
     {
     	xmlVersion = xmlPlugins.at(i).firstChildElement("version");
     	if(xmlVersion.isNull())
     	{
     		QMessageBox::critical(this, "XML - Parser",
         		"The file is not an valid file.\n"
         		+ xmlPlugins.at(i).toElement().tagName()
         		+ " - No tag version found!");
         	
         	continue;
     	}
     	
     	xmlUrl = xmlVersion.firstChildElement(URL);
     	if(xmlUrl.isNull())
     	{
     		QMessageBox::critical(this, "XML - Parser",
         		"The file is not an valid file.\n"
         		+ xmlPlugins.at(i).toElement().tagName()
         		+ " - No tag " + URL + " found!");
         	
         	continue;
     	}
     			newComponents.insert(xmlPlugins.at(i).toElement().attributeNode("uid").value(), new PluginModul
     				(BSPlgMgr::Version(xmlVersion.attributeNode("value").value()),
     				 xmlPlugins.at(i).toElement().attributeNode("name").value(),
     				 xmlUrl.text(),
     				 xmlPlugins.at(i).toElement().attributeNode("uid").value().toUInt()));
     }
     
     
     foreach(Modul* newModul, newComponents)
     {
     	int index = 0;
     	int insertAt = -1;
     	bool needNewVersion = true;
     	QTableWidgetItem* twGtLt;
     	QTableWidgetItem* twUid;
     	QTableWidgetItem* twModule;
     	QTableWidgetItem* twVersion;
     	QTableWidgetItem* twUpdate;
     	
     	foreach(Modul* oldModul, oldComponents)
     	{
     		if(newModul->equal(oldModul))
     		{
	     		if(newModul->getTyp() == 1)
		     	{
		     		PluginModul* newModulPlg = dynamic_cast<PluginModul*> (newModul);
		     		
		     		twUid = new QTableWidgetItem(QString().setNum(newModulPlg->uid));
		     	}
		     	else
		     	{
		     		twUid = new QTableWidgetItem("-");
		     	}
		     	
	     		twModule = new QTableWidgetItem(newModul->name);
	     		twVersion = new QTableWidgetItem(newModul->version.toString());
	     		
	     		if(newModul->version > oldModul->version)
	     		{
	     			twGtLt = new QTableWidgetItem(" <  ");
	     			needNewVersion = true;
	     		}
	     		else
	     		{
	     			twGtLt = new QTableWidgetItem(" >= ");
	     			needNewVersion = false;
	     		}
		     	
		     	insertAt = index;
			     
			    break;
     		}
			
     		
			index++;
     	}
	    
	    if(insertAt == -1)
	    {
	    	if(newModul->getTyp() == 1)
	     	{
	     		PluginModul* newModulPlg = dynamic_cast<PluginModul*> (newModul);
	     		
	     		twUid = new QTableWidgetItem(QString().setNum(newModulPlg->uid));
	     	}
	     	else
	     	{
	     		twUid = new QTableWidgetItem("-");
	     	}
	     	
	    	twModule = new QTableWidgetItem(newModul->name);
	     	twVersion = new QTableWidgetItem(newModul->version.toString());
	     	twGtLt = new QTableWidgetItem(" <  ");
	     	
	    	insertAt = m_ui.show_updates->rowCount();
			m_ui.show_updates->setRowCount(insertAt + 1);
	    }
	    
	    twUpdate = new QTableWidgetItem();
	    
	    if(needNewVersion)
		{
			twUpdate->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			twUpdate->setCheckState(Qt::Checked);
		}
		else
		{
			twUpdate->setFlags(!Qt::ItemIsEnabled);
			twUpdate->setCheckState(Qt::Unchecked);
		}
	    
		m_ui.show_updates->setItem(insertAt, 3, twGtLt);
		m_ui.show_updates->setItem(insertAt, 4, twUid);
		m_ui.show_updates->setItem(insertAt, 5, twModule);
		m_ui.show_updates->setItem(insertAt, 6, twVersion);
		m_ui.show_updates->setItem(insertAt, 7, twUpdate);
	}

	m_ui.updateButton->setEnabled(true);
}

void UpdateDialog::update()
{
	m_ui.startChancelButton->setText("&Cancel");
	
	QUrl url(m_ui.pathToInfoFile->text());
	QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
	
	m_ui.curFile->setText(fileName);
	
	http->setHost(url.host(), url.port() != -1 ? url.port() : 80);
	
	infoFileBuf->open(QBuffer::ReadWrite);
	
	http->get(url.path(), infoFileBuf);
}

void UpdateDialog::slotUpdateClicked()
{
	m_ui.show_updates->setEnabled(false);
	m_ui.pathToInfoFile->setEnabled(true);
	
	int index = m_ui.show_updates->rowCount();
	
	while(index-- > 0)
	{
		QTableWidgetItem* actItem = m_ui.show_updates->item(index, 7);
		
		if(actItem->checkState() == Qt::Checked)
		{
			QTableWidgetItem* id = m_ui.show_updates->item(index, 4);
			
			if(id->text() == "-")
				id = m_ui.show_updates->item(index, 5);
			
			QString key = id->text();
			
			selectedModule.insert(key, newComponents[key]->url);
		}
	}
	
	QMessageBox::information(this, "Updater", "Please close the Blacksun editor!");
	
	nrOfFiles = selectedModule.size();
	
	downloadQueue();
}

void UpdateDialog::setFileCount(int act, int from)
{
	m_ui.fileCount->setText("File " + QString().setNum(act) + " / " + QString().setNum(from));
}

void UpdateDialog::downloadQueue()
{
	setFileCount(nrOfFiles - selectedModule.size(), nrOfFiles);
	
	if(selectedModule.size() == 0)
	{
		addLog("Update finished!");
		
		m_ui.updateButton->setEnabled(false);
	
		m_ui.curFile->setText("");
		m_ui.transfered->setText("0 B/ 0 B");
		m_ui.progressBar->setValue(0);
		m_ui.pathToInfoFile->setText("http://people.fh-landshut.de/~szeltne/InfoFile.xml");
		
		return;
	}
	
	QMap<QString, QString>::iterator i = selectedModule.begin();
	QString strUrl = i.value();
	
	QUrl url(strUrl);
 	QFileInfo fileInfo(url.path());
 	QString fileName = fileInfo.fileName();
	
	m_ui.curFile->setText(fileName);
	m_ui.pathToInfoFile->setText(strUrl);
	
	selectedModule.remove(i.key());
	
	infoFileBuf->close();
	
	delete infoFileBuf;
    
    infoFileBuf = new QBuffer(this);
	
	update();
}

void UpdateDialog::saveFile()
{
	QUrl url(m_ui.pathToInfoFile->text());
 	QFileInfo fileInfo(url.path());
 	QString fileName = fileInfo.fileName();
	QDir pluginDir(QCoreApplication::applicationDirPath());
	
	if(!( fileName.contains("BlackSun", Qt::CaseInsensitive)
	   || fileName.contains("Core", Qt::CaseInsensitive)
	   || fileName.contains("GUI", Qt::CaseInsensitive)
	   || fileName.contains("PlgMgr", Qt::CaseInsensitive)
	   || fileName.contains("Renderer", Qt::CaseInsensitive)
	   || fileName.contains("Scenegraph", Qt::CaseInsensitive)))
	{
		pluginDir.cd("Plugins");
	}
	
	QFile* file = new QFile(pluginDir.path () + "/" + fileName);
	
	if(file->exists())
		file->copy(file->fileName()
			+ QDateTime::currentDateTime().toString("dd-MM-yy_hh-mm-ss")
			+ ".backup");
	
	if (!file->open(QIODevice::WriteOnly))
	{
        QMessageBox::critical(this, "Error",
                                  tr("Unable to save the file %1: %2.")
                                  .arg(fileName).arg(file->errorString()));
         delete file;
         file = 0;
         return;
	}
	
	file->write(infoFileBuf->data());
	
	downloadQueue();
}
