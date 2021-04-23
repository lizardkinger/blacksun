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
 *  File:       ImporterMD2Loader.cpp
 *  Created:    30.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/ImporterMD2Loader.h"

using namespace BSRenderer;

bool ImporterMD2Loader::execute(const string& sFileName, vector<int>& indices, 
	vector<Vertex>& verts)
{
	if(sFileName == "")
		return false;	
	
	MD2_MODELL model;
	ifstream f(sFileName.c_str(), ios::binary);
	
	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}
	
	//Read header
	if(readHeader(f, model)==false)
	{
		stringstream msg;
		msg << "Invalid Q2 header in  \"" << sFileName << "\"";
		LOG_Error(msg.str());
		return false;
	}

	//Read all parts of the file
	readSkin(f, model);
	readTexCoord(f, model);
	readTriangle(f, model);
	readFramesAndVertices(f, model);

	//Close file
	f.close();

	//Build frames
	setUpFrames(model);	

	//Generate the vertices for the scene
	generateVertices(model, indices, verts, 0, 0);
	
	releaseModel(model);
	
	return true;
}

void ImporterMD2Loader::releaseModel(MD2_MODELL& model)
{
	//Delete frames
	for(unsigned int f=0; f<model.Frames.size(); f++)
	{
		if(model.Frames[f].pFinalVerts)
		{
			delete[] model.Frames[f].pFinalVerts;
			model.Frames[f].pFinalVerts = NULL;
		}
	}	
	
	//Delte keys
	delete[] model.Key.pKey;
}

bool ImporterMD2Loader::readHeader(ifstream& f, MD2_MODELL& model)
{
	f.read(reinterpret_cast<char*>(&model.Header), sizeof(MD2_HEADER));

	//Check magic number of file
	if(model.Header.nMagicNumber != MD2MAGICNUMBER)
	{
		return false;
	}

	//Check version
	if(model.Header.nVersion!=MD2VERSION)
	{
		return false;
	}

	return true;	
}

void ImporterMD2Loader::readSkin(ifstream& f, MD2_MODELL& model)
{
	//Go to the skin-chunk
	f.seekg(model.Header.nOffsetSkins);

	//Alle Skins einlesen
	for(int n=0; n<model.Header.nNumSkins; n++)
	{
		MD2_SKIN skin;
		f.read(reinterpret_cast<char*>(&skin), sizeof(MD2_SKIN));
		model.Skins.push_back(skin);
	}
}

void ImporterMD2Loader::readTexCoord(ifstream& f, MD2_MODELL& model)
{
	//Go to the texcoord-chunk
	f.seekg(model.Header.nOffsetTexCoords);

	//Alle Skins einlesen
	for(int t=0; t<model.Header.nNumTexCoords; t++)
	{
		MD2_TEXCOORD texcoord;
		f.read(reinterpret_cast<char*>(&texcoord), sizeof(MD2_TEXCOORD));
		model.TexCoords.push_back(texcoord);
	}	
}

void ImporterMD2Loader::readTriangle(ifstream& f, MD2_MODELL& model)
{
	//Go to the triangle-chunk
	f.seekg(model.Header.nOffsetTriangles);

	//Alle Skins einlesen
	for(int t=0; t<model.Header.nNumTriangles; t++)
	{
		MD2_TRIANGLE triangle;
		f.read(reinterpret_cast<char*>(&triangle), sizeof(MD2_TRIANGLE));
		model.Triangles.push_back(triangle);
	}
}

void ImporterMD2Loader::readFramesAndVertices(ifstream& f, MD2_MODELL& model)
{
	unsigned char chBigBuffer[50000];
	MD2_FRAMEALIAS* pTempFrame = (MD2_FRAMEALIAS*)chBigBuffer;

	//Go to the chunk
	f.seekg(model.Header.nOffsetFrames);

	//Alle Frames einlesen
	for(int fr=0; fr<model.Header.nNumFrames; fr++)
	{
		MD2_FRAME frame;

		//Read frame in temporary buffer
		f.read(reinterpret_cast<char*>(pTempFrame), model.Header.nFrameSize);

		frame.pFinalVerts = new MD2_VERTEX[model.Header.nNumVertices];

		//Copy name of temp. buffer to normal struct
		strcpy(frame.chName, pTempFrame->chName);
		
		//Create all vertices for the frame
		for(int v=0; v<model.Header.nNumVertices; v++)
		{
			//Calculate vertex
			frame.pFinalVerts[v].fVertex[0] = pTempFrame->aliasVerts[v].byVertex[0] * 
				pTempFrame->fScale[0] + pTempFrame->fTranslate[0];
			frame.pFinalVerts[v].fVertex[1] = pTempFrame->aliasVerts[v].byVertex[1] * 
				pTempFrame->fScale[1] + pTempFrame->fTranslate[1];
			frame.pFinalVerts[v].fVertex[2] = pTempFrame->aliasVerts[v].byVertex[2] * 
				pTempFrame->fScale[2] + pTempFrame->fTranslate[2];
		}

		model.Frames.push_back(frame);
	}	
}

void ImporterMD2Loader::setUpFrames(MD2_MODELL& model)
{
	model.Key.nNumKeys = 0;

	char chName[16] = {0};
	char chLastName[16] = {0};
	char chFirstName[16] = {0};

	for(int f=0; f<model.Header.nNumFrames; f++)
	{
		strcpy(chName, model.Frames[f].chName);
		int nStrLen = strlen(chName);

		for(int sl=0; sl<nStrLen; sl++)
		{
			if((chName[sl]<'A') || (chName[sl]>'z'))
			{
				//It's an integer, replace it with \0
				chName[sl] = '\0';

				if(strcmp(chLastName, chName))
				{
					strcpy(chLastName, chName);
					model.Key.nNumKeys++;
				}

				if(!strcmp(chFirstName, ""))
				{
					strcpy(chFirstName, chName);
				}

				break;
			}
		}
	}

	model.Key.pKey = new MD2_KEYFRAME[model.Key.nNumKeys];

	strcpy(chName, "");
	strcpy(chLastName, chFirstName);
	int nCurrentKey = 0;

	model.Key.pKey[0].nStart = 0;
	strcpy(model.Key.pKey[0].chName, chFirstName);

	for(int f=0; f<model.Header.nNumFrames; f++)
	{
		strcpy(chName, model.Frames[f].chName);
		int nStrLen = strlen(chName);

		for(int sl=0; sl<nStrLen; sl++)
		{
			if((chName[sl]<'A') || (chName[sl]>'z'))
			{
				//It's an integer, replace it with \0
				chName[sl] = '\0';

				if(strcmp(chLastName, chName))
				{
					model.Key.pKey[nCurrentKey].nEnd = f-1;
					nCurrentKey++;
					model.Key.pKey[nCurrentKey].nStart = f;

					strcpy(chLastName, chName);
					strcpy(model.Key.pKey[nCurrentKey].chName, chName);
				}
				else
				{
					model.Key.pKey[nCurrentKey].nEnd = f;
				}

				break;
			}
		}
	}	
}

void ImporterMD2Loader::generateVertices(MD2_MODELL& model, vector<int>& indices, 
	vector<Vertex>& verts, int nAnimationKey, int nFrame)
{
	indices.clear();
	verts.clear();
	
	//Pointer to the vertices
	MD2_VERTEX* pVertFrame = model.Frames[nFrame].pFinalVerts;
	
	//Walk through all triangles
	for(int f=0; f<model.Header.nNumTriangles; f++)
	{
		//Walk through the three vertices
		for(int t=0; t<3; t++)
		{
			int nIndex = model.Triangles[f].sVertexIndex[t];
			int nTexIndex = model.Triangles[f].sTexIndex[t];
			
			Vertex newVert;
			newVert.vPos = Vector(pVertFrame[nIndex].fVertex[1],
							pVertFrame[nIndex].fVertex[2],
							pVertFrame[nIndex].fVertex[0]);
			newVert.vPos *= 0.075;
			
			//Set MD2-Skin-coordinates
			newVert.setTexCoord(-1, model.TexCoords[nTexIndex].su/static_cast<double>(model.Header.nSkinWidth),
				1.0 - model.TexCoords[nTexIndex].sv/static_cast<double>(model.Header.nSkinHeight));
				
			verts.push_back(newVert);
			
			//Create index
			indices.push_back(indices.size());
		}
	}
	
	//Calculate normals and swap face
	Vector vNormal;
	for(unsigned int i=0; i<verts.size(); i+=3)
	{
		//Swap face
		Vertex temp = verts[i+0];
		verts[i+0] = verts[i+1];
		verts[i+1] = temp;
		
		//Calculate normal
		vNormal = (verts[i+1].vPos-verts[i+0].vPos) ^ (verts[i+2].vPos-verts[i+0].vPos);
		if(vNormal != Vector(0.0, 0.0, 0.0))
			vNormal.normalize();
			
		verts[i+0].vNormal = verts[i+1].vNormal = verts[i+2].vNormal = vNormal;		
	}	
}
