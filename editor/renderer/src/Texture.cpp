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
 *  File:       Texture.cpp
 *  Created:    28.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/Texture.h"

namespace BSRenderer
{

void Texture::set(int nTexStage) const
{
	if(nTexStage<0 || nTexStage>(MAXTEXTURES-1))
		nTexStage = 0;
			
	glActiveTextureARB(GL_TEXTURE0_ARB+nTexStage);
	glBindTexture(GL_TEXTURE_2D, m_nBindID);
	glEnable(GL_TEXTURE_2D);
	
}	

void Texture::release() const
{
	if(glIsTexture(m_nBindID))
	{
		GLuint nTex[] = {m_nBindID};
		glDeleteTextures(1, nTex);
	}	
}
	
}
