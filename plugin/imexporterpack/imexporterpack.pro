#
# Projectfile for the Sphere-Generator
#
# by Xexis
#

# Headers:
HEADERS += ./include/ImExporterPack.h
HEADERS += ./include/ImExporterPackInterface.hh
HEADERS += ./include/Importer.h
HEADERS += ./include/Importer3DS.h
HEADERS += ./include/Importer3DSTypes.h
HEADERS += ./include/Importer3DSLoader.h
HEADERS += ./include/ImporterMD2.h
HEADERS += ./include/ImporterMD2Types.h
HEADERS += ./include/ImporterMD2Loader.h
HEADERS += ./include/ImporterOBJ.h
HEADERS += ./include/ImporterOBJTypes.h
HEADERS += ./include/ImporterOBJLoader.h
HEADERS += ./include/Exporter.h
HEADERS += ./include/ExporterOBJ.h
HEADERS += ./include/ExporterOBJWriter.h

# Sources:
SOURCES += ./src/ImExporterPack.cpp
SOURCES += ./src/Importer.cpp
SOURCES += ./src/Importer3DS.cpp
SOURCES += ./src/Importer3DSLoader.cpp
SOURCES += ./src/ImporterMD2.cpp
SOURCES += ./src/ImporterMD2Loader.cpp
SOURCES += ./src/ImporterOBJ.cpp
SOURCES += ./src/ImporterOBJLoader.cpp
SOURCES += ./src/ExporterOBJ.cpp
SOURCES += ./src/ExporterOBJWriter.cpp

# Forms:
FORMS += ./forms/Importer3DSDialog.ui
FORMS += ./forms/ImporterOBJDialog.ui

# The name of the output file (ending is add automatically):
TARGET = ImExporterPack

# The Path of the PluginInterface.hh:
INCPATH += ./../../editor/pluginsystem/include
INCPATH += ./../../editor/scenegraph/include

# For the QT - Libs:
CONFIG += QT
CONFIG += Console

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

		LIBS += ./../../editor/Linux/debug/libScenegraph.so
		LIBS += ./../../editor/Linux/debug/libRenderer.so
		LIBS += ./../../editor/Linux/debug/libCore.so
		LIBS += ./../../editor/Linux/debug/libGUI.so
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./Linux/release
		OBJECTS_DIR = ./Linux/release

		LIBS += ./../../editor/Linux/release/libScenegraph.so
		LIBS += ./../../editor/Linux/release/libRenderer.so
		LIBS += ./../../editor/Linux/release/libCore.so
		LIBS += ./../../editor/Linux/release/libGUI.so
	}
}

