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
 *  File:       SplitFace.h
 *  Created:    15.05.2007
 *  Author:     Philipp Gruber (Psirus)
 *
 **************************************************************************/

#ifndef SPLITFACE_H_
#define SPLITFACE_H_

#include "SplitFaceInterface.hh"
#include "./../../editor/scenegraph/include/SceneGraph.h"

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class SplitFace, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The face splitter
*
* This plugin class splits all faces into 2 to 6 subfaces
*/
class SplitFace : public SplitFaceInterface
{
	Q_OBJECT

public: 
	/*! \brief The default constructor
	 */
	SplitFace();

	/*! The destructor
	 */
	virtual ~SplitFace();

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
		return "Philipp Gruber";
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
	Version version;		/*!< \brief The version of the plugin*/

	QString m_sName;		/*!< \brief Name of the plugin*/
	QString m_sDescription;	/*!< \brief Description of the plugin*/
	
	QAction* m_pSplitFace;	/*!< \brief Action for the menu item*/

public slots:
	void execute(int nNumSplits);	/*!< \brief Slot for executing this plugin*/
	
	/*!< \brief Slot for splitting into 2 subfaces
	 */
	void execute2Subfaces()
	{
		execute(2);	
	}

	/*!< \brief Slot for splitting into 3 subfaces
	 */
	void execute3Subfaces()
	{
		execute(3);	
	}

	/*!< \brief Slot for splitting into 4 subfaces
	 */	
	void execute4Subfaces()
	{
		execute(4);	
	}
	
	/*!< \brief Slot for splitting into 5 subfaces
	 */	
	void execute5Subfaces()
	{
		execute(5);	
	}

	/*!< \brief Slot for splitting into 6 subfaces
	 */	
	void execute6Subfaces()
	{
		execute(6);	
	}
					
private:
	/** \brief Returns the menu to add the plugin menu
	 *
	 * \param pMenu The pointer where to store the menu pointer
	 */
	void getMenu(QMenu** pMenu);

	/** \brief Splits the face into subfaces
	 *
	 * \param p The polygon which should be split
	 * \param nNumSplits The number of subfaces that should be generated
	 */
	void splitFace(BSScene::Polygon* p, int nNumSplits);

	/** \brief Splits the face into 2 subfaces
	 *
	 * \param v1 The first vertex of the face
	 * \param v2 The second vertex of the face
	 * \param v3 The third vertex of the face
	 * \param indRes The array of indices where to store the resulting face indices
	 * \param vertsRes The array of vertices where to store the resulting face vertices
	 */	
	void splitFaceTo2(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3, vector<int>& indRes, vector<BSRenderer::Vertex>& vertsRes);

	/** \brief Splits the face into 3 subfaces
	 *
	 * \param v1 The first vertex of the face
	 * \param v2 The second vertex of the face
	 * \param v3 The third vertex of the face
	 * \param indRes The array of indices where to store the resulting face indices
	 * \param vertsRes The array of vertices where to store the resulting face vertices
	 */	
	void splitFaceTo3(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3, vector<int>& indRes, vector<BSRenderer::Vertex>& vertsRes);

	/** \brief Splits the face into 4 subfaces
	 *
	 * \param v1 The first vertex of the face
	 * \param v2 The second vertex of the face
	 * \param v3 The third vertex of the face
	 * \param indRes The array of indices where to store the resulting face indices
	 * \param vertsRes The array of vertices where to store the resulting face vertices
	 */	
	void splitFaceTo4(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3, vector<int>& indRes, vector<BSRenderer::Vertex>& vertsRes);

	/** \brief Splits the face into 4 subfaces
	 *
	 * \param v1 The first vertex of the face
	 * \param v2 The second vertex of the face
	 * \param v3 The third vertex of the face
	 * \param indRes The array of indices where to store the resulting face indices
	 * \param vertsRes The array of vertices where to store the resulting face vertices
	 */	
	void splitFaceTo5(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3, vector<int>& indRes, vector<BSRenderer::Vertex>& vertsRes);

	/** \brief Splits the face into 5 subfaces
	 *
	 * \param v1 The first vertex of the face
	 * \param v2 The second vertex of the face
	 * \param v3 The third vertex of the face
	 * \param indRes The array of indices where to store the resulting face indices
	 * \param vertsRes The array of vertices where to store the resulting face vertices
	 */	
	void splitFaceTo6(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3, vector<int>& indRes, vector<BSRenderer::Vertex>& vertsRes);
		
	/** \brief Calculates the average of two vertices
	 *
	 * \param v1 The first vertex
	 * \param v2 The second vertex
	 * \return Vertex The average vertex
	 */			
	BSRenderer::Vertex averageVertex(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2) const;					

	/** \brief Calculates the average of three vertices
	 *
	 * \param v1 The first vertex
	 * \param v2 The second vertex
	 * \param v3 The third vertex
	 * \return Vertex The average vertex
	 */			
	BSRenderer::Vertex averageVertex(const BSRenderer::Vertex& v1, const BSRenderer::Vertex& v2,
		const BSRenderer::Vertex& v3) const;

};

#endif /*SPLITFACE_H_*/
