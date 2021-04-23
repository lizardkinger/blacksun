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
#include "../include/pluginmanager.h"

namespace BSGui
{

PluginManager::PluginManager()
        : ConfigInterface()
{

    //ATTENTION:    In this class you will find casts from qulonglong to UID and vica versa
    //              This is valid becouse UID is a unsigned long int and qulonglong is a 
    //              unsigned long long int (unsigned __int64 on Windows) therefore you can
    //              cast a UID to an qulonglong and you can cast this value back to an UID

    //              BUT DO NEVER STORE A REAL unsgined long long IN A BSTreeWidgetItem WITH
    //              A BSTreeWidgetItem::UIDRole role. YOU WILL GET AN INT OVERFLOW!
    
    ui.setupUi(this);

    ui.pluginList->setColumnCount(2);


    pluginManager = BSPlgMgr::PlgMgr::getInstance();
    availablePlugins = pluginManager->getAvailablePlugins();
    //pluginManager->installPlg("/home/hunter/blacksun-svn/blacksun/plugin/housegenerator/Linux/debug/libHouseGen.so");
    //pluginManager->installPlg("/home/hunter/blacksun-svn/blacksun/plugin/pyramidalgenerator/Linux/debug/libPyrGen.so");
    //pluginManager->installPlg("/home/hunter/blacksun-svn/blacksun/plugin/boxgenerator/Linux/debug/libBoxGen.so");

    setupPluginList();

    for (int i = 0 ; i < availablePlugins.count() ; i++)
    {
        addPlugin(availablePlugins.at(i));
    }

	// Fill the options:
	QSettings settings;
	ui.pluginPath->setText(QString(settings.value("PlgMgr/SearchPath").toByteArray()));
	/*switch(settings.value("PlgMgr/Options").toInt())
	{
	case 1:	ui.rbInstall->setChecked(true); break;
	case 2: ui.rbInstallLoad->setChecked(true); break;
	default:ui.rbNone->setChecked(true); break;
	}*/

    int autoMode = settings.value("PlgMgr/Options").toInt();
    if(autoMode == 1)
    {
        ui.autosearchBox->setChecked(true);
        ui.autoloadBox->setChecked(false);
    }
    else if(autoMode == 2)
    {
        ui.autosearchBox->setChecked(true);
        ui.autoloadBox->setChecked(true);
    }
    else
    {
        ui.autosearchBox->setChecked(false);
        ui.autoloadBox->setChecked(false);
    }


    QCompleter* completer = new QCompleter(ui.pluginPath);

    QDirModel* model = new QDirModel(completer);
    model->setSorting(QDir::IgnoreCase);
    completer->setModel(model);

    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui.pluginPath->setCompleter(completer);
    

    connect(ui.pluginList, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, SLOT(setAboutStackIndex(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));
    connect(ui.unloadButton, SIGNAL(clicked()), this, SLOT(unloadButtonClicked()));
    connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installButtonClicked()));
    connect(ui.uninstallButton, SIGNAL(clicked()), this, SLOT(uninstallButtonClicked()));
    connect(ui.changePath, SIGNAL(clicked()), this, SLOT(changeSearchPath()));

    connect(pluginManager, SIGNAL(stateChanged(UID)), this, SLOT(changeState(UID)));
    connect(pluginManager, SIGNAL(plgRemoved(UID)), this, SLOT(removePlugin(UID)));
    connect(pluginManager, SIGNAL(plgInstalled(UID)), this, SLOT(pluginInstalled(UID)));
}

void PluginManager::setupPluginList()
{
    QStringList labels;
    labels << "Name" << "Version";
    ui.pluginList->setHeaderLabels(labels);
    ui.pluginList->header()->setResizeMode(0, QHeaderView::Stretch);
}

//TODO: Let this function not just simply delete all pluginWidgets but update them
/*void PluginManager::regeneratePluginList()
{
    availablePlugins = pluginManager->getAvailablePlugins();

    //Remove all items from the pluginlist and delete them
    while(ui.pluginList->topLevelItemCount() > 0)
    {
        delete ui.pluginList->takeTopLevelItem(0);
    }
    //and clear it
    ui.pluginList->clear();

    //Remove all items from the about stack and delete them
    QList<QWidget* > widgetesToDelete;
    //We save the widgets in a List so we can delete them later
    for (int i = 0 ; i < ui.aboutStack->count() ; i++)
    {
        widgetesToDelete.append(ui.aboutStack->widget(i));
    }

    while(ui.aboutStack->count() > 0)
    {
        ui.aboutStack->removeWidget(ui.aboutStack->widget(0));
    }

    while (!widgetesToDelete.isEmpty())
    {
        delete widgetesToDelete.takeFirst();
    }

    for (int i = 0 ; i < availablePlugins.count() ; i++)
    {
        addPlugin(availablePlugins.at(i));
    }
}*/

void PluginManager::addPlugin(UID pluginToAdd)
{
    //Insert the treewidget items
    QString pluginName = pluginManager->getName(pluginToAdd);
    Version pluginVersion = pluginManager->getVersion(pluginToAdd);
    QString versionString;
    versionString += QString::number(pluginVersion.MAJOR_RELEASE) += ".";
    versionString += QString::number(pluginVersion.MINOR_RELEASE) += ".";
    versionString += QString::number(pluginVersion.PATCH_LEVEL);
    QString pluginDescription = pluginManager->getAboutData(pluginToAdd);

    QStringList listToInsert;
    listToInsert << pluginName << versionString;

    BSTreeWidgetItem* pluginWidget = new BSTreeWidgetItem(listToInsert);

    pluginWidget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //Store the data in the pluginWidget!
    pluginWidget->setData(0, BSTreeWidgetItem::UIDRole, qulonglong(pluginToAdd));
    pluginWidget->setData(0, BSTreeWidgetItem::IndexRole, ui.aboutStack->count());

    if (pluginManager->getStatus(pluginToAdd))
    {
        pluginWidget->setData(0, Qt::DecorationRole, QIcon(":stop-go/media/ok.png"));
    }
    else
    {
        pluginWidget->setData(0, Qt::DecorationRole, QIcon(":stop-go/media/stop.png"));
    }


    int row = ui.pluginList->topLevelItemCount();
    ui.pluginList->insertTopLevelItem(row, pluginWidget);

    //insert the textedits
    QTextEdit* textEditToAdd = new QTextEdit(pluginDescription);
    textEditToAdd->setReadOnly(true);
    
    ui.aboutStack->addWidget(textEditToAdd);
}

void PluginManager::installButtonClicked()
{
    QString pathToNewPlugin = 
    QFileDialog::getOpenFileName(0," Open File", QDir::currentPath(), "Shared Objects (*.dll *.so)");

    if (pathToNewPlugin != "")
    {
        throwMessage(pluginManager->installPlg(pathToNewPlugin));
    }
    //regeneratePluginList();
}

void PluginManager::uninstallButtonClicked()
{
    QList<QTreeWidgetItem*> selectedItems = ui.pluginList->selectedItems();
    for(int i = 0 ; i < selectedItems.count() ; i++)
    {
        pluginManager->uninstallPlg(UID(selectedItems.at(i)->data(0, BSTreeWidgetItem::UIDRole).toULongLong()));
    }
}

void PluginManager::throwMessage(int pluginManagerFlag)
{
    switch(pluginManagerFlag)
    {
        case BSPlgMgr::NOT_INSTALLED:
        {
            QMessageBox::critical(this, "Error", "This Plugin was not installed");
            break;
        }
        case BSPlgMgr::ALREADY_RUNNING:
        {
            QMessageBox::critical(this, "Error", "This Plugin is already running");
            break;
        }
        case BSPlgMgr::ALREADY_STOPPED:
        {
            QMessageBox::critical(this, "Error", "This Plugin is already stopped");
            break;
        }
        case BSPlgMgr::ALREADY_INSTALLED:
        {
            QMessageBox::critical(this, "Error", "This Plugin is already installed");
            break;
        }
        case BSPlgMgr::FILE_NOT_FOUND:
        {
            QMessageBox::critical(this, "Error", "File not found");
            break;
        }
        case BSPlgMgr::VERSION_DISPUTE:
        {
            QMessageBox::critical(this, "Error", "Version dispute");
            break;
        }
        case BSPlgMgr::NOT_A_VALID_PLUGIN:
        {
            QMessageBox::critical(this, "Error", "This is not a valid plugin");
            break;
        }
        case BSPlgMgr::VERSION_ERROR:
        {
            QMessageBox::critical(this, "Error", "Version error");
            break;
        }
        case BSPlgMgr::PLG_SPECIFIC_ERROR:
        {
            QMessageBox::critical(this, "Error", "Unknown error");
            break;
        }
    }
}

void PluginManager::loadButtonClicked()
{
    QList<QTreeWidgetItem*> selectedItems = ui.pluginList->selectedItems();
    for(int i = 0 ; i < selectedItems.count() ; i++)
    {
        throwMessage(pluginManager->loadPlugin(UID(selectedItems.at(i)->data(0, BSTreeWidgetItem::UIDRole).toULongLong())));
    }
}

void PluginManager::unloadButtonClicked()
{
    QList<QTreeWidgetItem*> selectedItems = ui.pluginList->selectedItems();
    for(int i = 0 ; i < selectedItems.count() ; i++)
    {
        throwMessage(pluginManager->unloadPlugin(UID(selectedItems.at(i)->data(0, BSTreeWidgetItem::UIDRole).toULongLong())));
    }
}

void PluginManager::changeSearchPath()
{
	QString dir = QFileDialog::getExistingDirectory(this, "Select directory",
		ui.pluginPath->text());
	
	if(dir != "")
    {
        ui.pluginPath->setText(dir);
    }
}
    

void PluginManager::pluginInstalled(UID uid)
{
    addPlugin(uid);
}

void PluginManager::removePlugin(UID uid)
{
    int numberOfItems = ui.pluginList->topLevelItemCount();
    for(int i = 0 ; i < numberOfItems ; i++)
    {
        if((UID(ui.pluginList->topLevelItem(i)->data(0, BSTreeWidgetItem::UIDRole).toULongLong())) == uid)
        {
            delete ui.pluginList->takeTopLevelItem(i);
            break;
        }
    }
}

void PluginManager::changeState(UID uid)
{
    int numberOfItems = ui.pluginList->topLevelItemCount();
    for(int i = 0 ; i < numberOfItems ; i++ )
    {
        QTreeWidgetItem* current = ui.pluginList->topLevelItem(i);
        if((UID(current->data(0, BSTreeWidgetItem::UIDRole).toULongLong())) == uid)
        {
            if (pluginManager->getStatus(uid))
            {
                current->setData(0, Qt::DecorationRole, QIcon(":stop-go/media/ok.png"));
            }
            else
            {
                current->setData(0, Qt::DecorationRole, QIcon(":stop-go/media/stop.png"));
            }
        }
    }
}

void PluginManager::setAboutStackIndex(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }
    ui.aboutStack->setCurrentIndex(current->data(0, BSTreeWidgetItem::IndexRole).toInt());
}

PluginManager::~PluginManager()
{
	// Save settings:
	QSettings settings;
	settings.setValue("PlgMgr/SearchPath", ui.pluginPath->text().toAscii());

    int installFlag = 3;
    if(ui.autosearchBox->isChecked())
    {
        if(ui.autoloadBox->isChecked())
        {
            installFlag = 2;
        }
        else
        {
            installFlag = 1;
        }
    }
    if(ui.autoloadBox->isChecked())
    {
        
    }

	settings.setValue("PlgMgr/Options",installFlag);
}

QString PluginManager::getDescription() const
{
    return QString("Load and unload Plugins");
}

QString PluginManager::getShortName() const
{
    return QString("Pluginmanager");
}

QIcon PluginManager::getIcon() const
{
    return QIcon(":configdialog/media/exec.png");
}

void PluginManager::dialogAccept()
{
}

void PluginManager::dialogReject()
{
}

void PluginManager::reset()
{
}

}

