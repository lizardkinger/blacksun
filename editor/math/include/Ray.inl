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
 *  Module:     Math (BlackSun)
 *  File:       Ray.inl
 *  Created:    11.11.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef RAY_INL_
#define RAY_INL_

namespace BSMath
{

template<class T>
void Ray<T>::set(const Vector3<T>& vOrig, const Vector3<T>& vDir)
{
	m_vOrig = vOrig;
	m_vDir = vDir;
}

template<class T>
Ray<T> Ray<T>::getNormalize() const
{
	Ray<T> temp = (*this);
	temp.normalize();
	return temp;
}

template<class T>
Ray<T>& Ray<T>::operator =(const Ray<T>& r)
{
	m_vOrig = r.m_vOrig;
	m_vDir = r.m_vDir;
}

template<class T>
Ray<T>& Ray<T>::operator *=(const Matrix4x4<T>& m)
{
	(*this) = (*this) * m;
	return (*this);
}	

template<class T>
Ray<T> Ray<T>:: operator *(const Matrix4x4<T>& m) const
{
	Ray<T> res = (*this);
	
	Matrix4x4<T> mTemp = m;

	//Invert the translation
	res.m_vOrig.x -= mTemp._14;
	res.m_vOrig.y -= mTemp._24;
	res.m_vOrig.z -= mTemp._34;

	//Remove the translation
	mTemp._14 = mTemp._24 = mTemp._34 = static_cast<T>(0.0);

	//Invert the matrix and apply it to the ray
	mTemp.inverse();

	res.m_vOrig = res.m_vOrig * mTemp;
	res.m_vDir = res.m_vDir * mTemp;
	
}

}

#endif /*RAY_INL_*/
