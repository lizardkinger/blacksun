#
# Projectfile for the tubegenerator
#


# Headers:
HEADERS += ./include/TubeGen.h
HEADERS += ./include/TubeGenInterface.hh

# Sources:
SOURCES += ./src/TubeGen.cpp

#Forms:
FORMS +=./include/settings.ui

# Resources
RESOURCES += tubegenerator_icon.qrc

# The name of the output file (ending is add automatically):
TARGET = TubeGen

# The Path of the PluginInterface.hh
INCPATH += ./../../editor/pluginsystem/include
INCPATH += ./../../editor/scenegraph/include

# For the QT - Libs:
CONFIG += QT
QT += xml

# Type of the output:
TEMPLATE = lib
CONFIG += dll

QT += opengl

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
   		
   		LIBS += ./../../editor/Win32/debug/Scenegraph.dll
   		LIBS += ./../../editor/Win32/debug/Renderer.dll
   		LIBS += ./../../editor/Win32/debug/Core.dll
   		LIBS += ./../../editor/Win32/debug/GUI.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./Win32/release
		OBJECTS_DIR = ./Win32/release
   		
   		LIBS += ./../../editor/Win32/release/Scenegraph.dll
   		LIBS += ./../../editor/Win32/release/Renderer.dll
   		LIBS += ./../../editor/Win32/release/Core.dll
   		LIBS += ./../../editor/Win32/release/GUI.dll
	}
}
unix {
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./Linux/debug
		OBJECTS_DIR = ./Linux/debug
   		
   		LIB += ./../../editor/Linux/debug/libScenegraph.so
   		LIB += ./../../editor/Linux/debug/libRenderer.so
   		LIB += ./../../editor/Linux/debug/libCore.so
   		LIB += ./../../editor/Linux/debug/libGUI.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./Linux/release
		OBJECTS_DIR = ./Linux/release
   		
   		LIB += ./../../editor/Linux/release/libScenegraph.so
   		LIB += ./../../editor/Linux/release/libRenderer.so
   		LIB += ./../../editor/Linux/release/libCore.so
   		LIB += ./../../editor/Linux/release/libGUI.so
	}
}
