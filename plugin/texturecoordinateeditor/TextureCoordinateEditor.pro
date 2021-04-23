#
# Projectfile for the Sphere-Generator
#
# by Xexis
#

# Headers:
HEADERS += ./include/TextureCoordinateEditor.h
HEADERS += ./include/TextureCoordinateEditorInterface.hh
HEADERS += ./include/TexCoordEditor.h
HEADERS += ./include/TexCoordWorkArea.h

# Sources:
SOURCES += ./src/TextureCoordinateEditor.cpp
SOURCES += ./src/TexCoordEditor.cpp
SOURCES += ./src/TexCoordWorkArea.cpp

# Forms:
FORMS += ./forms/TexCoordEditorDialog.ui

# The name of the output file (ending is add automatically):
TARGET = TextureCoordinateEditor

# The Path of the PluginInterface.hh:
INCPATH += ./../../editor/pluginsystem/include
INCPATH += ./../../editor/scenegraph/include

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

		LIBS += ./../../editor/Win32/debug/Scenegraph.dll
		LIBS += ./../../editor/Win32/debug/Renderer.dll
		LIBS += ./../../editor/Win32/debug/Core.dll
		LIBS += ./../../editor/Win32/debug/GUI.dll
		LIBS += ./../../editor/Win32/debug/PlgMgr.dll
		#LIBS += ./../../plugin/materialeditor/Win32/debug/MaterialEditor.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./Win32/release
		OBJECTS_DIR = ./Win32/release

		LIBS += ./../../editor/Win32/release/Scenegraph.dll
		LIBS += ./../../editor/Win32/release/Renderer.dll
		LIBS += ./../../editor/Win32/release/Core.dll
		LIBS += ./../../editor/Win32/release/GUI.dll
		LIBS += ./../../editor/Win32/release/PlgMgr.dll
		#LIBS += ./../../plugin/materialeditor/Win32/release/MaterialEditor.dll
	}
}
unix {
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./Linux/debug
		OBJECTS_DIR = ./Linux/debug

		LIBS += ./../../editor/Linux/debug/libScenegraph.so
		LIBS += ./../../editor/Linux/debug/libRenderer.so
		LIBS += ./../../editor/Linux/debug/libCore.so
		LIBS += ./../../editor/Linux/debug/libGUI.so
		LIBS += ./../../editor/Linux/debug/libPlgMgr.so
		#LIBS += ./../../plugin/materialeditor/Linux/debug/libMaterialEditor.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./Linux/release
		OBJECTS_DIR = ./Linux/release

		LIBS += ./../../editor/Linux/release/libScenegraph.so
		LIBS += ./../../editor/Linux/release/libRenderer.so
		LIBS += ./../../editor/Linux/release/libCore.so
		LIBS += ./../../editor/Linux/release/libGUI.so
		LIBS += ./../../editor/Linux/release/libPlgMgr.so
		#LIBS += ./../../plugin/materialeditor/Linux/debug/libMaterialEditor.so
	}
}

