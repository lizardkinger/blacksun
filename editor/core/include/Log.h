/***************************************************************************
 *   Copyright (C) 2006 by Philipp Gruber
 *   pgruber@fh-landshut.de
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
 *  Module:     Logger (BlackSun)
 *  File:       Log.h
 *  Created:    15.11.2006
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
 
#ifndef LOG_H_
#define LOG_H_

///////////////////////////////////////////////////////////
// for the Versions - Info
///////////////////////////////////////////////////////////
#include "./../../pluginsystem/include/BSBaseTypes.h"

// Includes
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>
//Added by Thomas Tischler needed for signals and slots
#include <QObject>
using namespace std;

/*! \file
* 
*  \brief Describes the Class Log, their methodes and attributes.
*  \author Philipp Gruber
*/

/*!
 * \brief For an easier way to write an Error-Message into the Log and get the
 * 			line and file where the message was written.
 * 
 * \param details The message which can be written into the Log for error-details.
 */
#define LOG_Error(details) (BSCore::Log::getInstance()-> \
		message(BSCore::LOG_Error, details, __LINE__, __FILE__, __FUNCTION__)) 
		
/*!
 * \brief For an easier way to write an OK-Message into the Log and get the
 * 			line and file where the message was written.
 * 
 * \param details The message which can be written into the Log for details.
 */
#define LOG_Ok(details) (BSCore::Log::getInstance()-> \
		message(BSCore::LOG_Ok, details, __LINE__, __FILE__, __FUNCTION__))
		
/*! 
 * \brief For an easier way to write a warning into the Log and get the
 * 			line and file where the message was written.
 * 
 * \param details The message which can be written into the Log for warning-details.
 */
#define LOG_Warning(details) (BSCore::Log::getInstance()-> \
		message(BSCore::LOG_Warning, details, __LINE__, __FILE__, __FUNCTION__))
		
/*! 
 * \brief For an easier way to write any other message into the Log and get the
 * 			line and file where the message was written.
 * 
 * \param details The message which can be written into the Log for details.
 */
#define LOG_Other(details) (BSCore::Log::getInstance()-> \
		message(BSCore::LOG_Other, details, __LINE__, __FILE__, __FUNCTION__))
		
/*!
 * \brief For an easier way to write a topic into the Log.
 * 
 * \param topic The topic which should be written into the Log.
 */
#define LOG_TOPIC(topic) (BSCore::Log::getInstance()->writeTopic(topic))

namespace BSCore
{
	
	/*!
	 * \brief The enum Types describes the kinds of log-messages which are possible.
	 */
	enum Types
	{
		LOG_Ok,
		LOG_Warning,
		LOG_Error,
		LOG_Other
	};
	
	/*!
	 * \brief The structure LogContents describes a log-entry.
	 * 
	 * A log-entry consists of a state, a message, the line-number, the file, the function and the time where/when the message was written.
	 */
	struct LogContents
	{
		const char* state;
		const char* message;
		int line;
		const char* file;
		const char* function;
		const char* time;
	};
	
    //QObject is needed in order to provide QT's signal's and slot's
    class Log : public QObject
	{
        
        Q_OBJECT
        
		private:
			//Pointer at the instance
			static Log* _instance;
			
			//Variable to check if the logfile is alredy initialised.
			bool initialized;
			
			//The output filestream to the logfile.
			ofstream log;
			
			//Stores all log-entrys for the GUI-output.
			vector<LogContents> logList;
			
			/*!
			 * \brief Writes the message into the logfile and stores it for 
			 * 			the GUI-output.
			 * 
			 * \param type The kind of the message.
			 * \param msg The message with additional informations.
			 * \param line The line where the log-entry was written.
			 * \param file The file in which the log-entry was written.
			 * \param fun The function where the log-entry was written.
			 * \param color The Hex-Code of the color the log-entry should be
			 * 			written in.
			 */
			void writeToLog(const string& type, const string& msg, int line, 
					const string& file, const string& fun, const string& color);
					
			/*!
			 * \brief Returns the current time of the system.
			 */
			string getTime();

		protected:
			/*!
			 * \brief The default constructor.
			 * 
			 * The default constructor sets the check variable "initialized" 
			 * 		false.
			 */
			Log();
			
		public:
			/*!
			 * \brief The Version
			 */
			static BSPlgMgr::Version version;
		
			/*!
			 * \brief The destructor
			 * 
			 * The destructor writes the end of the logfile, closes the
			 * 		filestream and sets the check variable "initializes" false.
			 */
			virtual ~Log();
			
			/*!
			 * \brief Initialiezes the logfile
			 * 
			 * Opens the filestream, wirtes the header into the logfile and sets
			 * 			the check variable "initialized" true.
			 */
			void init();
			
			/*!
			 * \brief Returns the instance.
			 * 
			 * If not already an instance exists, the instance will be creates 
			 * 			and returned.
			 */
			static Log* getInstance();
			
			/*!
			 * \brief Evaluates the messagetype to write it into the log.
			 */
			void message(Types msgType, const string& msg, int line, 
					const string& file, const string& fun);
					
			/*!
			 * \brief Writes a Topic into the logfile.
			 * 
			 * \param topic The Topic.
			 */
			void writeTopic(const string& topic);
			
			/*!
			 * \brief Cancels logging.
			 * 
			 * Calls the destructor and deletes the instance.
			 */
			void kill();
			
			/*!
			 * \brief Returns a ponter at the Loglist.
			 */
			vector<LogContents>*  getLogList();
            

            /*!
             * \brief Emitted when a new log entry is added.
             * 
             * This signal is emitted when a log entry has changed.
             * The parameter is the index of the new log Entry
             */
        signals:
            void logChanged(BSCore::LogContents* temp);
	};
}

#endif /*LOG_H_*/
