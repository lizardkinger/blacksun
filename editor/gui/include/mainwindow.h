/***************************************************************************
*   Copyright (C) 2006 by Thomas Tischler
*   ttischl1@fh-landshut.de
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
****************************************************************************
*
*  Module:     GUI (BlackSun)
*  File:       mainwindow.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/


/** \file mainwindow.h
 * \brief Defines MainWindow class.
 * 
 * \author Thomas Tischler.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QDebug>
#include "glWidget.h"
#include "./../ui_MainWindow.h"
#include "./../ui_selectButtons.h"
#include "./../ui_objectExplorer.h"
#include "./../ui_rotateButtons.h"
#include "./../include/bsgroupbox.h"
#include "./../include/glwidgetcontainer.h"
#include "logwindow.h"
#include "configbase.h"
#include "screenshotmaker.h"
#include "./../../core/include/Log.h"
#include "./../../core/include/UR_Manager.h"
#include "objecttoolboxitem.h"
#include "./../../renderer/include/RendererSettings.h"
#include "guialgoconst.h"

///////////////////////////////////////////////////////////
// for the Versions - Info
///////////////////////////////////////////////////////////
#include "./../../pluginsystem/include/BSBaseTypes.h"

//using namespace BSScene;

/*! \brief The gui component
*
*/
namespace BSGui
{

/*! \brief The Applications mainwindow.
* This widget represenst the meinwindow of BlackSun.
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/*!
	 * \brief The Version
	 */
	static BSPlgMgr::Version version;

public:
    virtual ~MainWindow();
    static MainWindow* getInstance();

  	/*!
	* \brief Returns the avilable items in the Toolbox
	*/
    const QList<BSToolBoxItem*> getAvailableToolBoxItems();

  	/*!
	* \brief Adds an item to the toolbox
	* 
	* \param name This will be displayed as a label for your item
	* \param itemToAdd The Widget you want to add
	*/
    void addToolBoxItem(const QString& name, BSToolBoxItem* itemToAdd);

    enum
    {
        toolBarLeft, /*!< Toolbar is on the left side */
        toolBarRight /*!< Toolbar is on the right side */
    };

    /*!
	* \brief Sets the toolbars position to pos
	* 
	* \param pos The toolbar position: either toolBarLeft or toolBarRight
	*/
    void setToolBarPosition(int pos);

    /*!
	* \brief Returns the toolbar position
	* 
	* \return The toolbar position: either toolBarLeft or toolBarRight
	*/
    int getToolBarPosition() const ;

    //static QStringList getSuffixFromFilter(const QString& filter);

    /*!
	* \brief Returns the glwidget with the spcified key
	* \param key The key. Valid keys are: top(for top widget), side(for side widget), front(for front widget), all or 3D(for the prepective widget) 
	* \return A pointer to the glwidget.
	*/
    GLWidget* getGLWidget(QString key) const;

    /*!
	* \brief Returns the Tool Bar
	* 
	* \return A Pointer to the Tool Bar
    */
    QToolBar* getToolBar() const {return ui.toolBar;}

    /*!
	* \brief Returns the Menu Bar
	* 
	* \return A Pointer to the Menu Bar
    */
    QMenuBar* getMenuBar() const {return ui.menuBar;}

    /*!
	* \brief Returns the Shared Widget
	* 
	* \return A Pointer to the Shared widget that is shaed by all glwidget
    */
    QGLWidget* getSharedWidget() const {return sharedWidget;}

    /*!
	* \brief Returns the Widgets Container
	* 
	* \return A Pointer to the Shared widget that is shaed by all glwidget
    */
    GLWidgetContainer* getContainer() const {return container;}

    /*!
	* \brief Returns the Snap to Grid Checkbox in the statusbar of the mainwindow
	* 
	* \return A Pointer to the Snap to Grid Checkbox in the statusbar of the mainwindow
    */
    QCheckBox* getSnapToGridBox() const {return snapToGridBox;}
    
    /*!
	* \brief Returns the position label in the statusbar of the mainwindow
	* 
	* \return A Pointer to the position label in the statusbar of the mainwindow
    */
    QLabel* getStatusBarPosLabel() const {return posLabel;}

public slots:
    /*!
	* \brief Ends the programm
    */
    void quit();

    /*!
	* \brief Shows the settings dialog
	* 
	* \return If the user exepts the dialog the return value is true otherwise false
    */
    bool displaySettingsDialog();

    /*!
	* \brief Shows the about dialog
    */
    void showAboutDialog();

    /*!
	* \brief Shows the about Qt dialog
    */
    void showAboutQtDialog();

    /*!
	* \brief Shows a temporary message in the statusbar
	* 
	* \param message The message to show
    */
    void showStatusbarMessage(const QString& message);

    /*!
	* \brief Sets the toolbar to the left position
    */
    void setToolbarLeft();

    /*!
	* \brief Sets the toolbar to the right position
    */
    void setToolbarRight();

    /*!
	* \brief Sets the RealtimeMirror of the XY Axis
	* 
	* \param status If this is true mirroring is enabled otherwise nothing will happen
    */
    void setRealtimeMirrorXY(bool status);
    /*!
	* \brief Sets the RealtimeMirror of the YZ Axis
	* 
	* \param status If this is true mirroring is enabled otherwise nothing will happen
    */
    void setRealtimeMirrorYZ(bool status);
    /*!
	* \brief Sets the RealtimeMirror of the XZ Axis
	* 
	* \param status If this is true mirroring is enabled otherwise nothing will happen
    */
    void setRealtimeMirrorXZ(bool status);
    /*!
	* \brief Disbales realtime mirroring
	* 
	* \param status If this is true mirroring will be diabled otherwise nothing will happen
    */
    void setRealtimeMirrorNone(bool status);

    /*!
	* \brief Shows the screenshot dialog
    */
    void takeScreenshot();
    
    /*!
	* \brief Chages the windowtitle
	* 
	* \param message The new Windowtitle
    */
    void changeWindowTitle(const QString& message);

    /*!
	* \brief Opens the project homepage in the users preferred browser
    */
    void openProjectHomepage();
    /*!
	* \brief Opens the API-Doku in the users preferred browser
    */
    void openDoc();
    /*!
	* \brief Opens the users manual in the users preferred browser
    */
    void openManual();
    
    /*!
	* \brief Enable or disable drawing the backface
	* 
	* \param status If the backface should be drawn
    */
    void setDrawBackfaces(bool status);


protected:
    /*!
	* \brief Handels the close event
    */
    void closeEvent(QCloseEvent *event);
    /*!
	* \brief Handels the show event
    */
    void showEvent(QShowEvent* event);

private:
    /*!
	* \brief The Constructor
    */
    MainWindow();
    static MainWindow* _instance; /*!< \brief The instance of this class */

    /*!
	* \brief Wirtes the Ssttings
    */
    void writeSettings();

    /*!
	* \brief Reads the settings of the renderer
    */
    void readRendererSettings();

    /*!
	* \brief Reads the Settings
    */
    void readSettings();

    /*!
	* \brief Sets up the glwidgets
    */
    void createStdGLWidgets();

    /*!
	* \brief Sets up the ui. The widgets etc.
    */
    void initializeUI();

    /*QDockWidget* top;
    QDockWidget* side;
    QDockWidget* front;
    QDockWidget* all;*/

    GLWidgetContainer* container; /*!< \brief The container for the glwidgets */

    QCheckBox* snapToGridBox; /*!< \brief The snap to grid checkbox */

    Ui_MainWindow ui; /*!< \brief The ui class generated by uic */
    QToolBox* toolBox; /*!< \brief The tool box of the mainwindow */
    //Ui_toolBoxWidget toolBox_ui;

    LogWindow* theLoggingWindow; /*!< \brief the log window */

    QFrame* seperatorLine; /*!< \brief The seperator line between the toolbox and the container */

    QGLWidget* sharedWidget; /*!< \brief A pointer to the glwidget that is shaed by the other glwidgets */

    QLabel* posLabel; /*!< \brief The pos label for the mouse coords */

    bool showFirstTime; /*!< \brief This is true if the window is shown for the first time */

private slots:
    /*!
	* \brief Enable or disbale the snap to grid box
	* 
	* \param state The state to gridbox should be set to see Qt::CheckState
    */
    void snapToGridBoxClicked(int state);

    /*!
	* \brief Sets the position label
	* 
	* \param pos The position the label should be set to
    */
    void glWidgetMouseChanged(QPointF pos);

};
}
#endif
