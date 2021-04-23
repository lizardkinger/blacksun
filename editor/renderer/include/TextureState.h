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
 *  File:       TextureState.h
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef TEXTURESTATE_H_
#define TEXTURESTATE_H_

#include "RendererTypes.h"

/*! \file
* 
*  \brief Describes the class TextureState, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


namespace BSRenderer
{

/*! \brief Describes texture settings for a texture stage 
*
* Describes all properties and settings that can be set to customize the
* combination of two textures
*/
class TextureState
{
public:
	/*!
	* \brief The standard constructor
	*/
	TextureState();
	
	/*!
	* \brief Sets the texture state
	* 
	* Sets all properties and settings for the textures in OpenGL
	* 
	* \param nStage The texture stage where to set the properties
	*/	
	void set(int nStage) const;
	
	/*!
	* \brief Set how the color- and alpha-values are combined
	* 
	* \param bRGBMethode true for set how to combine the color-values, false for the 
	*   alpha-values
	* \param cm Defines how to combine the color-/alpha-values
	*/	
	void setCombineMethode(bool bRGBMethode, TextureCombineMethode cm);
	
	/*!
	* \brief Return how the color- and alpha-values are combined
	* 
	* \param bRGBMethode true for return how to combine the color-values, false for the 
	*   alpha-values
	* \return TextureCombineMethode How the color-/alpha-values are combined
	*/
	TextureCombineMethode getCombineMethode(bool bRGBMethode) const;

	/*!
	* \brief Set the source of a color- or alpha-value
	* 
	* \param src The color-/alpha-value that should be set
	* \param op The source for this value
	*/	
	void setSourceCombine(TextureCombine src, TextureOperation op)
	{
		m_texOpSource[src] = op;	
	}
	
	/*!
	* \brief Returns the source of a color- or alpha-value
	* 
	* \param src The color-/alpha-value that should be returnd
	* \return TextureOperation The source for this value
	*/	
	TextureOperation getSourceCombine(TextureCombine src) const
	{
		return m_texOpSource[src];
	}

	/*!
	* \brief Set how a part of a color-/alpha-value that should be calculated
	* 
	* \param op The color-/alpha-value that should be caluclated
	* \param blend How the value should be calculated
	*/	
	void setCombineOperand(TextureCombineOperand op, TextureBlend blend)
	{
		m_texCombOperand[op] = blend;	
	}

	/*!
	* \brief Returns how a part of a color-/alpha-value that should be calculated
	* 
	* \param op The color-/alpha-value that should be returned
	* \return TextureBlend How the value should be calculated
	*/		
	TextureBlend getCombineOperand(TextureCombineOperand op) const
	{
		return m_texCombOperand[op];
	}
	
	/*!
	* \brief Set how the texture should be repeated
	* 
	* \param bRepeatU true to set the repepition of the U-coordinate, false for
	*   the V-coordinate
	* \param repeat How to repeat the texture
	*/	
	void setRepeat(bool bRepeatU, TextureRepeat repeat);
	
	/*!
	* \brief Returns how the texture should be repeated
	* 
	* \param bRepeatU true to return the repepition of the U-coordinate, false for
	*   the V-coordinate
	* \return TextureRepeat How to repeat the texture
	*/	
	TextureRepeat getRepeat(bool bRepeatU) const;
	
	/*!
	* \brief Set the color scaling
	* 
	* Set the color scaling, that can be 1, 2 or 4. If another value is passed,
	* the next value possible value is used.
	* 
	* \param nScaling The color scaling
	*/
	void setColorScaling(int nScaling);
	
	/*!
	* \brief Return the color scaling
	* 
	* \return int The color scaling
	*/	
	int getColorScaling() const
	{
		return m_nColorScaling;
	}

private:
	/*! \brief Converts a TextureCombineMethode into GLint 
	*
	* \param cm The TextureCombineMethode which should be converted
	* \return GLint The OpenGL-value
	*/	
	GLint convertToGL(TextureCombineMethode cm) const;
	
	/*! \brief Converts a TextureOperation into GLenum 
	*
	* \param op The TextureOperation which should be converted
	* \return GLenum The OpenGL-value
	*/	
	GLenum convertToGL(TextureOperation op) const;
	
	/*! \brief Converts a TextureBlend into GLenum 
	*
	* \param blend The TextureBlend which should be converted
	* \return GLenum The OpenGL-value
	*/	
	GLenum convertToGL(TextureBlend blend) const;
	
	/*! \brief Converts a TextureRepeat into GLenum 
	*
	* \param repeat The TextureRepeat which should be converted
	* \return GLenum The OpenGL-value
	*/	
	GLenum convertToGL(TextureRepeat repeat) const;

private:
	int m_nColorScaling; /*!< \brief The color scaling, can be 1, 2 or 4 */
	TextureCombineMethode m_texCombMethode[2]; /*!< \brief Defines how the color- and alpha-values are combined */
	TextureOperation m_texOpSource[MAX_TEX_COMBINE]; /*!< \brief Defines the source of each color- and alpha-value */
	TextureBlend m_texCombOperand[MAX_TEX_COMBINE_OPERAND]; /*!< \brief Defines which part of each color- and alpha-value should be taken */
	TextureRepeat m_texRepeat[2]; /*!< \brief The type of texture repetition */
};
	
}

#endif /*TEXTURESTATE_H_*/
