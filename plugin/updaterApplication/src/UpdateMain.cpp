#include <QApplication>

#include "./../include/UpdateDialog.h"

int main(int argc, char *argv[])
{
     QApplication app(argc, argv);
     UpdateDialog updateWin;
     updateWin.show();
     return updateWin.exec();
}

/*#include "../include/UpdateModul.h"
#include <QtDebug>
#include <QMessageBox>

UpdateModul::UpdateModul(QObject * parent)
    : QObject(parent), buf(new QBuffer(this))
{
}


UpdateModul::~UpdateModul()
{
}

void UpdateModul::startUpdate()
{
	QUrl url(m_Address);
	
	http = new QHttp(this);
	
	connect(http, SIGNAL(requestStarted(int)),
		this, SLOT(reqStarted(int)));
	connect(http, SIGNAL(requestFinished(int, bool)),
		this, SLOT(reqFinished(int, bool)));
	connect(http, SIGNAL(stateChanged(int)),
		this, SLOT(newState(int)));
	connect(http, SIGNAL(dataSendProgress(int, int)),
		this, SLOT(dataSendInfo(int , int)));
	connect(http, SIGNAL(dataReadProgress(int , int)),
		this, SLOT(dataReadInfo(int, int)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
		this, SLOT(resHeaderReceived(const QHttpResponseHeader &)));
	connect(http, SIGNAL(readyRead(const QHttpResponseHeader &)),
		this, SLOT(okRead(const QHttpResponseHeader &)));
	connect(http, SIGNAL(done(bool)),
		this, SLOT(httpDone(bool)));
 	
 	buf->open(QBuffer::ReadWrite);
 	
 	http->setHost(url.host(), url.port() != -1 ? url.port() : 80);
     if (!url.userName().isEmpty())
         http->setUser(url.userName(), url.password());
    
    http->get(url.path(), buf);
}

void UpdateModul::cancelUpdate()
{
	emit newJob("Update canceled");
	http->abort();
	emit finished();
}

void UpdateModul::reqStarted(int id)
{
	QString msg = "Request with ID " + QString::number(id) + " started";
	
	emit newJob(msg);
}
	
void UpdateModul::reqFinished(int id, bool error)
{
	QString msg = "Request with ID " + QString::number(id) + " finished"
		+ ((error)? " not correct" : " correct");
	
	emit newJob(msg);
}

void UpdateModul::newState(int state)
{
	QString msg;
	
	switch(state)
	{
	case QHttp::Unconnected:
		msg = "Switch to State Unconnected";
		break;
	case QHttp::HostLookup:
		msg = "Switch to State HostLookup";
		break;
	case QHttp::Connecting:
		msg = "Switch to State Connecting";
		break;
	case QHttp::Sending:
		msg = "Switch to State Sending";
		break;
	case QHttp::Reading:
		msg = "Switch to State Reading";
		break;
	case QHttp::Connected:
		msg = "Switch to State Connected";
		break;
	case QHttp::Closing:
		msg = "Switch to State Closing";
		break;
	default:
		msg = "Warning!! Switch to an undefined State";
	}
	
	emit newJob(msg);
}

void UpdateModul::dataSendInfo(int done, int total)
{
	if(done == total)
	{
		QString msg = "Send " + QString::number(total) + " Bytes";
	
		emit newJob(msg);
	}
	
	emit progress(done, total);
}
void UpdateModul::dataReadInfo(int done, int total)
{
	if(done == total)
	{
		QString msg = "Recive " + QString::number(total) + " Bytes";
	
		emit newJob(msg);
	}
	
	emit progress(done, total);
}
void UpdateModul::resHeaderReceived(const QHttpResponseHeader & resp)
{
	QString msg = "Response Header received";
	
	emit newJob(msg);
}

void UpdateModul::okRead(const QHttpResponseHeader & resp)
{
	QString msg = "Ready Read";
	
	emit newJob(msg);
}

void UpdateModul::httpDone(bool error)
{
	QString msg = QString("Finished with ")
		+ ((error)? " Error" : " no Error");
	
	emit newJob(msg);
    
    QMessageBox::question(NULL, "info", QString().number(buf->size()));
    
	emit finished();
}
*/
