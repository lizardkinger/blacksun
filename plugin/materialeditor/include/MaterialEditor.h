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
 *  Module:     Plugin (BlackSun)
 *  File:       MaterialEditor.h
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#ifndef MATERIALEDITOR_H_
#define MATERIALEDITOR_H_

#include <QtGui>

#include "./../../editor/renderer/include/Renderer.h"
#include <QGLWidget>
#include "../ui_MaterialEditor.h"

using namespace BSRenderer;

/*! \file
* 
*  \brief Describes the class MaterialEditor, their methodes and attributes.
*  \author Reinhard Jeschull
* 
*/


/*! \brief The material editor
*
* A material editor which allows a user to manage, load, save and delete materials.
* All material-values can be changed, the changes are shown in a preview-window.
*/
class MaterialEditor : public QWidget
{
	Q_OBJECT
	
public:
	/*! \brief Constructor 
	*
	* \param parent The parent widget
	*/
	MaterialEditor(QWidget* parent = 0);
	
	/*! \brief Refresh the material editor 
	*
	* Refresh the material editor by updating the material list and
	* showing the last selected material.
	*/	
	void refreshEditor();
	
private:
	/*! \brief Initializes the tree widget for the texture states
	* 
	* Initializes the tree widget for the textures states and insert
	* comboboxes to set the values 
	*/
	void initTextureStateTreeWidget();
	
	/*! \brief Refresh the list of available materials 
	*
	* Refresh the tree widget which stores the available materials
	*/	
	void refreshAvailableMaterials();
	
	/*! \brief Enables/Disables the widgets if needed 
	*
	* Enables all important widgets if a valid material is selected,
	* otherwise disables the widgets.
	*/	
	void checkEnableMaterialValues();
	
	/*! \brief Creates a texture list 
	*
	* Creates a texture list for the combo boxes and generates
	* a list to allocate texture-name and texture-ID
	*/	
	void createTextureList();
	
	/*! \brief Shows a specific material in the editor 
	*
	* \param nMatID The ID of the material which should be shown in
	* 		the editor
	*/	
	void showMaterialInEditor(int nMatID);
	
	/*! \brief Updates a material 
	*
	* \param nMatID The ID of the material which should be updated
	*/	
	void updateMaterial(int nMatID);
	
	/*! \brief Converts a BSMath::Color to QColor (with alpha) 
	*
	* \param c The color which should be converted
	* \return QColor The converted color 
	*/	
	QColor ColorToQColor(const Color& c)
	{
		return QColor(static_cast<int>(c.r * 255), 
					  static_cast<int>(c.g * 255), 
					  static_cast<int>(c.b * 255), 
					  static_cast<int>(c.a * 255));
	}
	
	/*! \brief Converts a QColor to BSMath::Color (with alpha) 
	*
	* \param c The color which should be converted
	* \return Color The converted color
	*/	
	Color QColorToColor(const QColor& c)
	{
		Color res(c.redF(), c.greenF(), c.blueF(), c.alphaF());
		if(isNearTo(res.a, 1.0, 0.003))
			res.a = 1.0;
		return res;
	}	

private slots:
	/*! \brief Slot for quit
	*/
	void quitEditor();

	/*! \brief Slot for clicking a material (to show them in the editor) 
	*
	* \param item The widget item thas was clicked
	* \param column The clicked column
	*/	
	void materialClicked(QTreeWidgetItem * item, int column);
	
	/*! \brief Slot for clicking/changing a texturestate-combobox 
	*
	* \param index The new index of the texturestate-combobox
	*/
	void texturestateClicked(int index);
	
	/*! \brief Slot for clicking/changing a texture-combobox 
	*
	* \param index The new index of the texture-combobox
	*/	
	void textureClicked(int index);
	
	/*! \brief Slot for changing the material name 
	*
	* \param sText The new materal name
	*/	
	
	void materialNameChanged(const QString &sText);
	
	/*! \brief Slot for changing the specular factor 
	*
	* \param d The new specular factor
	*/	
	void materialSpecularFactorChanged(double d);
	
	/*! \brief Slot for changing a material color
	*/	
	void materialColorChanged();
	
	/*! \brief Slot for clicking the 'Load'-Button
	*/	
	void loadMaterial();
	
	/*! \brief Slot for clicking the 'Save'-Button
	*/	
	void saveMaterial();
	
	/*! \brief Slot for clicking the 'Delete'-Button
	*/	
	void deleteMaterial();

	/*! \brief Slot for clicking the 'Create new'-Button
	*/		
	void newMaterial();	

private:

	/*!
	* \brief Structure to store a name-ID-pair of a texture
	*/
	struct TexItem
	{
		/*! \brief Constructor which sets the values 
		*
		* \param s The name of the texture
		* \param id The ID of the texture
		*/
		TexItem(const QString& s, int id): sName(s), nID(id) {}
		
		QString sName;	/*!< \brief The name of the texture*/
		int nID;		/*!< \brief The ID of the texture*/
	};

private:
	Ui_MaterialEditor m_ui; /*!< \brief The UI-Object*/
	
	QStringList m_listColorScaling; 	/*!< \brief List with all color scaling strings*/
	QStringList m_listCombineMethode; 	/*!< \brief List with all combine methode strings*/
	QStringList m_listOperation; 		/*!< \brief List with all texture operation strings*/
	QStringList m_listBlendRGB; 		/*!< \brief List with all RGB-blending strings*/
	QStringList m_listBlendAlpha; 		/*!< \brief List with all alpha-blending strings*/
	QStringList m_listRepeat; 			/*!< \brief List with all texture repeat strings*/
	QStringList m_listTextures; 		/*!< \brief List with all texture-names - Just for the first start*/
	
	/* 0      = Color scaling
	 * 1-2    = Combine methode (RGB, Alpha)
	 * 3-8    = Operation
	 * 9-14   = Blending (RGB, Alpha)
	 * 15-16  = Repeat
	 * 17     = Texture
	 * */
	QComboBox* m_valuesComboBox[MAXTEXTURES][18]; /*!< \brief Array with all combo boxes for every texture state*/
	
	/* 0      = Color scaling
	 * 1-2    = Combine methode (RGB, Alpha)
	 * 3-8    = Operation
	 * 9-14   = Blending (RGB, Alpha)
	 * 15-16  = Repeat
	 * 17     = MainTreeWidget (TopLevel)
	 * */
	QTreeWidgetItem* m_widgetItems[MAXTEXTURES][18];  /*!< \brief Array with all widget items for every texture state*/
	
	int m_nSelectedMaterialID;  	/*!< \brief ID of the currently selected material*/
	bool m_bEnableMaterialSlots;  	/*!< \brief Are the material slots activated?*/
	bool m_bEnableTextureSlots;  	/*!< \brief Are the texture slots activated?*/
	QList<TexItem> m_texListItems;  /*!< \brief Allocation-list of texture-name and texture-ID*/
};

#endif /*MATERIALEDITOR_H_*/
