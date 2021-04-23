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
 *  File:       MaterialManager.cpp
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#include "../include/TextureManager.h"
#include "../include/MaterialManager.h"

namespace BSRenderer
{
	
MaterialManager* MaterialManager::m_instance = NULL;

MaterialManager* MaterialManager::getInstance()
{
	static MaterialManagerWaechter w;

	if(m_instance == NULL)
	{
		m_instance = new MaterialManager();
	}
	
	return m_instance;
}	

MaterialManager::MaterialManager()
{
    m_matStandard[0].setName("None");
    materialSignalMapper = new QSignalMapper(this);
    connect(materialSignalMapper, SIGNAL(mapped(int)), this, SIGNAL(materialChanged(int)));
}

int MaterialManager::addMaterial(Material* mat, int nID)
{	
	//No special key should be used, so we start the search at key = 1
	//Or its the reserved key area	
	if((nID == -1) || (nID < 0))
	{
		nID = 1;
	}
	
	while(m_materials.find(nID)!=m_materials.end())
	{
		nID++;
	}
    pair< map<int, Material*>::iterator, bool> returnedPair = m_materials.insert(make_pair(nID, mat));
    
    if(returnedPair.second == true)
    {
        connect(mat, SIGNAL(changed()), materialSignalMapper, SLOT(map()));
        materialSignalMapper->setMapping(mat, nID);
        emit materialAdded(nID);
    }
	return nID;
}

void MaterialManager::removeAllMaterials()
{
    while(!m_materials.empty())
    {
        removeMaterial(begin()->first);
    }
}
	
void MaterialManager::removeMaterial(int nID)
{
    Material* materialToDelete = m_materials.find(nID)->second;
    m_materials.erase(nID);	
    delete materialToDelete;
    emit materialRemoved(nID);
}

Material* MaterialManager::getMaterial(int nID)
{
	if(nID <= -2 && nID >= -11)
		return &m_matStandard[-1*nID-2];
	else if(nID <= -100 && nID > (-100-NUMPLUGINMATERALS))
		return &m_matPlugin[-1*nID-100]; 
	
	map<int, Material*>::iterator it = m_materials.find(nID);
	
	if(it != m_materials.end())
	{
		return it->second;
	}
	
	return &m_matStandard[0]; 
}

bool MaterialManager::saveMaterial(int nID, const string& sFileName)
{
	Material* pMat = getMaterial(nID);
	
	//Material not found, so exit
	if(pMat == getStandardMaterial())
	{
		return false;
	}
	
	return pMat->save(sFileName);	
}

int MaterialManager::loadMaterial(int nID, const string& sFileName)
{
	Material* newMat = new Material;
	if(newMat->load(sFileName) == false)
		return getStandardMaterialID();
	
	return addMaterial(newMat, nID);
}

bool MaterialManager::appendMaterialsToFile(const string& sFileName)
{
	ofstream f(sFileName.c_str(), ios::binary | ios::app);
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't append material data to file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}
	
	//Write materials to the file
	UINT nNumMat = getNumMaterials();
	f.write(reinterpret_cast<char*>(&nNumMat), sizeof(UINT));
	MaterialIterator itMat;
	for(itMat = begin(); itMat != end(); itMat++)
	{
		//Write ID of the material
		int nID = itMat->first;
		f.write(reinterpret_cast<char*>(&nID), sizeof(int));
		
		//Append material data
		getMaterial(nID)->appendToFile(f);
	}	
	
	return true;	
}	

bool MaterialManager::loadMaterialsFromFilePos(const string& sFileName, int nPos)
{
	ifstream f(sFileName.c_str(), ios::binary);
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file to read material data: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}	
	
	//Walk to the file position
	f.seekg(nPos);
	
	//Read materials to the file
	UINT nNumMat = getNumMaterials();
	f.read(reinterpret_cast<char*>(&nNumMat), sizeof(UINT));
	for(unsigned int m=0; m<nNumMat; m++)
	{
		//Write ID of the material
		int nID = 0;
		f.read(reinterpret_cast<char*>(&nID), sizeof(int));
		
		//Load material
		Material* newMat = new Material;
		newMat->loadFromFilePos(f, f.tellg());
		
		//Add new material to the material manager
		//int nLoadedID = addMaterial(newMat, nID);
		addMaterial(newMat, nID);
	}		
	
	return true;
}
	
}
