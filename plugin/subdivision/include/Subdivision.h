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
 *  File:       Subdivision.h
 *  Created:    05.05.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef SUBDIVISION_H_
#define SUBDIVISION_H_

#include "SubdivisionInterface.hh"
#include <SceneGraph.h>
#include <set>

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class Subdivision, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/

const int NUMBETATABLE = 200; /*!< \brief Number of elements in beta table*/


/*! \brief The subdivision plugin
*
* This plugin class subdivides all selected meshes to make them smoother
*/
class Subdivision : public SubdivisionInterface
{
	Q_OBJECT

public: 
	/*! \brief The default constructor
	 */
	Subdivision();

	/*! \brief The destructor
	 */
	virtual ~Subdivision();

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
	/** \brief Returns the menu to add the plugin menu
	 *
	 * \param pMenu The pointer where to store the menu pointer
	 */
	void getMenu(QMenu** pMenu);

private: 
	Version version;				/*!< \brief The version of the plugin*/
	QAction* m_pActionSubdivide;	/*!< \brief Action for the menu item*/

	QString m_sName;		/*!< \brief Name of the plugin*/
	QString m_sDescription;	/*!< \brief Description of the plugin*/
	
	double m_betaTable[NUMBETATABLE]; 	/*!< \brief Precalculated table for beta*/
	
	QProgressDialog* m_progressDialog; 	/*!< \brief The progress dialog*/

public slots:
	void execute();	/*!< \brief Slot for executing this plugin*/
	
private:
	/*! \brief Subdivides a polygon
	 * 
	 * Subdivides a polygon by applying the catmull clark algorithm
	 * 
	 * \param p The polygon that should be manipulated
	 * \param vertsOriginal The vertices of the original mesh
	 * \param indicesOriginal The indices of the original mesh
	 */
	void subdividePolygon(BSScene::Polygon* p, const vector<BSRenderer::Vertex>& vertsOriginal,
		const vector<int> indicesOriginal);

	/*! \brief Returns neighbour vertex positions from a specific vertex position
	 * 
	 * Stores all neighbour vertex position of a specic position in an array without
	 * dublicates
	 * 
	 * \param vPos Position of the vertex
	 * \param posNeighbours The array where to store the found neighbour positions 
	 *     (without dublicates)
	 * \param vertsOriginal The vertices of the original mesh
	 * \param indicesOriginal The indices of the original mesh
	 */	
	void getNeighbours(Vector vPos, vector<Vector>& posNeighbours, const vector<BSRenderer::Vertex>& vertsOriginal,
		const vector<int> indicesOriginal);

	/*! \brief Checks wheater two vectors are nearly equal (with epsilon)
	 * 
	 * \param v1 The first vector
	 * \param v2 The second vector
	 * \param dEpsilon Epsilon, the maximum absoulte difference between each component
	 * \return bool Returns true, if the two vectors are nearly equal, otherwise false
	 */			
	bool isVectorNearTo(const Vector& v1, const Vector& v2, double dEpsilon) const;
	
	/*! \brief Calculates the vector sum
	 * 
	 * \param v The array with all vectors
	 * \param vRelationPos The relation point
	 * \return Vector The sum of all vectors
	 */				
	Vector calcVectorSum(const vector<Vector>& v, const Vector vRelationPos) const;
	
	/*! \brief Adds a vector to a array by avoiding dublicates
	 * 
	 * \param v The array where to store the vector
	 * \param vec The array that should be added
	 */	
	void addVector(vector<Vector>& v, const Vector& vec) const;

	/*! \brief Calculates beta
	 * 
	 * \param n Value for the forumula parameter 'n'
	 * \param bIgnoreTable If true, the value is calculated again, without using the
	 *     beta table.
	 * \return double The value for beta
	 */		
	double calcBeta(int n, bool bIgnoreTable = false) const;
	
	/*! \brief Calculates alpha
	 * 
	 * \param n Value for the forumula parameter 'n'
	 * \param dBeta Value for the formula parameter 'beta'
	 * \return double The value for alpha
	 */	
	double calcAlpha(int n, double dBeta) const;	
	
};

#endif /*SUBDIVISION_H_*/
