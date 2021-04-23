#
# Projectfile for the application BlackSun
#
# by Xexis
#

# Headers:
HEADERS += ./../core/include/Log.h
HEADERS += ./../gui/include/mainwindow.h
HEADERS += ./../pluginsystem/include/PlgMgr.h
HEADERS += ./../scenegraph/include/SceneManager.h

# Sources:
SOURCES += ./src/main.cpp

# The name of the output file (ending is add automatically):
TARGET = BlackSun

# Resources for the project
RESOURCES     =  app.qrc

# For the QT - Libs:
CONFIG += QT
CONFIG += uitools

# For the openGL support:
QT += opengl

#For the xml support:
QT += xml

# Type of the output:
TEMPLATE = app

# Reset the debug/release option
CONFIG -= debug_and_release
CONFIG -= debug
CONFIG -= release

CONFIG += $$mode

# The destination of the output:
win32 {

	# Windows Resources
	RC_FILE += BlackSun_resource.rc

	debug {
		message(switch to Win32/debug)
    	DESTDIR = ./../Win32/debug
   		OBJECTS_DIR = ./Win32/debug
   		
		# For the Core - Libs
		LIBS += ./../Win32/debug/Core.dll
		LIBS += ./../Win32/debug/GUI.dll
		LIBS += ./../Win32/debug/PlgMgr.dll
		LIBS += ./../Win32/debug/Renderer.dll
		LIBS += ./../Win32/debug/Scenegraph.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./../Win32/release
		OBJECTS_DIR = ./Win32/release
   		
		# For the Core - Libs
		LIBS += ./../Win32/release/Core.dll
		LIBS += ./../Win32/release/GUI.dll
		LIBS += ./../Win32/release/PlgMgr.dll
		LIBS += ./../Win32/release/Renderer.dll
		LIBS += ./../Win32/release/Scenegraph.dll
	}
}
unix {
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./../Linux/debug
		OBJECTS_DIR = ./Linux/debug
   		
		# For the Core - Libs
		LIBS += ./../Linux/debug/libCore.so
		LIBS += ./../Linux/debug/libGUI.so
		LIBS += ./../Linux/debug/libPlgMgr.so
		LIBS += ./../Linux/debug/libRenderer.so
		LIBS += ./../Linux/debug/libScenegraph.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./../Linux/release
		OBJECTS_DIR = ./Linux/release
   		
		# For the Core - Libs
		LIBS += ./../Linux/release/libCore.so
		LIBS += ./../Linux/release/libGUI.so
		LIBS += ./../Linux/release/libPlgMgr.so
		LIBS += ./../Linux/release/libRenderer.so
		LIBS += ./../Linux/release/libScenegraph.so
	}
}
