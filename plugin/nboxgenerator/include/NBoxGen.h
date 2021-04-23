/***************************************************************************
 *   Copyright (C) 2006-07 by Philipp Gruber
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
 *  Module:     Plugin (BlackSun)
 *  File:       NBoxGen.h
 *  Created:    04.03.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/
#ifndef NBOXGEN_H_
#define NBOXGEN_H_

#include "NBoxGenInterface.hh"
#include "./../ui_settings.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class NBoxGen, their methodes and attributes.
*  \author Philipp Gruber
* 
*/

/*! \brief The nBox generator
*
* This plugin class generates a box with n Corners and adds it to the scene
*/
class NBoxGen : public NBoxGenInterface
{
	Q_OBJECT
private:
	QPushButton* createnBoxButton;	/*!< \brief The button for the toolbox*/
	Version nBoxGenVersion;			/*!< \brief The version of the plugin*/
	bool 	sharedVertices;			/*!< \brief If the Vertices shoud be shared*/
	bool 	centered;				/*!< \brief If the Top and Bottom part should be claculated from the center*/
	int 	corners;				/*!< \brief The number of corners*/
	int		parts;					/*!< \brief The numer of parts of the height*/
	double	radius;					/*!< \brief The radian*/
	double	height;					/*!< \brief The height*/
    
    Ui_settings ui;					/*!< \brief The widget for the properties*/
	
public:
	/*! \brief The default constructor
     */
	NBoxGen();
	
	/*! \brief The destructor
     */	
	virtual ~NBoxGen();
	
	// Functions, that implements from PlgInt:
    
    /*! \brief Returns the UID.
     * 
     * \return the UID of this plugin.
     */
    UID getUID();
    
    /*! \brief Returns the name.
     * 
     * \return the name of the plugin.
     */
    QString getName();
    
    /*! \brief Returns the autor's name.
     * 
     * \return the name of the autor.
     */
    QString getAutor();
    
    /*! \brief Returns the description.
     * 
     * Hyperlinks (http://... or mailto:...) will be translated into 'active'
     * hyperlinks, so the user can click it.
     * 
     * \return the description of the plugin.
     * */
    QString getDescription();
    
    /*! \brief Returns the version.
     * 
     * \return the version of the plugin.
     */
    Version getVersion();
    
    /*! \brief The init - function.
     * 
     * This is the first function, that the pluginsystem call. In these
     * function it's necessary for the plugin to communication with the facade.
     * goes wrong in the initialisation, the function returns false. If these
     * happen, the plugin will not load.
     * 
     * \return say, if an error is occurred or not.
     */
    bool loadPlg();
    
    /*! \brief The install - function.
     * 
     * This is the first function, that the pluginsystem call.
     * 
     * \return Returns true, if plugin was installed correctly, otherwise false.
     */
    bool installPlg() {return true;}
    
    /*! \brief The uninstall - function.
     * 
     * This function is called, if the Plugin is uninstalled by the user. It
     * can do here things (e.g. remove the plugin control in the gui), before
     * the Konstruktor is called.
     * 
     * \return Returns true, if plugin was uninstalled correctly, otherwise false.
     */
    bool uninstallPlg() {return true;}
    
    /*! \brief The unload - function.
     * 
     * This function is called, if the Plugin is unloaded by the user. Here
     * should the Plugin remove all it's control elements (e.g. from the GUI).
     * 
     * \return Returns true, if plugin was unloaded correctly, otherwise false.
     */
    bool unloadPlg();
    
    /*! \brief The close - function.
     * 
     * This function is called, if the application is finished from the user.
     * The Plugin should do their clear jobs (e.g. let of memory)
     */
    void closePlg() {}
    
    public slots:
    	void createBox();	/*!< \brief Slot for executing this plugin*/
};

#endif /*NBOXGENERATOR_H_*/
