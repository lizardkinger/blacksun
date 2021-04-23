## Definitions:

QMAKE_COMMAND := qmake
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

cleanwin: clean_file
	$(DEL_WIN) .\core\Win32
	$(DEL_WIN) .\pluginsystem\Win32
	$(DEL_WIN) .\renderer\Win32
	$(DEL_FILE_WIN) .\renderer\object_script.Renderer
	$(DEL_WIN) .\scenegraph\Win32
	$(DEL_FILE_WIN) .\scenegraph\object_script.Scenegraph
	$(DEL_WIN) .\gui\Win32
	$(DEL_FILE_WIN) .\gui\object_script.GUI
	$(DEL_WIN) .\blacksun\Win32
	$(DEL_WIN) .\Win32
	@echo 'cleanwin complete.'

cleanlinux: clean_file
	$(DEL_LINUX) ./core/Linux
	$(DEL_LINUX) ./pluginsystem/Linux
	$(DEL_LINUX) ./renderer/Linux
	$(DEL_LINUX) ./renderer/object_script.Renderer
	$(DEL_LINUX) ./scenegraph/Linux
	$(DEL_LINUX) ./scenegraph/object_script.Scenegraph
	$(DEL_LINUX) ./gui/Linux
	$(DEL_LINUX) ./gui/object_script.GUI
	$(DEL_LINUX) ./blacksun/Linux
	$(DEL_LINUX) ./Linux
	@echo 'cleanlinux complete.'

clean_file:
	$(MAKE_COMMAND) -C ./core/ distclean
	$(MAKE_COMMAND) -C ./pluginsystem/ distclean
	$(MAKE_COMMAND) -C ./renderer/ distclean
	$(MAKE_COMMAND) -C ./scenegraph/ distclean
	$(MAKE_COMMAND) -C ./gui/ distclean
	$(MAKE_COMMAND) -C ./blacksun/ distclean


### run:
run : $(DIR)/$(PRE)Log.$(POST) \
        $(DIR)/$(PRE)PlgMgr.$(POST) \
        $(DIR)/$(PRE)Renderer.$(POST) \
        $(DIR)/$(PRE)Scenegraph.$(POST) \
        $(DIR)/$(PRE)GUI.$(POST) \
        $(DIR)/BlackSun


$(DIR)/$(PRE)Log.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./core/makefile mode=$(MODE) ./core/Core.pro
	$(MAKE_COMMAND) -C ./core

$(DIR)/$(PRE)PlgMgr.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./pluginsystem/makefile mode=$(MODE) ./pluginsystem/PlgMgr.pro
	$(MAKE_COMMAND) -C ./pluginsystem
	
$(DIR)/$(PRE)Renderer.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./renderer/makefile mode=$(MODE) ./renderer/Renderer.pro
	$(MAKE_COMMAND) -C ./renderer

$(DIR)/$(PRE)Scenegraph.$(POST) : 
	$(QMAKE_COMMAND) -makefile -o ./scenegraph/makefile mode=$(MODE) ./scenegraph/Scenegraph.pro
	$(MAKE_COMMAND) -C ./scenegraph

$(DIR)/$(PRE)GUI.$(POST) : $(DIR)/$(PRE)PlgMgr.$(POST)
	$(QMAKE_COMMAND) -makefile -o ./gui/makefile mode=$(MODE) ./gui/GUI.pro
	$(MAKE_COMMAND) -C ./gui

$(DIR)/BlackSun :$(DIR)/$(PRE)Log.$(POST) \
        $(DIR)/$(PRE)PlgMgr.$(POST) \
        $(DIR)/$(PRE)GUI.$(POST) \
        $(DIR)/$(PRE)Renderer.$(POST) \
        $(DIR)/$(PRE)Scenegraph.$(POST)
	$(QMAKE_COMMAND) -makefile -o ./blacksun/makefile mode=$(MODE) ./blacksun/BlackSun.pro
	$(MAKE_COMMAND) -C ./blacksun

