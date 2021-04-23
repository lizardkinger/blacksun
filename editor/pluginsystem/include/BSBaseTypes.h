/***************************************************************************
 *   Copyright (C) 2007 by Stefan Zeltner (Xexis)
 *   szeltne@fh-landshut.de
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
 *  Module:     Pluginsystem (BlackSun)
 *  File:       BSBaseTypes.h
 *  Created:    29.03.2007
 *  Author:     Stefan Zeltner (Xexis)
 *
 **************************************************************************/
#ifndef BSBASETYPES_H_
#define BSBASETYPES_H_

#include "./../../core/include/Types.h"
#include <QString>
#include <QStringList>

namespace BSPlgMgr
{
/*! \var typedef unsigned long UID
 * \brief The type definition for the UID.
 * 
 * The UID (Unique IDentity) is a 32 bit number. In the ideal case are the
 * numbers of each class different. To reach this, it's ideal to use an hash
 * algorithm like crc32 and feed it with the content of the plugin class. An
 * online crc32 tool is on http://serversniff.de/.
 * 
 */
typedef UINT UID;

/*! \brif A class for the administration of the version.
 * 
 */
class Version
{
public:
	/*! The major release number of the version.*/
	const UINT MAJOR_RELEASE;
	/*! The minor release number of the version.*/
	const UINT MINOR_RELEASE;
	/*! The patch level number of the version.*/
	const UINT PATCH_LEVEL;
	
	/*! The constructor of Version create a constant version.
	 * 
	 * \param iMa the major release.
	 * \param iMi the minor release.
	 * \param iPl the patch level.
	 */
	Version(UINT iMa, UINT iMi, UINT iPl)
		: MAJOR_RELEASE(iMa), MINOR_RELEASE(iMi), PATCH_LEVEL(iPl) {}
	
	
	/*! The constructor of Version create a constant version from a string.
	 * 
	 * \param str the string.
	 */
	Version(const QString& str)
		: MAJOR_RELEASE(str.split(".").value(0).toUInt()),
		  MINOR_RELEASE(str.split(".").value(1).toUInt()),
		  PATCH_LEVEL(str.split(".").value(2).toUInt()) {}
	
	/*! The operator==.
	 * 
	 * \param v right side parameter
	 * \return true, if this member variables equals v one.
	 */
	bool operator== (const Version& v) const
	{
		return   (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE == v.MINOR_RELEASE)
			   && (this->PATCH_LEVEL   == v.PATCH_LEVEL);
	}
	
	/*! The operator<.
	 * 
	 * \param v right side parameter
	 * \return true, if this member variables are lower than v one.
	 */
	bool operator< (const Version& v) const
	{
		return   (this->MAJOR_RELEASE  < v.MAJOR_RELEASE)
			||	  (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE  < v.MINOR_RELEASE)
			||    (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE == v.MINOR_RELEASE)
			   && (this->PATCH_LEVEL    < v.PATCH_LEVEL);
	}

	/*! The operator>.
	 * 
	 * \param v right side parameter
	 * \return true, if this member variables are higher than v one.
	 */
	bool operator> (const Version& v) const
	{
		return   (this->MAJOR_RELEASE  > v.MAJOR_RELEASE)
			||	  (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE  > v.MINOR_RELEASE)
			||    (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE == v.MINOR_RELEASE)
			   && (this->PATCH_LEVEL    > v.PATCH_LEVEL);
	}
	
	/*! The operator>=.
	 * 
	 * \param v right side parameter
	 * \return true, if this member variables are higher than or equal v one.
	 */
	bool operator>= (const Version& v) const
	{
		return   (this->MAJOR_RELEASE  > v.MAJOR_RELEASE)
			||	  (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE  > v.MINOR_RELEASE)
			||    (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE == v.MINOR_RELEASE)
			   && (this->PATCH_LEVEL   >= v.PATCH_LEVEL);
	}
	
	/*! The operator>=.
	 * 
	 * \param v right side parameter
	 * \return true, if this member variables are lower than v one.
	 */
	bool operator<= (const Version& v) const
	{
		return   (this->MAJOR_RELEASE  < v.MAJOR_RELEASE)
			||	  (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE  < v.MINOR_RELEASE)
			||    (this->MAJOR_RELEASE == v.MAJOR_RELEASE)
			   && (this->MINOR_RELEASE == v.MINOR_RELEASE)
			   && (this->PATCH_LEVEL   <= v.PATCH_LEVEL);
	}
	
	/*! Convert the version into a string.
	 * 
	 *\return The string.
	 */
	QString toString () const
	{
		return QString().setNum(MAJOR_RELEASE) + "."
			+ QString().setNum(MINOR_RELEASE) + "."
			+ QString().setNum(PATCH_LEVEL);
	}
};

}

#endif /*BSBASETYPES_H_*/
