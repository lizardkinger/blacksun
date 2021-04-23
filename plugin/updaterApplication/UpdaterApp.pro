#
# Projectfile for the Updater Application
#
# by Xexis
#

# Headers:
HEADERS += ./include/UpdateDialog.h
HEADERS += ./include/Modul.h
HEADERS += ./include/PluginModul.h

# Sources:
SOURCES += ./src/UpdateDialog.cpp
SOURCES += ./src/UpdateMain.cpp

# Forms:
FORMS += ./forms/UpdateDialog.ui

# The name of the output file (ending is add automatically):
TARGET = UpdaterApp

# For the QT - Libs:
CONFIG += QT

# Type of the output:
TEMPLATE = app

# For the Network support:
QT += network

# For the XML support:
QT += xml

# The Path of the PluginInterface.hh:
INCPATH += ./../../editor/pluginsystem/include

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
	}
	release {
		message(switch to Win32/release)
		DESTDIR = ./Win32/release
		OBJECTS_DIR = ./Win32/release
	}
}
unix {
	debug {
		message(switch to Linux/debug)
		DESTDIR = ./Linux/debug
		OBJECTS_DIR = ./Linux/debug
	}
	release {
		message(switch to Linux/release)
		DESTDIR = ./Linux/release
		OBJECTS_DIR = ./Linux/release
	}
}

