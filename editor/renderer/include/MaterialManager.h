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
 *  Module:     Renderer (BlackSun)
 *  File:       MaterialManager.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include "RendererTypes.h"
#include "Material.h"
#include <QSignalMapper>

/*! \file
* 
*  \brief Describes the class MaterialManager, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

typedef map<int, Material*>::const_iterator MaterialIterator;

const int NUMPLUGINMATERALS = 10;

/*! \brief The manager for the materials 
*
* Loads and manages materials. It combines user defined materials and reserved standard 
* materials.
*/
class MaterialManager : public QObject
{
    
	Q_OBJECT
    
public:
	/*!
	* \brief Returns the instance of the material manager to give access to it
	*
	* \return MaterialManager* The instance of the material manager
	*/
	static MaterialManager* getInstance();
	
	/*!
	* \brief Adds a material
	*
	* \param mat The material which should be added
	* \param nID The ID that should be used. If there is already a material
	*   on this ID, the next free ID will be used.\n
	*   Information: IDs smaller/equal than -1 are reserved for the standard materials
	* \return int The material ID
	*/		
	int addMaterial(Material* mat, int nID = -1);
	
	/*!
	* \brief Creates a new material
	*
	* \param nID The ID that should be used. If there is already a material
	*   on this ID, the next free ID will be used.\n
	*   Information: IDs smaller/equal than -1 are reserved for the standard materials
	* \return int The material ID
	*/	
	int createNewMaterial(int nID = -1)
	{
		return addMaterial(new Material("unnamedMat"), nID);	
	}
	
	/*!
	* \brief Returns a pointer to the material assigned to the ID
	*
	* \param nID The ID if the material
	* \return Material* Returns the pointer, if the ID was found,
	*   otherwise a pointer to the standard material will be returned.
	*/		
	Material* getMaterial(int nID);
	
	//Reserved materials can't be deleted
	/*!
	* \brief Removes a material
	*
	* \param nID The ID of the material that should be removed.
	*   Reserved materials (ID <= -1) can't be removed!
	*/	
	void removeMaterial(int nID);
	
	/*!
	* \brief Removes all materials
	*
	* Removes all materials and frees memory
	*/			
	void removeAllMaterials();
	
	/*!
	* \brief Returns the number of materials
	*
	* \return UINT The number of materials
	*/
	UINT getNumMaterials() const
	{
		return m_materials.size();
	}

	/*!
	* \brief Save the material in a file
	*
	* \param nID The ID of the material which should be saved
	* \param sFileName The file name
	* \return bool True, if the material was saved correctly, otherwise false
	*/	
	bool saveMaterial(int nID, const string& sFileName);

	/*!
	* \brief Load the material from a file
	*
	* \param nID The ID that should be used. If there is already a material
	*   on this ID, the next free ID will be used.\n
	*   Information: IDs smaller/equal than -1 are reserved for the standard materials
	* \param sFileName The file name
	* \return bool True, if the material was saved correctly, otherwise false
	*/	
	int loadMaterial(int nID, const string& sFileName);
	
	/*!
	* \brief Saves all materials by appending the data to a file
	* 
	* Saves all available materials with texture informations by appending
	* the data to a specific file
	*
	* \param sFileName The file name
	* \return bool True, if the materials were saved correctly, otherwise false
	*/		
	bool appendMaterialsToFile(const string& sFileName);
	
	/*!
	* \brief Load material from a file and a specific file position
	*
	* Loads all available materials with texture informations from a
	* specific file position
	* 
	* \param sFileName The file name
	* \param nPos The file position in bytes, where the material data is stored
	* 	in the file
	* \return bool True, if the materials were loaded correctly, otherwise false
	*/		
	bool loadMaterialsFromFilePos(const string& sFileName, int nPos);

	/*!
	* \brief Returns pointer to a materials, reserved for plugins
	*
	* \param nNr Number of the reserved material
	* \return Material* Pointer to the material
	*/
	Material* getPluginMaterial(int nNr)
	{
		if(nNr > 0 && nNr<(NUMPLUGINMATERALS)) 
			return &m_matPlugin[nNr];
		
		return &m_matPlugin[0];	
	}

	/*!
	* \brief Returns ID of the plugin reserved materials
	*
	* \param nNr Number of the reserved material
	* \return int ID of the plugin reserved material
	*/
	int getPluginMaterialID(int nNr) const
	{
		return -100 - nNr;
	}
	
	/*!
	* \brief Returns pointer to the standard material
	*
	* \return Material* Pointer to the standard material
	*/
	Material* getStandardMaterial()
	{
		return &m_matStandard[0];	
	}
	
	/*!
	* \brief Returns ID of the standard material
	*
	* \return int ID of the standard material
	*/
	int getStandardMaterialID() const
	{
		return -2;
	}

	/*!
	* \brief Returns pointer to the wireframe material
	*
	* \return Material* Pointer to the wireframe material
	*/	
	Material* getWireframeMaterial()
	{
		return &m_matStandard[1];	
	}

	/*!
	* \brief Returns ID of the wireframe material
	*
	* \return int ID of the wireframe material
	*/		
	int getWireframeMaterialID() const
	{
		return -3;	
	}
	
	/*!
	* \brief Returns pointer to the wireframe-selection material
	*
	* \return Material* Pointer to the wireframe-selection material
	*/
	Material* getWireframeSelectionMaterial()
	{
		return &m_matStandard[2];	
	}

	/*!
	* \brief Returns ID of the wireframe-selection material
	*
	* \return int ID of the wireframe-selection material
	*/	
	int getWireframeSelectionMaterialID() const
	{
		return -4;	
	}
	
	/*!
	* \brief Returns pointer to the point material
	*
	* \return Material* Pointer to the point material
	*/
	Material* getPointMaterial()
	{
		return &m_matStandard[3];	
	}

	/*!
	* \brief Returns ID of the point material
	*
	* \return int ID of the point material
	*/		
	int getPointMaterialID() const
	{
		return -5;	
	}
	
	/*!
	* \brief Returns pointer to the point-selection material
	*
	* \return Material* Pointer to the point-selection material
	*/
	Material* getPointSelectionMaterial()
	{
		return &m_matStandard[4];	
	}

	/*!
	* \brief Returns ID of the point-selection material
	*
	* \return int ID of the point-selection material
	*/		
	int getPointSelectionMaterialID() const
	{
		return -6;	
	}
	
	/*!
	* \brief Returns pointer to the freeze material
	*
	* \return Material* Pointer to the freeze material
	*/	
	Material* getFreezeMaterial()
	{
		return &m_matStandard[5];	
	}

	/*!
	* \brief Returns ID of the freeze material
	*
	* \return int ID of the freeze material
	*/		
	int getFreezeMaterialID() const
	{
		return -7;	
	}
	
	/*!
	* \brief Returns pointer to the normal material
	*
	* \return Material* Pointer to the normal material
	*/
	Material* getNormalMaterial()
	{
		return &m_matStandard[6];	
	}

	/*!
	* \brief Returns ID of the normal material
	*
	* \return int ID of the normal material
	*/		
	int getNormalMaterialID() const
	{
		return -8;	
	}
	
	/*!
	* \brief Returns pointer to the selection-AABB material
	*
	* \return Material* Pointer to the selection-AABB material
	*/
	Material* getSelectionAABBMaterial()
	{
		return &m_matStandard[7];	
	}

	/*!
	* \brief Returns ID of the selection-AABB material
	*
	* \return int ID of the selection-AABB material
	*/		
	int getSelectionAABBMaterialID() const
	{
		return -9;	
	}	
	
	/*!
	* \brief Returns pointer to the major-grid material
	*
	* \return Material* Pointer to the major-grid material
	*/
	Material* getGridMajorLineMaterial()
	{
		return &m_matStandard[8];	
	}
	
	/*!
	* \brief Returns ID of the major-grid material
	*
	* \return int ID of the major-grid material
	*/	
	int getGridMajorLineMaterialID() const
	{
		return -10;	
	}			
	
	/*!
	* \brief Returns pointer to the minor-grid material
	*
	* \return Material* Pointer to the minor-grid material
	*/
	Material* getGridMinorLineMaterial()
	{
		return &m_matStandard[9];	
	}

	/*!
	* \brief Returns ID of the minor-grid material
	*
	* \return int ID of the minor-grid material
	*/		
	int getGridMinorLineMaterialID() const
	{
		return -11;	
	}

	/*!
	* \brief Returns an iterator to the first material
	*
	* \return MaterialInterator The iterator
	*/			
	MaterialIterator begin()
	{
		return m_materials.begin();	
	}

	/*!
	* \brief Returns an iterator to the last material
	*
	* \return MaterialInterator The iterator
	*/				
	MaterialIterator end()
	{
		return m_materials.end();	
	}
	
	
private:
	/*!
	* \brief The standard constructor
	*/
	MaterialManager();
	
	/*!
	* \brief The destructor
	*/	
	virtual ~MaterialManager()
	{
	}
	
	/*! \brief The guard for the material manager singleton 
	*
	* The guard for the material manager singleton arrange that the destructor
	* of the singleton will be called
	*/	
	class MaterialManagerWaechter 
	{
	public:
		/*!
		* \brief The destructor
		*/
		virtual ~MaterialManagerWaechter() 
		{
			if(MaterialManager::m_instance != NULL)
				delete MaterialManager::m_instance;
		}
	};
	friend class MaterialManagerWaechter;

private:
	map<int, Material*> m_materials; /*!< \brief Map with all materials and their IDs */
	
	/* 0 = ID( -2) = Standard
	   1 = ID( -3) = Wireframe
	   2 = ID( -4) = WireframeSelection
	   3 = ID( -5) = Point
	   4 = ID( -6) = PointSelection
	   5 = ID( -7) = Freeze
	   6 = ID( -8) = Normal
	   7 = ID( -9) = SelectionAABB;
	   8 = ID(-10) = MajorGridLine
	   9 = ID(-11) = MinorGridLine;*/	
	Material m_matStandard[10];	 /*!< \brief Array with all standard materials (reserved) */
	
	Material m_matPlugin[NUMPLUGINMATERALS]; /*!< \brief Array with materials, that can be used for plugins */
	
	static MaterialManager* m_instance; /*!< \brief The instance of the texture manager singleton*/
                                        
    QSignalMapper* materialSignalMapper; /*!< \brief Signal mapper*/
                                        
                                        
signals:
    //This is emitted when a material is added the parameter is the material ID
	/*!
	* \brief Signal for adding a material
	*/		    
    void materialAdded(int);

    //This is emitted when a material is removed the parameter is the material ID
	/*!
	* \brief Signal for removing a material
	*/		    
    void materialRemoved(int);
    
    //This signal is emittet when the material is changed the parameter is the id of the material
	/*!
	* \brief Signal for changing a value
	*/		        
    void materialChanged(int);
};
	
}

#endif /*MATERIALMANAGER_H_*/
