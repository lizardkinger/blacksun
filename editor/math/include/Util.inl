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
 *  File:       Util.inl
 *  Created:    25.10.2006
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef UTIL_INL_
#define UTIL_INL_

namespace BSMath
{
	
template<class T> T randomDecimal(const T& min, const T& max)
{
	if(min == max)
		return min;
	
	return ((std::rand() * (max - min) / RAND_MAX) + min);
}

template<class T> T randomInteger(const T& min, const T& max)
{
	if(min==max)
	{
		return min;
	}

	return((std::rand() % (max - min + 1)) + min);
}

template<class T> T degToRad(const T& deg)
{
	//Rad = Deg * (Pi / 180)
	return deg * 0.01745329251994329577;	
}

template<class T> T radToDeg(const T& rad)
{
	//Rad = Deg * (180 / Pi)
	return rad * 57.2957795130823208768;
}

template<class T> bool isPow2(const T& n)
{
	return (!n || !(n & (n - 1)));
}

template<class T> unsigned long nextPow2(const T& n)
{
	if(n<=0)
	{
		return 1;
	}
	
	unsigned long nNextPow = 1;
	while(nNextPow < (unsigned int)n)
	{
		nNextPow <<= 1;
	}		

	return nNextPow;		
}

template<class T> unsigned long prevPow2(const T& n)
{
	if(n<=0)
	{
		return 1;
	}
	
	unsigned long nPrevPow = nextPow2(n);
	if(nPrevPow == (unsigned int)n)
	{
		return nPrevPow;
	}
	
	return (nPrevPow >> 1);
}

template<class T> T cubicRoot(const T& n)
{
	return std::pow(n, 1.0/3.0);
}

template<class T> bool isNearTo(const T& a, const T& b)
{
	
	return (std::abs(a-b) <= EPSILON);	
}

template<class T> bool isNearTo(const T& a, const T& b, const T& delta)
{
	return (abs(a-b) <= delta);
}

template<class T> double calcDecade(const T& d)
{	
	T temp = abs(d);
	double v = 1.0;
	
	if(static_cast<double>(temp) > 1.0)
	{
		while(temp > v)
			v *= 10.0;
			
		v *= 0.1;
	}
	else
	{
		while(temp < v)
			v *= 0.1;
	}
	
	return v;
}
		
}

#endif /*UTIL_INL_*/
