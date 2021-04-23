#
# Projectfile for the pluginsystem
#
# by Xexis
#

# Headers:
HEADERS += ./include/PlgMgr.h
HEADERS += ./include/BSBaseTypes.h
HEADERS += ./include/PluginInterface.hh
HEADERS += ./include/DynCompLoader.h
HEADERS += ./include/BSPlugin.h
HEADERS += ./include/SearchForPlugins.h

# Sources:
SOURCES += ./src/PlgMgr.cpp
SOURCES += ./src/SearchForPlugins.cpp
SOURCES += ./src/BSPlugin.cpp

# The name of the output file (ending is add automatically):
TARGET = PlgMgr

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
	RC_FILE += PlgMgr_resource.rc

	debug {
		message(switch to Win32/debug)
    	DESTDIR = ./../Win32/debug
   		OBJECTS_DIR = ./Win32/debug
		
   		LIBS += ./../Win32/debug/Core.dll
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./../Win32/release
		OBJECTS_DIR = ./Win32/release
		
   		LIBS += ./../Win32/release/Core.dll
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
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./../Linux/release
		OBJECTS_DIR = ./Linux/release
		
   		LIBS += ./../Linux/release/libCore.so
	}
}