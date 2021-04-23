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
 *  File:       Material.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "RendererTypes.h"
#include "Texture.h"
#include "TextureState.h"

/*! \file
* 
*  \brief Describes the class Material, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief A material 
*
* A Material, that specifies all material properties for the light model 
* 
*/
class Material : public QObject
{
    
	Q_OBJECT

public:
	/*!
	* \brief The constructor
	* 
	* Initializes a standard materal with a specific name
	* 
	* \param sName The name of the material
	*/	
	Material(const string& sName = "unnamed");
    
    /*!
     * \brief The copy constructor
     * 
     * Initializes a standard materal with a specific name
     * 
     * \param otherMaterial The material to be copied
     */	
    
    Material(const Material& otherMaterial);
	
	/*!
	* \brief Sets and activates the material
	* 
	* Sets and activates the materal by setting up the material properties in
	* OpenGl and activate all textures used for this texture
	*/		
	void set();	

	/*!
	* \brief Save the material in a file
	*
	* \param sFileName The file name
	* \return bool True, if the material was saved correctly, otherwise false
	*/
	bool save(const string& sFileName);

	/*!
	* \brief Load the material from a file
	*
	* \param sFileName The file name
	* \return bool True, if the material was saved correctly, otherwise false
	*/	
	bool load(const string& sFileName);

	/*!
	* \brief Saves the material by appending the data to a file
	* 
	* \param f The file where to append the data
	* \return bool True, if the materials were saved correctly, otherwise false
	*/	
	bool appendToFile(ofstream& f);
	
	/*!
	* \brief Load material from a file and a specific file position
	* 
	* \param f The file where to load the data
	* \param nPos The file position in bytes, where the material data is stored
	* 	in the file
	* \return bool True, if the materials were loaded correctly, otherwise false
	*/		
	bool loadFromFilePos(ifstream& f, int nPos);

	/*!
	* \brief Set the name of the material
	* 
	* \param sName The name of the material
	*/	
	void setName(const string& sName)
	{
		m_sName = sName;	
        emit changed();
	}
	
	/*!
	* \brief Returns the name of the material
	* 
	* \return string The name of the material
	*/		
	string getName() const
	{
		return m_sName;	
	}
	
	/*!
	* \brief Set the color of the material
	* 
	* \param cColor The color of the material
	*/		
	void setColor(const Color& cColor)
	{
		m_cColor = cColor;	
        emit changed();
	}
	
	/*!
	* \brief Returns the color of the material
	* 
	* \return Color The color of the materal
	*/		
	Color getColor() const
	{
		return m_cColor;	
	}

	/*!
	* \brief Set the alpha value (transparency)
	* 
	* \param dAlpha The alpha value
	*/			
	void setAlpha(double dAlpha)
	{
		m_cColor.a = dAlpha;
        emit changed();
	}
	
	/*!
	* \brief Returns the alpha value (transparency)
	* 
	* \return double The alpha value
	*/	
	double getAlpha() const
	{
		return m_cColor.a;
	}

	/*!
	* \brief Returns whether the material have one ore more alpha-colors
	* 
	* \return bool True, if one color have an alpha-component != 1.0, otherwise false
	*/	
	bool hasAlpha() const
	{
		return !isNearTo(m_cColor.a,    1.0) || 
			   !isNearTo(m_cAmbient.a,  1.0) ||
			   !isNearTo(m_cDiffuse.a,  1.0) ||
			   !isNearTo(m_cSpecular.a, 1.0) ||
			   !isNearTo(m_cEmissive.a, 1.0);
	}

	/*!
	* \brief Set the ambient color
	* 
	* \param cAmbient The ambient color
	*/
	void setAmbient(const Color& cAmbient)
	{
		m_cAmbient = cAmbient;	
        emit changed();
	}

	/*!
	* \brief Returns the ambient color
	* 
	* \return Color The ambient color
	*/	
	Color getAmbient() const
	{
		return m_cAmbient;	
	}
	
	/*!
	* \brief Set the diffuse color
	* 
	* \param cDiffuse The diffuse color
	*/	
	void setDiffuse(const Color& cDiffuse)
	{
		m_cDiffuse = cDiffuse;	
        emit changed();
	}

	/*!
	* \brief Returns the diffuse color
	* 
	* \return Color The diffuse color
	*/		
	Color getDiffuse() const
	{
		return m_cDiffuse;	
	}

	/*!
	* \brief Set the specular color
	* 
	* \param cSpecular The specular color
	*/
	void setSpecular(const Color& cSpecular)
	{
		m_cSpecular = cSpecular;	
        emit changed();
	}
	
	/*!
	* \brief Returns the specular color
	* 
	* \return Color The specular color
	*/		
	Color getSpecular() const
	{
		return m_cSpecular;
	}
	
	/*!
	* \brief Set the emissive color
	* 
	* \param cEmissive The emissive color
	*/	
	void setEmissive(const Color& cEmissive)
	{
		m_cEmissive = cEmissive;	
        emit changed();
	}
	
	/*!
	* \brief Returns the emissive color
	* 
	* \return Color The emissive color
	*/		
	Color getEmissive() const
	{
		return m_cEmissive;
	}

	/*!
	* \brief Set the specular factor
	* 
	* \param dSpecularFactor The specular factor
	*/		
	void setSpecularFactor(double dSpecularFactor)
	{
		m_dSpecularFactor = dSpecularFactor;
        emit changed();
	}
	
	/*!
	* \brief Returns the specular factor
	* 
	* \return double The specular factor
	*/	
	double getSpecularFactor() const
	{
		return m_dSpecularFactor;
	}
	
	/*!
	* \brief Returns the number of valid textures (TexID != -1)
	* 
	* \return int Number of valid textures
	*/	
	int getNumValidTextures() const;
	
	/*!
	* \brief Fills the array with the valid texture IDs
	* 
	* \param nTexStages The array where the valid texture IDs should be stored
	* 	Warning: The array must be allocated with the correct size, no
	* 	memory will be allocated!! The array must have at least NumValidTextures
	* 	elements.
	*/	
	void getValidTextureStages(int* nTexStages) const;
	
	/*!
	* \brief Set a texture to a specific texture stage
	* 
	* \param nTexStage The texture stage
	* \param nTexID The texture ID
	*/		
	void setTexture(int nTexStage, int nTexID);
	
	/*!
	* \brief Returns the texture ID of a specific texture stage
	* 
	* \param nTexStage The texture stage
	* \return int The texture ID
	*/		
	int getTexture(int nTexStage) const;

	/*!
	* \brief Returns the pointer to the texture state of a specific texture stage
	* 
	* \param nTexStage The texture stage where to get the stage from
	* \return TextureState* Pointer to the texture state
	*/		
	TextureState* getTextureState(int nTexStage);
	
	/*!
	* \brief Set's up a default material
	* 
	* Set's up a default material by setting a material valus to the
	* default values (name excluded!)
	*/	
	void setToDefault();

public:
	static int nMagicNumber; /*!< \brief Magic number for the material file */
	
private:
	string m_sName;		/*!< \brief The name of the material */
	Color m_cColor;		/*!< \brief The color */
	Color m_cAmbient;   /*!< \brief The ambient color */
	Color m_cDiffuse;   /*!< \brief The diffuse color */
	Color m_cSpecular;  /*!< \brief The specular color */
	Color m_cEmissive;  /*!< \brief The emissive color */
	double m_dSpecularFactor; /*!< \brief The specular color */
	
	int m_tex[MAXTEXTURES];  /*!< \brief Texture-ID for every texture stage */
	TextureState m_texState[MAXTEXTURES];  /*!< \brief Texture state for every texture stage */
                                           
signals:
	/*!
	* \brief Signal for changing a value
	*/		
	void changed();
};
	
}

#endif /*MATERIAL_H*/
