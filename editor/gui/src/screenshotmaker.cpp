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
 
#include "./../include/screenshotmaker.h"
#include "./../include/mainwindow.h"

namespace BSGui
{

ScreenshotMaker::ScreenshotMaker(QWidget* parent)
    :QDialog(parent), screenshotDirectory(QDir::currentPath() + QString("/screenshots/")), projectName("unnamed"), lockWidthHeight(false)
{
    ui.setupUi(this);
    delete ui.previewStack->currentWidget();
    
    QCompleter* completer = new QCompleter(ui.fileBox);
    QDirModel* model = new QDirModel(completer);
    model->setSorting(QDir::IgnoreCase);
    completer->setModel(model);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui.fileBox->setCompleter(completer);
    
    if(!screenshotDirectory.exists())
    {
        screenshotDirectory.mkpath(screenshotDirectory.absolutePath());
    }
    
    ui.fileBox->addItem(QDir::toNativeSeparators(screenshotDirectory.path() + "/"));
    
    
    QList<QByteArray> formatList = QImageWriter::supportedImageFormats();
    foreach(QByteArray current, formatList)
    {
        supportedFormats.append(QString(current));
    }
    
    ui.fileType->addItems(supportedFormats);
    ui.fileType->setCurrentIndex(ui.fileType->findText("png"));
    
    knownResolutions.append("800x640");
    knownResolutions.append("1024x800");
    knownResolutions.append("1280x1024");
    knownResolutions.append("1600x1280");
    
    ui.resolutionCombo->addItems(knownResolutions);
    
    addGLWidget("Top");
    addGLWidget("Front");
    addGLWidget("Side");
    addGLWidget("3D");
    
    checkDestination();
    
    connect(ui.buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
    connect(ui.fileType, SIGNAL(currentIndexChanged(QString)), this, SLOT(setSuffix(QString)));
    connect(ui.fileType, SIGNAL(currentIndexChanged(QString)), this, SLOT(setQuality(QString)));
    connect(ui.fileBox, SIGNAL(editTextChanged(QString)), this, SLOT(checkDestination()));
    connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(fileButtonClicked()));
    connect(ui.resolutionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(resolutionComboChanged()));
    connect(ui.widthBox, SIGNAL(valueChanged(int)), this, SLOT(widthBoxChanged()));
    connect(ui.heightBox, SIGNAL(valueChanged(int)), this, SLOT(heightBoxChanged()));
    connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
    connect(ui.autoFileExtension, SIGNAL(clicked(bool)), this, SLOT(autoFileExtensionClicked(bool)));
    connect(ui.byFileExtension, SIGNAL(clicked(bool)), this, SLOT(byFileExtensionClicked(bool)));
    connect(ui.byFileExtension, SIGNAL(stateChanged(int)), this, SLOT(checkDestination()));
    
    setFreeFile(projectName);
    
    resolutionComboChanged();
    aspectRatio = (double)ui.widthBox->value()/(double)ui.heightBox->value();
    readSettings();
    setQuality(ui.fileType->currentText());
}

void ScreenshotMaker::setQuality(QString format)
{
    if(format == "jpg" || format == "jpeg")
    {
        ui.qualityBox->setEnabled(true);
    }
    else
    {
        ui.qualityBox->setEnabled(false);
    }
}

void ScreenshotMaker::checkAddRemoveButtons()
{
    QString resString = QString::number(ui.widthBox->value()) +"x"+ QString::number(ui.heightBox->value());
    if(ui.resolutionCombo->findText(resString) == -1)
    {
        ui.addButton->setEnabled(true);
        ui.removeButton->setEnabled(false);
    }
    else
    {
        if(!knownResolutions.contains(resString))
        {
            ui.removeButton->setEnabled(true);
        }
        else
        {
            ui.removeButton->setEnabled(false);
        }
        ui.addButton->setEnabled(false);
    }
}

void ScreenshotMaker::widthBoxChanged()
{
    if(!lockWidthHeight)
    {
        if(ui.aspectRatio->isChecked() && ui.costumResolution->isChecked())
        {
            lockWidthHeight = true;
            ui.heightBox->setValue(qRound(ui.widthBox->value()/aspectRatio));
            lockWidthHeight = false;
        }
        else
        {
            aspectRatio = (double)ui.widthBox->value()/(double)ui.heightBox->value();
        }
    }
    checkAddRemoveButtons();
}

void ScreenshotMaker::byFileExtensionClicked(bool status)
{
    if(status)
    {
        ui.autoFileExtension->setChecked(false);
    }
}

void ScreenshotMaker::autoFileExtensionClicked(bool status)
{
    if(status)
    {
        ui.byFileExtension->setChecked(false);
    }
}

void ScreenshotMaker::heightBoxChanged()
{
    if(!lockWidthHeight)
    {
        if(ui.aspectRatio->isChecked() && ui.costumResolution->isChecked())
        {
            lockWidthHeight = true;
            ui.widthBox->setValue(qRound(ui.heightBox->value()*aspectRatio));
            lockWidthHeight = false;
        }
        else
        {
            aspectRatio = (double)ui.widthBox->value()/(double)ui.heightBox->value();
        }
    }
    checkAddRemoveButtons();
}

void ScreenshotMaker::checkDestination()
{
    QString fileType = getCurrentFileType();
    setQuality(fileType);
    QFileInfo file = QDir::fromNativeSeparators(ui.fileBox->currentText());
    if(file.isDir() || !supportedFormats.contains(fileType))
    {
        ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
    else
    {
        ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
    }
}

void ScreenshotMaker::addGLWidget(const QString& key)
{
    GLWidget* widgetToClone = MainWindow::getInstance()->getGLWidget(key);
    
    OrthogonalType orthoType = widgetToClone->getOrthoType();
    ViewMode viewMode = widgetToClone->getViewMode();
    FillMode fm =  widgetToClone->getFillMode();
            
    bool isOtrho = false;
    if(viewMode == VIEWMODE_Orthogonal)
    {
        isOtrho = true;
    }
    
    BSRenderWidget* widgetToAdd = new BSRenderWidget(this, isOtrho, fm, orthoType);
    
    widgetToAdd->setGrid(widgetToAdd->getGrid());
    widgetToAdd->setWireframe(widgetToAdd->getWireframe());
    widgetToAdd->setWireframeOverlay(widgetToAdd->getWireframeOverlay());
    widgetToAdd->setPointMode(widgetToAdd->getPointMode());
    widgetToAdd->setTextured(widgetToAdd->getTextured());
    widgetToAdd->setNormals(widgetToAdd->getNormals());
    
    ui.previewStack->addWidget(widgetToAdd);
    
}

void ScreenshotMaker::setSuffix(QString suffix)
{
    suffix = suffix.toLower();
    QFileInfo file = QDir::fromNativeSeparators(ui.fileBox->currentText());
    QString oldSuffix = file.suffix();
    if(oldSuffix != "")
    {
        QString fileName = file.completeBaseName();
        fileName = fileName + "." + suffix;
        fileName = file.path() + "/" + fileName;
        ui.fileBox->setEditText(QDir::toNativeSeparators(fileName));
    }
}

void ScreenshotMaker::setFreeFile(QString fileName)
{
    
    if(fileName == QString())
    {
        fileName = projectName;
    }
    
    QFileInfo currentFile = QDir::fromNativeSeparators(ui.fileBox->currentText());
    QDir currentDir;
    if(!currentFile.isDir())
    {
        currentDir = currentFile.dir();
    }
    else
    {
        currentDir = QDir::fromNativeSeparators(ui.fileBox->currentText());
    }
    
    fileName = QFileInfo(fileName).completeBaseName();
    
    bool freeNameFound = false;
    int i = 0;
    QFileInfo newFile;
    
    while(!freeNameFound)
    {
        if(i == 0)
        {
            newFile = QFileInfo(currentDir, fileName + "." + ui.fileType->currentText().toLower());
        }
        else
        {
            newFile = QFileInfo(currentDir, fileName + QString::number(i) + "." + ui.fileType->currentText().toLower());
        }
        if(!newFile.exists())
        {
            freeNameFound = true;
        }
        else
        {
            i++;
        }
    }
    QString finalPath = QDir::toNativeSeparators(newFile.absoluteFilePath());
    ui.fileBox->setEditText(finalPath);
    
    int beginHighlight = finalPath.lastIndexOf(QDir::toNativeSeparators("/")) + 1;
    int highlightLength = newFile.baseName().count();
    
    ui.fileBox->lineEdit()->setSelection(beginHighlight, highlightLength);
    ui.fileBox->setFocus(Qt::OtherFocusReason);
}

void ScreenshotMaker::fileButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select directory", ui.fileBox->currentText());
    if(dir != "")
    {
        ui.fileBox->setEditText(dir);
        setFreeFile();
        if(ui.fileBox->findText(dir) == -1)
        {
            ui.fileBox->addItem(QDir::toNativeSeparators(dir));
        }
    }
}

QString ScreenshotMaker::getCurrentFileType()
{
    QString destination = QDir::fromNativeSeparators(ui.fileBox->currentText());
    QString fileType;
    if(ui.byFileExtension->isChecked())
    {
        QFileInfo file = QDir::fromNativeSeparators(ui.fileBox->currentText());
        fileType = file.suffix();
    }
    else if(ui.autoFileExtension->isChecked())
    {
        QFileInfo file = QDir::fromNativeSeparators(ui.fileBox->currentText());
        fileType = file.suffix();
        if(fileType == "")
        {
            fileType = ui.fileType->currentText().toLower();
            destination = destination + "." + fileType;
        }
    }
    else
    {
        fileType = ui.fileType->currentText().toLower();
    }
    
    return fileType;
}

void ScreenshotMaker::saveButtonClicked()
{
    QString destination = QDir::fromNativeSeparators(ui.fileBox->currentText());
    QString fileType = getCurrentFileType();
    if(supportedFormats.contains(fileType))
    {
        if(QFileInfo(destination).exists())
        {
            if(QMessageBox::question(
                this,
                "Overwrite file?", 
                QString("The file: ") + destination + 
                QString(" already exists. Do you want to overwrite it?"),
                QMessageBox::Save | QMessageBox::Cancel)
                == QMessageBox::Cancel)
            {
                return;
            }
        }
        BSRenderWidget* widget = qobject_cast<BSRenderWidget*>(ui.previewStack->currentWidget());
        QImage image = widget->renderPixmap(ui.widthBox->value(), ui.heightBox->value()).toImage();
        
        int quality = 0;
        if(ui.qualitySlider->isEnabled())
        {
            quality = ui.qualitySlider->value();
        }
        
        bool succ = image.save(destination, fileType.toAscii().constData(), quality);
        widget->resetViewport();
        if (!succ)
        {
            QMessageBox::critical(this, "Error", QString("The file: ") + destination + QString(" could not be written"));
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", QString("File type: ") + fileType + QString(" is not supported"));
    }
}

void ScreenshotMaker::resolutionComboChanged()
{
    QString resString = ui.resolutionCombo->currentText();
    int mid = resString.indexOf("x");
    int width = resString.left(mid).toInt();
    int height = resString.right(resString.count() - mid - 1).toInt();
    
    ui.widthBox->setValue(width);
    ui.heightBox->setValue(height);
}

void ScreenshotMaker::addButtonClicked()
{
    QString resString = QString::number(ui.widthBox->value()) +"x"+ QString::number(ui.heightBox->value());
    ui.resolutionCombo->addItem(resString);
    checkAddRemoveButtons();
}

void ScreenshotMaker::removeButtonClicked()
{
    QString resString = QString::number(ui.widthBox->value()) +"x"+ QString::number(ui.heightBox->value());
    int index = ui.resolutionCombo->findText(resString);
    ui.resolutionCombo->removeItem(index);
    checkAddRemoveButtons();
}

ScreenshotMaker::~ScreenshotMaker()
{
    writeSettings();
}

void ScreenshotMaker::readSettings()
{
    QSettings settings;
    settings.beginGroup("ScreenshotMaker");
    
    int size = settings.beginReadArray("resCombo");
    for(int i = 0 ; i < size ; i++)
    {
        settings.setArrayIndex(i);
        ui.resolutionCombo->addItem(settings.value("resolution").toString());
    }
    
    settings.endArray();
    
    ui.autoFileExtension->setChecked(settings.value("autoFileExtension", true).toBool());
    ui.byFileExtension->setChecked(settings.value("byFileExtension", false).toBool());
    
    ui.fileType->setCurrentIndex(supportedFormats.indexOf(settings.value("fileType", "png").toString()));
    ui.qualitySpinbox->setValue(settings.value("qualitySpinbox", 90).toInt());
    ui.resolutionCombo->
            setCurrentIndex(ui.resolutionCombo->findText(settings.value("resolutionCombo", knownResolutions[0]).toString()));
    ui.costumResolution->setChecked(settings.value("costumResolution", false).toBool());
    ui.aspectRatio->setChecked(settings.value("aspectRatio", false).toBool());
    
    settings.endGroup();
}

void ScreenshotMaker::writeSettings()
{
    QSettings settings;
    settings.beginGroup("ScreenshotMaker");
    
    settings.remove("resCombo");
    
    settings.beginWriteArray("resCombo");
    
    int size = ui.resolutionCombo->count();
    int newEntryCounter = 0;
    for(int i = 0 ; i < size ; i++)
    {
        QString text = ui.resolutionCombo->itemText(i);
        if(!knownResolutions.contains(text))
        {
            settings.setArrayIndex(newEntryCounter);
            settings.setValue("resolution", text);
            newEntryCounter++;
        }
    }
    settings.endArray();
    
    settings.setValue("autoFileExtension", ui.autoFileExtension->isChecked());
    settings.setValue("byFileExtension", ui.byFileExtension->isChecked());
    settings.setValue("fileType", ui.fileType->currentText());
    settings.setValue("qualitySpinbox", ui.qualitySpinbox->value());
    settings.setValue("resolutionCombo", ui.resolutionCombo->currentText());
    settings.setValue("costumResolution", ui.costumResolution->isChecked());
    settings.setValue("aspectRatio", ui.aspectRatio->isChecked());
    
    settings.endGroup();
}

}
