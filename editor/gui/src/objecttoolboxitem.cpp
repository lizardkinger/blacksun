/***************************************************************************
        *   Copyright (C) 2006 by The Hunter   *
        *   hunter@localhost   *
        *                                                                         *
        *   This program is free software; you can redistribute it and/or modify  *
        *   it under the terms of the GNU General Public License as published by  *
        *   the Free Software Foundation; either version 2 of the License, or     *
        *   (at your option) any later version.                                   *
        *                                                                         *
        *   This program is distributed in the hope that it will be useful,       *
        *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
        *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
        *   GNU General Public License for more details.                          *
        *                                                                         *
        *   You should have received a copy of the GNU General Public License     *
        *   along with this program; if not, write to the                         *
        *   Free Software Foundation, Inc.,                                       *
        *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
        
#include "./../include/objecttoolboxitem.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../include/guialgoconst.h"

namespace BSGui
{
    
using namespace BSScene;

ObjectToolBoxItem::ObjectToolBoxItem(QWidget* parent)
    :BSToolBoxItem (parent)
{
    
    QVBoxLayout* firstToolBoxWidgetLayout = new QVBoxLayout;
    
    selectButtonsGroupBox = new BSGroupBox("Selection mode");
    selectButtonsGroupBox->setObjectName("selectButtonsGroupBox");
    
    modeButtonsGroupBox = new BSGroupBox("Action mode");
    modeButtonsGroupBox->setObjectName("modeButtonsGroupBox");
    
    commonObjectsGroupBox = new BSGroupBox("Common objects");
    commonObjectsGroupBox->setObjectName("commonObjectsGroupBox");
    
    modificatorGroupBox = new BSGroupBox("Modificators");
    modificatorGroupBox->setObjectName("modificatorGroupBox");
    
    objectExplorerGroupBox = new BSGroupBox("Object Explorer");
    objectExplorerGroupBox->setObjectName("objectExplorerGroupBox");

    selectButtons_ui.setupUi(selectButtonsGroupBox);
    
    setupModeButtons();
    setupObjectsButtons();
    setupModifiersButtons();
    
    objectExplorer_ui.setupUi(objectExplorerGroupBox);

    firstToolBoxWidgetLayout->addWidget(selectButtonsGroupBox);
    firstToolBoxWidgetLayout->addWidget(modeButtonsGroupBox);
    firstToolBoxWidgetLayout->addWidget(commonObjectsGroupBox);
    firstToolBoxWidgetLayout->addWidget(modificatorGroupBox);
    firstToolBoxWidgetLayout->addWidget(objectExplorerGroupBox);
    firstToolBoxWidgetLayout->addStretch();

    setLayout(firstToolBoxWidgetLayout);
    
    connect(selectButtons_ui.objectButton, SIGNAL(clicked()), this, SLOT(objectButtonClicked()));
    connect(selectButtons_ui.surfaceButton, SIGNAL(clicked()), this, SLOT(surfaceButtonClicked()));
    connect(selectButtons_ui.vertexButton, SIGNAL(clicked()), this, SLOT(vertexButtonClicked()));
    
    buttonGroup = new QButtonGroup(this);
    
    setupButtonGroups();
}

bool ObjectToolBoxItem::addWidgetToGroupBox(const QString& objectName, QWidget* widgetToAdd)
{
    bool returnValue = BSToolBoxItem::addWidgetToGroupBox(objectName, widgetToAdd);
    setupButtonGroups();
    return returnValue;
}

void ObjectToolBoxItem::insertPluginWidget(QWidget* widgetToInsert, QWidget* prevWidget)
{
    int pos = 0;
    QList<BSGroupBox*> groupBoxses = findChildren<BSGroupBox*>();
    foreach(BSGroupBox* current, groupBoxses)
    {
        if(current == prevWidget)
        {
            pos = layout()->indexOf(prevWidget) + 1;
        }
    }
    qobject_cast<QVBoxLayout*>(layout())->insertWidget(pos, widgetToInsert);
}

void ObjectToolBoxItem::setupModeButtons()
{
    QGridLayout* layout = new BSFlowLayout();
    modeButtonsGroupBox->setLayout(layout);
    
    QPushButton* selectButton = makeIconButton(QString(":/media/select_32.png"), QString("Select"));
    QPushButton* moveButton = makeIconButton(QString(":/media/move_32.png"), QString("Move"));
    QPushButton* rotateButton = makeIconButton(QString(":/media/rotate_32.png"), QString("Rotate"));
    QPushButton* scaleButton = makeIconButton(QString(":/media/scale_32.png"), QString("Scale"));
    
    layout->addWidget(selectButton, 0, 0);
    layout->addWidget(moveButton, 0, 1);
    layout->addWidget(rotateButton, 0, 2);
    layout->addWidget(scaleButton, 0, 3);
    
    connect(selectButton, SIGNAL(clicked()), this, SLOT(selectButtonClicked()));
    connect(moveButton, SIGNAL(clicked()), this, SLOT(moveButtonClicked()));
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateButtonClicked()));
    connect(scaleButton, SIGNAL(clicked()), this, SLOT(scaleButtonClicked()));

}

void ObjectToolBoxItem::setupObjectsButtons()
{
	BSFlowLayout* layout = new BSFlowLayout();
    layout->setPreferredColumnCount(4);
    commonObjectsGroupBox->setLayout(layout);
	
	QPushButton* polyButton = makeIconButton(QString(":/media/addPolygon_32.png"), QString("Polygon"));
    QPushButton* vertexButton = makeIconButton(QString(":/media/addVertex_32.png"), QString("Vertex"));
    
    layout->addWidget(polyButton, 1, 0);
    layout->addWidget(vertexButton, 1, 1);
    
    connect(polyButton, SIGNAL(clicked()), this, SLOT(createPolyButtonClicked()));
    connect(vertexButton, SIGNAL(clicked()), this, SLOT(createVertButtonClicked()));
}

void ObjectToolBoxItem::setupModifiersButtons()
{
	BSFlowLayout* layout = new BSFlowLayout();
    layout->setPreferredColumnCount(4);
    modificatorGroupBox->setLayout(layout);
}

QPushButton* ObjectToolBoxItem::makeIconButton(QString icon, QString tooltip, bool stdSize)
{
	
	QPushButton* pb = new QPushButton(QIcon(icon), "");
	
    if(stdSize)
    {
        pb->setMaximumSize(standardButtonSize);
        pb->setMinimumSize(standardButtonSize);
    }
    pb->setIconSize(standardIconSize);
    pb->setToolTip(tooltip);
    pb->setCheckable(true);
    pb->setAutoExclusive(true);
    
    return pb;
}

void ObjectToolBoxItem::setupButtonGroups()
{
    QList<QPushButton*> modeButtons = modeButtonsGroupBox->findChildren<QPushButton*>();
    foreach(QPushButton* current, modeButtons)
    {
        buttonGroup->addButton(current);
    }
    
    QList<QPushButton*> commonObjects = commonObjectsGroupBox->findChildren<QPushButton*>();
    foreach(QPushButton* current, commonObjects)
    {
        buttonGroup->addButton(current);
    }
    
    QList<QPushButton*> modificators = modificatorGroupBox->findChildren<QPushButton*>();
    foreach(QPushButton* current, modificators)
    {
        buttonGroup->addButton(current);
    }
}


void ObjectToolBoxItem::objectButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setSelectionMode(SELMODE_Mesh);
}
void ObjectToolBoxItem::surfaceButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setSelectionMode(SELMODE_Polygon);
}
void ObjectToolBoxItem::vertexButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setSelectionMode(SELMODE_Vertex);
}

void ObjectToolBoxItem::selectButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setActionMode(SCENEACTION_Selection);
}

void ObjectToolBoxItem::moveButtonClicked()
{
	SceneManager *sm = SceneManager::getInstance();
    sm->setActionMode(SCENEACTION_Move);
}

void ObjectToolBoxItem::rotateButtonClicked()
{
	SceneManager *sm = SceneManager::getInstance();
   	sm->setActionMode(SCENEACTION_Rotate);
}

void ObjectToolBoxItem::scaleButtonClicked()
{
	SceneManager *sm = SceneManager::getInstance();
   	sm->setActionMode(SCENEACTION_Scale);
}

void ObjectToolBoxItem::extrudeButtonClicked()
{
	SceneManager *sm = SceneManager::getInstance();
    sm->setActionMode(SCENEACTION_None);
}

void ObjectToolBoxItem::createPolyButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setActionMode(SCENEACTION_Creation);
    sm->setCreationMode(CREATEMODE_Polygon);
}

void ObjectToolBoxItem::createVertButtonClicked()
{
    SceneManager *sm = SceneManager::getInstance();
    sm->setActionMode(SCENEACTION_Creation);
    sm->setCreationMode(CREATEMODE_Vertex);
}

}
