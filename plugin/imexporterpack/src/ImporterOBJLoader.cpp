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
 *  File:       ImporterOBJLoader.cpp
 *  Created:    08.04.2007 
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/ImporterOBJLoader.h"

using namespace BSRenderer;

bool ImporterOBJLoader::execute(const string& sFileName)
{
	if(sFileName == "")
		return false;	
	
	ifstream f(sFileName.c_str());
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}	
	
	m_materials.clear();
	
	string sBuffer;
	
	while(!f.eof())
	{
		getline(f, sBuffer);
		
		//Is it a empty line or a comment line?
		if(sBuffer == "" || sBuffer[0] == '#')
			continue;
			
		//Read in relation to the line type
		char chTyp = toupper(sBuffer[0]);
		switch(chTyp)
		{
			case 'G':
			{
				//Read group
				m_mesh.groups.push_back(OBJ_GROUP());
				m_mesh.groups[m_mesh.groups.size()-1].sName = sBuffer.substr(2, sBuffer.size());
			}break;
			case 'V':
			{
				//Read vertex data
				readVertexData(sBuffer);
			}break;
			case 'F':
			{
				//Read face data
				readFaceData(sBuffer);
			}break;
			case 'U':
			{
				//Read material name
				readMaterialName(sBuffer);
			}break;
			case 'M':
			{
				//Load material
				loadMaterials(sBuffer);
			}break;	
		}
				
	}
	
	f.close();
		
	//There are no materials loaded
	if(m_materials.size() == 0)
	{
		//Set all materals of the groups to 'default'
		for(unsigned int g=0; g<m_mesh.groups.size(); g++)
		{
			m_mesh.groups[g].sMaterial = "Default";	
		}	
	}
		
	return true;
}

void ImporterOBJLoader::release()
{	
	m_mesh.vertices.clear();
	m_mesh.normals.clear();
	m_mesh.texcoords.clear();
	
	//Delete face data
	for(unsigned int g=0; g<m_mesh.groups.size(); g++)
	{
		for(unsigned int f=0; f<m_mesh.groups[g].faces.size(); f++)
		{
			if(m_mesh.groups[g].faces[f].verts != NULL)
			{
				delete[] m_mesh.groups[g].faces[f].verts;
				m_mesh.groups[g].faces[f].verts = NULL;
			}
	
			if(m_mesh.groups[g].faces[f].normals != NULL)
			{
				delete[] m_mesh.groups[g].faces[f].normals;
				m_mesh.groups[g].faces[f].normals = NULL;
			}
			
			if(m_mesh.groups[g].faces[f].texcoords != NULL)
			{
				delete[] m_mesh.groups[g].faces[f].texcoords;
				m_mesh.groups[g].faces[f].texcoords = NULL;
			}			
		}
	}
	
	m_mesh.groups.clear();
}

void ImporterOBJLoader::readVertexData(string& s)
{
	//Read in relation to the line type
	char chTyp = toupper(s[1]);
	switch(chTyp)
	{
		case ' ':
		{
			//Read position data
			istringstream line(s.substr(2, s.length()));
			OBJ_VERTEX vert;
			line >> vert.x >> vert.y >> vert.z;
			m_mesh.vertices.push_back(vert);					
		}break;
		case 'N':
		{
			//Read normal data
			istringstream line(s.substr(3, s.length()));
			OBJ_NORMAL norm;
			line >> norm.nx >> norm.ny >> norm.nz;
			m_mesh.normals.push_back(norm);
		}break;
		case 'T':
		{
			//Read texture coordinate data
			istringstream line(s.substr(3, s.length()));
			OBJ_TEXCOORD texcoord;
			line >> texcoord.tu >> texcoord.tv;
			m_mesh.texcoords.push_back(texcoord);			
			
		}break;	
	}	
}

void ImporterOBJLoader::readFaceData(string& s)
{
	//Data is stored in format:
	// f f f   OR   f//n f//n f//n   OR   f/t/n f/t/n f/t/n   (or with four pairs for quad)
	// f/n f/n f/n   possible???
	
	int nNumData = getNumFaceData(s);
	vector<string> tokens;
	split(s.substr(2, s.length()), ' ', '/', tokens);
	
	OBJ_FACE face;
	
	//Create face in relation to number of face data
	switch(nNumData)
	{
		case 1: //Fill face witch position
		{
			face.nNumVerts = tokens.size() / nNumData;
				
			//Allocate memory 
			face.verts     = new int[face.nNumVerts];
			
			for(int f=0; f<face.nNumVerts; f++)
			{
				face.verts[f]     = atoi(tokens[f*1+0].c_str()) - 1;	
			}			
		}break;
		case 2: //Fill face witch position/normal
		{
			face.nNumVerts = face.nNumNormals = tokens.size() / nNumData;
				
			//Allocate memory 
			face.verts     = new int[face.nNumVerts];
			face.normals   = new int[face.nNumNormals];
			
			for(int f=0; f<face.nNumVerts; f++)
			{
				face.verts[f]     = atoi(tokens[f*2+0].c_str()) - 1;
				face.normals[f]   = atoi(tokens[f*2+1].c_str()) - 1;
			}			
		}break;
		case 3: //Fill face witch position/texcoord/normal
		{
			face.nNumVerts = face.nNumNormals = 
				face.nNumTexCoords = tokens.size() / nNumData;
				
			//Allocate memory 
			face.verts     = new int[face.nNumVerts];
			face.texcoords = new int[face.nNumTexCoords];
			face.normals   = new int[face.nNumNormals];
			
			for(int f=0; f<face.nNumVerts; f++)
			{
				face.verts[f]     = atoi(tokens[f*3+0].c_str()) - 1;
				face.texcoords[f] = atoi(tokens[f*3+1].c_str()) - 1;
				face.normals[f]   = atoi(tokens[f*3+2].c_str()) - 1;	
			}			
		}break;	
	}
	
	
	m_mesh.groups[m_mesh.groups.size()-1].faces.push_back(face);
	
}

void ImporterOBJLoader::readMaterialName(string& s)
{
	if(toUpperCase(s.substr(0,6)) != "USEMTL")
		return;
		
	m_mesh.groups[m_mesh.groups.size()-1].sMaterial = s.substr(7, s.size()); 
}

void ImporterOBJLoader::loadMaterials(string& s)
{
	//Is it a material-file?
	if(toUpperCase(s.substr(0,6)) != "MTLLIB")
		return;
	
	string sFileName = s.substr(7, s.size());
	
	
	//Append to material-directory
	if(m_sMatDirectory != "")
	{
		#ifdef WIN32
		sFileName = m_sMatDirectory + "/" + sFileName;
		#else
		sFileName = m_sMatDirectory + "\\" + sFileName;
		#endif 
	}	
	
	m_mesh.sMaterialFile = sFileName;
	
	if(sFileName == "")
		return;	
	
	ifstream f(sFileName.c_str());
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open OBJ-Materialfile: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return;
	}
	
	string sBuffer;
	while(!f.eof())
	{
		getline(f, sBuffer);
		
		//Is it a empty line or a comment line?
		if(sBuffer == "" || sBuffer[0] == '#')
			continue;
			
		//Read in relation to the line type
		char chTyp = toupper(sBuffer[0]);
		switch(chTyp)
		{
			case 'N':
			{
				//Read specular power
				if(toupper(sBuffer[1])=='S')
				{
					m_materials[m_materials.size()-1].fShininess = 
						atof(sBuffer.substr(2, sBuffer.length()).c_str());
				}
				//New material found
				else if(toupper(sBuffer[1])=='E')
				{
					m_materials.push_back(OBJ_MATERIAL());
					m_materials[m_materials.size()-1].sName = sBuffer.substr(7, sBuffer.length());
				}
			}break;
			case 'K':
			{
				//Read ambient color
				if(toupper(sBuffer[1])=='A')
				{
					istringstream line(sBuffer.substr(2, sBuffer.length()));
					line >> m_materials[m_materials.size()-1].cAmbient[0] >>
						m_materials[m_materials.size()-1].cAmbient[1] >>
						m_materials[m_materials.size()-1].cAmbient[2];
				}
				//Read diffuse color
				if(toupper(sBuffer[1])=='D')
				{
					istringstream line(sBuffer.substr(2, sBuffer.length()));
					line >> m_materials[m_materials.size()-1].cDiffuse[0] >>
						m_materials[m_materials.size()-1].cDiffuse[1] >>
						m_materials[m_materials.size()-1].cDiffuse[2];
				}
				//Read specular color
				if(toupper(sBuffer[1])=='S')
				{
					istringstream line(sBuffer.substr(2, sBuffer.length()));
					line >> m_materials[m_materials.size()-1].cSpecular[0] >>
						m_materials[m_materials.size()-1].cSpecular[1] >>
						m_materials[m_materials.size()-1].cSpecular[2];
				}
			}break;
			case 'M':
			{
				//Read texture name
				if(toUpperCase(sBuffer.substr(0, 6))=="MAP_KD")
				{
					m_materials[m_materials.size()-1].sTexName = 
						sBuffer.substr(7, sBuffer.length());
				}
			}break;
			case 'I':
			{
				//Read illuminiation
				if(toUpperCase(sBuffer.substr(0, 5))=="ILLUM")
				{
					m_materials[m_materials.size()-1].nIllumination = 
						atoi(sBuffer.substr(5, sBuffer.length()).c_str()); 	
				}
			}break;	
			case 'T':
			{
				//Read alpha
				if(toUpperCase(sBuffer.substr(0, 2))=="TR")
				{
					m_materials[m_materials.size()-1].fAlpha = 
						atof(sBuffer.substr(2, sBuffer.length()).c_str()); 	
				}
			}break;	
			case 'D':
			{
				//Read alpha
				m_materials[m_materials.size()-1].fAlpha = 
						atof(sBuffer.substr(1, sBuffer.length()).c_str());
			}break;								
		}		
			
	}
	
	f.close();	

}

void ImporterOBJLoader::setUpMeshesAndMaterials(int nIndex, vector<int>& indices, vector<Vertex>& verts, Material& mat,
	const string& sTextureDirectory, bool bDisableLoadingTextures)
{
	string sMatName;
	if(nIndex < 0 || nIndex>(static_cast<int>(m_materials.size())-1))
		sMatName = "DEFAULT";
	else
		sMatName = m_materials[nIndex].sName;
	
	Vertex newVert;
	
	for(unsigned int g=0; g<m_mesh.groups.size(); g++)
	{
		OBJ_GROUP* gr = &(m_mesh.groups[g]);
		
		//Group with same material found
		if(toUpperCase(gr->sMaterial) == toUpperCase(sMatName))
		{
			for(unsigned int n=0; n<gr->faces.size(); n++)
			{
				OBJ_FACE* f = &(gr->faces[n]);
				
				switch(f->nNumVerts)
				{
					//Its a triangle
					case 3:
					{
						verts.push_back(createVertex(*f, 0));
						verts.push_back(createVertex(*f, 1));
						verts.push_back(createVertex(*f, 2));
						
						indices.push_back(indices.size());
						indices.push_back(indices.size());
						indices.push_back(indices.size());
					}break;
					//Its a quad
					case 4:
					{
						//First face
						verts.push_back(createVertex(*f, 0));
						verts.push_back(createVertex(*f, 1));
						verts.push_back(createVertex(*f, 2));

						//Second face
						verts.push_back(createVertex(*f, 2));
						verts.push_back(createVertex(*f, 3));
						verts.push_back(createVertex(*f, 0));
						
						//First face
						indices.push_back(indices.size());
						indices.push_back(indices.size());
						indices.push_back(indices.size());
						
						//Second face
						indices.push_back(indices.size());
						indices.push_back(indices.size());
						indices.push_back(indices.size());													
					}break;
					//Its a polygon
					default:
					{
						//Create a triangulated polygon by handle the data
						//as a triangle-strip
						for(int n=1; n<(f->nNumVerts-1); n++)
						{
							verts.push_back(createVertex(*f, 0));
							verts.push_back(createVertex(*f, n));
							verts.push_back(createVertex(*f, n+1));
							
							indices.push_back(indices.size());
							indices.push_back(indices.size());
							indices.push_back(indices.size());							
						}
					}break;	
				}
					
			} 
		}	
	}
	
	//Create material
	if(verts.size() != 0)
	{
		if(nIndex > 0 || nIndex<=(static_cast<int>(m_materials.size())-1))
		{
			OBJ_MATERIAL* m = &(m_materials[nIndex]);
			
			mat.setName(sMatName);
			mat.setAmbient(Color(m->cAmbient[0], m->cAmbient[1], m->cAmbient[2]));
			mat.setDiffuse(Color(m->cDiffuse[0], m->cDiffuse[1], m->cDiffuse[2], m->fAlpha));
			mat.setSpecular(Color(m->cSpecular[0], m->cSpecular[1], m->cSpecular[2]));
			mat.setColor(Color(m->cDiffuse[0], m->cDiffuse[1], m->cDiffuse[2]));
			mat.setSpecularFactor(m->fShininess);
			
			
			if(m->sTexName != "" && !bDisableLoadingTextures)
			{
				string sTexName = m->sTexName;
				if(sTextureDirectory != "")
				{
					#ifdef WIN32
					sTexName = sTextureDirectory + "/" + sTexName;
					#else
					sTexName = sTextureDirectory + "\\" + sTexName;
					#endif 
				}
				
				mat.setTexture(0, TextureManager::getInstance()->loadTexture(sTexName));
				
			}	
			
			if(bDisableLoadingTextures==false)
				if(m->sTexName != "")	
					mat.setTexture(0, TextureManager::getInstance()->loadTexture(m->sTexName));		
		}
	}
	
}

void ImporterOBJLoader::getMesh(int nIndex, vector<int>& indices, vector<Vertex>& verts,
	Material& mat, const string& sTextureDirectory, bool bDisableLoadingTextures)
{
	//Build the meshes with the loaded data
	setUpMeshesAndMaterials(nIndex, indices, verts, mat, sTextureDirectory,
		bDisableLoadingTextures);
}

void ImporterOBJLoader::split(const string& s, char c1, char c2, vector<string>& res)
{
	res.push_back("");
	
	//Walk through all chars
	for(unsigned int c=0; c<s.length(); c++)
	{
		//Is a valid char, so append it
		if(s[c]!=c1 && s[c]!=c2)
		{
			res[res.size()-1] = res[res.size()-1] + string(1, s[c]);
		}
		else
		{
			//create a new string if there is'n already a empty string
			if(res[res.size()-1]!="")
				res.push_back("");
		}
	}
	
	//Delete the last item, if it's empty
	if(res[res.size()-1]=="")
		res.erase(res.end());
}

string ImporterOBJLoader::toUpperCase(const string& s)
{
	string sRes = s;
	for(unsigned c=0; c<s.length(); c++)
		sRes[c] = toupper(s[c]);	
		
	return sRes;
}

int ImporterOBJLoader::getNumFaceData(string& s)
{
	if(s.find_first_of("//") != string::npos)
		return 2;
	else if(s.find_first_of("/") != string::npos)
		return 3;
	
	return 1;	
}

Vertex ImporterOBJLoader::createVertex(OBJ_FACE& f, int nVertID)
{
	Vertex v;
	
	v.vPos.x = m_mesh.vertices[f.verts[nVertID]].x;
	v.vPos.y = m_mesh.vertices[f.verts[nVertID]].y;
	v.vPos.z = m_mesh.vertices[f.verts[nVertID]].z;
	
	//Normals available?
	if(f.nNumNormals>0)
	{
		v.vNormal.x = m_mesh.normals[f.verts[nVertID]].nx;
		v.vNormal.y = m_mesh.normals[f.verts[nVertID]].ny;
		v.vNormal.z = m_mesh.normals[f.verts[nVertID]].nz;
		
		if(v.vNormal != Vector(0.0, 0.0, 0.0))
			v.vNormal.normalize();			
	}
	
	//Texture-coordinates available?
	if(f.nNumTexCoords>0)
	{
		v.setTexCoord(-1, m_mesh.texcoords[f.verts[nVertID]].tu,
			m_mesh.texcoords[f.verts[nVertID]].tv);		
	}	
	
	
	return v;	
}
