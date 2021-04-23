#
# Projectfile for the Updater
#
# by Xexis
#

# Headers:
HEADERS += ./include/Updater.h
HEADERS += ./include/Updater.hh

# Sources:
SOURCES += ./src/Updater.cpp

# The name of the output file (ending is add automatically):
TARGET = Updater

# The Path of the PluginInterface.hh:
INCPATH += ./../../editor/pluginsystem/include
INCPATH += ./../../editor/gui/include
INCPATH += ./../../editor/scenegraph/include/
INCPATH += ./../../editor/core/include/
INCPATH += ./../../editor/renderer/include/

# For the QT - Libs:
CONFIG += QT

# Type of the output:
TEMPLATE = lib
CONFIG += dll

# For the OpenGl support:
QT += opengl

# For the XML support:
QT += xml

# Reset the debug/release option
CONFIG -= debug_and_release
CONFIG -= debug
CONFIG -= release

CONFIG += $$mode

# The destination of the output:
win32 {
	debug {
		message(switch to Win32/debug)
		DESTDIR = ./Win32/debug
		OBJECTS_DIR = ./Win32/debug
		
		LIBS += ./../../editor/Win32/debug/Core.dll
		LIBS += ./../../editor/Win32/debug/GUI.dll
		LIBS += ./../../editor/Win32/debug/PlgMgr.dll
		LIBS += ./../../editor/Win32/debug/Renderer.dll
		LIBS += ./../../editor/Win32/debug/Scenegraph.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./Win32/release
		OBJECTS_DIR = ./Win32/release
		
		LIBS += ./../../editor/Win32/release/Core.dll
		LIBS += ./../../editor/Win32/release/GUI.dll
		LIBS += ./../../editor/Win32/release/PlgMgr.dll
		LIBS += ./../../editor/Win32/release/Renderer.dll
		LIBS += ./../../editor/Win32/release/Scenegraph.dll
	}
}
unix {
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./Linux/debug
		OBJECTS_DIR = ./Linux/debug
		
		LIBS += ./../../editor/Linux/debug/libGUI.so
		LIBS += ./../../editor/Linux/debug/libGUI.so
		LIBS += ./../../editor/Linux/debug/libPlgMgr.so
		LIBS += ./../../editor/Linux/debug/libRenderer.so
		LIBS += ./../../editor/Linux/debug/libScenegraph.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./Linux/release
		OBJECTS_DIR = ./Linux/release
		
		LIBS += ./../../editor/Linux/release/libGUI.so
		LIBS += ./../../editor/Linux/release/libGUI.so
		LIBS += ./../../editor/Linux/release/libPlgMgr.so
		LIBS += ./../../editor/Linux/release/libRenderer.so
		LIBS += ./../../editor/Linux/release/libScenegraph.so
	}
}

