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

#ifndef UPDATEDIALOG_H_
#define UPDATEDIALOG_H_

#include "./../ui_UpdateDialog.h"
#include "Modul.h"
#include <BSBaseTypes.h>
#include <QHttp>
#include <QBuffer>
#include <QHttpResponseHeader>
#include <QDomDocument>
#include <QList>
#include <QMap>

/*! \file
* 
*  \brief Describes the class UpdateDialog, their methodes and attributes.
*  \author Stefan Zeltner
* 
*/

/*! \brief The UpdateDialog
*
* This class describe the dialog-window form the updater.
*/
class UpdateDialog : public QDialog
{
	Q_OBJECT;
	
public:
	/*! \brief The constructor
	 *
	 * \param parent The parent
	 *
	 */	
	UpdateDialog(QWidget* parent = 0);
	
	/*! \brief The destructor
	 */
	virtual ~UpdateDialog();
	
private:
	Ui_onlineUpdate m_ui;					/*!< \brief For the communication with the dialog-window*/
	bool m_activeDownload;					/*!< \brief A flag which shows the state*/
	QHttp* http;							/*!< \brief The http - object for downloading files*/
	QBuffer* infoFileBuf;					/*!< \brief The buffer for the downloaded file*/
	QDomDocument xmlInfoFile;				/*!< \brief The object for parsing the XML-file*/
    QList<Modul*> oldComponents;			/*!< \brief A list with all old components*/
    QMap<QString, Modul*> newComponents;	/*!< \brief A list with all new components*/
    QMap<QString, QString> selectedModule;	/*!< \brief Components, checked for update*/
    const BSPlgMgr::Version version;		/*!< \brief The version of this class*/
    int nrOfFiles;							/*!< \brief The number of files for updating*/
    
    /*! \brief Parse the XML-file
	 */
    void parseXml();
    
    /*! \brief Save the downloaded file and make a backup of the old
     * 
     * Format: <old-name>dd-MM-yy_hh-mm-ss.backup
	 */
    void saveFile();
    
    /*! \brief Download all selected module
	 */
    void downloadQueue();
    
    /*! \brief Show the state of the download on the dialog
     * 
	 * \param act The current file
	 * \param from Number of all files
	 */
    void setFileCount(int act, int from);

protected slots:
    
    /*! \brief Add a message in theto the log
     * 
	 * \param msg This message will displayed in the log
	 */
	void addLog(const QString& msg);
    
    /*! \brief Show the download-progress
     * 
	 * \param value The current value
	 * \param max The maximum value
	 */
	void showProgress(int value, int max);
    
    /*! \brief Slot for click the button 'Start' or the button 'Cancel'
	 */
	void slotClickStart();
    
    /*! \brief Slot for click the button 'Update'
	 */
	void slotUpdateClicked();
	
    /*! \brief Cancel the updat action
	 */
	void cancel();
    
    /*! \brief Start the update
	 */
	void update();
private slots:
    
    /*! \brief Slot to handle the requestStarted(int id ) signal
     * 
	 * \param id The id of the started request
	 */
	void _requestStarted(int id );
    
    /*! \brief Slot to handle the requestFinished(int id, bool error) signal
     * 
	 * \param id The id of the finished request
	 * \param error Is false, if the request finished correct
	 */
	void _requestFinished(int id, bool error);
    
    /*! \brief Slot to handle the stateChanged(int state) signal
     * 
	 * \param state The new state of the system
	 */
	void _stateChanged(int state);
    
    /*! \brief Slot to handle the responseHeaderReceived(const QHttpResponseHeader& resp) signal
     * 
	 * \param resp The Http response header
	 */
	void _responseHeaderReceived(const QHttpResponseHeader& resp);
    
    /*! \brief Slot to handle the done(bool error) signal
     * 
	 * \param error Is false, if the download finished correct
	 */
	void _done(bool error);

signals:
	void startClicked();
	void cancelClicked();
};

#endif /*UPDATEDIALOG_H_*/
