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
 
#include "./../include/renderersettingsdialog.h"

namespace BSGui
{
RendererSettingsDialog::RendererSettingsDialog()
{
    ui.setupUi(this);
    
    renderer = BSRenderer::Renderer::getInstance();
    rendererSettings = renderer->getSettings();
    
    setColors();
    setNumbers();
    setCheckboxses();
    
    connect(ui.saveColorButton, SIGNAL(clicked()), this, SLOT(saveColorButtonClicked()));
    connect(ui.loadColorButton, SIGNAL(clicked()), this, SLOT(loadColorButtonClicked()));
    connect(ui.resetColorsButton, SIGNAL(clicked()), this, SLOT(resetColorsButtonClicked()));
    
    
    connect(ui.lineColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.lineSelectedColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.pointColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.pointSelectedColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.freezeColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.normalColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.selectionAABBColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.orthogonalViewColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.perspectiveViewColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.majorGridLineColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.minorGridLineColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.rubberBandColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    connect(ui.overpaintingColorLabel, SIGNAL(colorChanged(QColor)), this, SLOT(optionsChanged()));
    
    connect(ui.gridSize, SIGNAL(valueChanged(int)), this, SLOT(optionsChanged()));
    connect(ui.lineWidth, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.pointSize, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.normalScaling, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.selectionAABBScaling, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.wireframeOverlayScaling, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.cameraFOV, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.nearPlane, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.farPlane, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    connect(ui.mouseWheelSpeed, SIGNAL(valueChanged(double)), this, SLOT(optionsChanged()));
    
    connect(ui.cullingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(optionsChanged()));
    connect(ui.lineSmoothingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(optionsChanged()));
    connect(ui.pointSmoothingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(optionsChanged()));
    connect(ui.polygonSmoothingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(optionsChanged()));
}

void RendererSettingsDialog::optionsChanged()
{
    emit statusChanged(checkOptionsChanged());
}

void RendererSettingsDialog::dialogAccept()
{
    writeColors();
    writeNumbers();
    writeCheckboxses();
    emit statusChanged(false);
}

void RendererSettingsDialog::reset()
{
    setColors();
    setNumbers();
    setCheckboxses();
}

void RendererSettingsDialog::setColors()
{
    Color wireframeColor = rendererSettings->getWireframeColor();
    ui.lineColorLabel->setColor(ColorToQColor(wireframeColor));
    
    Color wireframeSelectionColor = rendererSettings->getWireframeSelectionColor();
    ui.lineSelectedColorLabel->setColor(ColorToQColor(wireframeSelectionColor));
    
    Color pointColor = rendererSettings->getPointColor();
    ui.pointColorLabel->setColor(ColorToQColor(pointColor));
    
    Color selectedPointsColor = rendererSettings->getPointSelectionColor();
    ui.pointSelectedColorLabel->setColor(ColorToQColor(selectedPointsColor));
    
    Color freezedColor = rendererSettings->getFreezeColor();
    ui.freezeColorLabel->setColor(ColorToQColor(freezedColor));
    
    Color normalColor = rendererSettings->getNormalColor();
    ui.normalColorLabel->setColor(ColorToQColor(normalColor));
    
    Color selectionAABBColor = rendererSettings->getSelectionAABBColor();
    ui.selectionAABBColorLabel->setColor(ColorToQColor(selectionAABBColor));
    
    Color colorOrtho = rendererSettings->getClearColorOrtho();
    ui.orthogonalViewColorLabel->setColor(ColorToQColor(colorOrtho));
    
    Color colorPerspective = rendererSettings->getClearColorPerspective();
    ui.perspectiveViewColorLabel->setColor(ColorToQColor(colorPerspective));
    
    Color majorLineColor = rendererSettings->getGridMajorLineColor();
    ui.majorGridLineColorLabel->setColor(ColorToQColor(majorLineColor));
    
    Color minorLineColor = rendererSettings->getGridMinorLineColor();
    ui.minorGridLineColorLabel->setColor(ColorToQColor(minorLineColor));
    
    ui.rubberBandColorLabel->setColor(MainWindow::getInstance()->getContainer()->getSelectionBoxColor());
    
    ui.overpaintingColorLabel->setColor(MainWindow::getInstance()->getContainer()->getOverpaintingColor());
}

void RendererSettingsDialog::writeColors()
{
    QColor wireframeColor = ui.lineColorLabel->getColor();
    rendererSettings->setWireframeColor(QColorToColor(wireframeColor));
    
    QColor wireframeSelectionColor = ui.lineSelectedColorLabel->getColor();
    rendererSettings->setWireframeSelectionColor(QColorToColor(wireframeSelectionColor));
    
    QColor pointColor = ui.pointColorLabel->getColor();
    rendererSettings->setPointColor(QColorToColor(pointColor));
    
    QColor selectedPointsColor = ui.pointSelectedColorLabel->getColor();
    rendererSettings->setPointSelectionColor(QColorToColor(selectedPointsColor));
    
    QColor freezedColor = ui.freezeColorLabel->getColor();
    rendererSettings->setFreezeColor(QColorToColor(freezedColor));
    
    QColor normalColor = ui.normalColorLabel->getColor();
    rendererSettings->setNormalColor(QColorToColor(normalColor));
    
    QColor selectionAABBColor = ui.selectionAABBColorLabel->getColor();
    rendererSettings->setSelectionAABBColor(QColorToColor(selectionAABBColor));
    
    QColor colorOrtho = ui.orthogonalViewColorLabel->getColor();
    rendererSettings->setClearColorOrtho(QColorToColor(colorOrtho));
    
    QColor colorPerspective = ui.perspectiveViewColorLabel->getColor();
    rendererSettings->setClearColorPerspective(QColorToColor(colorPerspective));
    
    QColor majorLineColor = ui.majorGridLineColorLabel->getColor();
    rendererSettings->setGridMajorLineColor(QColorToColor(majorLineColor));
    
    QColor minorLineColor = ui.minorGridLineColorLabel->getColor();
    rendererSettings->setGridMinorLineColor(QColorToColor(minorLineColor));
    
    MainWindow::getInstance()->getContainer()->setSelectionBoxColor(ui.rubberBandColorLabel->getColor());
    
    MainWindow::getInstance()->getContainer()->setOverpaintingColor(ui.overpaintingColorLabel->getColor());
}

void RendererSettingsDialog::setNumbers()
{
    int gridSize = rendererSettings->getGridSize();
    ui.gridSize->setValue(gridSize);

    double lineWidth = rendererSettings->getLineWidth();
    ui.lineWidth->setValue(lineWidth);
    
    double pointSize = rendererSettings->getPointSize();
    ui.pointSize->setValue(pointSize);
    
    double normalScaling = rendererSettings->getNormalScaling();
    ui.normalScaling->setValue(normalScaling);
    
    double selectionAABBScaling = rendererSettings->getSelectionAABBScaling();
    ui.selectionAABBScaling->setValue(selectionAABBScaling);
    
    double wireframeOverlayScaling = rendererSettings->getWireframeOverlayScaling();
    ui.wireframeOverlayScaling->setValue(wireframeOverlayScaling);
    
    double FOV = rendererSettings->getFOV();
    ui.cameraFOV->setValue(FOV);
    
    double nearPlane = rendererSettings->getNearPlane();
    ui.nearPlane->setValue(nearPlane);
    
    double farPlane = rendererSettings->getFarPlane();
    ui.farPlane->setValue(farPlane);
    
    double mouseWheelSpeed = rendererSettings->getMouseWheelSpeed();
    ui.mouseWheelSpeed->setValue(mouseWheelSpeed);
}

void RendererSettingsDialog::writeNumbers()
{
    int gridSize = ui.gridSize->value();
    rendererSettings->setGridSize(gridSize);

    double lineWidth = ui.lineWidth->value();
    rendererSettings->setLineWidth(lineWidth);
    
    double pointSize = ui.pointSize->value();
    rendererSettings->setPointSize(pointSize);
    
    double normalScaling = ui.normalScaling->value();
    rendererSettings->setNormalScaling(normalScaling);
    
    double selectionAABBScaling =  ui.selectionAABBScaling->value();
    rendererSettings->setSelectionAABBScaling(selectionAABBScaling);
    
    double wireframeOverlayScaling = ui.wireframeOverlayScaling->value();
    rendererSettings->setWireframeOverlayScaling(wireframeOverlayScaling);
    
    double FOV = ui.cameraFOV->value();
    rendererSettings->setFOV(FOV);
    
    double nearPlane = ui.nearPlane->value();
    rendererSettings->setNearPlane(nearPlane);
    
    double farPlane = ui.farPlane->value();
    rendererSettings->setFarPlane(farPlane);
    
    double mouseWheelSpeed = ui.mouseWheelSpeed->value();
    rendererSettings->setMouseWheelSpeed(mouseWheelSpeed);
}

void RendererSettingsDialog::saveColorButtonClicked()
{
    QFileDialog* fileDialog = new QFileDialog(this, "Please select the destination", QDir::currentPath(), "Blacksun Color Schemes (*.bsscheme)");
    fileDialog->setDefaultSuffix("bsscheme");
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    
    QString pathToFile = "";
    if (fileDialog->exec())
    {
        QStringList pathToFiles;
        pathToFiles = fileDialog->selectedFiles();
        if (!pathToFiles.isEmpty())
        {
            pathToFile = pathToFiles.at(0);
        }
    }
    
    if (pathToFile == "")
    {
        return;
    }
    
    QFile file(pathToFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }
    
    QTextStream outputStream(&file);
    
    outputStream << getColorScheme();
}

void RendererSettingsDialog::loadColorButtonClicked()
{
    QString pathToFile = QFileDialog::getOpenFileName(this, "Please select the Scheme File", QDir::currentPath(), "Blacksun Color Schemes (*.bsscheme)");
    
    if (pathToFile == "")
    {
        return;
    }
    
    QFile file(pathToFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }

    QString colorScheme;
    while (!file.atEnd())
    {
        colorScheme.append(file.readLine());
    }
    
    
    if(!setColorScheme(colorScheme))
    {
        QMessageBox::critical(this, "Error", "File is not a valid Blacksun Scheme file");
    }
}

void RendererSettingsDialog::setCheckboxses()
{
    ui.cullingCheckBox->setChecked(rendererSettings->getEnableFrustumCulling());
    ui.lineSmoothingCheckBox->setChecked(rendererSettings->getLineSmoothing());
    ui.pointSmoothingCheckBox->setChecked(rendererSettings->getPointSmoothing());
    ui.polygonSmoothingCheckBox->setChecked(rendererSettings->getPolygonSmoothing());
}

void RendererSettingsDialog::writeCheckboxses()
{
    rendererSettings->enableFrustumCulling(ui.cullingCheckBox->isChecked());
    rendererSettings->setLineSmoothing(ui.lineSmoothingCheckBox->isChecked());
    rendererSettings->setPointSmoothing(ui.pointSmoothingCheckBox->isChecked());
    rendererSettings->setPolygonSmoothing(ui.polygonSmoothingCheckBox->isChecked());
}

QString RendererSettingsDialog::getColorScheme()
{
    QDomDocument scheme("BSColorScheme");
    QDomElement root = scheme.createElement("ColorScheme");
    scheme.appendChild(root);
    
    QDomElement wireframeColor = scheme.createElement("wireframeColor");
    wireframeColor.setAttribute("Red", ui.lineColorLabel->getColor().redF());
    wireframeColor.setAttribute("Green", ui.lineColorLabel->getColor().greenF());
    wireframeColor.setAttribute("Blue", ui.lineColorLabel->getColor().blueF());
    root.appendChild(wireframeColor);
    
    QDomElement wireframeSelectionColor = scheme.createElement("wireframeSelectionColor");
    wireframeSelectionColor.setAttribute("Red", ui.lineSelectedColorLabel->getColor().redF());
    wireframeSelectionColor.setAttribute("Green", ui.lineSelectedColorLabel->getColor().greenF());
    wireframeSelectionColor.setAttribute("Blue", ui.lineSelectedColorLabel->getColor().blueF());
    root.appendChild(wireframeSelectionColor);
    
    QDomElement pointColor = scheme.createElement("pointColor");
    pointColor.setAttribute("Red", ui.pointColorLabel->getColor().redF());
    pointColor.setAttribute("Green", ui.pointColorLabel->getColor().greenF());
    pointColor.setAttribute("Blue", ui.pointColorLabel->getColor().blueF());
    root.appendChild(pointColor);
    
    QDomElement selectedPointsColor = scheme.createElement("selectedPointsColor");
    selectedPointsColor.setAttribute("Red", ui.pointSelectedColorLabel->getColor().redF());
    selectedPointsColor.setAttribute("Green", ui.pointSelectedColorLabel->getColor().greenF());
    selectedPointsColor.setAttribute("Blue", ui.pointSelectedColorLabel->getColor().blueF());
    root.appendChild(selectedPointsColor);
    
    QDomElement freezedColor = scheme.createElement("freezedColor");
    freezedColor.setAttribute("Red", ui.freezeColorLabel->getColor().redF());
    freezedColor.setAttribute("Green", ui.freezeColorLabel->getColor().greenF());
    freezedColor.setAttribute("Blue", ui.freezeColorLabel->getColor().blueF());
    root.appendChild(freezedColor);
    
    QDomElement normalColor = scheme.createElement("normalColor");
    normalColor.setAttribute("Red", ui.normalColorLabel->getColor().redF());
    normalColor.setAttribute("Green", ui.normalColorLabel->getColor().greenF());
    normalColor.setAttribute("Blue", ui.normalColorLabel->getColor().blueF());
    root.appendChild(normalColor);
    
    QDomElement selectionAABBColor = scheme.createElement("selectionAABBColor");
    selectionAABBColor.setAttribute("Red", ui.selectionAABBColorLabel->getColor().redF());
    selectionAABBColor.setAttribute("Green", ui.selectionAABBColorLabel->getColor().greenF());
    selectionAABBColor.setAttribute("Blue", ui.selectionAABBColorLabel->getColor().blueF());
    root.appendChild(selectionAABBColor);
    
    QDomElement colorOrtho = scheme.createElement("colorOrtho");
    colorOrtho.setAttribute("Red", ui.orthogonalViewColorLabel->getColor().redF());
    colorOrtho.setAttribute("Green", ui.orthogonalViewColorLabel->getColor().greenF());
    colorOrtho.setAttribute("Blue", ui.orthogonalViewColorLabel->getColor().blueF());
    root.appendChild(colorOrtho);
    
    QDomElement colorPerspective = scheme.createElement("colorPerspective");
    colorPerspective.setAttribute("Red", ui.perspectiveViewColorLabel->getColor().redF());
    colorPerspective.setAttribute("Green", ui.perspectiveViewColorLabel->getColor().greenF());
    colorPerspective.setAttribute("Blue", ui.perspectiveViewColorLabel->getColor().blueF());
    root.appendChild(colorPerspective);
    
    QDomElement majorLineColor = scheme.createElement("majorLineColor");
    majorLineColor.setAttribute("Red", ui.majorGridLineColorLabel->getColor().redF());
    majorLineColor.setAttribute("Green", ui.majorGridLineColorLabel->getColor().greenF());
    majorLineColor.setAttribute("Blue", ui.majorGridLineColorLabel->getColor().blueF());
    root.appendChild(majorLineColor);
    
    QDomElement minorLineColor = scheme.createElement("minorLineColor");
    minorLineColor.setAttribute("Red", ui.minorGridLineColorLabel->getColor().redF());
    minorLineColor.setAttribute("Green", ui.minorGridLineColorLabel->getColor().greenF());
    minorLineColor.setAttribute("Blue", ui.minorGridLineColorLabel->getColor().blueF());
    root.appendChild(minorLineColor);
    
    QDomElement rubberBandColor = scheme.createElement("rubberBandColor");
    rubberBandColor.setAttribute("Red", ui.rubberBandColorLabel->getColor().redF());
    rubberBandColor.setAttribute("Green", ui.rubberBandColorLabel->getColor().greenF());
    rubberBandColor.setAttribute("Blue", ui.rubberBandColorLabel->getColor().blueF());
    root.appendChild(rubberBandColor);
    
    QDomElement overpaintingColor = scheme.createElement("overpaintingColor");
    overpaintingColor.setAttribute("Red", ui.overpaintingColorLabel->getColor().redF());
    overpaintingColor.setAttribute("Green", ui.overpaintingColorLabel->getColor().greenF());
    overpaintingColor.setAttribute("Blue", ui.overpaintingColorLabel->getColor().blueF());
    root.appendChild(overpaintingColor);
    
    return scheme.toString();
}

bool RendererSettingsDialog::setColorScheme(const QString& colorScheme)
{
    QDomDocument scheme("BSColorScheme");
    scheme.setContent(colorScheme);
    QDomElement root = scheme.documentElement();
    if ( root.tagName() != "ColorScheme")
    {
        return false;
    }
    
    QDomNode currentNode = root.firstChild();
    while(!currentNode.isNull())
    {
        QDomElement currentElement = currentNode.toElement();
        {
            if(!currentElement.isNull())
            {
                QString currentTagName = currentElement.tagName();
                if(currentTagName == "wireframeColor")
                {
                    ui.lineColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "wireframeSelectionColor")
                {
                    ui.lineSelectedColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "pointColor")
                {
                    ui.pointColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "selectedPointsColor")
                {
                    ui.pointSelectedColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "freezedColor")
                {
                    ui.freezeColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "normalColor")
                {
                    ui.normalColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "selectionAABBColor")
                {
                    ui.selectionAABBColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "colorOrtho")
                {
                    ui.orthogonalViewColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "colorPerspective")
                {
                    ui.perspectiveViewColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "majorLineColor")
                {
                    ui.majorGridLineColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "minorLineColor")
                {
                    ui.minorGridLineColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "rubberBandColor")
                {
                    ui.rubberBandColorLabel->setColor(colorFromElement(currentElement));
                }
                else if(currentTagName == "overpaintingColor")
                {
                    ui.overpaintingColorLabel->setColor(colorFromElement(currentElement));
                }
            }
        }
        
        currentNode = currentNode.nextSibling();
    }
    return true;
}

bool RendererSettingsDialog::checkOptionsChanged()
{
    if(
       ui.lineColorLabel->getColor() == ColorToQColor(rendererSettings->getWireframeColor())
       &&
       ui.lineSelectedColorLabel->getColor() == ColorToQColor(rendererSettings->getWireframeSelectionColor())
       &&
       ui.pointColorLabel->getColor() == ColorToQColor(rendererSettings->getPointColor())
       &&
       ui.pointSelectedColorLabel->getColor() == ColorToQColor(rendererSettings->getPointSelectionColor())
       &&
       ui.freezeColorLabel->getColor() == ColorToQColor(rendererSettings->getFreezeColor())
       &&
       ui.normalColorLabel->getColor() == ColorToQColor(rendererSettings->getNormalColor())
       &&
       ui.selectionAABBColorLabel->getColor() == ColorToQColor(rendererSettings->getSelectionAABBColor())
       &&
       ui.orthogonalViewColorLabel->getColor() == ColorToQColor(rendererSettings->getClearColorOrtho())
       &&
       ui.perspectiveViewColorLabel->getColor() == ColorToQColor(rendererSettings->getClearColorPerspective())
       &&
       ui.majorGridLineColorLabel->getColor() == ColorToQColor(rendererSettings->getGridMajorLineColor())
       &&
       ui.minorGridLineColorLabel->getColor() == ColorToQColor(rendererSettings->getGridMinorLineColor())
       &&
       ui.rubberBandColorLabel->getColor() == MainWindow::getInstance()->getContainer()->getSelectionBoxColor()
       &&
       ui.overpaintingColorLabel->getColor() == MainWindow::getInstance()->getContainer()->getOverpaintingColor()
       &&
       ui.gridSize->value() == rendererSettings->getGridSize()
       &&
       ui.lineWidth->value() == rendererSettings->getLineWidth()
       &&
       ui.pointSize->value() == rendererSettings->getPointSize()
       &&
       ui.normalScaling->value() == rendererSettings->getNormalScaling()
       &&
       ui.selectionAABBScaling->value() == rendererSettings->getSelectionAABBScaling()
       &&
       ui.wireframeOverlayScaling->value() == rendererSettings->getWireframeOverlayScaling()
       &&
       ui.cameraFOV->value() == rendererSettings->getFOV()
       &&
       ui.nearPlane->value() == rendererSettings->getNearPlane()
       &&
       ui.farPlane->value() == rendererSettings->getFarPlane()
       &&
       ui.mouseWheelSpeed->value() == rendererSettings->getMouseWheelSpeed()
       &&
       ui.cullingCheckBox->isChecked() == rendererSettings->getEnableFrustumCulling()
       &&
       ui.lineSmoothingCheckBox->isChecked() == rendererSettings->getLineSmoothing()
       &&
       ui.pointSmoothingCheckBox->isChecked() == rendererSettings->getPointSmoothing()
       &&
       ui.polygonSmoothingCheckBox->isChecked() == rendererSettings->getPolygonSmoothing()
      )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void RendererSettingsDialog::resetColorsButtonClicked()
{
    QString stdColor = "";
    stdColor.append("<!DOCTYPE BSColorScheme>");
    stdColor.append("<ColorScheme>");
    stdColor.append("<wireframeColor Blue=\"1\" Red=\"1\" Green=\"1\" />");
    stdColor.append("<wireframeSelectionColor Blue=\"0\" Red=\"1\" Green=\"0\" />");
    stdColor.append("<pointColor Blue=\"0\" Red=\"1\" Green=\"1\" />");
    stdColor.append("<selectedPointsColor Blue=\"0\" Red=\"1\" Green=\"0.5000076295109483\" />");
    stdColor.append("<freezedColor Blue=\"1\" Red=\"0.5499961852445259\" Green=\"0.6\" />");
    stdColor.append("<normalColor Blue=\"1\" Red=\"1\" Green=\"0\" />");
    stdColor.append("<selectionAABBColor Blue=\"0\" Red=\"0\" Green=\"1\" />");
    stdColor.append("<colorOrtho Blue=\"0.8\" Red=\"0.8\" Green=\"0.8\" />");
    stdColor.append("<colorPerspective Blue=\"0.6999923704890516\" Red=\"0.2\" Green=\"0.5000076295109483\" />");
    stdColor.append("<majorLineColor Blue=\"0\" Red=\"0.5000076295109483\" Green=\"0.5000076295109483\" />");
    stdColor.append("<minorLineColor Blue=\"0.6999923704890516\" Red=\"0.6999923704890516\" Green=\"0.6999923704890516\" />");
    stdColor.append("<rubberBandColor Blue=\"1.0\" Red=\"0\" Green=\"0\" />");
    stdColor.append("<overpaintingColor Blue=\"0\" Red=\"0\" Green=\"0\" />");
    stdColor.append("</ColorScheme>");
    
    setColorScheme(stdColor);
}

QColor RendererSettingsDialog::colorFromElement(QDomElement e)
{
    qreal red = e.attribute("Red").toDouble();
    qreal green = e.attribute("Green").toDouble();
    qreal blue = e.attribute("Blue").toDouble();
    
    return QColor::fromRgbF(red, green, blue);
}
}
