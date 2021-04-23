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
 *  File:       Color.inl
 *  Created:    24.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/
 
#ifndef COLOR_INL_
#define COLOR_INL_

namespace BSMath
{
	
template<class T> 
ColorRGB<T> ColorRGB<T>::white 		= ColorRGB<T>(1.00, 1.00, 1.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::black 		= ColorRGB<T>(0.00, 0.00, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::red 			= ColorRGB<T>(1.00, 0.00, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkRed 		= ColorRGB<T>(0.50, 0.00, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::green 		= ColorRGB<T>(0.00, 1.00, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkGreen  	= ColorRGB<T>(0.00, 0.50, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::blue 		= ColorRGB<T>(0.00, 0.00, 1.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkBlue 	= ColorRGB<T>(0.00, 0.00, 0.50);
template<class T> 
ColorRGB<T> ColorRGB<T>::cyan 		= ColorRGB<T>(0.00, 1.00, 1.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkCyan 	= ColorRGB<T>(0.00, 0.50, 0.50);
template<class T> 
ColorRGB<T> ColorRGB<T>::magenta 		= ColorRGB<T>(1.00, 0.00, 1.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkMagenta 	= ColorRGB<T>(0.50, 0.00, 0.50);
template<class T> 
ColorRGB<T> ColorRGB<T>::yellow 		= ColorRGB<T>(1.00, 1.00, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkYellow 	= ColorRGB<T>(0.50, 0.50, 0.00);
template<class T> 
ColorRGB<T> ColorRGB<T>::gray 		= ColorRGB<T>(0.63, 0.63, 0.63);
template<class T> 
ColorRGB<T> ColorRGB<T>::darkGray 	= ColorRGB<T>(0.50, 0.50, 0.50);
template<class T> 
ColorRGB<T> ColorRGB<T>::lightGray 	= ColorRGB<T>(0.75, 0.75, 0.75);


template<class T>
void ColorRGB<T>::set(const T& _r, const T& _g, const T& _b, const T& _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

template<class T>
void ColorRGB<T>::set(const Vector3<T>& v, const T& _a)
{
	r = v.x;
	g = v.y;
	b = v.z;
	a = _a;
}

template<class T>
void ColorRGB<T>::set(const unsigned long& col)
{
	T mul = 1.0 / 255.0;
	
	//Split the color components from the value
	a = ((col & 0xFF000000) >> 24) * mul;
	r = ((col & 0x00FF0000) >> 16) * mul;
	g = ((col & 0x0000FF00) >> 8) * mul;
	b = ((col & 0x000000FF)) * mul;				
}

template<class T>
void ColorRGB<T>::invert(bool bInvertAlpha)
{
	r = 1.0 - r;
	g = 1.0 - g;
	b = 1.0 - b;
	if(bInvertAlpha)
		a = 1.0 - a;	
}

template<class T>
ColorRGB<T> ColorRGB<T>::getInvert(bool bInvertAlpha) const
{
	if(bInvertAlpha)
		return ColorRGB<T>(1.0 - r, 1.0 - g, 1.0 - b, 1.0 - a);
		
	return ColorRGB<T>(1.0 - r, 1.0 - g, 1.0 - b, a);	
}

template<class T>
unsigned long ColorRGB<T>::getDWORD() const
{
	T _r = r;
	T _g = g;
	T _b = b;
	T _a = a;
	
	//Clamp the values into the correct limits
	if(r<0.0) _r = 0.0;
	if(r>1.0) _r = 1.0;
	if(g<0.0) _g = 0.0;
	if(g>1.0) _g = 1.0;
	if(b<0.0) _b = 0.0;
	if(b>1.0) _b = 1.0;
	if(a<0.0) _a = 0.0;
	if(a>1.0) _a = 1.0;	
	
	//Calculate the DWORD
	return (((unsigned long)(_a * 255.0)) << 24) |
		   (((unsigned long)(_r * 255.0)) << 16) |
		   (((unsigned long)(_g * 255.0)) << 8 ) |
		   ((unsigned long)(_b * 255.0));
}

template<class T>
void ColorRGB<T>::getFloatArray(float* pArr)
{
	if(pArr!=0)
	{
		pArr[0] = static_cast<float>(r);
		pArr[1] = static_cast<float>(g);
		pArr[2] = static_cast<float>(b);
		pArr[3] = static_cast<float>(a);
	}	
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator =  (const ColorRGB<T>& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	
	return (*this);
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator += (const ColorRGB<T>& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	
	return (*this);		
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator -= (const ColorRGB<T>& c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	
	return (*this);		
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator *= (const ColorRGB<T>& c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;
	
	return (*this);		
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator *= (const T& s)
{
	r *= s;
	g *= s;
	b *= s;
	a *= s;
	
	return (*this);		
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator /= (const ColorRGB<T>& c)
{
	assert((c.r != static_cast<T>(0.0)) && (c.g != static_cast<T>(0.0)) && 
		   (c.b != static_cast<T>(0.0)) && (c.a != static_cast<T>(0.0)));
	
	r /= c.r;
	g /= c.g;
	b /= c.b;
	a /= c.a;
	
	return (*this);
}

template<class T>
ColorRGB<T>& ColorRGB<T>::operator /= (const T& s)
{
	assert(s != static_cast<T>(0.0));
	
	T mul = 1.0/s;
	r *= mul;
	g *= mul;
	b *= mul;
	a *= mul;
	
	return (*this);
}

template<class T>
ColorRGB<T> ColorRGB<T>::min(const ColorRGB<T>& c1, const ColorRGB<T>& c2)
{
	return ColorRGB<T>(std::min(c1.r, c2.r), std::min(c1.g, c2.g), 
					std::min(c1.b, c2.b), std::min(c1.a, c2.a));
}

template<class T>
ColorRGB<T> ColorRGB<T>::max(const ColorRGB<T>& c1, const ColorRGB<T>& c2)
{
	return ColorRGB<T>(std::max(c1.r, c2.r), std::max(c1.g, c2.g), 
					std::max(c1.b, c2.b), std::max(c1.a, c2.a));		
}

template<class T>
ColorRGB<T> ColorRGB<T>::random(bool bRandomAlpha)
{
	T m = 1.0 / RAND_MAX;
	
	if(bRandomAlpha)
		return ColorRGB<T>(std::rand() * m, std::rand() * m, std::rand() * m, 1.0);
	
	return ColorRGB<T>(std::rand() * m, std::rand() * m, std::rand() * m, std::rand() * m);
}
	
}

#endif /*COLOR_INL_*/
