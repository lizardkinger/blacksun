#
# Projectfile for the scenegraph
#
# by tkuhndo
#

# Headers:
HEADERS += ./include/SceneObject.h
HEADERS += ./include/Polygon.h
HEADERS += ./include/Mesh.h
HEADERS += ./include/Scene.h
HEADERS += ./include/SelectionBuffer.h
HEADERS += ./include/MenuHandler.h
HEADERS += ./include/SceneManager.h
HEADERS += ./include/SceneGraph.h
HEADERS += ./include/SceneVertex.h
HEADERS += ./include/SelectedVertex.h
HEADERS += ./include/TextureOffRepDlg.h
HEADERS += ./include/Cmd_insertObject.h
HEADERS += ./include/SceneUtils.h
HEADERS += ./include/SceneAction2D.h

# Sources:	

SOURCES += ./src/Polygon.cpp
SOURCES += ./src/Mesh.cpp
SOURCES += ./src/Scene.cpp
SOURCES += ./src/SelectionBuffer.cpp
SOURCES += ./src/MenuHandler.cpp
SOURCES += ./src/SceneManager.cpp
SOURCES += ./src/SceneVertex.cpp
SOURCES += ./src/TextureOffRepDlg.cpp
SOURCES += ./src/Cmd_insertObject.cpp
SOURCES += ./src/SceneUtils.cpp

# Forms:
FORMS += ./forms/TextureOffRepDlg.ui

# The name of the output file (ending is add automatically):
TARGET = Scenegraph

# For the QT - Libs:
CONFIG += QT

# Type of the output:
TEMPLATE = lib
CONFIG += dll

# Reset the debug/release option
CONFIG -= debug_and_release
CONFIG -= debug
CONFIG -= release

CONFIG += $$mode

# The destination of the output:
win32 {

	# Windows Resources
	RC_FILE += Scenegraph_resource.rc

	debug {
		message(switch to Win32/debug)
    	DESTDIR = ./../Win32/debug
   		OBJECTS_DIR = ./Win32/debug
   		
   		LIBS += ./../Win32/debug/Core.dll
		LIBS += ./../Win32/debug/Renderer.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./../Win32/release
		OBJECTS_DIR = ./Win32/release
		
		LIBS += ./../Win32/release/Core.dll
		LIBS += ./../Win32/release/Renderer.dll
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
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./../Linux/release
		OBJECTS_DIR = ./Linux/release
		
		LIBS += ./../Linux/release/libCore.so
		LIBS += ./../Linux/release/libRenderer.so
	}
}