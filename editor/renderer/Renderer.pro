#
# Projectfile for the renderer
#
# by tkuhndo
#

# Headers:
HEADERS += ./include/CacheObject.h
HEADERS += ./include/Camera.h
HEADERS += ./include/CameraArcball.h
HEADERS += ./include/CameraOrthogonal.h
HEADERS += ./include/Material.h
HEADERS += ./include/MaterialManager.h
HEADERS += ./include/RenderCache.h
HEADERS += ./include/Renderer.h
HEADERS += ./include/RendererSettings.h
HEADERS += ./include/RendererTypes.h
HEADERS += ./include/Texture.h
HEADERS += ./include/TextureLoaderPlugin.h
HEADERS += ./include/TextureManager.h
HEADERS += ./include/TextureState.h
HEADERS += ./contrib/GLee.h

# Sources:	
SOURCES += ./src/CacheObject.cpp
SOURCES += ./src/Camera.cpp
SOURCES += ./src/CameraArcball.cpp
SOURCES += ./src/CameraOrthogonal.cpp
SOURCES += ./src/Material.cpp
SOURCES += ./src/MaterialManager.cpp
SOURCES += ./src/RenderCache.cpp
SOURCES += ./src/Renderer.cpp
SOURCES += ./src/RendererSettings.cpp
SOURCES += ./src/Texture.cpp
SOURCES += ./src/TextureLoaderPlugin.cpp
SOURCES += ./src/TextureManager.cpp
SOURCES += ./src/TextureState.cpp
SOURCES += ./contrib/GLee.c

# The name of the output file (ending is add automatically):
TARGET = Renderer

# For the QT - Libs:
CONFIG += QT

# For OpenGL - Libs:
QT += opengl

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
	RC_FILE += Renderer_resource.rc

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