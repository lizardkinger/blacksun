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
 *  File:       Material.cpp
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#include "../include/Material.h"
#include "../include/Renderer.h"

namespace BSRenderer
{

int Material::nMagicNumber = 24022007;

Material::Material(const string& sName)
    : QObject(), m_sName(sName)
{
	setToDefault();	
}

Material::Material(const Material& otherMaterial)
    : QObject()
{
    m_dSpecularFactor = otherMaterial.m_dSpecularFactor;
    m_sName = otherMaterial.m_sName;
    m_cColor = otherMaterial.m_cColor;
    m_cAmbient = otherMaterial.m_cAmbient;
    m_cDiffuse = otherMaterial.m_cDiffuse;
    m_cSpecular = otherMaterial.m_cSpecular;
    m_cEmissive = otherMaterial.m_cEmissive;
    m_dSpecularFactor = otherMaterial.m_dSpecularFactor;
    
    for(int i = 0 ; i < MAXTEXTURES ; i++)
    {
        m_tex[i] = otherMaterial.m_tex[i];
        m_texState[i] = otherMaterial.m_texState[i];
    }
}

void Material::set()
{	
	int nTexStage = 0;
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	
	//First deactive all textures
	for(int t=0; t<MAXTEXTURES; t++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+t);
		glDisable(GL_TEXTURE_2D);		
	}
	
	for(int t=0; t<MAXTEXTURES; t++)
	{
		//Texture is not valid
		if(m_tex[t] == -1)
		{
			//Disable the stage
			glActiveTextureARB(GL_TEXTURE0_ARB+t);
			glDisable(GL_TEXTURE_2D);
			continue;
		}
		
		//Set the texture and the texture-state
		m_texState[t].set(nTexStage);
		TextureManager::getInstance()->getTexture(m_tex[t])->set(nTexStage);
		
		nTexStage++;
	}
	
	//No textures are set, so the texturing can be disabled
	if(nTexStage==0)
		glDisable(GL_TEXTURE_2D);
			
	//glMaterial does not support double-values. So it must be
	//converted into a float-array
	float fAmbient[4], fDiffuse[4], fSpecular[4], fEmissive[4];
	m_cAmbient.getFloatArray(&fAmbient[0]);
	m_cDiffuse.getFloatArray(&fDiffuse[0]);
	m_cSpecular.getFloatArray(&fSpecular[0]);
	m_cEmissive.getFloatArray(&fEmissive[0]);
	
	//Set the material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fEmissive);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_dSpecularFactor);
}

bool Material::save(const string& sFileName)
{
	ofstream f(sFileName.c_str(), ios::binary);

	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}

	if(appendToFile(f)==false)
		return false;
	
	f.close();
	
	stringstream msg;
	msg << "Material saved: \"" << sFileName << "\"";
	LOG_Ok(msg.str());

	return true;	
}

bool Material::load(const string& sFileName)
{
	ifstream f(sFileName.c_str(), ios::binary);

	//File can't be opened
	if(f.rdstate())
	{
		stringstream msg;
		msg << "Can't open file: \"" << sFileName << "\"";
		LOG_Error(msg.str());		
		return false;
	}
	
	loadFromFilePos(f, 0);
	
	f.close();

	stringstream msg;
	msg << "Material loaded: \"" << sFileName << "\"";
	LOG_Ok(msg.str());
    
    emit changed();

	return true;	
}
		
bool Material::appendToFile(ofstream& f)
{
	//Write magic number of material file
	f.write(reinterpret_cast<char*>(&nMagicNumber), sizeof(nMagicNumber));
	
	//Write the material name
	UINT nLength = m_sName.length();
	f.write(reinterpret_cast<char*>(&nLength), sizeof(nLength));
	
	for(UINT i=0; i<nLength; i++)
		f.write(reinterpret_cast<char*>(&m_sName[i]), sizeof(m_sName[i]));	
	
	//Write texture-ID with path if needed
	TextureManager* pTexMgr = TextureManager::getInstance();
	vector<int> texturesNeeded;
	for(int t=0; t<MAXTEXTURES; t++)
	{
		if(m_tex[t] < 0)
			continue;
			
		for(unsigned int i=0; i<texturesNeeded.size(); i++)
		{
			//Texture is already in list
			if(texturesNeeded[i] == m_tex[t])
				break;	
		}
		
		texturesNeeded.push_back(m_tex[t]);
	}
	int nTexNum = texturesNeeded.size();
	
	f.write(reinterpret_cast<char*>(&nTexNum), sizeof(UINT));
	for(int t=0; t<nTexNum; t++)
	{
		
		//Write ID of the texture
		f.write(reinterpret_cast<char*>(&texturesNeeded[t]), sizeof(int));
		
		//Write the texture name/path
		string sTexName = pTexMgr->getTexture(texturesNeeded[t])->getTextureInfo()->sName;
		UINT nLength = sTexName.length();
		f.write(reinterpret_cast<char*>(&nLength), sizeof(nLength));
		
		for(UINT i=0; i<nLength; i++)
			f.write(reinterpret_cast<char*>(&sTexName[i]), sizeof(sTexName[i]));		
	}	

	
	//Write the pure material data
	f.write(reinterpret_cast<char*>(&m_cColor),    sizeof(Color));
	f.write(reinterpret_cast<char*>(&m_cAmbient),  sizeof(Color));
	f.write(reinterpret_cast<char*>(&m_cDiffuse),  sizeof(Color));
	f.write(reinterpret_cast<char*>(&m_cSpecular), sizeof(Color));
	f.write(reinterpret_cast<char*>(&m_cEmissive), sizeof(Color));
	f.write(reinterpret_cast<char*>(&m_dSpecularFactor), sizeof(m_dSpecularFactor));
	
	//Write the max. texture stages per material
	int nMaxTex = MAXTEXTURES;
	f.write(reinterpret_cast<char*>(&nMaxTex), sizeof(nMaxTex));
	
	//Write the textur stages of the material
	for(int i=0; i<MAXTEXTURES; i++)
	{
		f.write(reinterpret_cast<char*>(&m_tex[i]), sizeof(int));
		f.write(reinterpret_cast<char*>(&m_texState[i]), sizeof(TextureState));			
	}
	
	return true;	
}
		
bool Material::loadFromFilePos(ifstream& f, int nPos)
{
	//Walk to the file position
	f.seekg(nPos, ios::beg);
		
	//Read magic number of material file
	int nNumber;
	f.read(reinterpret_cast<char*>(&nNumber), sizeof(nNumber));
	
	//Its no material file, so return
	if(nNumber != nMagicNumber)
	{
		stringstream msg;
		msg << "Material-version file not supported: " << nNumber;
		LOG_Error(msg.str());		
		return false;	
	}
	
	//Read the material name
	UINT nLength;
	f.read(reinterpret_cast<char*>(&nLength), sizeof(nLength));
	m_sName.resize(nLength);
	
	for(UINT i=0; i<nLength; i++)
		f.read(reinterpret_cast<char*>(&m_sName[i]), sizeof(m_sName[i]));	
	
	vector<int> texturesNeededID;
	vector<string> texturesNeededName;

	UINT nNumTex = 0;
	f.read(reinterpret_cast<char*>(&nNumTex), sizeof(UINT));
	
	for(unsigned int t=0; t<nNumTex; t++)
	{
		//Read ID of the texture
		int nID = -1;
		f.read(reinterpret_cast<char*>(&nID), sizeof(int));
		
		//Write the texture name/path
		string sTexName;
		UINT nLength = 0;
		f.read(reinterpret_cast<char*>(&nLength), sizeof(nLength));
		sTexName.resize(nLength);
		
		for(UINT i=0; i<nLength; i++)
			f.read(reinterpret_cast<char*>(&sTexName[i]), sizeof(sTexName[i]));
			
		texturesNeededID.push_back(nID);
		texturesNeededName.push_back(sTexName);
	}	
	
	//Read the pure material data
	f.read(reinterpret_cast<char*>(&m_cColor),    sizeof(Color));
	f.read(reinterpret_cast<char*>(&m_cAmbient),  sizeof(Color));
	f.read(reinterpret_cast<char*>(&m_cDiffuse),  sizeof(Color));
	f.read(reinterpret_cast<char*>(&m_cSpecular), sizeof(Color));
	f.read(reinterpret_cast<char*>(&m_cEmissive), sizeof(Color));
	f.read(reinterpret_cast<char*>(&m_dSpecularFactor), sizeof(m_dSpecularFactor));
	
	//Read the max. texture stages per material
	int nMaxTex;
	f.read(reinterpret_cast<char*>(&nMaxTex), sizeof(nMaxTex));
	
	//Read the textur stages of the material
	for(int i=0; i<nMaxTex; i++)
	{
		f.read(reinterpret_cast<char*>(&m_tex[i]), sizeof(int));
		f.read(reinterpret_cast<char*>(&m_texState[i]), sizeof(TextureState));
		
		//Load texture, if needed
		if(m_tex[i]>=0)
		{
			TextureManager* pTexMgr = TextureManager::getInstance();
			
			//Search for the texture-Name
			for(unsigned int t=0; t<texturesNeededID.size(); t++)
			{
				//ID found?
				if(texturesNeededID[t] == m_tex[i])
				{
					//Load texture
					m_tex[i] = pTexMgr->loadTexture(texturesNeededName[t], m_tex[t]);
					break;		
				}	
			}	
		}			
	}
	
    emit changed();
    
	return true;
}		
		
int Material::getNumValidTextures() const
{
	int nNum = 0;
	
	for(int i=0; i<MAXTEXTURES; i++)
	{
		//Its valid, if the texture-id is not -1 and the texture is available
		if(m_tex[i] != -1 && 
			TextureManager::getInstance()->getTexture(m_tex[i]) != NULL)
		{
			nNum++;
		}	
	}
	
	return nNum;
}

void Material::getValidTextureStages(int* nTexStages) const
{
	int nNum = 0;
	
	for(int i=0; i<MAXTEXTURES; i++)
	{
		//Its valid, if the texture-id is not -1 and the texture is available
		if(m_tex[i] != -1 && 
			TextureManager::getInstance()->getTexture(m_tex[i]) != NULL)
		{
			nTexStages[nNum] = i;
			nNum++;
		}	
	}
}		
		
void Material::setTexture(int nTexStage, int nTexID)
{
	if((nTexStage >= 0) && (nTexStage < MAXTEXTURES))
		m_tex[nTexStage] = nTexID;
    emit changed();
}	

int Material::getTexture(int nTexStage) const
{
	if((nTexStage >= 0) && (nTexStage < MAXTEXTURES))
		return m_tex[nTexStage];
	
	return -1;	
}

TextureState* Material::getTextureState(int nTexStage)
{
	if((nTexStage >= 0) && (nTexStage < MAXTEXTURES))
		return &m_texState[nTexStage];
		
	return NULL;	
}

void Material::setToDefault()
{
	m_cColor.set(1.0, 1.0, 1.0);
	m_cAmbient.set(0.2, 0.2, 0.2, 1.0);
	m_cDiffuse.set(0.8, 0.8, 0.8);
	m_cSpecular.set(0.1, 0.1, 0.1);
	m_cEmissive.set(0.0, 0.0, 0.0); 
	m_dSpecularFactor = 0.0;

	//Set the used textures to NULL, so that the texture do not use
	//any textures
	for(int t=0; t<MAXTEXTURES; t++)
		m_tex[t] = -1;
		
	//The first version must be set with other values than the others
	m_texState[0].setCombineMethode(false, TEXMET_Replace);
	//m_texState[0].setSourceCombine(TEXCOMB_ColorArg1, TEXOP_Disable);
	m_texState[0].setCombineOperand(TEXCOMBOP_RGB0, TEXBLEND_SrcColor);

	//m_texState[0].setCombineMethode(true, TEXMET_Replace);
	//m_texState[0].setCombineMethode(false, TEXMET_Disable);
	//m_texState[0].setSourceCombine(TEXCOMB_ColorArg0, TEXOP_Texture);
	//m_texState[0].setSourceCombine(TEXCOMB_AlphaArg0, TEXOP_Disable);
	//m_texState[0].setCombineOperand(TEXCOMBOP_RGB0, TEXBLEND_SrcColor);
	
	//m_texState[1].setSourceCombine(TEXCOMB_AlphaArg0, TEXOP_Previous);
	//m_texState[1].setCombineOperand(TEXCOMBOP_Alpha0, TEXBLEND_One);
	//m_texState[1].setCombineMethode(false, TEXMET_Replace);
}	
	
}
