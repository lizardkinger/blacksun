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

#include "../include/configbase.h"
#include <iostream>
namespace BSGui
{
ConfigBase::ConfigBase()
    :QDialog()
{
    ui.setupUi(this);
    
    deaktivateResetButton();
    deaktivateApplyButton();

    ui.configList->setViewMode(QListView::IconMode);
    ui.configList->setMovement(QListView::Static);
    ui.configList->setMaximumWidth(140);
    ui.configList->setMinimumWidth(140);
    ui.configList->setWrapping(false);
    ui.configList->setFlow(QListView::TopToBottom);
    
    addNewConfiguration(new PluginManager);
    addNewConfiguration(new StyleConfig);
    addNewConfiguration(new RendererSettingsDialog);
    addNewConfiguration(new MouseConfig);
    
    connect(ui.configList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), 
            this, SLOT(displayItem(QListWidgetItem*, QListWidgetItem*)));
    
}

void ConfigBase::displayItem(QListWidgetItem* current, QListWidgetItem* previous)
{
    
    if (!current)
    {
        current = previous;
    }

    bool goToNextPage = true;

    int previousIndex = ui.configList->row(previous);
    if (previousIndex >= 0)
    {
        if (configInterfaceList.at(previousIndex)->isModified())
        {
            goToNextPage = saveChanges();
            if (!goToNextPage)
            {
                ui.configList->setCurrentItem(previous);
            }
        }
    }
    if (goToNextPage)
    {
        ui.settingsLabel->setText("<b>" +
        configInterfaceList.at(ui.configList->row(current))->getDescription() + "</b>");
    
        ui.configWidgets->setCurrentIndex(ui.configList->row(current));
    }
}

void ConfigBase::aktivateResetButton()
{
    ui.buttonBox->button(QDialogButtonBox::Reset)->setEnabled(true);
}

void ConfigBase::deaktivateResetButton()
{
    ui.buttonBox->button(QDialogButtonBox::Reset)->setEnabled(false);
}

void ConfigBase::aktivateApplyButton()
{
    ui.buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void ConfigBase::deaktivateApplyButton()
{
    ui.buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void ConfigBase::currentDialogModified(bool status)
{
    if (status)
    {
        aktivateResetButton();
        aktivateApplyButton();
    }
    else
    {
        deaktivateResetButton();
        deaktivateApplyButton();
    }
}

bool ConfigBase::saveChanges()
{
    int currentIndex = ui.configWidgets->currentIndex();
    Q_ASSERT(configInterfaceList.at(currentIndex)->isModified());

    int result = QMessageBox::question(this, tr("Save Changes?"), tr("Do you want to save the changes in this dialog?"), QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Cancel);
    
    if(result & QMessageBox::Cancel)
    {
        return false;
    }
    else if (result & QMessageBox::Apply)
    {
        dialogAccepted();
        return true;
    }
    else
    {
        resetCurrentDialog();
        return true;
    }
}

void ConfigBase::dialogAccepted()
{
    int currentIndex = ui.configWidgets->currentIndex();
    if (currentIndex >= 0)
    {
        configInterfaceList.at(currentIndex)->dialogAccept();
    }
}

void ConfigBase::resetCurrentDialog()
{
    int currentIndex = ui.configWidgets->currentIndex();
    if (currentIndex >= 0)
    {
        configInterfaceList.at(currentIndex)->reset();
    }
}

void ConfigBase::addNewConfiguration(ConfigInterface* interfaceToAdd)
{
    configInterfaceList.append(interfaceToAdd);
    ui.configWidgets->addWidget(interfaceToAdd);

    QListWidgetItem *buttonToAdd = new QListWidgetItem(ui.configList);
    buttonToAdd->setText(interfaceToAdd->getShortName());
    buttonToAdd->setTextAlignment(Qt::AlignHCenter);
    buttonToAdd->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    buttonToAdd->setToolTip(interfaceToAdd->getDescription());
    buttonToAdd->setIcon(interfaceToAdd->getIcon());
    buttonToAdd->setFont(QFont());
    
    //When the dialog was accepted by the user call the dialogAccepted slot
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));

    //When the user clicked the apply button accept the dialog
    connect(ui.buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(dialogAccepted()));


    connect(interfaceToAdd, SIGNAL(statusChanged(bool)), this, SLOT(currentDialogModified(bool)));


    //When the dialog was rejected call dialogReject on all configuration widgets
    connect(this, SIGNAL(rejected()), interfaceToAdd, SLOT(dialogReject()));

    //When the user clicks the Reset Button the current aktive dialog has to be resettet
    connect(ui.buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(resetCurrentDialog()));
}

ConfigBase::~ConfigBase()
{
}

}
