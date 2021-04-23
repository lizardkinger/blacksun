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
 *  File:       MaterialEditor.cpp
 *  Created:    08.03.2007
 *  Author:     Reinhard Jeschull (rjeschu)
 *
 **************************************************************************/

#include "../include/MatEditPreview.h"
#include "../include/MaterialEditor.h"

MaterialEditor::MaterialEditor(QWidget* parent)
 : QWidget(parent)
{
	//Initialize the UI
	m_ui.setupUi(this);
	
	//Intialize the ID of the selected material and the slot-flags
	m_nSelectedMaterialID = -1;
	m_bEnableMaterialSlots = false;
	m_bEnableTextureSlots = false;
	
	//Initialize the tree widget for the texture states
	initTextureStateTreeWidget();  
	
	//Set the geometry of the OpenGl-preview-widget and create it
	QRect r = m_ui.previewWidget->frameGeometry();
	m_ui.gridLayout1->addWidget(m_ui.previewWidget);
	delete m_ui.previewWidget;
	m_ui.previewWidget = new MatEditPreview(m_ui.previewGroupBox, FILL_Textured, &m_nSelectedMaterialID);
	m_ui.previewWidget->setGeometry(r);
	m_ui.gridLayout1->addWidget(m_ui.previewWidget, 0, 0, 1, 1);
    
    //Connect slots
	connect(m_ui.OKButton, SIGNAL(clicked()), this , SLOT(quitEditor()));
	connect(m_ui.loadButton, SIGNAL(clicked()), this , SLOT(loadMaterial()));
	connect(m_ui.saveButton, SIGNAL(clicked()), this , SLOT(saveMaterial()));
	connect(m_ui.deleteButton, SIGNAL(clicked()), this , SLOT(deleteMaterial()));
	connect(m_ui.newButton, SIGNAL(clicked()), this , SLOT(newMaterial()));
	connect(m_ui.availableMaterialsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this , SLOT(materialClicked(QTreeWidgetItem *, int)));
	connect(m_ui.nameLineEdit, SIGNAL(textEdited(const QString &)), this , SLOT(materialNameChanged(const QString &)));
	connect(m_ui.specularFactorSpin, SIGNAL(valueChanged(double)), this , SLOT(materialSpecularFactorChanged(double)));
	
	//Connect color label slots
	connect(m_ui.colorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(materialColorChanged()));
	connect(m_ui.ambientColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(materialColorChanged()));
	connect(m_ui.diffuseColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(materialColorChanged()));
	connect(m_ui.specularColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(materialColorChanged()));
	connect(m_ui.emissiveColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(materialColorChanged()));
	
	//Refresh list with available materials
	refreshAvailableMaterials();
}

void MaterialEditor::initTextureStateTreeWidget()
{
	//Fill the lists for the QComboBoxes
	m_listColorScaling << "1";
	m_listColorScaling << "2";
	m_listColorScaling << "4";
	
	m_listCombineMethode << "Replace";
	m_listCombineMethode << "Modulate";
	m_listCombineMethode << "Add";
	m_listCombineMethode << "AddSigned";
	m_listCombineMethode << "Substract";
	m_listCombineMethode << "Interpolate";
	m_listCombineMethode << "Disable";	
	
	m_listOperation << "PrimaryColor";
	m_listOperation << "Texture";
	m_listOperation << "Constant";
	m_listOperation << "Previous";
	m_listOperation << "Disable";
	
	m_listBlendRGB << "One";
	m_listBlendRGB << "Zero";
	m_listBlendRGB << "SrcColor";
	m_listBlendRGB << "OneMinusSrcColor";

	m_listBlendAlpha << "One";
	m_listBlendAlpha << "Zero";
	m_listBlendAlpha << "SrcAlpha";
	m_listBlendAlpha << "OneMinusSrcAlpha"; 	
	
	m_listRepeat << "Repeat";
	m_listRepeat << "Clamp";
	
	m_listTextures << "-No-";
	m_listTextures << "Load from file ...";
	m_listTextures << "Wood.bmp";
	m_listTextures << "Stone.tga";
	m_listTextures << "Car.bmp";
	m_listTextures << "Stone2.tga";
	
	//Set the column header of the tree widget
	m_ui.texturestateTreeWidget->setColumnCount(2);
	
	QStringList headerText;
	headerText << "Name";
	headerText << "Value";
	m_ui.texturestateTreeWidget->setHeaderLabels(headerText);
	
	//Create all comboboxes
	for(int i=0; i<MAXTEXTURES; i++)
	{
		for(int v=0; v<18; v++)
		{
			m_valuesComboBox[i][v] = new QComboBox(NULL);	
		}
		
		m_valuesComboBox[i][0 ]->addItems(m_listColorScaling);
		m_valuesComboBox[i][1 ]->addItems(m_listCombineMethode);
		m_valuesComboBox[i][2 ]->addItems(m_listCombineMethode);
		m_valuesComboBox[i][3 ]->addItems(m_listOperation);
		m_valuesComboBox[i][4 ]->addItems(m_listOperation);
		m_valuesComboBox[i][5 ]->addItems(m_listOperation);
		m_valuesComboBox[i][6 ]->addItems(m_listOperation);
		m_valuesComboBox[i][7 ]->addItems(m_listOperation);
		m_valuesComboBox[i][8 ]->addItems(m_listOperation);
		m_valuesComboBox[i][9 ]->addItems(m_listBlendRGB);
		m_valuesComboBox[i][10]->addItems(m_listBlendRGB);
		m_valuesComboBox[i][11]->addItems(m_listBlendRGB);
		m_valuesComboBox[i][12]->addItems(m_listBlendAlpha);
		m_valuesComboBox[i][13]->addItems(m_listBlendAlpha);
		m_valuesComboBox[i][14]->addItems(m_listBlendAlpha);
		m_valuesComboBox[i][15]->addItems(m_listRepeat);
		m_valuesComboBox[i][16]->addItems(m_listRepeat);	
		m_valuesComboBox[i][17]->addItems(m_listTextures);
	}	
	
	//Build now the tree widget structure
	QList<QTreeWidgetItem*> listItems;
	for(int i=0; i<MAXTEXTURES; i++)    
    {
    	//Create top level widget item ...
    	m_widgetItems[i][17] = new QTreeWidgetItem();
    	listItems.push_back(m_widgetItems[i][17]);
    	m_widgetItems[i][17]->setText(0, "TextureState "+ QString::number(i));
    	
    	
    	//... and the childs for the top level widget item
    	 QList<QTreeWidgetItem*> childs;
    	 for(int c=0; c<17; c++)
    	 {
    	 	m_widgetItems[i][c] = new QTreeWidgetItem();
    	 	childs.push_back(m_widgetItems[i][c]);	
    	 }
    	 
    	 childs[0 ]->setText(0, "Color scaling");
    	 childs[1 ]->setText(0, "Combine RGB");
    	 childs[2 ]->setText(0, "Combine Alpha");
    	 childs[3 ]->setText(0, "ColorArg 0");
    	 childs[4 ]->setText(0, "ColorArg 1");
    	 childs[5 ]->setText(0, "ColorArg 2");
    	 childs[6 ]->setText(0, "AlphaArg 0");
    	 childs[7 ]->setText(0, "AlphaArg 1");
    	 childs[8 ]->setText(0, "AlphaArg 2");
    	 childs[9 ]->setText(0, "Blending RGB0");
    	 childs[10]->setText(0, "Blending RGB1");
    	 childs[11]->setText(0, "Blending RGB2");
    	 childs[12]->setText(0, "Blending Alpha0");
    	 childs[13]->setText(0, "Blending Alpha1");
    	 childs[14]->setText(0, "Blending Alpha2");
    	 childs[15]->setText(0, "Repeat U");
    	 childs[16]->setText(0, "Repeat V");    	     	     	 
    	 
    	 //Insert children to the top level widget item
    	 listItems[i]->insertChildren(0, childs);
    }
	
	//Add all top level widget items
    m_ui.texturestateTreeWidget->addTopLevelItems(listItems);
    
    //Add a QComboBox to all tree view widget items
    for(int i=0; i<MAXTEXTURES; i++)
    {
    	//Set the combo box to choose the texture ...
		m_ui.texturestateTreeWidget->setItemWidget(
			m_widgetItems[i][17], 1, m_valuesComboBox[i][17]);
			
		//... and connect it with a slot
		connect(m_valuesComboBox[i][17], SIGNAL(currentIndexChanged(int)), this , SLOT(textureClicked(int)));
    	
    	//Set the combo boxes for all texture state settings and connect it with a slot
    	for(int c=0; c<17; c++)
    	{
    		 m_ui.texturestateTreeWidget->setItemWidget(
    		 	m_widgetItems[i][c], 1, m_valuesComboBox[i][c]);
			connect(m_valuesComboBox[i][c], SIGNAL(currentIndexChanged(int)), this , SLOT(texturestateClicked(int)));	
    	}
    }
    
    //Set the column width
    m_ui.texturestateTreeWidget->setColumnWidth(0, m_ui.texturestateTreeWidget->width()/2);	
}

void MaterialEditor::quitEditor()
{
	setVisible(false);	
}

void MaterialEditor::refreshEditor()
{
	//Refresh the list of available materials and show the current material
	createTextureList();
	refreshAvailableMaterials();
	showMaterialInEditor(m_nSelectedMaterialID);	
}

void MaterialEditor::refreshAvailableMaterials()
{
	//Reset the material tree widget
	MaterialManager* matMgr = Renderer::getInstance()->getMaterialManager();
	m_ui.availableMaterialsTreeWidget->clear();
	m_ui.availableMaterialsTreeWidget->setColumnCount(2);
	m_ui.availableMaterialsTreeWidget->setRootIsDecorated(false);
	
	//Set the header of the material tree widget
	QStringList headerText;
	headerText << "ID";
	headerText << "Name";
	m_ui.availableMaterialsTreeWidget->setHeaderLabels(headerText);	
		
	//Walk through all materials
	MaterialIterator it;
	bool bFoundSelected = false;
	int nCount = 0;
	for(it = matMgr->begin(); it != matMgr->end(); it++)
	{
		//Create a new widget item for the material ...
		QTreeWidgetItem* newItem = new QTreeWidgetItem();
		
		//... and set the material data
		newItem->setText(0, QString::number(it->first));
		newItem->setText(1, QString(it->second->getName().c_str()));
				
		if(it->first == m_nSelectedMaterialID)
			bFoundSelected = true;
		
		//... then add the new item to the widget
		m_ui.availableMaterialsTreeWidget->addTopLevelItem(newItem);
		
		nCount++;
	}
	
	//No materials available or the previous selected material is'n valid
	//anymore, so deaktive the material
	if(bFoundSelected==false || nCount==0)
	{
		m_nSelectedMaterialID = -1;
	}
	
	//Set the column-width of the ID
	m_ui.availableMaterialsTreeWidget->setColumnWidth(0, 20);
	
	//Check, if the widgets must be enabled or disabled
	checkEnableMaterialValues();
}

void MaterialEditor::texturestateClicked(int)
{
	//Execute the slot?
	if(m_bEnableMaterialSlots)
	{
		//Update the selected material
		updateMaterial(m_nSelectedMaterialID);
	}
}

void MaterialEditor::textureClicked(int index)
{
	//Is a valid material selected and the slot ready to execute?
	if(m_nSelectedMaterialID == -1 || m_bEnableTextureSlots == false)
		return;
	
	//The item 'Load from file' was selected
	if(index == 1)
	{
		TextureManager* texMgr = Renderer::getInstance()->getTextureManager();
		QString filter;
		QString allFilter;
		
		//Walk through all available texture loader plugins and built up the file-filter
		//for example:  "All supported (*.bmp *.jpg);;Bitmap (*.bmp);;JPEG-File (*.jpg)"
		TextureLoaderPluginIterator it;
		int nCount = 0;
		for(it = texMgr->beginTextureLoaderPlugins(); it != texMgr->endTextureLoaderPlugins(); it++)
		{
			//Add a seperator-string if needed
			if(nCount != 0)
				filter += ";;";
			
			//Append a new filter-substring
			filter += it->second->getFileDescription().c_str();
			filter += " (*.";
			filter += it->second->getFileExtension().c_str();
			filter += ")";
			
			if(nCount != 0)
				allFilter += " ";
			allFilter += "*.";
			allFilter += it->second->getFileExtension().c_str();

			nCount++;
		}		
		
		//Texture loader plugins found?
		if(nCount==0)
		{
			//Show error message box
			QMessageBox::critical(this, "Texture error", "Unable to load textures, because there are no texture loader plugins loaded");	
		}
		else
		{
			//Add a file-filter to show all supported file-types
			filter = "All supported (" + allFilter + ");;" + filter;
			
			//Show the open-file-dialog 
			QStringList sFile = QFileDialog::getOpenFileNames(this,
				"Load texture file", QDir::currentPath(),
				filter);
				
			//Load all textures
			if(sFile.count() != 0)
			{
				for(int s=0; s<sFile.size(); s++)
				{
					texMgr->loadTexture(sFile[s].toStdString());
				}	
			}	
		}		
	}
	else
	{		
		MaterialManager* matMgr = Renderer::getInstance()->getMaterialManager();
		Material* pMat = matMgr->getMaterial(m_nSelectedMaterialID);
		
		//Walk through all texture stages and set the material with the texture-item-array	
		for(int i=0; i<MAXTEXTURES; i++)
		{
			pMat->setTexture(i, m_texListItems[m_valuesComboBox[i][17]->currentIndex()].nID);
		}
	}
	
	//Create a new texture list
	createTextureList();
	//Refresh the window by showing the material in the editor
	showMaterialInEditor(m_nSelectedMaterialID);
	
}

void MaterialEditor::createTextureList()
{
	TextureManager* texMgr = Renderer::getInstance()->getTextureManager();
	
	//Clear the list to store the allocation of item-name and texture-ID 
	m_texListItems.clear();
	m_texListItems.push_back(TexItem("-No-", -1));
	m_texListItems.push_back(TexItem("Load from file ...", -1));
	
	//Walk through all textures
	TextureIterator it;
	for(it = texMgr->beginTextures(); it != texMgr->endTextures(); it++)
	{		
		//Add a new element to the allocation-array
		m_texListItems.push_back(TexItem(it->second.getTextureInfo()->sName.c_str(),
			it->first));
		
		//Split file name from path (for example: "C:/Sample/Pic.bmp" => "Pic.bmp")
		QStringList list = m_texListItems[m_texListItems.size()-1].sName.split("/");
		m_texListItems[m_texListItems.size()-1].sName = list[list.size()-1];
		list = m_texListItems[m_texListItems.size()-1].sName.split("\\");
		m_texListItems[m_texListItems.size()-1].sName = list[list.size()-1];
	}
	
	//Deactive the slot
	m_bEnableTextureSlots = false;
	for(int i=0; i<MAXTEXTURES; i++)
	{
		//Delete all items of the combo box
		m_valuesComboBox[i][17]->clear();
		
		//Insert all elements of the allocation-array in the combo box
		for(int t = 0; t < m_texListItems.size(); t++)
		{ 
			m_valuesComboBox[i][17]->addItem(m_texListItems[t].sName);
		}	
	}
	//Reactive the slot
	m_bEnableTextureSlots = true;
}

void MaterialEditor::materialNameChanged(const QString&)
{
	//Execute the slot?
	if(m_bEnableMaterialSlots)
	{
		//Update the material and refresh the list
		updateMaterial(m_nSelectedMaterialID);
		refreshAvailableMaterials();
	}
}

void MaterialEditor::materialSpecularFactorChanged(double)
{
	//Execute the slot?
	if(m_bEnableMaterialSlots)
	{
		//Update the material
		updateMaterial(m_nSelectedMaterialID);
	}		
}

void MaterialEditor::materialClicked(QTreeWidgetItem * item, int)
{
	//Deactive the slot
	m_bEnableMaterialSlots = false;
	
	//Get the material ID from the selected item
	m_nSelectedMaterialID = item->text(0).toInt();
	
	//Check, if the widgets must be enabled or disabled
	checkEnableMaterialValues();
	
	//Is it a valid material?
	if(m_nSelectedMaterialID != -1)
	{
		//Create new texture list
		createTextureList();
		//Show the selected material
		showMaterialInEditor(m_nSelectedMaterialID);	
	}
	
	//Reactivate the slot
	m_bEnableMaterialSlots = true;
}

void MaterialEditor::materialColorChanged()
{
	//Execute the slot
	if(m_bEnableMaterialSlots)
	{
		//Update the material
		updateMaterial(m_nSelectedMaterialID);
	}			
}

void MaterialEditor::checkEnableMaterialValues()
{
	//Enable the widgets against the the selected material
	m_ui.texturestateGroupBox->setEnabled(m_nSelectedMaterialID != -1);
	m_ui.generalGroupBox->setEnabled(m_nSelectedMaterialID != -1);
	m_ui.saveButton->setEnabled(m_nSelectedMaterialID != -1);
}

void MaterialEditor::loadMaterial()
{
	//Open the open-file dialog
	QStringList files = QFileDialog::getOpenFileNames(this,
		"Load material file", QDir::currentPath(),
		"Blacksun material file (*.bsmaterial)");

	//Load all materials	
	bool bNewMat = false;
 	for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
 	{
		int nNewMaterial = Renderer::getInstance()->getMaterialManager()->loadMaterial(-1, (*it).toStdString());
		
		//Select the last material
		if(nNewMaterial != -1)
		{
			m_nSelectedMaterialID = nNewMaterial;
			bNewMat = true;
		}
	}
	
	//Refresh the list of materials
	refreshAvailableMaterials();
	checkEnableMaterialValues();
	
	//A new material loaded?
	if(bNewMat)
	{
		//Show it in the editor
		showMaterialInEditor(m_nSelectedMaterialID);	
	}
}

void MaterialEditor::saveMaterial()
{
	//Is it a valid material-ID?
	if(m_nSelectedMaterialID == -1)
		return;
		
	//Show the save-file-dialog
	QString file = QFileDialog::getSaveFileName(this,
		"Save material file", QDir::currentPath(),
		"Blacksun material file (*.bsmaterial)");
	
	if(file != "")
	{
		//Save the materal
		Renderer::getInstance()->getMaterialManager()->saveMaterial(m_nSelectedMaterialID, file.toStdString());
	}
}

void MaterialEditor::deleteMaterial()
{
	//Is it a valid material-ID?
	if(m_nSelectedMaterialID == -1)
		return;
		
	//Remove material
	Renderer::getInstance()->getMaterialManager()->removeMaterial(m_nSelectedMaterialID);
	
	//Refresh the material-list
	refreshAvailableMaterials();
	
	//Select the first material (if possible)
	QTreeWidgetItem* pItem = m_ui.availableMaterialsTreeWidget->topLevelItem(0);
	if(pItem == NULL)
	{
		m_nSelectedMaterialID = -1;	
	}
	else
	{
		m_nSelectedMaterialID = pItem->text(0).toInt();
	}
	
	//Show the materal in the editor
	checkEnableMaterialValues();
	showMaterialInEditor(m_nSelectedMaterialID);
}

void MaterialEditor::newMaterial()
{
	bool bOK;
	//Show dialog to enter a material name
	QString text = QInputDialog::getText(this, tr("Material name"),
		tr("Please enter a material name:"), QLineEdit::Normal, "unnamed", &bOK);
		
	if(!bOK || text.isEmpty())
		return; 
         
	int nNewMaterial = Renderer::getInstance()->getMaterialManager()->createNewMaterial(-1);
	Renderer::getInstance()->getMaterialManager()->getMaterial(nNewMaterial)->setName(text.toStdString());
	bool bNewMat = false;
	
	//Select new materal, if no material is currently selected
	if(m_nSelectedMaterialID == -1)
	{
		bNewMat = true;
		m_nSelectedMaterialID = nNewMaterial;
	}

	//Refresh the list of materials
	refreshAvailableMaterials();
	checkEnableMaterialValues();	
	
	//A new material loaded?
	if(bNewMat)
	{
		//Show it in the editor
		showMaterialInEditor(m_nSelectedMaterialID);	
	}
}

void MaterialEditor::showMaterialInEditor(int nMatID)
{	
	//Is it a valid texture
	if(nMatID < -1)
		return;
		
	MaterialManager* matMgr = Renderer::getInstance()->getMaterialManager();
	Material* pMat = matMgr->getMaterial(nMatID);
	
	//Set the general material settings
	m_ui.nameLineEdit->setText(pMat->getName().c_str());
	m_ui.colorLabel->setColor(ColorToQColor(pMat->getColor()));
	m_ui.ambientColorLabel->setColor(ColorToQColor(pMat->getAmbient()));
	m_ui.diffuseColorLabel->setColor(ColorToQColor(pMat->getDiffuse()));
	m_ui.specularColorLabel->setColor(ColorToQColor(pMat->getSpecular()));
	m_ui.emissiveColorLabel->setColor(ColorToQColor(pMat->getEmissive()));
	m_ui.specularFactorSpin->setValue(pMat->getSpecularFactor());

	//Set texture and texture states
	for(int t=0; t<MAXTEXTURES; t++)
	{
		TextureState* pState = pMat->getTextureState(t);
		
		//Color scaling			
		if(pState->getColorScaling()==1)
		{
			m_valuesComboBox[t][0]->setCurrentIndex(0);
		}
		else if(pState->getColorScaling()==2)
		{
			m_valuesComboBox[t][0]->setCurrentIndex(1);
		}
		else
		{
			m_valuesComboBox[t][0]->setCurrentIndex(2);
		}
		
		//Combine methode (RGB, Alpha)
		m_valuesComboBox[t][1 ]->setCurrentIndex(static_cast<int>(pState->getCombineMethode(true )));
		m_valuesComboBox[t][2 ]->setCurrentIndex(static_cast<int>(pState->getCombineMethode(false)));
		
		//Texture operation
		m_valuesComboBox[t][3 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_ColorArg0)));
		m_valuesComboBox[t][4 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_ColorArg1)));
		m_valuesComboBox[t][5 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_ColorArg2)));
		m_valuesComboBox[t][6 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_AlphaArg0)));
		m_valuesComboBox[t][7 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_AlphaArg1)));
		m_valuesComboBox[t][8 ]->setCurrentIndex(static_cast<int>(pState->getSourceCombine(TEXCOMB_AlphaArg2)));
		
		//Texture blending (RGB)
		m_valuesComboBox[t][9 ]->setCurrentIndex(static_cast<int>(pState->getCombineOperand(TEXCOMBOP_RGB0)));
		m_valuesComboBox[t][10]->setCurrentIndex(static_cast<int>(pState->getCombineOperand(TEXCOMBOP_RGB1)));
		m_valuesComboBox[t][11]->setCurrentIndex(static_cast<int>(pState->getCombineOperand(TEXCOMBOP_RGB2)));


		//Texture blending (Alpha)
		int nVal1 = static_cast<int>(pState->getCombineOperand(TEXCOMBOP_Alpha0));
		int nVal2 = static_cast<int>(pState->getCombineOperand(TEXCOMBOP_Alpha1));
		int nVal3 = static_cast<int>(pState->getCombineOperand(TEXCOMBOP_Alpha2));
		if(nVal1 > 3) nVal1 -= 2;
		if(nVal2 > 3) nVal2 -= 2;
		if(nVal3 > 3) nVal3 -= 2;
		
		m_valuesComboBox[t][12]->setCurrentIndex(nVal1);
		m_valuesComboBox[t][13]->setCurrentIndex(nVal2);
		m_valuesComboBox[t][14]->setCurrentIndex(nVal3);
		
		//Texture repeat
		m_valuesComboBox[t][15]->setCurrentIndex(static_cast<int>(pState->getRepeat(true )));
		m_valuesComboBox[t][16]->setCurrentIndex(static_cast<int>(pState->getRepeat(false)));
		
		//Set texture
		m_bEnableTextureSlots = false; //Deactive the slot
		int nTex = pMat->getTexture(t);
		int nComboBoxIndex = 0;
		for(int tex=0; tex<m_texListItems.size(); tex++)
		{ 
			if(m_texListItems[tex].nID == nTex)
			{
				nComboBoxIndex = tex;
				break;	
			}
		}
		
		m_valuesComboBox[t][17]->setCurrentIndex(nComboBoxIndex);
		
		m_bEnableTextureSlots = true; //Reactive the slot
	}	
}

void MaterialEditor::updateMaterial(int nMatID)
{
	if(nMatID < 0)
		return;
		
	MaterialManager* matMgr = Renderer::getInstance()->getMaterialManager();
	Material* pMat = matMgr->getMaterial(nMatID);
	
	//Update general settings
	pMat->setName(m_ui.nameLineEdit->text().toStdString());
	pMat->setColor(QColorToColor(m_ui.colorLabel->getColor()));
	pMat->setAmbient(QColorToColor(m_ui.ambientColorLabel->getColor()));
	pMat->setDiffuse(QColorToColor(m_ui.diffuseColorLabel->getColor()));
	pMat->setSpecular(QColorToColor(m_ui.specularColorLabel->getColor()));
	pMat->setEmissive(QColorToColor(m_ui.emissiveColorLabel->getColor()));
	pMat->setSpecularFactor(m_ui.specularFactorSpin->value());
	
	//Set texture and texture states
	for(int t=0; t<MAXTEXTURES; t++)
	{
		TextureState* pState = pMat->getTextureState(t);
		
		//Color scaling
		if(m_valuesComboBox[t][0]->currentIndex()==0)
		{
			pState->setColorScaling(1);
		}
		else if(m_valuesComboBox[t][0]->currentIndex()==1)
		{
			pState->setColorScaling(2);	
		}
		else
		{
			pState->setColorScaling(4);
		}
		
		//Combine methode (RGB, Alpha)
		pState->setCombineMethode(true,  static_cast<TextureCombineMethode>(m_valuesComboBox[t][1 ]->currentIndex()));
		pState->setCombineMethode(false, static_cast<TextureCombineMethode>(m_valuesComboBox[t][2 ]->currentIndex()));
		
		//Texture operation
		pState->setSourceCombine(TEXCOMB_ColorArg0, static_cast<TextureOperation>(m_valuesComboBox[t][3 ]->currentIndex()));
		pState->setSourceCombine(TEXCOMB_ColorArg1, static_cast<TextureOperation>(m_valuesComboBox[t][4 ]->currentIndex()));
		pState->setSourceCombine(TEXCOMB_ColorArg2, static_cast<TextureOperation>(m_valuesComboBox[t][5 ]->currentIndex()));
		pState->setSourceCombine(TEXCOMB_AlphaArg0, static_cast<TextureOperation>(m_valuesComboBox[t][6 ]->currentIndex()));
		pState->setSourceCombine(TEXCOMB_AlphaArg1, static_cast<TextureOperation>(m_valuesComboBox[t][7 ]->currentIndex()));
		pState->setSourceCombine(TEXCOMB_AlphaArg2, static_cast<TextureOperation>(m_valuesComboBox[t][8 ]->currentIndex()));
	
		//Texture blending (RGB)
		pState->setCombineOperand(TEXCOMBOP_RGB0, static_cast<TextureBlend>(m_valuesComboBox[t][9 ]->currentIndex()));
		pState->setCombineOperand(TEXCOMBOP_RGB1, static_cast<TextureBlend>(m_valuesComboBox[t][10]->currentIndex()));
		pState->setCombineOperand(TEXCOMBOP_RGB2, static_cast<TextureBlend>(m_valuesComboBox[t][11]->currentIndex()));
		
		//Texture blending (Alpha)
		int nVal1 = m_valuesComboBox[t][12]->currentIndex();
		int nVal2 = m_valuesComboBox[t][13]->currentIndex();
		int nVal3 = m_valuesComboBox[t][14]->currentIndex();
		if(nVal1 > 1) nVal1 += 2;
		if(nVal2 > 1) nVal2 += 2;
		if(nVal3 > 1) nVal3 += 2;
		
		pState->setCombineOperand(TEXCOMBOP_Alpha0, static_cast<TextureBlend>(nVal1));
		pState->setCombineOperand(TEXCOMBOP_Alpha1, static_cast<TextureBlend>(nVal2));
		pState->setCombineOperand(TEXCOMBOP_Alpha2, static_cast<TextureBlend>(nVal3));
		
		//Texture repeat
		pState->setRepeat(true,  static_cast<TextureRepeat>(m_valuesComboBox[t][15]->currentIndex()));
		pState->setRepeat(false, static_cast<TextureRepeat>(m_valuesComboBox[t][16]->currentIndex()));		
		
		//Set texture
		//pMat->setTexture(t, m_texListItems[m_valuesComboBox[t][17]->currentIndex()].nID);
	}
}
