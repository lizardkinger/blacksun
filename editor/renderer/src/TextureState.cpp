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
 *  File:       TextureState.cpp
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/TextureState.h"

namespace BSRenderer
{
	
TextureState::TextureState()
{
	//The U and V-coordinate of the texture should be repeated
	m_texRepeat[0] = TEXREP_Repeat;
	m_texRepeat[1] = TEXREP_Repeat;
	
	//RGB-Texture enabled (modulate it), Alpha too
	m_texCombMethode[0] = TEXMET_Modulate;
	m_texCombMethode[1] = TEXMET_Disable;
	
	m_texOpSource[TEXCOMB_ColorArg0] = TEXOP_Texture;
	m_texOpSource[TEXCOMB_ColorArg1] = TEXOP_Previous;
	m_texOpSource[TEXCOMB_ColorArg2] = TEXOP_Disable;
	
	m_texOpSource[TEXCOMB_AlphaArg0] = TEXOP_Disable;
	m_texOpSource[TEXCOMB_AlphaArg1] = TEXOP_Disable;
	m_texOpSource[TEXCOMB_AlphaArg2] = TEXOP_Disable;
	
	m_texCombOperand[TEXCOMBOP_RGB0] = TEXBLEND_SrcColor;
	m_texCombOperand[TEXCOMBOP_RGB1] = TEXBLEND_SrcColor;
	m_texCombOperand[TEXCOMBOP_RGB2] = TEXBLEND_SrcColor;
	
	m_texCombOperand[TEXCOMBOP_Alpha0] = TEXBLEND_SrcAlpha;
	m_texCombOperand[TEXCOMBOP_Alpha1] = TEXBLEND_SrcAlpha;
	m_texCombOperand[TEXCOMBOP_Alpha2] = TEXBLEND_SrcAlpha;	
	
	m_nColorScaling = 1;
}

void TextureState::set(int nStage) const
{
	glActiveTextureARB(GL_TEXTURE0_ARB+nStage);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	
	//Set the repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertToGL(m_texRepeat[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convertToGL(m_texRepeat[1]));
	
	//Set the color combine settings
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, convertToGL(m_texCombMethode[0]));
		
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, convertToGL(m_texOpSource[TEXCOMB_ColorArg0]));
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, convertToGL(m_texOpSource[TEXCOMB_ColorArg1]));
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, convertToGL(m_texOpSource[TEXCOMB_ColorArg2]));

	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_RGB0]));
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_RGB1]));
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_RGB2]));

	//Set the alpha combine settings
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, convertToGL(m_texCombMethode[1]));
	
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, convertToGL(m_texOpSource[TEXCOMB_AlphaArg0]));
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, convertToGL(m_texOpSource[TEXCOMB_AlphaArg1]));
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, convertToGL(m_texOpSource[TEXCOMB_AlphaArg2]));

	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_Alpha0]));
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_Alpha1]));
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, convertToGL(m_texCombOperand[TEXCOMBOP_Alpha2]));



	//Set color-scaling
	glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, m_nColorScaling);	
}

void TextureState::setCombineMethode(bool bRGBMethode, TextureCombineMethode cm)
{
	if(bRGBMethode)
	{
		m_texCombMethode[0] = cm;
	}
	else
	{
		m_texCombMethode[1] = cm;
	}
}

TextureCombineMethode TextureState::getCombineMethode(bool bRGBMethode) const
{
	if(bRGBMethode)
	{
		return m_texCombMethode[0];
	}
		
	return m_texCombMethode[1];
}

void TextureState::setRepeat(bool bRepeatU, TextureRepeat repeat)
{
	if(bRepeatU)
	{
		m_texRepeat[0] = repeat;
	}
	else
	{
		m_texRepeat[1] = repeat;	
	}
		
}

TextureRepeat TextureState::getRepeat(bool bRepeatU) const
{
	if(bRepeatU)
	{
		return m_texRepeat[0];	
	}
	
	return m_texRepeat[1];
}

void TextureState::setColorScaling(int nScaling)
{
	if(nScaling == 1 || nScaling == 2 || nScaling == 3)
	{
		m_nColorScaling = nScaling;	
	}
	else if(nScaling>=4)
	{
		m_nColorScaling = 4;
	}
	else if(nScaling <=0)
	{
		m_nColorScaling = 0;	
	}
	else
	{
		m_nColorScaling = prevPow2(nScaling);
	}
}

GLint TextureState::convertToGL(TextureCombineMethode cm) const
{
	switch(cm)
	{
		case TEXMET_Replace:	return GL_REPLACE;
		case TEXMET_Modulate:	return GL_MODULATE;
		case TEXMET_Add:		return GL_ADD;
		case TEXMET_AddSigned:	return GL_ADD_SIGNED_ARB;	
		case TEXMET_Subtract:	return GL_SUBTRACT_ARB;
		case TEXMET_Interpolate: return GL_INTERPOLATE_ARB;
		case TEXMET_Disable:	return GL_MODULATE;
	}

	return GL_ZERO;
}

GLenum TextureState::convertToGL(TextureOperation op) const
{
	switch(op)
	{
		case TEXOP_PrimaryColor: return GL_PRIMARY_COLOR_ARB;
		case TEXOP_Texture:		 return GL_TEXTURE;
		case TEXOP_Constant:	 return GL_CONSTANT_ARB;
		case TEXOP_Previous:	 return GL_PREVIOUS_ARB;
		case TEXOP_Disable:		 return GL_ZERO;
	}
	
	return GL_ZERO;	
}

GLenum TextureState::convertToGL(TextureBlend blend) const
{
	switch(blend)
	{
		case TEXBLEND_One:				return GL_ONE;
		case TEXBLEND_Zero:				return GL_ZERO;
		case TEXBLEND_SrcColor:			return GL_SRC_COLOR;
		case TEXBLEND_OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
		case TEXBLEND_SrcAlpha:			return GL_SRC_ALPHA;
		case TEXBLEND_OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	}
	
	return GL_SRC_COLOR;		
}

GLenum TextureState::convertToGL(TextureRepeat repeat) const
{
	switch(repeat)
	{
		case TEXREP_Repeat: return GL_REPEAT;
		case TEXREP_Clamp:	return GL_CLAMP;
	}
	
	return GL_REPEAT;
}

}
