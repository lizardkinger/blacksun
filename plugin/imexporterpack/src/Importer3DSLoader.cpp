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
 *  File:       Importer3DSLoader.cpp
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/Importer3DSLoader.h"

using namespace BSRenderer;

bool Importer3DSLoader::execute(const string& sFileName)
{
	if(sFileName == "")
		return false;	
	
	ifstream f(sFileName.c_str(), ios::binary);
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}	
	
	m_mesh.clear();
	m_materials.clear();
	
	//Read the primary Chunk
	A3DS_CHUNK chunkPrimary;
	readChunk(f, chunkPrimary);
	
	//Is the file a 3DS?
	if(chunkPrimary.ID != A3DS_PRIMARY_ID)
	{
		stringstream msg;
		msg << "File isn't a correct 3DS-file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}	
	
	//Read all chunks
	readSubChunk(f, chunkPrimary);
	
	f.close();
	
	cout << "NumMeshes: " << m_mesh.size() << endl;
	cout << "NumMaterials: " << m_materials.size() << endl;
	
	return true;
}

void Importer3DSLoader::release()
{
	for(unsigned int i=0; i<m_mesh.size(); i++)
	{
		//Delete faces
		if(m_mesh[i].pFaces != NULL)
		{
			delete[] m_mesh[i].pFaces;
			m_mesh[i].pFaces = NULL;	
		}
		
		//Delete vertices
		if(m_mesh[i].pVertices != NULL)
		{
			delete[] m_mesh[i].pVertices;
			m_mesh[i].pVertices = NULL;	
		}		
		
		//Delete texture coordinates
		if(m_mesh[i].pTexCoords != NULL)
		{
			delete[] m_mesh[i].pTexCoords;
			m_mesh[i].pTexCoords = NULL;	
		}	
		
		m_mesh[i].nTotalFaces = 0;
		m_mesh[i].nTotalVertices = 0;
		m_mesh[i].nTotalTexCoords = 0;
	}

	m_mesh.clear();
	m_materials.clear();	
}

void Importer3DSLoader::readSubChunk(ifstream& f, A3DS_CHUNK& chunk)
{

	//Read until the end of the chunk is reached
	while(chunk.nBytesRead+1 < chunk.nLength)
	{
		//Read next chunk
		A3DS_CHUNK chunkCur;
		readChunk(f, chunkCur);
		
		switch(chunkCur.ID)
		{
			case A3DS_EDIT3DS:
			{
				//Read the next chunk
				readSubChunk(f, chunkCur);
			}break;
			case A3DS_NAMED_OBJECT:
			{
				//Add a new mesh to the list
				m_mesh.push_back(A3DS_MESH());
				
				string sName = getNextString(f);
				chunkCur.nBytesRead += sName.length();
				
				m_mesh[m_mesh.size()-1].sName = sName;
				
				//Read the next chunk
				readSubChunk(f, chunkCur);					
			}break;
			case A3DS_OBJ_MESH:
			{
				//Read the next chunk
				readSubChunk(f, chunkCur);				
			}break;
			case A3DS_MAT:
			{
				//Add a new material to the list
				m_materials.push_back(A3DS_MATERIAL());
				
				//Read the next chunk
				readSubChunk(f, chunkCur);
			}break;
			case A3DS_MAT_NAME:
			{
				  //Read name and save it
				  string sName = getNextString(f);
				  chunkCur.nBytesRead += sName.length();
				  m_materials[m_materials.size()-1].sName = sName;
			}break;
			case A3DS_MAT_TEXFILENAME:
			{
				  //Read texture file name and save it
				  string sName = getNextString(f);
				  chunkCur.nBytesRead += sName.length();
				  m_materials[m_materials.size()-1].sTextureName = sName;
			}break;
			case A3DS_MAT_DIFFUSE:
			{
				//Read diffuse color
				m_materials[m_materials.size()-1].cDiffuse = readColor(f, chunkCur);		
			}break;
			case A3DS_MAT_AMBIENT:
			{
				//Read ambient color
				m_materials[m_materials.size()-1].cAmbient = readColor(f, chunkCur);						
			}break;
			case A3DS_MAT_SPECULAR:
			{
				//Read specular color
				m_materials[m_materials.size()-1].cSpecular = readColor(f, chunkCur);			
			}break;
			case A3DS_MAT_EMISSIVE:
			{
				//Read emissive color
				m_materials[m_materials.size()-1].cEmissive = readColor(f, chunkCur);			
			}break;
			case A3DS_MAT_SHININESS:
			{
				//Read shininess factor
				readShininess(f, chunkCur);				
			}break;
			case A3DS_MAT_SPECULARPOWER:
			{
				//Read specular power
				readSpecularPower(f, chunkCur);
			}break;
			case A3DS_MAT_TRANSPERENCY:
			{
				//Read transperency
				readTransperency(f, chunkCur);
			}break;
			case A3DS_MAT_TEXMAP:
			{
				//Read the next chunk
				readSubChunk(f, chunkCur);					
			}break;
			case A3DS_MESH_FACES:
			{
				//Read triangles
				readFaces(f, chunkCur);					
			}break;
			case A3DS_MESH_VERTICES:
			{
				//Read vertices
				readVertices(f, chunkCur);				
			}break;
			case A3DS_MESH_TEX_VERT:
			{
				//Read texture coordinates
				readTexCoords(f, chunkCur);				
			}break;
			case A3DS_MESH_MATER:
			{
				//Read material indices
				readMeshMaterials(f, chunkCur);				
			}break;
			case A3DS_MESH_SMOOTH_GROUP:
			{
				//Read smoothing groups
				readSmoothingGroups(f, chunkCur);	
			}break;
			case A3DS_MESH_XFMATRIX:
			{
				cout << "TODO: 3DS-ReadMatrix" << endl;
				moveToNextChunk(f, chunkCur);
			}break;
			default:
			{
				//Skip unsupported chunks
				moveToNextChunk(f, chunkCur);
			}break;				
		}
		
		//Add the loaded bytes
		chunk.nBytesRead += chunkCur.nLength;
	}
}

void Importer3DSLoader::moveToNextChunk(ifstream& f, A3DS_CHUNK& chunk)
{
	//Big buffer
	int buff[50000];
   
   //Read the data to skip the chunk
   f.read(reinterpret_cast<char*>(buff), chunk.nLength - chunk.nBytesRead);
}

string Importer3DSLoader::getNextString(ifstream& f)
{
	char buffer[100] = { 0 };
	int nIndex = 0;

	// Read first character
	f.read(reinterpret_cast<char*>(buffer), sizeof(char));
      
	//Read the other characters, until '\0' is reached
	while(buffer[nIndex] != 0)
	{
		nIndex++;
		f.read(reinterpret_cast<char*>(&buffer[nIndex]), sizeof(char));
	}
	
	string sString = string(buffer);
	
	return sString;
}

void Importer3DSLoader::readChunk(ifstream& f, A3DS_CHUNK& chunk)
{
	
	f.read(reinterpret_cast<char*>(&chunk.ID), sizeof(chunk.ID));
	f.read(reinterpret_cast<char*>(&chunk.nLength), sizeof(chunk.nLength));
	chunk.nBytesRead = sizeof(chunk.ID) + sizeof(chunk.nLength);
}

A3DS_COLOR Importer3DSLoader::readColor(ifstream& f, A3DS_CHUNK& chunk)
{
	A3DS_COLOR color;
	char header[6];

	//Read color value.
	f.read(reinterpret_cast<char*>(header), sizeof(char)*6);
	f.read(reinterpret_cast<char*>(&color), sizeof(color));
	
	chunk.nBytesRead += sizeof(char)*6 + sizeof(color);

	// Skip the rest of the chunk.
	moveToNextChunk(f, chunk);
	
	return color;
}

void Importer3DSLoader::readShininess(ifstream& f, A3DS_CHUNK& chunk)
{
	//Read next chunk
	A3DS_CHUNK chunkCur;
	readChunk(f, chunkCur);
	
	//An integer is stored
	if(chunkCur.ID == A3DS_INT_PERCENTAGE)
	{
		unsigned int nPercentage = 0;
		f.read(reinterpret_cast<char*>(&nPercentage), 2);
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + 2;
		
		m_materials[m_materials.size()-1].fShininess = nPercentage * 0.01;
	}
	//A float is stored
	else
	{
		f.read(reinterpret_cast<char*>(&m_materials[m_materials.size()-1].fShininess), sizeof(float));
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + sizeof(float);
	}	
}

void Importer3DSLoader::readSpecularPower(ifstream& f, A3DS_CHUNK& chunk)
{
	//Read next chunk
	A3DS_CHUNK chunkCur;
	readChunk(f, chunkCur);
	
	//An integer is stored
	if(chunkCur.ID == A3DS_INT_PERCENTAGE)
	{
		unsigned int nPercentage = 0;
		f.read(reinterpret_cast<char*>(&nPercentage), 2);
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + 2;
		
		m_materials[m_materials.size()-1].fSpecularPower = nPercentage * 0.01;
	}
	//A float is stored
	else
	{
		f.read(reinterpret_cast<char*>(&m_materials[m_materials.size()-1].fSpecularPower), sizeof(float));
		
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + sizeof(float);
	}
	
}

void Importer3DSLoader::readTransperency(ifstream& f, A3DS_CHUNK& chunk)
{
	//Read next chunk
	A3DS_CHUNK chunkCur;
	readChunk(f, chunkCur);
	
	//An integer is stored
	if(chunkCur.ID == A3DS_INT_PERCENTAGE)
	{
		unsigned int nPercentage = 0;
		f.read(reinterpret_cast<char*>(&nPercentage), 2);
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + 2;
		
		m_materials[m_materials.size()-1].fTransparency = nPercentage * 0.01;
	}
	//A float is stored
	else
	{
		f.read(reinterpret_cast<char*>(&m_materials[m_materials.size()-1].fTransparency), sizeof(float));
		
		chunk.nBytesRead += sizeof(A3DS_CHUNK) + sizeof(float);
	}	
}

void Importer3DSLoader::readMeshMaterials(ifstream& f, A3DS_CHUNK& chunk)
{
	string sName = "";
	int nMatID = 0;
	unsigned int nNumMatIndices = 0;
	unsigned short *pMatIndices = NULL;
	
	//Read materal name and number of material indices
	sName = getNextString(f);
	f.read(reinterpret_cast<char*>(&nNumMatIndices), 2);
	chunk.nBytesRead += sName.length() + 2;
	
	pMatIndices = new unsigned short[nNumMatIndices];
	
	//Read indices
	f.read(reinterpret_cast<char*>(pMatIndices), nNumMatIndices * sizeof(unsigned short));
	chunk.nBytesRead += nNumMatIndices * sizeof(unsigned short);
	
	//Try to find the material
	for(unsigned int i=0; i<m_materials.size(); i++)
	{
		//Material found
		if(sName == m_materials[i].sName)
		{
			nMatID = i;	
		}	
	}

	//Add material id to the faces
	for(unsigned int i=0; i<nNumMatIndices; i++)
	{
		m_mesh[m_mesh.size()-1].pFaces[pMatIndices[i]].nMatID = nMatID;
	}
}

void Importer3DSLoader::readFaces(ifstream& f, A3DS_CHUNK& chunk)
{
	unsigned int nNumFaces = 0;
	A3DS_FILEFACE *pFaces;
		
	//Read number of vertices
	f.read(reinterpret_cast<char*>(&nNumFaces), 2);
	chunk.nBytesRead += 2;	
	
	//Read faces
	pFaces = new A3DS_FILEFACE[nNumFaces];
	f.read(reinterpret_cast<char*>(pFaces), nNumFaces * sizeof(A3DS_FILEFACE));
	chunk.nBytesRead += nNumFaces * sizeof(A3DS_FILEFACE);
		
	//Copy face data to the mesh
	m_mesh[m_mesh.size()-1].pFaces = new A3DS_FACE[nNumFaces];
	m_mesh[m_mesh.size()-1].nTotalFaces = nNumFaces;
	
	for(unsigned int i=0; i<nNumFaces; i++)
	{
		m_mesh[m_mesh.size()-1].pFaces[i].nIndices[0] = pFaces[i].nIndices[0];
		m_mesh[m_mesh.size()-1].pFaces[i].nIndices[1] = pFaces[i].nIndices[1];
		m_mesh[m_mesh.size()-1].pFaces[i].nIndices[2] = pFaces[i].nIndices[2];		
	}
	
	//Delete unused memory
	delete[] pFaces;
	
	//Read next chunk
	readSubChunk(f, chunk);
}

void Importer3DSLoader::readVertices(ifstream& f, A3DS_CHUNK& chunk)
{
	unsigned int nNumVerts = 0;
	A3DS_VERTEX *pVerts;
		
	//Read number of vertices
	f.read(reinterpret_cast<char*>(&nNumVerts), 2);
	chunk.nBytesRead += 2;
	
	//Read vertices
	pVerts = new A3DS_VERTEX[nNumVerts];
	f.read(reinterpret_cast<char*>(pVerts), nNumVerts * sizeof(A3DS_VERTEX));
	chunk.nBytesRead += nNumVerts * sizeof(A3DS_VERTEX);
		
	//Save the data to the mesh
	m_mesh[m_mesh.size()-1].pVertices = pVerts;
	m_mesh[m_mesh.size()-1].nTotalVertices = nNumVerts;
	
	// Skip the rest of the chunk.
	moveToNextChunk(f, chunk);
}

void Importer3DSLoader::readSmoothingGroups(ifstream& f, A3DS_CHUNK& chunk)
{
	int nNumFaces = m_mesh[m_mesh.size()-1].nTotalFaces;
	A3DS_FACE* pFaces = m_mesh[m_mesh.size()-1].pFaces;
	
	for(int i=0; i<nNumFaces; i++)
	{
		f.read(reinterpret_cast<char*>(&(pFaces[i].nSmoothingGroup)), sizeof(unsigned long));
	}
	
	chunk.nBytesRead += nNumFaces * sizeof(unsigned long);
}

void Importer3DSLoader::readTexCoords(ifstream& f, A3DS_CHUNK& chunk)
{
	
	unsigned int nNumTexCoords = 0;
	A3DS_TEXCOORD *pTexCoords;
		
	//Read number of vertices
	f.read(reinterpret_cast<char*>(&nNumTexCoords), 2);
	chunk.nBytesRead += 2;
	
	//Read texture coordinates
	pTexCoords = new A3DS_TEXCOORD[nNumTexCoords];
	f.read(reinterpret_cast<char*>(pTexCoords), nNumTexCoords * sizeof(A3DS_TEXCOORD));
	chunk.nBytesRead += nNumTexCoords * sizeof(A3DS_TEXCOORD);
		
	//Save the data to the mesh
	m_mesh[m_mesh.size()-1].pTexCoords = pTexCoords;
	m_mesh[m_mesh.size()-1].nTotalTexCoords = nNumTexCoords;
	
	// Skip the rest of the chunk.
	moveToNextChunk(f, chunk);
}

void Importer3DSLoader::setUpMeshesAndMaterials(int nIndex, vector<int>& indices, vector<Vertex>& verts, Material& mat,
	const string& sTextureDirectory, bool bDisableLoadingTextures, bool bNegateVTextureCoordinate)
{
	
	if(nIndex > static_cast<int>((m_materials.size()-1)))
		return;
	
	//Create the material
	A3DS_MATERIAL* pM = &m_materials[nIndex];
	double d = 1.0 / 255;
	mat.setName(pM->sName);
	mat.setAmbient( Color(pM->cAmbient.r*d,  pM->cAmbient.g*d,  pM->cAmbient.b*d));
	mat.setDiffuse( Color(pM->cDiffuse.r*d,  pM->cDiffuse.g*d,  pM->cDiffuse.b*d, 1.0-pM->fTransparency));
	mat.setSpecular(Color(pM->cSpecular.r*d, pM->cSpecular.g*d, pM->cSpecular.b*d));
	mat.setEmissive(Color(pM->cEmissive.r*d, pM->cEmissive.g*d, pM->cEmissive.b*d));	
	//mat.setColor(Color(1.0, 1.0, 1.0));
	mat.setColor(mat.getDiffuse());
	mat.setSpecularFactor(pM->fSpecularPower*100);
	
	if(pM->sTextureName != "" && !bDisableLoadingTextures)
	{
		string sTexName = pM->sTextureName;
		if(sTextureDirectory != "")
		{
			#ifdef WIN32
			sTexName = sTextureDirectory + "/" + sTexName;
			#else
			sTexName = sTextureDirectory + "/" + sTexName;
			#endif 
		}
		
		mat.setTexture(0, TextureManager::getInstance()->loadTexture(sTexName));
		
	}	
	
	//Create the vertex and index data
	Vector vNormalSum;
	Vector v1, v2, v3;
	Vertex newVert;
	int nCounter = 0;
	
	for(unsigned int i=0; i<m_mesh.size(); i++)
	{
		A3DS_MESH* m = &(m_mesh[i]);
		
		for(unsigned int f=0; f<m->nTotalFaces; f++)
		{
			//Is it the right material?
			if(m->pFaces[f].nMatID != nIndex)
				continue;
				
			//Walk through all three vertices of the face
			for(int v=0; v<3; v++)
			{
				//Reset the normal vector
				vNormalSum.set(0.0, 0.0, 0.0);
				
				//Find normals to generate smoothed normals
				bool bFound = false;
				for(unsigned int fs=0; fs<m->nTotalFaces; fs++)
				{
					bFound = false;
					//Vertex shared?
					if((m->pFaces[f].nMatID == nIndex) && (m->pFaces[f].nSmoothingGroup == m->pFaces[fs].nSmoothingGroup))
					{
						//Walk through all three vertices of the face
						for(int vs=0; vs<3; vs++)
						{
							//The two vertices are the same? => Smooth edge found
							if(m->pFaces[f].nIndices[v] == m->pFaces[fs].nIndices[vs])
							{
								bFound = true;
								break;	
							}
						}
						
						//Shared vertex found 
						if(bFound)
						{
							v1.x = -m->pVertices[m->pFaces[fs].nIndices[0]].y;
							v1.y =  m->pVertices[m->pFaces[fs].nIndices[0]].z;
							v1.z = -m->pVertices[m->pFaces[fs].nIndices[0]].x;

							v2.x = -m->pVertices[m->pFaces[fs].nIndices[1]].y;
							v2.y =  m->pVertices[m->pFaces[fs].nIndices[1]].z;
							v2.z = -m->pVertices[m->pFaces[fs].nIndices[1]].x;
							
							v3.x = -m->pVertices[m->pFaces[fs].nIndices[2]].y;
							v3.y =  m->pVertices[m->pFaces[fs].nIndices[2]].z;
							v3.z = -m->pVertices[m->pFaces[fs].nIndices[2]].x;							
							
							//Sum up the normal vector	
							vNormalSum += ((v2 - v1) ^ (v3 - v1));
						}
					}
				}
				
				//Set up new vertex
				newVert.vPos.x = -m->pVertices[m->pFaces[f].nIndices[v]].y;
				newVert.vPos.y =  m->pVertices[m->pFaces[f].nIndices[v]].z;
				newVert.vPos.z = -m->pVertices[m->pFaces[f].nIndices[v]].x;
				if(m->pFaces[f].nIndices[v] < m->nTotalTexCoords) //Texture coordinate available?
				{
					if(bNegateVTextureCoordinate)
					{
						newVert.setTexCoord(-1,
							m->pTexCoords[m->pFaces[f].nIndices[v]].tu,
							1.0-m->pTexCoords[m->pFaces[f].nIndices[v]].tv);						
					}
					else
					{
						newVert.setTexCoord(-1,
							m->pTexCoords[m->pFaces[f].nIndices[v]].tu,
							m->pTexCoords[m->pFaces[f].nIndices[v]].tv);						
					}
				}				
				newVert.vNormal = vNormalSum;
				if(newVert.vNormal != Vector(0.0, 0.0, 0.0))
					newVert.vNormal.normalize();
				
				//Add the new vertex to the list
				verts.push_back(newVert);
				//Add new intex to the list
				indices.push_back(nCounter);
				nCounter++;		
						
			}	
		}
	}
}

void Importer3DSLoader::getMesh(int nIndex, vector<int>& indices, vector<Vertex>& verts,
	Material& mat, const string& sTextureDirectory, bool bDisableLoadingTextures,
	bool bNegateVTextureCoordinate)
{
	//Build the meshes with the loaded data
	setUpMeshesAndMaterials(nIndex, indices, verts, mat, sTextureDirectory,
		bDisableLoadingTextures, bNegateVTextureCoordinate);
}

