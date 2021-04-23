#
# Projectfile for the core
#
# by Xexis
#

# Headers:
HEADERS += ./include/Log.h
HEADERS += ./include/Types.h
HEADERS += ./include/UR_Manager.h

# Sources:
SOURCES += ./src/Log.cpp
SOURCES += ./src/UR_Manager.cpp

# The name of the output file (ending is add automatically):
TARGET = Core

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
	RC_FILE += Core_resource.rc

	debug {
		message(switch to Win32/debug)
    	DESTDIR = ./../Win32/debug
   		OBJECTS_DIR = ./Win32/debug
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./../Win32/release
		OBJECTS_DIR = ./Win32/release
	}
}
unix {
	
	#Fileversion
	VERSION = 1.0.0
	
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./../Linux/debug
		OBJECTS_DIR = ./Linux/debug
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./../Linux/release
		OBJECTS_DIR = ./Linux/release
	}
}
