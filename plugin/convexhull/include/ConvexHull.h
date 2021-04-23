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
 *  File:       ConvexHull.h
 *  Created:    14.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef CONVEXHULL_H_
#define CONVEXHULL_H_

#include "ConvexHullInterface.hh"
#include "./../../renderer/include/RendererTypes.h" //For Vertex
#include <stack>

#ifdef WIN32
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

/*! \file
* 
*  \brief Describes the class ConvexHull, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The convex hull generator
*
* This plugin class generates a convex hull from a number of vertices and adds 
* it to as a new mesh to the scene
*/
class ConvexHull : public ConvexHullInterface
{
	Q_OBJECT

public: 
	/*! \brief The default constructor
	 */
	ConvexHull();

	/*! \brief The destructor
	 */
	virtual ~ConvexHull();

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
	/*!
	* \brief The edge structure
	*/
	struct Edge
	{
		/*!
		* \brief The constructor
		* 
		* \param vS The start position of the edge
		* \param vE The end position of the edge
		*/	
		Edge(Vector& vS, Vector& vE)
		 : vStart(vS), vEnd(vE)
		{
		}
		
		Vector vStart;	/*!< \brief The start position*/
		Vector vEnd;	/*!< \brief The end position*/
		
		/*! 
		* \brief Comparison operator 
		*/		
		bool operator == (const Edge& e) const
		{
			return ((vStart==e.vStart) && (vEnd==e.vEnd));
		}		
	};

	/*! \brief Calculates the convex hull
	 * 
	 * Calculates the convex hull from a number of single vertices and stores the resulting mesh
	 * 
	 * \param points The array of points where to calculate the convex hull from
	 * \param indices The reference to the index-array where to store the result
	 * \param verts The reference to the vertex-array where to store the result
	 */	
	void createConvexHull(const vector<Vector>& points, vector<int>& indices, vector<BSRenderer::Vertex>& verts);
	
	/*! \brief Returns the position with the minimum y-coordinate
	 * 
	 * \param points The array of points
	 * \return Vector The position with the minimum y-coordinate
	 */	
	Vector findBottomPoint(const vector<Vector>& points);
	
	/*! \brief Returns the first point to start the algorithm
	 * 
	 * \param points The array of the points where to calculate the convex hull from
	 * \param vBottom The position of the point with the minimum y-coordinate
	 * \return Vector The position for the first edge
	 */	
	Vector findFirstEdgePoint(const vector<Vector>& points, const Vector& vBottom);
	
	/*! \brief Creates a 3D-Plane
	 * 
	 * \param v1 The first position on the plane
	 * \param v2 The second position of the plane
	 * \param v3 The third position of the plane
	 * \return Plane<double> The plane that contains all three points
	 */	
	Plane<double> createPlane(const Vector& v1, const Vector& v2, const Vector& v3);

	/*! \brief Creates a 2D-Plane (parallel to Y-Axis)
	 * 
	 * \param v1 The first position on the plane
	 * \param v2 The second position of the plane
	 * \return Plane<double> The plane that contains all two points
	 */	
	Plane<double> create2DPlane(const Vector& v1, const Vector& v2);
	
	/*! \brief Returns the next position for creating a new edge of the convex hull
	 * 
	 * \param points The array of the points where to calculate the convex hull from
	 * \param e The last calculated edge
	 * \return Vector The next position
	 */	
	Vector findEdgePoint(const vector<Vector>& points, const Edge& e);
	
	/*! \brief Puts an edge on a stack
	 * 
	 * \param stack The stack
	 * \param e The edge that should be put on the stack
	 */	
	void putOnStack(stack<Edge>& stack, const Edge& e);
	
	/*! \brief Adds a face by avoiding dublicates
	 * 
	 * \param indices The reference to the index-array where to store the result
	 * \param verts The reference to the vertex-array where to store the result
	 * \param v1 The first position of the face
	 * \param v2 The second position of the face
	 * \param v3 The third position of the face
	 * \return Plane<double> The plane that contains all two points
	 */	
	bool addFace(vector<int>& indices, vector<BSRenderer::Vertex>& verts, const Vector& v1,
		const Vector& v2, const Vector& v3);	

private: 
	Version version;		/*!< \brief The version of the plugin*/
	QPushButton* button;	/*!< \brief The button for the toolbox*/

	QString m_sName;		/*!< \brief Name of the plugin*/
	QString m_sDescription;	/*!< \brief Description of the plugin*/

public slots:
	void execute();	/*!< \brief Slot for executing this plugin*/
};

#endif /*CONVEXHULL_H_*/
