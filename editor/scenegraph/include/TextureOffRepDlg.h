/***************************************************************************
 *   Copyright (C) 2006 by Thomas Kuhndörfer
 *   tkuhndo@fh-landshut.de
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
 *  Module:     SceneGraph (BlackSun)
 *  File:       TextureOffRepDlg.h
 *  Created:    03.04.2007
 *  Author:     Thomas Kuhndörfer (tkuhndo)
 *
 **************************************************************************/
 
#ifndef TEXTURECOORDINATES_H_
#define TEXTURECOORDINATES_H_

///////////////////////////////////////////////////////////
// Blacksun-Includes for Qt
///////////////////////////////////////////////////////////
#include <QtGui>

///////////////////////////////////////////////////////////
// Blacksun-Includes for corresponding Qt-Dialog
///////////////////////////////////////////////////////////
#include "./../ui_TextureOffRepDlg.h"
        
namespace BSScene
{
       
/*! \brief The TextureOffRepDlg object 
*
* A concrete class that represents the dialog for setting the repeat and offset values
* 
*/ 
class TextureOffRepDlg : public QDialog
{
    Q_OBJECT
            
public:
    
	/*!
	* \brief Constructor
	* 
	*/	    
    TextureOffRepDlg();
    
	/*!
	* \brief Deonstructor
	* 
	*/	    
    virtual ~TextureOffRepDlg();
    
    /*!
	* \brief Set the texture coordinate for the selected objects
	* 
	* \param offU The U offset value 
	* \param offV The V offset value 
	* \param repU The U repeat value 
	* \param repV The V repeat value 
	*/	
	void setCoordinates(const double offU, const double offV, const double repU,const  double repV);
	
	
    /*!
	* \brief Get the texture coordinate for the selected objects
	* 
	* \param offU The U offset value 
	* \param offV The V offset value 
	* \param repU The U repeat value 
	* \param repV The V repeat value 
	*/		
	void getCoordinates(double *offU, double *offV, double *repU, double *repV) const;
	  
private:
    /*!
	* \brief Calls the scenegraph for redrawing the scene
	*/
	void updateScene();  
    
public slots:

    /*!
	* \brief Slot that will be called if the corresponding Offset-U value has changed
	* 
	* \param val The new value, changed by the user within the dialog
	*/
	void offsetUChanged(int val);
	
	
    /*!
	* \brief Slot that will be called if the corresponding Offset-V value has changed
	* 
	* \param val The new value, changed by the user within the dialog
	*/	
	void offsetVChanged(int val);


    /*!
	* \brief Slot that will be called if the corresponding Repeat-U value has changed
	* 
	* \param val The new value, changed by the user within the dialog
	*/
	void repeatUChanged(int val);
	
    /*!
	* \brief Slot that will be called if the corresponding Repeat-V value has changed
	* 
	* \param val The new value, changed by the user within the dialog
	*/	
	void repeatVChanged(int val);    
	

private:
	Ui_TextureOffRepDlg m_ui; /*!< \brief The UI-Object*/   

	double m_textureOffset[2];      /*!< \brief Saved values for the offset */ 
	double m_textureRepeat[2];   	/*!< \brief Saved values for the repeat */ 
	 
};

}

#endif /*TEXTURECOORDINATES_H_*/
