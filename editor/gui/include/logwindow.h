/***************************************************************************
*   Copyright (C) 2006 by Thomas Tischler
*   ttischl1@fh-landshut.de
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
****************************************************************************
*
*  Module:     GUI (BlackSun)
*  File:       logwindow.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

        
/** \file logwindow.h
 * \brief Defines LogWindow class.
 * 
 * \author Thomas Tischler.
 */
#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QtGui>
#include "./../ui_logwindow.h"
#include "./../../core/include/Log.h"
        
namespace BSGui
{
/*! \brief A widget to display log entrys.
* This widget allows to view the log entrys.
*/
class LogWindow : public QWidget
{
    Q_OBJECT
            
    public:
        /*!
        * \brief The Constructor of the Widget
        * 
        * \param parent The Widgets parent
        */
        LogWindow(QWidget* parent = 0);
        
    public slots:
        /*!
        * \brief This slot is called when a new entry in the log has been made.
        * \param logEntry The new log entry.
        */
        void logChanged(BSCore::LogContents* logEntry);
        /*!
         * \brief This slot is called when a new entry in the log has been made.
         * \param min The minmum slider value.
         *  \param max The maximum slider value.
        */
        void setSliderToMaximum(int min, int max);
        
    protected:
        /*!
        * \brief Handels the close event.
        * \param event The new log entry.
        */
        virtual void closeEvent(QCloseEvent* event);
        
    private:
        
        Ui_logWidget ui; /*!< \brief The ui class */
        QScrollBar* scrollBar; /*!< \brief The scroll bar */
        
    signals:
    /*!
     * \brief This signal is emitted when the window is closed.
    */
        void windowClosed();
};

}

#endif
