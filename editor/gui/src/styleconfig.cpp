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
#include "../include/styleconfig.h"

namespace BSGui
{

StyleConfig::StyleConfig()
        : ConfigInterface(), previewStyle(NULL)
{
    //QApplication::setStyle ("motif");
    ui.setupUi(this);
    
    QString beginStyleKey = QApplication::style()->objectName();
    
    ui.styleSelector->addItems(QStyleFactory::keys());

    ui.styleSelector->setCurrentIndex(ui.styleSelector->findText(beginStyleKey, Qt::MatchFixedString));
    
    connect(ui.styleSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT(changePreview(QString)));
}

void StyleConfig::reset()
{
    ui.styleSelector->setCurrentIndex(ui.styleSelector->findText(QApplication::style()->objectName(), Qt::MatchFixedString));
}

//Get the discription of the Config Dialog
QString StyleConfig::getDescription() const
{
    return QString("Change Look & Feel");
}
//Get the Name of the Config Dialog
QString StyleConfig::getShortName() const
{
    return QString("Look & Feel");
}
//Get the Icon of the Config Dialog
QIcon StyleConfig::getIcon() const
{
    return QIcon(":configdialog/media/exec.png");
}

void StyleConfig::changePreview(QString styleString)
{
    QStyle* style = QStyleFactory::create(styleString);

    if (!style)
    {
        return;
    }

    ui.previewGroupBox->setStyle(style);
    ui.previewRadio_1->setStyle(style);
    ui.previewRadio_2->setStyle(style);
    ui.previewLine->setStyle(style);
    ui.previewCheckBox->setStyle(style);
    //ui.previewProgressBar->setStyle(style);
    ui.previewSlider->setStyle(style);
    ui.previewSpinBox->setStyle(style);
    ui.previewPushButton->setStyle(style);
    ui.previewComboBox->setStyle(style);
    ui.previewScrollBar->setStyle(style);

    emit statusChanged(styleChanged());

    delete previewStyle;

    previewStyle = style;
}

bool StyleConfig::styleChanged()
{
    if (ui.styleSelector->currentText().compare(QApplication::style()->objectName(), Qt::CaseInsensitive) == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void StyleConfig::dialogAccept()
{
    QApplication::setStyle(ui.styleSelector->currentText());
    emit statusChanged(false);
}

void StyleConfig::dialogReject()
{
}

StyleConfig::~StyleConfig()
{
}

}


