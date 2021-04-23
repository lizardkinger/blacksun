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
 *  File:       Log.cpp
 *  Created:    15.11.2006
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
 
#include "./../include/Log.h"

namespace BSCore
{
	//Clear instance
	Log* Log::_instance = NULL;
	
	//Set version
	BSPlgMgr::Version Log::version(1,0,0);
	
	//Constructor
	Log::Log()
	{
		initialized = false;
	}
	
	//Destructor
	Log::~Log()
	{
		log << "</table>\n\n";
		log << "<p align=\"center\">&nbsp;</p>\n";
		log.flush();
		log.close();
		
		initialized = false;
	}
	
	//Initialize the Logfile
	void Log::init()
	{
		//Open the filestream
		log.open("Logfile.html");
		
		//Write header into the logfile
		log << "<p align=\"center\"><font size=\"7\" ";
		log << "color=\"#000000\"><b><u>BlackSun Logfile</u></b></font></p>\n";
		log << "<p align='center'><a target='_new 'href=\'";
		log << "http://sourceforge.net/projects/blacksun/'>Homepage</a></p>\n";
		log << "<p align=\"center\">&nbsp;</p>\n";
		log << "<p align=\"center\">&nbsp;</p>\n";
		log << "<table cellspacing=\"1\" align=\"none\" border=\"1\" ";
		log << "width=\"100%\"cellpadding=\"1\">";
		log << "<tr>";
		log << "<td align=\"center\" width=\"11%\"><b><font ";
		log << "color=\"#000000\">STATE</font></b></td>";
		log << "<td align=\"center\" width=\"30%\"><b><font ";
		log << "color=\"#000000\">MESSAGE</font></td>";
		log << "<td align=\"center\" width=\"5%\"><b><font ";
		log << "color=\"#000000\">LINE</font></td>";
		log << "<td align=\"center\" width=\"16%\"><b><font ";
		log << "color=\"#000000\">FILE</font></td>";
		log << "<td align=\"center\" width=\"28%\"><b><font ";
		log << "color=\"#000000\">FUNCTION</font></td>";
		log << "<td align=\"center\" width=\"10%\"><b><font ";
		log << "color=\"#000000\">TIME</font></td>";
		log << "<tr>";
		log.flush();
		
		//Set check variable "initialized" true
		initialized = true;
	}
	
	//Create instance if it doesn´t exist an retrun it
	Log* Log::getInstance()
	{
		if(_instance == NULL)
		{
			_instance = new Log();
			_instance->init();
		}
		
		return _instance;
	}
	
	//Evaluate the messagetype and call function "writeToLog" with the write 
	//parameters.
	void Log::message(Types msgType, const string& msg, int line, 
				const string& file, const string& fun)
	{
		switch(msgType)
		{
			case LOG_Ok:
			{
				writeToLog("OK", msg, line, file, fun, "#008000");
			}break;
			
			case LOG_Warning:
			{
				writeToLog("WARNING", msg, line, file, fun, "#FFA500");
			}break;
			
			case LOG_Error:
			{
				writeToLog("ERROR", msg, line, file, fun, "#FF0000");
			}break;
			
			case LOG_Other:
			{
				writeToLog("OTHER", msg, line, file, fun, "	#0000FF");
			}break;
			
			default:
			{
				log << "<br> An error appeared while logging! <br>";
				log.flush(); 
			}break;
		}
	}
	
	//Write the message into the log and stores it
	void Log::writeToLog(const string& type, const string& msg, int line, 
				const string& file, const string& fun, const string& color)
	{
		//If there is no message
		string wMsg;
		if(msg == "" || msg == " ")
		{
			wMsg = "-/-";
		}
		else
		{
			wMsg = msg;
		}
		
		//get the current systemtime
		string time = getTime();
		
		//write into the logfile
		log << "<tr>\n";
		log << "<td align=\"center\" width=\"11%\"><b><font color=\"" << color;
		log << "\">" << type << "</font></b></td>\n";
		log << "<td align=\"center\" width=\"30%\"><font color=\"" << color;
		log << "\">" << wMsg << "</font></td>\n";
		log << "<td align=\"center\" width=\"5%\"><font color=\"" << color;
		log << "\">" << line << "</font></td>\n";
		log << "<td align=\"center\" width=\"16%\"><font color=\"" << color;
		log << "\">" << file << "</font></td>\n";
		log << "<td align=\"center\" width=\"28%\"><font color=\"" << color;
		log << "\">" << fun << "()</font></td>\n";
		log << "<td align=\"center\" width=\"10%\"><font color=\"" << color;
		log << "\">" << time << "</font></td>\n";		
		log << "<tr>\n\n";
			
		log.flush();
		
		//store the logmessage
		LogContents temp;
		temp.state = type.c_str();
		temp.message = msg.c_str();
		temp.line = line;
		temp.file = file.c_str();
		temp.function = fun.c_str();
		temp.time = time.c_str();
		logList.push_back(temp);
        
        emit logChanged(&temp);
	}
	
	//writes a topic into the logfile
	void Log::writeTopic(const string& topic)
	{
		//write the topic into the logfile
		log << "</table>\n\n";
		log << "<br><table cellspacing=\"1\" cellpadding=\"1\" ";
		log << "border=\"1\" width=\"100%%\" ";
		log << "bgcolor=\"#DFDFE5\">\n<td>\n<p align=\"center\">\n<font ";
		log << "size=\"+2\" ><b>";
		log << topic;
		log << "</font>\n</td>\n</tr>\n</table>\n";
		log << "<table cellspacing=\"1\" align=\"none\" border=\"1\" ";
		log << "width=\"100%\"cellpadding=\"1\">";
		log.flush();
	}
	
	//get the systemtime
	string Log::getTime()
	{
		//outputstring
		stringstream sTime;
	
		//timestamp in seconds
		time_t Zeitstempel;
		
		//timestamp as date
  		tm *nun;
  		
  		//current time
   		Zeitstempel = time(0);
   		
   		//transform into date
   		nun = localtime(&Zeitstempel);
	
		//write time from date into outputstring
		if(nun->tm_min < 10)
		{
			sTime  << nun->tm_hour << ":0" << nun->tm_min;
		}
		else
		{
		sTime  << nun->tm_hour << ":" << nun->tm_min;
		}
		return sTime.str();
	}
	
	//cancel logging
	void Log::kill()
	{
		_instance->~Log();
		delete _instance;
	}
	
	//retrun the loglist
	vector<LogContents>*  Log::getLogList()
	{
		return &logList;
	}
}
