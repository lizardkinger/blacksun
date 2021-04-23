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
 *  File:       RendererSettings.cpp
 *  Created:    06.12.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/RendererSettings.h"
#include "../include/Renderer.h"

namespace BSRenderer
{

void RendererSettings::setGlOptions()
{
	setLineWidth(getLineWidth());
	setPointSize(getPointSize());
	setLineSmoothing(getLineSmoothing());
	setPointSmoothing(getPointSmoothing());
	setPolygonSmoothing(getPolygonSmoothing());
}
	
void RendererSettings::setNearPlane(double dNearPlane)
{
	Renderer::getInstance()->setNearPlane(dNearPlane);	
}

double RendererSettings::getNearPlane() const
{
	return Renderer::getInstance()->getNearPlane();	
}

void RendererSettings::setFarPlane(double dFarPlane)
{
	Renderer::getInstance()->setFarPlane(dFarPlane);	
}

double RendererSettings::getFarPlane() const
{
	return Renderer::getInstance()->getFarPlane();
}

void RendererSettings::setFOV(double dFOV)
{
	Renderer::getInstance()->setFOV(dFOV);	
}

double RendererSettings::getFOV() const
{
	return Renderer::getInstance()->getFOV();
}
	
}
