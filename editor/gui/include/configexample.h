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
*  File:       configexample.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/

#ifndef CONFIGEXAMPLE_H
#define CONFIGEXAMPLE_H

#include "configinterface.h"
#include "../ui_testWidget.h"
        
class ConfigExample : public ConfigInterface
{
public:
    ConfigExample();
    virtual ~ConfigExample();

    QString getDescription();
    QWidget* getUi();
    QString getShortName();
    QIcon getIcon();
    
public slots:
    void dialogAccept() {}
     
private:
    QWidget* myWidget;

};

#endif
