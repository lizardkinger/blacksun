#
# Projectfile for the gui
#
# by Xexis
#

# Headers:
HEADERS += ./include/configbase.h
HEADERS += ./include/configinterface.h
HEADERS += ./include/glWidget.h
HEADERS += ./include/mainwindow.h
HEADERS += ./include/pluginmanager.h
HEADERS += ./include/SplashScreen.h
HEADERS += ./include/styleconfig.h
HEADERS += ./include/logwindow.h
HEADERS += ./include/bsgroupbox.h
HEADERS += ./include/bstoolboxitem.h
HEADERS += ./include/objecttoolboxitem.h
HEADERS += ./include/glwidgetcontainer.h
HEADERS += ./include/bsflowlayout.h
HEADERS += ./include/renderersettingsdialog.h
HEADERS += ./include/bscolorchooser.h
HEADERS += ./include/bsglwidget.h
HEADERS += ./include/bsquadsplitterhandle.h
HEADERS += ./include/bsquadsplitter.h
HEADERS += ./include/bsquadsplitterhelper.h
HEADERS += ./include/bsquadsplittermidhandle.h
HEADERS += ./include/bsquadsplitterWidget.h
HEADERS += ./include/screenshotmaker.h
HEADERS += ./include/bsrenderwidget.h
HEADERS += ./include/objectexplorer.h
HEADERS += ./include/materialexplorer.h
HEADERS += ./include/mouseconfig.h
HEADERS += ./include/mousesettings.h
HEADERS += ./include/guialgoconst.h

# Sources:
SOURCES += ./src/configbase.cpp
SOURCES += ./src/glWidget.cpp
SOURCES += ./src/mainwindow.cpp
SOURCES += ./src/pluginmanager.cpp
SOURCES += ./src/SplashScreen.cpp
SOURCES += ./src/styleconfig.cpp
SOURCES += ./src/bstreewidgetitem.cpp
SOURCES += ./src/logwindow.cpp
SOURCES += ./src/bsgroupbox.cpp
SOURCES += ./src/bstoolboxitem.cpp
SOURCES += ./src/objecttoolboxitem.cpp
SOURCES += ./src/glwidgetcontainer.cpp
SOURCES += ./src/bsflowlayout.cpp
SOURCES += ./src/renderersettingsdialog.cpp
SOURCES += ./src/bscolorchooser.cpp
SOURCES += ./src/configinterface.cpp
SOURCES += ./src/bsglwidget.cpp
SOURCES += ./src/bsquadsplitterhandle.cpp
SOURCES += ./src/bsquadsplitter.cpp
SOURCES += ./src/bsquadsplitterhelper.cpp
SOURCES += ./src/bsquadsplittermidhandle.cpp
SOURCES += ./src/bsquadsplitterWidget.cpp
SOURCES += ./src/screenshotmaker.cpp
SOURCES += ./src/bsrenderwidget.cpp
SOURCES += ./src/objectexplorer.cpp
SOURCES += ./src/materialexplorer.cpp
SOURCES += ./src/mouseconfig.cpp
SOURCES += ./src/mousesettings.cpp
SOURCES += ./src/guialgoconst.cpp

# Forms:
FORMS += ./include/ConfigBase.ui
FORMS += ./include/MainWindow.ui
FORMS += ./include/lookandfeel.ui
FORMS += ./include/pluginConfig.ui
FORMS += ./include/logwindow.ui
FORMS += ./include/objectExplorer.ui
FORMS += ./include/rotateButtons.ui
FORMS += ./include/selectButtons.ui
FORMS += ./include/rendererSettings.ui
FORMS += ./include/screenshotMaker.ui
FORMS += ./include/mouseConfig.ui


# Resources
RESOURCES += actions.qrc

# The name of the output file (ending is add automatically):
TARGET = GUI

# For the QT - Libs:
CONFIG += QT

# Type of the output:
TEMPLATE = lib
CONFIG += dll

# For the openGL support:
QT += opengl

#For the XML support:
QT += xml

# Reset the debug/release option
CONFIG -= debug_and_release
CONFIG -= debug
CONFIG -= release

CONFIG += $$mode

# The destination of the output:
win32 {

	# Windows Resources
	RC_FILE += GUI_resource.rc

	debug {
		message(switch to Win32/debug)
    	DESTDIR = ./../Win32/debug
   		OBJECTS_DIR = ./Win32/debug
   		
   		LIBS += ./../Win32/debug/Core.dll
		LIBS += ./../Win32/debug/Renderer.dll
		LIBS += ./../Win32/debug/Scenegraph.dll   		
   		LIBS += ./../Win32/debug/PlgMgr.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./../Win32/release
		OBJECTS_DIR = ./Win32/release
		
		LIBS += ./../Win32/release/Core.dll
		LIBS += ./../Win32/release/Renderer.dll
		LIBS += ./../Win32/release/Scenegraph.dll		
   		LIBS += ./../Win32/release/PlgMgr.dll
	}
}
unix {
	
	#Fileversion
	VERSION = 1.0.0
	
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./../Linux/debug
		OBJECTS_DIR = ./Linux/debug
		
		LIBS += ./../Linux/debug/libCore.so
		LIBS += ./../Linux/debug/libRenderer.so
		LIBS += ./../Linux/debug/libScenegraph.so				
   		LIBS += ./../Linux/debug/libPlgMgr.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./../Linux/release
		OBJECTS_DIR = ./Linux/release
		
		LIBS += ./../Linux/release/libCore.so
		LIBS += ./../Linux/release/libRenderer.so
		LIBS += ./../Linux/release/libScenegraph.so		
   		LIBS += ./../Linux/release/libPlgMgr.so
	}
}
