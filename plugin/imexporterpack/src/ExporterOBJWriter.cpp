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
 *  File:       ExporterOBJWriter.cpp
 *  Created:    09.04.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/ExporterOBJWriter.h"

using namespace BSRenderer;

bool ExporterOBJWriter::execute(const string& sFileName, vector< vector<Vertex> >& verts,
		vector< vector<int> >& indices, vector<Material*>& materials)
{
	if(sFileName == "")
		return false;	
	
	if(verts.size()==0 || indices.size()==0)
		return false;
	
	ofstream f(sFileName.c_str());
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}	
	
	//Append a number to the material name to make them unique
	/*for(unsigned int m=0; m<materials.size(); m++)
	{
		stringstream s;
		s << materials[m]->getName() << "-" << rand() << m;
		materials[m]->setName(s.str());
	}*/
	
	//Create new indices and vertex lists
	vector<Vertex> v;
	vector< vector<int> > i;
	int nBias = 0;
	for(unsigned int g=0; g<verts.size(); g++)
	{
		if(verts[g].size() == 0 || indices[g].size() == 0)
			continue;
		
		vector<int> newIndices;
		
		//Copy vertices
		for(unsigned int n=0; n<verts[g].size(); n++)
			v.push_back(verts[g][n]);
		
		//Copy indices and add bias (for big list)
		for(unsigned int n=0; n<indices[g].size(); n++)
			newIndices.push_back(indices[g][n]+nBias+1);
		
		i.push_back(newIndices);
		
		//Create the new start index for new group
		nBias += verts[g].size();
	}
	
	if(v.size() == 0 || i.size() == 0)
		return false;
	
	//Write comment header
	f << "##" << endl;
	f << "## BlackSun generated .obj file" << endl;
	f << "##" << endl; 
	f << endl;

	//Split filename
	string sMaterialName = sFileName;
	int n = sMaterialName.find_last_of("/");
	if(n == -1)
		n = sMaterialName.find_last_of("\\");
	int m = sMaterialName.find_last_of(".");
	sMaterialName = sMaterialName.substr(n+1, m-n-1);
	sMaterialName += ".mtl";
	
	//Split extension from path for material file
	string sMaterialFullPath = sFileName.substr(0, sFileName.find_last_of("."));
	
	//Write material-lib
	f << "mtllib " << sMaterialName << endl;
	f << endl;
	
	writeVertices(f, v);
	writeGroups(f, i, materials);
	writeMaterialFile(sMaterialFullPath + ".mtl", materials);
	
	f.close();
		
	return true;
}

void ExporterOBJWriter::writeVertices(ofstream& f, vector<Vertex>& verts)
{	
	//Write the position data
	for(unsigned int v=0; v<verts.size(); v++)
	{
		f << "v " << static_cast<float>(verts[v].vPos.x) << " " <<
			static_cast<float>(verts[v].vPos.y) << " " <<
			static_cast<float>(verts[v].vPos.z) << endl;
	}
	//Write comment for number of vertices
	f << "# " << verts.size() << " vertices" << endl;
	f << endl;
	
	//Write the normal data
	for(unsigned int v=0; v<verts.size(); v++)
	{
		f << "vn " << static_cast<float>(verts[v].vNormal.x) << " " <<
			static_cast<float>(verts[v].vNormal.y) << " " <<
			static_cast<float>(verts[v].vNormal.z) << endl;
	}
	//Write comment for number of vertices
	f << "# " << verts.size() << " normals" << endl;
	f << endl;
	
	//Write the texture coordinates (First stage)
	for(unsigned int v=0; v<verts.size(); v++)
	{
		f << "vt " << static_cast<float>(verts[v].dU[0]) << " " <<
			static_cast<float>(verts[v].dV[0]) << endl;
	}
	//Write comment for number of vertices
	f << "# " << verts.size() << " texture coordinates" << endl;
	f << endl;		
	
	
}

void ExporterOBJWriter::writeGroups(ofstream& f,
	vector< vector<int> >& indices, vector<Material*>& materials)
{
	//Walk through all groups
	for(unsigned int g=0; g<indices.size(); g++)
	{
		f << "g " <<	materials[g]->getName() << endl; //TODO: Set name to mesh-name
		f << "usemtl " << materials[g]->getName() << endl;
		
		//Write all faces
		for(unsigned int i=0; i<(indices[g].size()/3); i++)
		{
			f << "f " << indices[g][i*3+0] << "/" << indices[g][i*3+0] << "/" << indices[g][i*3+0] << " " <<
				indices[g][i*3+1] << "/" << indices[g][i*3+1] << "/" << indices[g][i*3+1] << " " << 
				indices[g][i*3+2] << "/" << indices[g][i*3+2] << "/" << indices[g][i*3+2] << endl;
		}
		
		f << "# " << (indices[g].size()/3) << " faces" << endl;
		f << endl;
	}
}

void ExporterOBJWriter::writeMaterialFile(const string& sFileName,
	vector<Material*>& materials)
{
	ofstream f(sFileName.c_str());
	
		//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return;
	}
	
	//Write comment header
	f << "##" << endl;
	f << "## BlackSun generated .mtl file" << endl;
	f << "##" << endl; 
		
	Color c;
	for(unsigned int m=0; m<materials.size(); m++)
	{
		f << endl << endl;
		
		//Write material name
		f << "newmtl " << materials[m]->getName() << endl;
		
		//Write ambient color
		c = materials[m]->getAmbient();
		f << "Ka " << static_cast<float>(c.r) << " " << static_cast<float>(c.g) << " " <<
			static_cast<float>(c.b) << endl;

		//Write diffuse color
		c = materials[m]->getColor();
		f << "Kd " << static_cast<float>(c.r) << " " << static_cast<float>(c.g) << " " <<
			static_cast<float>(c.b) << endl;

		//Write specular color
		c = materials[m]->getSpecular();
		f << "Ks " << static_cast<float>(c.r) << " " << static_cast<float>(c.g) << " " <<
			static_cast<float>(c.b) << endl;
			
		//Write alpha
		f << "d " << static_cast<float>(materials[m]->getColor().a) << endl;
		
		//Write specular power
		f << "Ns " << static_cast<float>(materials[m]->getSpecularFactor()) << endl;
		
		//Write illumination typ
		f << "illum 2" << endl;
		
		//Write texture file name
		if(materials[m]->getTexture(0) != -1)
		{
			Texture* pTex = TextureManager::getInstance()->getTexture(materials[m]->getTexture(0));
			if(pTex != NULL)
			{
				f << "map_Ka " << pTex->getTextureInfo()->sName << endl;	
			} 
		}		
	}
	
	f.close();	
	
}
