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
#include "./../include/mainwindow.h"
#include "./../../scenegraph/include/SceneManager.h"
#include "./../../core/include/Log.h"
#include "./../../core/include/UR_Manager.h"
#include "./../../pluginsystem/include/PlgMgr.h"
#include "./../../renderer/include/Renderer.h"

namespace BSGui
{

MainWindow* MainWindow::_instance = NULL;

//Set version
BSPlgMgr::Version MainWindow::version(1,0,0);

MainWindow::MainWindow()
        : QMainWindow(), showFirstTime(true)

{
    setWindowState(Qt::WindowMaximized);
    initializeUI();
    //Read config
    readSettings();
    //ui.menuBar->actions();
}

void MainWindow::showEvent(QShowEvent* event)
{
    //Renderer Settings have to be modified after show()
    if(showFirstTime)
    {
        readRendererSettings();
        showFirstTime = false;
    }
    QMainWindow::showEvent(event);
}

void MainWindow::initializeUI()
{
    
    ui.setupUi(this);

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QHBoxLayout());

    centralWidget()->layout()->setMargin(5);
    centralWidget()->layout()->setSpacing(2);

    toolBox = new QToolBox(this);
    //toolBox->setMinimumWidth(280);
    //toolBox->setMaximumWidth(280);
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    seperatorLine = new QFrame(this);
    seperatorLine->setFrameShape(QFrame::VLine);
    seperatorLine->setFrameShadow(QFrame::Sunken);

    centralWidget()->layout()->addWidget(toolBox);
    centralWidget()->layout()->addWidget(seperatorLine);
    ui.actionToolbarLeft->setChecked(true);

    theLoggingWindow = new LogWindow(this);
    theLoggingWindow->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    ObjectToolBoxItem* objectWidget = new ObjectToolBoxItem(this);

    addToolBoxItem("Object", objectWidget);

    createStdGLWidgets();

    QActionGroup* toolBarPosition = new QActionGroup(this);
    toolBarPosition->addAction(ui.actionToolbarRight);
    toolBarPosition->addAction(ui.actionToolbarLeft);
    
    QActionGroup* rendererSettings = new QActionGroup(this);
    rendererSettings->addAction(ui.actionTextured);
    rendererSettings->addAction(ui.actionWireframe);
    rendererSettings->addAction(ui.actionWireframeOverlay);
    rendererSettings->addAction(ui.actionPointsOnly);
    rendererSettings->addAction(ui.actionSolid);
    
    QActionGroup* realtimeMirrors = new QActionGroup(this);
    realtimeMirrors->addAction(ui.actionRealtimeMirrorXAxis);
    realtimeMirrors->addAction(ui.actionRealtimeMirrorYAxis);
    realtimeMirrors->addAction(ui.actionRealtimeMirrorZAxis);
    realtimeMirrors->addAction(ui.actionRealtimeMirrorNone);
    
    ui.actionGrid->setChecked(container->getGrid());
    ui.actionNormals->setChecked(container->getNormals());
    ui.actionTextured->setChecked(container->getTextured());
    ui.actionWireframe->setChecked(container->getWireframe());
    ui.actionWireframeOverlay->setChecked(container->getWireframeOverlay());
    ui.actionPointsOnly->setChecked(container->getPointMode());
    
    snapToGridBox = new QCheckBox(tr("Snap to Grid"));
    statusBar()->addWidget(snapToGridBox);
    snapToGridBox->setFocusPolicy(Qt::NoFocus);
    
    posLabel = new QLabel("X: 0  Y: 0");
    statusBar()->addWidget(posLabel);

    //Make connections
    connect(ui.actionSnapToGrid, SIGNAL(triggered(bool)), snapToGridBox, SLOT(setChecked(bool)));
    connect(snapToGridBox, SIGNAL(stateChanged(int)), this, SLOT(snapToGridBoxClicked(int)));
    
    connect(ui.actionToolbarRight, SIGNAL(triggered()), this, SLOT(setToolbarRight()));
    connect(ui.actionToolbarLeft, SIGNAL(triggered()), this, SLOT(setToolbarLeft()));
    connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(displaySettingsDialog()));
    connect(ui.actionLogWindow, SIGNAL(triggered(bool)), theLoggingWindow, SLOT(setShown(bool)));
    connect(theLoggingWindow, SIGNAL(windowClosed()),ui.actionLogWindow, SLOT(toggle()));
    connect(ui.actionExit, SIGNAL(triggered()),this ,SLOT(quit()));
    connect(ui.actionAboutBlacksun, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui.actionAboutQt, SIGNAL(triggered()), this, SLOT(showAboutQtDialog()));
    connect(BSPlgMgr::PlgMgr::getInstance(), SIGNAL(setStatusBar(QString)), this, SLOT(showStatusbarMessage(QString)));
    connect(ui.actionTakeScreenshot, SIGNAL(triggered()), this, SLOT(takeScreenshot()));
    
    // add by Stefan for the Undo/Redo - System
    connect(ui.actionUndo, SIGNAL(triggered()), BSCore::UR_Manager::getInstance(), SLOT(undo()));
    connect(ui.actionRedo, SIGNAL(triggered()), BSCore::UR_Manager::getInstance(), SLOT(redo()));
    
    // scene related actions
    BSScene::SceneManager *sm = BSScene::SceneManager::getInstance();
    BSScene::MenuHandler *mh = sm->getMenuHandler();
    
    connect(ui.actionNew, SIGNAL(triggered()), mh, SLOT(file_new()));
    connect(ui.actionOpen, SIGNAL(triggered()), mh, SLOT(file_open()));
    connect(ui.actionMerge, SIGNAL(triggered()), mh, SLOT(file_merge()));
    connect(ui.actionSave, SIGNAL(triggered()), mh, SLOT(file_save()));
    connect(ui.actionSaveAs, SIGNAL(triggered()), mh, SLOT(file_save_as()));
    connect(ui.actionComment_scene, SIGNAL(triggered()), mh, SLOT(file_comment()));
   
   
   	connect(ui.actionCut, SIGNAL(triggered()), mh, SLOT(edit_cut()));
	connect(ui.actionCopy, SIGNAL(triggered()), mh, SLOT(edit_copy()));
	connect(ui.actionPaste, SIGNAL(triggered()), mh, SLOT(edit_paste()));
	connect(ui.actionDelete, SIGNAL(triggered()), mh, SLOT(edit_delete()));
	connect(ui.actionClone, SIGNAL(triggered()), mh, SLOT(edit_clone()));
	connect(ui.actionMirrorXAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_x()));
	connect(ui.actionMirrorYAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_y()));
	connect(ui.actionMirrorZAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_z()));
	connect(ui.actionMirrorWorldXAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_world_x()));
	connect(ui.actionMirrorWorldYAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_world_y()));
	connect(ui.actionMirrorWorldZAxis, SIGNAL(triggered()), mh, SLOT(edit_mirror_world_z()));	
	
	connect(ui.actionMakeMeshFromPolygons, SIGNAL(triggered()), mh, SLOT(edit_meshFromPolygons()));
	connect(ui.actionExtractPolygonsFromMesh, SIGNAL(triggered()), mh, SLOT(edit_extractPolygonsFromMesh()));
	connect(ui.actionMergeMeshes, SIGNAL(triggered()), mh, SLOT(edit_mergeMeshes()));
   
   	connect(ui.actionSelectAll, SIGNAL(triggered()), mh, SLOT(selection_select_all()));
   	connect(ui.actionSelectNothing, SIGNAL(triggered()), mh, SLOT(selection_select_none()));
   	connect(ui.actionSelectInvert, SIGNAL(triggered()), mh, SLOT(selection_select_invert()));
   	connect(ui.actionSelectIntersecting, SIGNAL(triggered()), mh, SLOT(selection_select_intersecting()));
   	
   	connect(ui.actionHideSelection, SIGNAL(triggered()), mh, SLOT(selection_hide_selection()));
	connect(ui.actionHideAll, SIGNAL(triggered()), mh, SLOT(selection_hide_all()));
	connect(ui.actionHideNothing, SIGNAL(triggered()), mh, SLOT(selection_hide_none()));
	connect(ui.actionHideInvert, SIGNAL(triggered()), mh, SLOT(selection_hide_invert()));

   	connect(ui.actionFreezeSelection, SIGNAL(triggered()), mh, SLOT(selection_freeze_selection()));
	connect(ui.actionFreezeAll, SIGNAL(triggered()), mh, SLOT(selection_freeze_all()));
	connect(ui.actionFreezeNothing, SIGNAL(triggered()), mh, SLOT(selection_freeze_none()));
	connect(ui.actionFreezeInvert, SIGNAL(triggered()), mh, SLOT(selection_freeze_invert()));
	
	connect(ui.actionArrangeTogether, SIGNAL(triggered()), mh, SLOT(selection_ArrangeTogether()));
	connect(ui.actionAlignXAxis, SIGNAL(triggered()), mh, SLOT(selection_AlignXAxis()));
	connect(ui.actionAlignYAxis, SIGNAL(triggered()), mh, SLOT(selection_AlignYAxis()));
	connect(ui.actionAlignZAxis, SIGNAL(triggered()), mh, SLOT(selection_AlignZAxis()));
	
	connect(ui.actionZoomToSelection, SIGNAL(triggered()), mh, SLOT(selection_zoomTo()));
	connect(ui.actionZoomToAll, SIGNAL(triggered()), mh, SLOT(selection_zoomAll()));
    
    connect(ui.actionTriangulate, SIGNAL(triggered()), mh, SLOT(modify_triangulate()));
    connect(ui.actionRemoveUnboundVertices, SIGNAL(triggered()), mh, SLOT(modify_removeUnboundVerts()));
    connect(ui.actionTurnVertexOrder, SIGNAL(triggered()), mh, SLOT(modify_turnVertexOrder()));
    
    
    connect(ui.actionMaterialCoordinates, SIGNAL(triggered()), mh, SLOT(material_coordinates()));
    connect(ui.actionMaterialRemove, SIGNAL(triggered()), mh, SLOT(material_remove()));
    
	connect(ui.actionResetViews, SIGNAL(triggered()), getContainer(), SLOT(resetViews()));
    
    connect(ui.actionSceneStatistics, SIGNAL(triggered()), mh, SLOT(view_statistics()));
    
    connect(ui.actionDrawBackfaces, SIGNAL(triggered(bool)), this, SLOT(setDrawBackfaces(bool)));
    
    // For updating window title   
    connect(sm,SIGNAL(changeWindowTitle(QString)),this, SLOT(changeWindowTitle(QString)));
    
    connect(ui.actionRealtimeMirrorXAxis, SIGNAL(triggered(bool)), this, SLOT(setRealtimeMirrorXY(bool)));
    connect(ui.actionRealtimeMirrorYAxis, SIGNAL(triggered(bool)), this, SLOT(setRealtimeMirrorYZ(bool)));
    connect(ui.actionRealtimeMirrorZAxis, SIGNAL(triggered(bool)), this, SLOT(setRealtimeMirrorXZ(bool)));
    connect(ui.actionRealtimeMirrorNone, SIGNAL(triggered(bool)), this, SLOT(setRealtimeMirrorNone(bool)));
    
    connect(ui.actionProjetHomepage, SIGNAL(triggered()), this, SLOT(openProjectHomepage()));
    connect(ui.actionDeveloperDocumentation, SIGNAL(triggered()), this, SLOT(openDoc()));
    connect(ui.actionUserManual, SIGNAL(triggered()), this, SLOT(openManual()));
}

void MainWindow::changeWindowTitle(const QString& message)
{
	QString title = QString("Blacksun") + message;
	this->setWindowTitle(title);
}

const QList<BSToolBoxItem*> MainWindow::getAvailableToolBoxItems()
{
    QList<BSToolBoxItem*> items;
    for(int i = 0 ; i < toolBox->count() ; i++)
    {
        items.append(qobject_cast<BSToolBoxItem*>(toolBox->widget(i)));
    }
    return items;
}

void MainWindow::addToolBoxItem(const QString& name, BSToolBoxItem* itemToAdd)
{
    toolBox->addItem(itemToAdd, name);
}

void MainWindow::readSettings()
{
    QSettings settings;
    setToolBarPosition(settings.value("mainwindow/toolbarPosition").toInt());
    restoreState(settings.value("mainwindow/defaultstate").toByteArray());
}

void MainWindow::takeScreenshot()
{
    ScreenshotMaker dialog(this);
    dialog.exec();
}

/*void MainWindow::takeScreenShot(const QString& viewToGrab)
{
    QFileDialog* dialog = new QFileDialog(this, "Choose Destination", QDir::currentPath());
    QStringList filters;
    filters << "JPEG (*.jpg *.jpeg)";
    filters << "BMP (*.bmp)";
    filters << "PNG (*.png)";
    filters << "PPM (*.ppm)";
    filters << "XBM (*.xbm)";
    filters << "PPM (*.ppm)";
    dialog->setFilters(filters);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
    {
        QString destination = dialog->selectedFiles()[0];
        if(destination != "")
        {
            GLWidget* widgetToGrab = getWidget(viewToGrab);
            Q_ASSERT(widgetToGrab != NULL);
            
            //QString suffix = getSuffixFromFilter(dialog->selectedFilter())[0];
            //Remove the *. from the suffix
            //suffix = suffix.remove(0, 2);
            
            QImage image;
            image = widgetToGrab->grabFrameBuffer();
            image.save(destination);
        }
    }
    delete dialog;
}*/

//This function is copied from QT source Code.
//This function takes a filter from a QFileDialog
//and returns a stringlist with all suffixes
//TODO make own file dialog class
/*QStringList MainWindow::getSuffixFromFilter(const QString& filter)
{
    const char *qt_file_dialog_filter_reg_exp =
            "([a-zA-Z0-9]*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$";
    QRegExp regexp(QString::fromLatin1(qt_file_dialog_filter_reg_exp));
    QString f = filter;
    int i = regexp.indexIn(f);
    if (i >= 0)
    {
        f = regexp.cap(2);
    }
    return f.split(QLatin1Char(' '), QString::SkipEmptyParts);
}*/

GLWidget* MainWindow::getGLWidget(QString key) const
{
    if(key == "3D")
    {
        key = "all";
    }
    key = key.toLower();
    GLWidget* current = NULL;
    for(int i = 0 ; i < container->getNumberOfWidgets() ; i++)
    {
        current = container->getWidget(i);
        if(current->objectName() == key)
        {
            return current;
        }
    }
    return NULL;
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("mainwindow/state", saveState());
    settings.setValue("mainwindow/toolbarPosition", getToolBarPosition());

    BSRenderer::RendererSettings* rendererSettings = Renderer::getInstance()->getSettings();

    settings.beginGroup("renderer");

    //Renderer Colors:

    Color wireframeColor = rendererSettings->getWireframeColor();
    settings.setValue("wireframeColor", ColorToQColor(wireframeColor));
    
    Color wireframeSelectionColor = rendererSettings->getWireframeSelectionColor();
    settings.setValue("wireframeSelectionColor", ColorToQColor(wireframeSelectionColor));
    
    Color pointColor = rendererSettings->getPointColor();
    settings.setValue("pointColor", ColorToQColor(pointColor));
    
    Color selectedPointsColor = rendererSettings->getPointSelectionColor();
    settings.setValue("selectedPointsColor", ColorToQColor(selectedPointsColor));
    
    Color freezedColor = rendererSettings->getFreezeColor();
    settings.setValue("freezedColor", ColorToQColor(freezedColor));
    
    Color normalColor = rendererSettings->getNormalColor();
    settings.setValue("normalColor", ColorToQColor(normalColor));
    
    Color selectionAABBColor = rendererSettings->getSelectionAABBColor();
    settings.setValue("selectionAABBColor", ColorToQColor(selectionAABBColor));
    
    Color colorOrtho = rendererSettings->getClearColorOrtho();
    settings.setValue("colorOrtho", ColorToQColor(colorOrtho));
    
    Color colorPerspective = rendererSettings->getClearColorPerspective();
    settings.setValue("colorPerspective", ColorToQColor(colorPerspective));
    
    Color majorLineColor = rendererSettings->getGridMajorLineColor();
    settings.setValue("majorLineColor", ColorToQColor(majorLineColor));
    
    Color minorLineColor = rendererSettings->getGridMinorLineColor();
    settings.setValue("minorLineColor", ColorToQColor(minorLineColor));
    
    settings.setValue("rubberBandColor", getContainer()->getSelectionBoxColor());
    
    settings.setValue("overpaintingColor", getContainer()->getOverpaintingColor());

    //Rendere Values

    int gridSize = rendererSettings->getGridSize();
    settings.setValue("gridSize", gridSize);

    double lineWidth = rendererSettings->getLineWidth();
    settings.setValue("lineWidth", lineWidth);
    
    double pointSize = rendererSettings->getPointSize();
    settings.setValue("pointSize", pointSize);
    
    double normalScaling = rendererSettings->getNormalScaling();
    settings.setValue("normalScaling", normalScaling);
    
    double selectionAABBScaling = rendererSettings->getSelectionAABBScaling();
    settings.setValue("selectionAABBScaling", selectionAABBScaling);
    
    double wireframeOverlayScaling = rendererSettings->getWireframeOverlayScaling();
    settings.setValue("wireframeOverlayScaling", wireframeOverlayScaling);
    
    double FOV = rendererSettings->getFOV();
    settings.setValue("FOV", FOV);
    
    double nearPlane = rendererSettings->getNearPlane();
    settings.setValue("nearPlane", nearPlane);
    
    double farPlane = rendererSettings->getFarPlane();
    settings.setValue("farPlane", farPlane);
    
    double mouseWheelSpeed = rendererSettings->getMouseWheelSpeed();
    settings.setValue("mouseWheelSpeed", mouseWheelSpeed);

    
    //Renderer booleans
    
    settings.setValue("frustumCulling", rendererSettings->getEnableFrustumCulling());
    settings.setValue("lineSmoothing", rendererSettings->getLineSmoothing());
    settings.setValue("pointSmoothing", rendererSettings->getPointSmoothing());
    settings.setValue("polygonSmoothing", rendererSettings->getPolygonSmoothing());

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    quit();
}

void MainWindow::setRealtimeMirrorXY(bool status)
{
    if(status)
    {
        BSRenderer::Renderer::getInstance()->getSettings()->setRealtimeMirror(BSRenderer::MIRROR_XY);
        BSScene::SceneManager::getInstance()->checkForRedraw(true);
    }
}
void MainWindow::setRealtimeMirrorYZ(bool status)
{
    if(status)
    {
        BSRenderer::Renderer::getInstance()->getSettings()->setRealtimeMirror(BSRenderer::MIRROR_YZ);
 		BSScene::SceneManager::getInstance()->checkForRedraw(true);        
    }
}
void MainWindow::setRealtimeMirrorXZ(bool status)
{
    if(status)
    {
        BSRenderer::Renderer::getInstance()->getSettings()->setRealtimeMirror(BSRenderer::MIRROR_XZ);
 		BSScene::SceneManager::getInstance()->checkForRedraw(true);        
    }
}

void MainWindow::setRealtimeMirrorNone(bool status)
{
    if(status)
    {
        BSRenderer::Renderer::getInstance()->getSettings()->setRealtimeMirror(BSRenderer::MIRROR_None);
 		BSScene::SceneManager::getInstance()->checkForRedraw(true);        
    }
}


void MainWindow::quit()
{
    writeSettings();
    _instance = NULL;
    this->deleteLater();
    QApplication::quit();
}

/*!
    \fn MainWindow::displaySettingsDialog()
 */
bool MainWindow::displaySettingsDialog()
{
    ConfigBase settingDialog;
    int result = settingDialog.exec();
    if (result == QDialog::Accepted)
    {
        return true;
    }
    return false;
}

void MainWindow::readRendererSettings()
{
    QSettings settings;

    //Renderer Colors:

    BSRenderer::RendererSettings* rendererSettings = Renderer::getInstance()->getSettings();

    settings.beginGroup("renderer");

    
    QVariant wireframeColorVariant = settings.value("wireframeColor");
    if (wireframeColorVariant.isValid())
    {
        QColor wireframeColor = wireframeColorVariant.value<QColor>();
        rendererSettings->setWireframeColor(QColorToColor(wireframeColor));
    }
    

    QVariant wireframeSelectionColorVariant  = settings.value("wireframeSelectionColor");
    if (wireframeSelectionColorVariant.isValid())
    {
        QColor wireframeSelectionColor = wireframeSelectionColorVariant.value<QColor>();
        rendererSettings->setWireframeSelectionColor(QColorToColor(wireframeSelectionColor));
    }
    

    QVariant pointColorVariant  = settings.value("pointColor");
    if (pointColorVariant.isValid())
    {
        QColor pointColor = pointColorVariant.value<QColor>();
        rendererSettings->setPointColor(QColorToColor(pointColor));
    }
    

    QVariant selectedPointsColorVariant  = settings.value("selectedPointsColor");
    if (selectedPointsColorVariant.isValid())
    {
        QColor selectedPointsColor = selectedPointsColorVariant.value<QColor>();
        rendererSettings->setPointSelectionColor(QColorToColor(selectedPointsColor));
    }
    

    QVariant freezedColorVariant  = settings.value("freezedColor");
    if (freezedColorVariant.isValid())
    {
        QColor freezedColor = freezedColorVariant.value<QColor>();
        rendererSettings->setFreezeColor(QColorToColor(freezedColor));
    }
    

    QVariant normalColorVariant  = settings.value("normalColor");
    if (normalColorVariant.isValid())
    {
        QColor normalColor = normalColorVariant.value<QColor>();
        rendererSettings->setNormalColor(QColorToColor(normalColor));
    }
    

    QVariant selectionAABBColorVariant  = settings.value("selectionAABBColor");
    if (selectionAABBColorVariant.isValid())
    {
        QColor selectionAABBColor = selectionAABBColorVariant.value<QColor>();
        rendererSettings->setSelectionAABBColor(QColorToColor(selectionAABBColor));
    }
    

    QVariant colorOrthoVariant = settings.value("colorOrtho");
    if (colorOrthoVariant.isValid())
    {
        QColor colorOrtho = colorOrthoVariant.value<QColor>();
        rendererSettings->setClearColorOrtho(QColorToColor(colorOrtho));
    }
    

    QVariant colorPerspectiveVariant  = settings.value("colorPerspective");
    if (colorPerspectiveVariant.isValid())
    {
        QColor colorPerspective = colorPerspectiveVariant.value<QColor>();
        rendererSettings->setClearColorPerspective(QColorToColor(colorPerspective));
    }
    

    QVariant majorLineColorVariant  = settings.value("majorLineColor");
    if (majorLineColorVariant.isValid())
    {
        QColor majorLineColor = majorLineColorVariant.value<QColor>();
        rendererSettings->setGridMajorLineColor(QColorToColor(majorLineColor));
    }
    

    QVariant minorLineColorVariant  = settings.value("minorLineColor");
    if (minorLineColorVariant.isValid())
    {
        QColor minorLineColor = minorLineColorVariant.value<QColor>();
        rendererSettings->setGridMinorLineColor(QColorToColor(minorLineColor));
    }
    
    QVariant rubberBandColorVariant  = settings.value("rubberBandColor", QColor::fromRgbF(1.0,0.0,0.0));
    if (minorLineColorVariant.isValid())
    {
        QColor rubberBandColor = rubberBandColorVariant.value<QColor>();
        getContainer()->setSelectionBoxColor(rubberBandColor);
    }
    
    QVariant overpaintingColorVariant  = settings.value("overpaintingColor", QColor::fromRgbF(1.0,0.0,0.0));
    if (minorLineColorVariant.isValid())
    {
        QColor overpaintingColor = overpaintingColorVariant.value<QColor>();
        getContainer()->setOverpaintingColor(overpaintingColor);
    }

    //Renderer Values

    int gridSize = settings.value("gridSize", 5).toInt();
    rendererSettings->setGridSize(gridSize);

    double lineWidth = settings.value("lineWidth", 1).toDouble();
    rendererSettings->setLineWidth(lineWidth);
    
    double pointSize = settings.value("pointSize", 5).toDouble();
    rendererSettings->setPointSize(pointSize);
    
    double normalScaling = settings.value("normalScaling", 0.5).toDouble();
    rendererSettings->setNormalScaling(normalScaling );
    
    double selectionAABBScaling = settings.value("selectionAABBScaling", 1.00).toDouble();
    rendererSettings->setSelectionAABBScaling(selectionAABBScaling);
    
    double wireframeOverlayScaling = settings.value("wireframeOverlayScaling", 1.0).toDouble();
    rendererSettings->setWireframeOverlayScaling(wireframeOverlayScaling);
    
    double FOV = settings.value("FOV", 45).toDouble();
    rendererSettings->setFOV(FOV);
    
    double nearPlane = settings.value("nearPlane", 0.1).toDouble();
    rendererSettings->setNearPlane(nearPlane);
    
    double farPlane = settings.value("farPlane", 10000).toDouble();
    rendererSettings->setFarPlane(farPlane);
    
    double mouseWheelSpeed = settings.value("mouseWheelSpeed", 0.05).toDouble();
    rendererSettings->setMouseWheelSpeed(mouseWheelSpeed);
    
    //Renderer Booleans
    
    rendererSettings->enableFrustumCulling(settings.value("frustumCulling", false).toBool());
    rendererSettings->setLineSmoothing(settings.value("lineSmoothing", true).toBool());
    rendererSettings->setPointSmoothing(settings.value("pointSmoothing", true).toBool());
    rendererSettings->setPolygonSmoothing(settings.value("polygonSmoothing", true).toBool());

    settings.endGroup();
}

void MainWindow::showAboutDialog()
{
    QString authors;
    QString url;
    QString version;
    QString information;
    QString aboutData;

    authors.append("<b>Authors:</b>");
    authors.append("<table width=400>");
    authors.append("<tr>");
    authors.append("<td>Philipp Gruber:</td> <td>Logger and Plugins</td>");
    authors.append("</tr>");
    authors.append("<tr>");
    authors.append("<td>Thomas Tischler:</td> <td>Graphical user interface</td>");
    authors.append("</tr>");
    authors.append("<tr>");
    authors.append(QString::fromUtf8("<td>Thomas Kuhndörfer:</td> <td>Scenegraph and Project Management</td>"));
    authors.append("</tr>");
    authors.append("<tr>");
    authors.append("<td>Reinhard Jeschull:</td> <td>Renderer and other OpenGL stuff</td>");
    authors.append("</tr>");
    authors.append("<tr>");
    authors.append("<td>Stefan Zeltner:</td> <td>Build Management and Pluginsystem</td>");
    authors.append("</tr>");
    authors.append("</table>");

    url = "<p>Please visit our <a href=\"http://sourceforge.net/projects/blacksun/\">Homepage</a></p>";

    version = "<b>Blacksun:</b>";
    version.append("<table width=200>");
    version.append("<tr>");
    version.append("<td>Logger:</td> <td>" + BSCore::Log::version.toString() + "</td>");
    version.append("</tr>");
    version.append("<tr>");
    version.append("<td>Undo/Redo - System:</td> <td>" + BSCore::UR_Manager::version.toString() + "</td>");
    version.append("</tr>");
    version.append("<tr>");
    version.append("<td>Gui:</td> <td>" + MainWindow::version.toString() + "</td>");
    version.append("</tr>");
    version.append("<tr>");
    version.append("<td>Pluginsystem</td> <td>" + BSPlgMgr::PlgMgr::version.toString() + "</td>");
    version.append("</tr>");
    version.append("<tr>");
    version.append("<td>Renderer:</td> <td>" + BSRenderer::Renderer::version.toString() + "</td>");
    version.append("</tr>");
    version.append("<tr>");
    version.append("<td>Scenegraph:</td> <td>" + BSScene::SceneManager::version.toString() + "</td>");
    version.append("</tr>");
    version.append("</table>");

    information = "<p>Blacksun is a free 3D modelling environment for static objects</p>";

    aboutData.append(version);
    aboutData.append(information);
    aboutData.append(authors);
    aboutData.append(url);

    QMessageBox::about(this, "About Blacksun",  aboutData);
}

void MainWindow::showAboutQtDialog()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::showStatusbarMessage(const QString& message)
{
    statusBar()->showMessage(message, 5000);
}

void MainWindow::setToolbarLeft()
{
    setToolBarPosition(toolBarLeft);
}

void MainWindow::setToolbarRight()
{
    setToolBarPosition(toolBarRight);
}

void MainWindow::createStdGLWidgets()
{
    GLWidget* front_glWidget = new GLWidget(this, true, FILL_Wireframe, ORTHO_Front);
    GLWidget* side_glWidget = new GLWidget(this, true, FILL_Wireframe, ORTHO_Left);
    GLWidget* top_glWidget = new GLWidget(this, true, FILL_Wireframe, ORTHO_Top);
    GLWidget* all_glWidget = new GLWidget(this, false, FILL_Wireframe);
    
    sharedWidget = front_glWidget->getShared();
    
    front_glWidget->setObjectName("front");
    side_glWidget->setObjectName("side");
    top_glWidget->setObjectName("top");
    all_glWidget->setObjectName("all");

    container = new GLWidgetContainer();
    container->addGLWidget(front_glWidget);
    container->addGLWidget(top_glWidget);
    container->addGLWidget(side_glWidget);
    container->addGLWidget(all_glWidget);
    centralWidget()->layout()->addWidget(container);

    container->restoreState();
    
    connect(container, SIGNAL(showGrid(bool)), ui.actionGrid, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showWireframe(bool)), ui.actionWireframe, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showWireframeOverlay(bool)), ui.actionWireframeOverlay, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showTextured(bool)), ui.actionTextured, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showPointsOnly(bool)), ui.actionPointsOnly, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showNormals(bool)), ui.actionNormals, SLOT(setChecked(bool)));
    connect(container, SIGNAL(showSolid(bool)), ui.actionSolid, SLOT(setChecked(bool)));
    
    connect(ui.actionGrid, SIGNAL(triggered(bool)), container, SLOT(setGrid(bool)));
    connect(ui.actionWireframe, SIGNAL(triggered(bool)), container, SLOT(setWireframe(bool)));
    connect(ui.actionWireframeOverlay, SIGNAL(triggered(bool)), container, SLOT(setWireframeOverlay(bool)));
    connect(ui.actionTextured, SIGNAL(triggered(bool)), container, SLOT(setTextured(bool)));
    connect(ui.actionPointsOnly, SIGNAL(triggered(bool)), container, SLOT(setPointMode(bool)));
    connect(ui.actionNormals, SIGNAL(triggered(bool)), container, SLOT(setNormals(bool)));
    connect(ui.actionSolid, SIGNAL(triggered(bool)), container, SLOT(setSolid(bool)));
    
    connect(front_glWidget, SIGNAL(mouseMovedRenderPosition(QPointF)), 
            this, SLOT(glWidgetMouseChanged(QPointF)));
    connect(side_glWidget, SIGNAL(mouseMovedRenderPosition(QPointF)), 
            this, SLOT(glWidgetMouseChanged(QPointF)));
    connect(top_glWidget, SIGNAL(mouseMovedRenderPosition(QPointF)), 
            this, SLOT(glWidgetMouseChanged(QPointF)));
    connect(all_glWidget, SIGNAL(mouseMovedRenderPosition(QPointF)), 
            this, SLOT(glWidgetMouseChanged(QPointF)));
}

void MainWindow::setDrawBackfaces(bool status)
{
    Renderer::getInstance()->getSettings()->setShowBackfaces(status);
    BSScene::SceneManager::getInstance()->checkForRedraw(true);
}

int MainWindow::getToolBarPosition() const
{
    if (centralWidget()->layout()->indexOf(toolBox) != 0)
    {
        return toolBarRight;
    }
    else
    {
        return toolBarLeft;
    }
}

void MainWindow::openProjectHomepage()
{
    QDesktopServices::openUrl(QUrl("http://sourceforge.net/projects/blacksun"));
}

void MainWindow::openDoc()
{
    QDesktopServices::openUrl(QUrl("http://sourceforge.net/projects/blacksun"));
}

void MainWindow::openManual()
{
    QDesktopServices::openUrl(QUrl("http://sourceforge.net/projects/blacksun"));
}

void MainWindow::setToolBarPosition(int pos)
{
    if (pos != getToolBarPosition())
    {
        QBoxLayout* layout = qobject_cast<QBoxLayout*>(centralWidget()->layout());
        if (layout == NULL)
        {
            qDebug() << "Error centralWidget has no QBoxLayout";
            return;
        }
        centralWidget()->layout()->removeWidget(seperatorLine);
        centralWidget()->layout()->removeWidget(toolBox);
        if (pos == toolBarLeft)
        {
            layout->insertWidget(0, seperatorLine);
            layout->insertWidget(0, toolBox);
            ui.actionToolbarLeft->setChecked(true);
        }
        else
        {
            layout->addWidget(seperatorLine);
            layout->addWidget(toolBox);
            ui.actionToolbarRight->setChecked(true);
        }
    }
}

void MainWindow::glWidgetMouseChanged(QPointF pos)
{
    posLabel->setText("X: " + QString::number(pos.x(), 'f', 4) + "  Y: " + QString::number(pos.y(), 'f', 4));
}

void MainWindow::snapToGridBoxClicked(int state)
{
    if(state == Qt::Unchecked)
    {
        ui.actionSnapToGrid->setChecked(false);
    }
    else
    {
        ui.actionSnapToGrid->setChecked(true);
    }
}

MainWindow::~MainWindow()
{
}

// for singleton:

MainWindow* MainWindow::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new MainWindow();
	}
	
	return _instance;
}

} // namespace BSGui
