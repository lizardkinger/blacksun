/***************************************************************************
 *   Copyright (C) 2006-07 by Reinhard Jeschull
 *   rjeschu@fh-landshut.de
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
 *  File:       GridGen.h
 *  Created:    02.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef GRIDGEN_H_
#define GRIDGEN_H_

#include "GridGenInterface.hh"
#include "./../../renderer/include/RendererTypes.h" //For Vertex
#include "./../ui_GridSettings.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class GridGen, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The grid generator
*
* This plugin class generates a grid and adds it to the scene
*/
class GridGen : public GridGenInterface
{
	Q_OBJECT

public: 
    /*! \brief The default constructor
     */
	GridGen();
	
    /*! \brief The destructor
     */		
	virtual ~GridGen();

	/** \brief Returns the UID.
	 *
	 * \return The UID of this plugin.
	 */
	UID getUID()
	{
		return uid;
	}

	/** \brief Returns the name.
	 *
	 * \return The name of the plugin.
	 */
	QString getName()
	{
		return m_sName;
	}

	/** \brief Returns the author's name.
	 *
	 * \return The name of the author.
	 */
	QString getAutor()
	{
		return "Reinhard Jeschull";
	}

	/** \brief Returns the description.
	 *
	 * Hyperlinks (http://... or mailto:...) will be translated into 'active'
	 * hyperlinks, so the user can click it
	 *
	 * \return The description of the plugin.
	 */
	QString getDescription()
	{
		return m_sDescription;
	}

	/** \brief Returns the version.
	 *
	 * \return The version of the plugin.
	 */
	Version getVersion()
	{
		return version;
	}

	/** \brief The init-methode.
	 *
	 * This is the first function, that the pluginsystem call. In these
	 * function it's necessary for the plugin to communication with the facade.
	 * goes wrong in the initialisation, the function returns false. If these
	 * happen, the plugin will not load.
	 *
	 * \return Returns true, if plugin was intizialized correctly, otherwise false.
	 */
	bool loadPlg();

   /*! \brief The install - function.
     * 
     * This is the first function, that the pluginsystem call.
     * 
     * \return Returns true, if plugin was installed correctly, otherwise false.
     */
	bool installPlg()
	{
		return true;
	}

    /*! \brief The uninstall - function.
     * 
     * This function is called, if the Plugin is uninstalled by the user. It
     * can do here things (e.g. remove the plugin control in the gui), before
     * the Konstruktor is called.
     * 
     * \return Returns true, if plugin was uninstalled correctly, otherwise false.
     */
	bool uninstallPlg()
	{
		return true;
	}

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
	void closePlg()
	{
	}
	
private:
	/*! \brief Creates a grid
	*
	* \param indices Reference to an array, where the indices should be stored
	* \param verts Reference to an array, where the vertices should be stored
	* \param dWidth Width of the grid (on X-Axis)
	* \param dHeight Height of the grid (on Y-Axis)
	* \param nSegmentsWidth Number of segments on X-Axis
	* \param nSegmentsHeight Number of segments on Z-Axis
	*/	
	void createGrid(vector<int>& indices, vector<BSRenderer::Vertex>& verts,
		double dWidth, double dHeight,
		int nSegmentsWidth, int nSegmentsHeight);
		
private: 
	Version version; 		/*!< \brief The version of the plugin*/
	QPushButton* button; 	/*!< \brief The button for the toolbox*/

	QString m_sName; 		/*!< \brief Name of the plugin*/
	QString m_sDescription; /*!< \brief Description of the plugin*/

	Ui_GridSettings m_ui; /*!< \brief The widget for the properties*/

public slots:
	void execute(); /*!< \brief Slot for executing this plugin*/
};

#endif /*GRIDGEN_H_*/
