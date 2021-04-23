## Definitions:

QMAKE_COMMAND := qmake-qt4
MAKE_COMMAND := make
DEL_WIN := rmdir /S /Q
DEL_FILE_WIN := del
DEL_LINUX := rm -rf

### all is not allowed
all :
	@echo 'Please select debugwin, debuglinux, releasewin, releaselinux'

### The cases:
debugwin : DIR = .\Win32\debug
debugwin : MODE = debug
debugwin : PRE = 
debugwin : POST = dll
debugwin : run
	@echo 'Build (Win32 : debug) complete.'

debuglinux : DIR = ./Linux/debug
debuglinux : MODE = debug
debuglinux : PRE = lib
debuglinux : POST = so
debuglinux : run
	@echo 'Build (Linux : debug) complete.'

releasewin : DIR = ./Win32/release
releasewin : MODE = release
releasewin : PRE = 
releasewin : POST = dll
releasewin : run
	@echo 'Build (Win32 : release) complete.'

releaselinux : DIR = ./Linux/release
releaselinux : MODE = release
releaselinux : PRE = lib
releaselinux : POST = so
releaselinux : run
	@echo 'Build (Linux : release) complete.'


### clean:
clean:
	@echo 'Please select cleanwin, cleanlinux'

cleanlinux: clean_file
	$(DEL_LINUX) ./blockgenerator/Linux
	$(DEL_LINUX) ./boxgenerator/Linux
	$(DEL_LINUX) ./convexhull/Linux
	$(DEL_LINUX) ./explode/Linux
	$(DEL_LINUX) ./extrude/Linux
	$(DEL_LINUX) ./geospheregenerator/Linux
	$(DEL_LINUX) ./gridgenerator/Linux
	$(DEL_LINUX) ./helixgenerator/Linux
	$(DEL_LINUX) ./imexporterpack/Linux
	$(DEL_LINUX) ./imexporterpack/object_script.ImExporterPack
	$(DEL_LINUX) ./insertvertex/Linux
	$(DEL_LINUX) ./makehole/Linux
	$(DEL_LINUX) ./manualedit/Linux
	$(DEL_LINUX) ./materialeditor/Linux
	$(DEL_LINUX) ./nboxgenerator/Linux
	$(DEL_LINUX) ./pyramidgenerator/Linux
	$(DEL_LINUX) ./revolving/Linux
	$(DEL_LINUX) ./smoothcorners/Linux
	$(DEL_LINUX) ./spheregenerator/Linux
	$(DEL_LINUX) ./spike/Linux
	$(DEL_LINUX) ./splitface/Linux	
	$(DEL_LINUX) ./subdivision/Linux
	$(DEL_LINUX) ./texturecoordinateeditor/Linux
	$(DEL_LINUX) ./textureloaderpack/Linux
	$(DEL_LINUX) ./textureloaderpack/object_script.object_script.TextureLoaderPack
	$(DEL_LINUX) ./torusgenerator/Linux
	$(DEL_LINUX) ./tubegenerator/Linux
	$(DEL_LINUX) ./updater/Linux
	$(DEL_LINUX) ./updaterApplication/Linux
	@echo 'cleanlinux complete.'

cleanwin: clean_file
	$(DEL_WIN) .\blockgenerator\Win32
	$(DEL_WIN) .\boxgenerator\Win32
	$(DEL_WIN) .\convexhull\Win32
	$(DEL_WIN) .\explode\Win32
	$(DEL_WIN) .\extrude\Win32
	$(DEL_WIN) .\geospheregenerator\Win32
	$(DEL_WIN) .\gridgenerator\Win32
	$(DEL_WIN) .\helixgenerator\Win32
	$(DEL_WIN) .\imexporterpack\Win32
	$(DEL_FILE_WIN) .\imexporterpack\object_script.ImExporterPack
	$(DEL_WIN) .\insertvertex\Win32
	$(DEL_WIN) .\makehole\Win32
	$(DEL_WIN) .\manualedit\Win32
	$(DEL_WIN) .\materialeditor\Win32
	$(DEL_WIN) .\nboxgenerator\Win32
	$(DEL_WIN) .\pyramidgenerator\Win32
	$(DEL_WIN) .\revolving\Win32
	$(DEL_WIN) .\smoothcorners\Win32
	$(DEL_WIN) .\spheregenerator\Win32
	$(DEL_WIN) .\spike\Win32
	$(DEL_WIN) .\splitface\Win32	
	$(DEL_WIN) .\subdivision\Win32
	$(DEL_WIN) .\texturecoordinateeditor\Win32
	$(DEL_WIN) .\textureloaderpack\Win32
	$(DEL_FILE_WIN) .\textureloaderpack\object_script.TextureLoaderPack
	$(DEL_WIN) .\torusgenerator\Win32
	$(DEL_WIN) .\tubegenerator\Win32
	$(DEL_WIN) .\updater\Win32
	$(DEL_WIN) .\updaterApplication\Win32
	@echo 'cleanwin complete.'
	
clean_file:
	$(MAKE_COMMAND) -C ./blockgenerator distclean
	$(MAKE_COMMAND) -C ./boxgenerator distclean
	$(MAKE_COMMAND) -C ./convexhull distclean
	$(MAKE_COMMAND) -C ./explode distclean
	$(MAKE_COMMAND) -C ./extrude distclean
	$(MAKE_COMMAND) -C ./geospheregenerator distclean
	$(MAKE_COMMAND) -C ./gridgenerator distclean
	$(MAKE_COMMAND) -C ./helixgenerator distclean
	$(MAKE_COMMAND) -C ./imexporterpack distclean
	$(MAKE_COMMAND) -C ./insertvertex distclean
	$(MAKE_COMMAND) -C ./makehole distclean
	$(MAKE_COMMAND) -C ./manualedit distclean
	$(MAKE_COMMAND) -C ./materialeditor distclean
	$(MAKE_COMMAND) -C ./nboxgenerator distclean
	$(MAKE_COMMAND) -C ./pyramidgenerator distclean
	$(MAKE_COMMAND) -C ./revolving distclean
	$(MAKE_COMMAND) -C ./smoothcorners distclean
	$(MAKE_COMMAND) -C ./spheregenerator distclean
	$(MAKE_COMMAND) -C ./spike distclean
	$(MAKE_COMMAND) -C ./splitface distclean	
	$(MAKE_COMMAND) -C ./subdivision distclean
	$(MAKE_COMMAND) -C ./texturecoordinateeditor distclean
	$(MAKE_COMMAND) -C ./textureloaderpack distclean
	$(MAKE_COMMAND) -C ./torusgenerator distclean
	$(MAKE_COMMAND) -C ./tubegenerator distclean
	$(MAKE_COMMAND) -C ./updater distclean
	$(MAKE_COMMAND) -C ./updaterApplication distclean


### run:   
run : $(DIR)/$(PRE)BoxGen.$(POST) \
        $(DIR)/$(PRE)NBoxGen.$(POST) \
		$(DIR)/$(PRE)SphereGen.$(POST) \
		$(DIR)/$(PRE)GridGen.$(POST) \
		$(DIR)/$(PRE)GeoSphereGen.$(POST) \
		$(DIR)/$(PRE)TorusGen.$(POST) \
		$(DIR)/$(PRE)HelixGen.$(POST) \
		$(DIR)/$(PRE)TextureLoaderPack.$(POST)\
		$(DIR)/$(PRE)PyramidGen.$(POST)\
		$(DIR)/$(PRE)TubeGen.$(POST) \
		$(DIR)/$(PRE)MaterialEditor.$(POST) \
		$(DIR)/$(PRE)Spike.$(POST) \
		$(DIR)/$(PRE)MakeHole.$(POST) \
		$(DIR)/$(PRE)ManualEdit.$(POST) \
		$(DIR)/$(PRE)ImExporterPack.$(POST) \
		$(DIR)/$(PRE)BlockGen.$(POST) \
		$(DIR)/$(PRE)ConvexHull.$(POST) \
		$(DIR)/$(PRE)Revolving.$(POST) \
		$(DIR)/$(PRE)Explode.$(POST) \
		$(DIR)/$(PRE)Extrude.$(POST) \
		$(DIR)/$(PRE)TextureCoordinateEditor.$(POST) \
		$(DIR)/$(PRE)Subdivision.$(POST) \
		$(DIR)/$(PRE)SmoothCorners.$(POST) \
		$(DIR)/$(PRE)InsertVertex.$(POST) \
		$(DIR)/$(PRE)SplitFace.$(POST) \
		$(DIR)/$(PRE)updater.$(POST) \
		$(DIR)/UpdaterApp

$(DIR)/$(PRE)BoxGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./boxgenerator/makefile mode=$(MODE) ./boxgenerator/BoxGen.pro
	$(MAKE_COMMAND) -C ./boxgenerator

$(DIR)/$(PRE)NBoxGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./nboxgenerator/makefile mode=$(MODE) ./nboxgenerator/NBoxGen.pro
	$(MAKE_COMMAND) -C ./nboxgenerator

$(DIR)/$(PRE)SphereGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./spheregenerator/makefile mode=$(MODE) ./spheregenerator/SphereGen.pro
	$(MAKE_COMMAND) -C ./spheregenerator

$(DIR)/$(PRE)GridGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./gridgenerator/makefile mode=$(MODE) ./gridgenerator/GridGen.pro
	$(MAKE_COMMAND) -C ./gridgenerator
	
$(DIR)/$(PRE)GeoSphereGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./geospheregenerator/makefile mode=$(MODE) ./geospheregenerator/GeoSphereGen.pro
	$(MAKE_COMMAND) -C ./geospheregenerator
	
$(DIR)/$(PRE)TorusGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./torusgenerator/makefile mode=$(MODE) ./torusgenerator/TorusGen.pro
	$(MAKE_COMMAND) -C ./torusgenerator
	
$(DIR)/$(PRE)HelixGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./helixgenerator/makefile mode=$(MODE) ./helixgenerator/HelixGen.pro
	$(MAKE_COMMAND) -C ./helixgenerator
	
$(DIR)/$(PRE)TextureLoaderPack.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./textureloaderpack/makefile mode=$(MODE) ./textureloaderpack/TextureLoaderPack.pro
	$(MAKE_COMMAND) -C ./textureloaderpack
	
$(DIR)/$(PRE)PyramidGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./pyramidgenerator/makefile mode=$(MODE) ./pyramidgenerator/PyramidGen.pro
	$(MAKE_COMMAND) -C ./pyramidgenerator
	
$(DIR)/$(PRE)TubeGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./tubegenerator/makefile mode=$(MODE) ./tubegenerator/TubeGen.pro
	$(MAKE_COMMAND) -C ./tubegenerator
	
$(DIR)/$(PRE)MaterialEditor.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./materialeditor/makefile mode=$(MODE) ./materialeditor/MaterialEditor.pro
	$(MAKE_COMMAND) -C ./materialeditor	
	
$(DIR)/$(PRE)Spike.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./spike/makefile mode=$(MODE) ./spike/Spike.pro
	$(MAKE_COMMAND) -C ./spike 
	
$(DIR)/$(PRE)MakeHole.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./makehole/makefile mode=$(MODE) ./makehole/MakeHole.pro
	$(MAKE_COMMAND) -C ./makehole 
	
$(DIR)/$(PRE)ImExporterPack.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./imexporterpack/makefile mode=$(MODE) ./imexporterpack/imexporterpack.pro
	$(MAKE_COMMAND) -C ./imexporterpack
	
$(DIR)/$(PRE)BlockGen.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./blockgenerator/makefile mode=$(MODE) ./blockgenerator/BlockGen.pro
	$(MAKE_COMMAND) -C ./blockgenerator
	
$(DIR)/$(PRE)ManualEdit.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./manualedit/makefile mode=$(MODE) ./manualedit/ManualEdit.pro
	$(MAKE_COMMAND) -C ./manualedit
	
$(DIR)/$(PRE)ConvexHull.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./convexhull/makefile mode=$(MODE) ./convexhull/ConvexHull.pro
	$(MAKE_COMMAND) -C ./convexhull
	
$(DIR)/$(PRE)Revolving.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./revolving/makefile mode=$(MODE) ./revolving/Revolving.pro
	$(MAKE_COMMAND) -C ./revolving
	
$(DIR)/$(PRE)Explode.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./explode/makefile mode=$(MODE) ./explode/Explode.pro
	$(MAKE_COMMAND) -C ./explode
	
$(DIR)/$(PRE)Extrude.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./extrude/makefile mode=$(MODE) ./extrude/Extrude.pro
	$(MAKE_COMMAND) -C ./extrude
	
$(DIR)/$(PRE)TextureCoordinateEditor.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./texturecoordinateeditor/makefile mode=$(MODE) ./texturecoordinateeditor/TextureCoordinateEditor.pro
	$(MAKE_COMMAND) -C ./texturecoordinateeditor

$(DIR)/$(PRE)Subdivision.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./subdivision/makefile mode=$(MODE) ./subdivision/Subdivision.pro
	$(MAKE_COMMAND) -C ./subdivision
	
$(DIR)/$(PRE)SmoothCorners.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./smoothcorners/makefile mode=$(MODE) ./smoothcorners/SmoothCorners.pro
	$(MAKE_COMMAND) -C ./smoothcorners

$(DIR)/$(PRE)InsertVertex.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./insertvertex/makefile mode=$(MODE) ./insertvertex/InsertVertex.pro
	$(MAKE_COMMAND) -C ./insertvertex

$(DIR)/$(PRE)SplitFace.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./splitface/makefile mode=$(MODE) ./splitface/SplitFace.pro
	$(MAKE_COMMAND) -C ./splitface

$(DIR)/$(PRE)updater.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./updater/makefile mode=$(MODE) ./updater/Updater.pro
	$(MAKE_COMMAND) -C ./updater
	
$(DIR)/UpdaterApp : 
	$(QMAKE_COMMAND) -makefile -o ./updaterApplication/makefile mode=$(MODE) ./updaterApplication/UpdaterApp.pro
	$(MAKE_COMMAND) -C ./updaterApplication
