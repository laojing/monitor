Empty                  =
Space                  = $(Empty) $(Empty)
temproot			   = $(subst /Source,$(Space),$(CURDIR))
DEV_ROOT			   = $(word 1,$(temproot))

Packages 		= \
	  			 Gui \
	  			 Project \

IncludeDirs 	= 

LibraryDirs 	= 

Libraries 		= 

GTKVERSION      = 3

LinkerOption    = -std=gnu99 `pkg-config --cflags --libs gtk+-$(GTKVERSION).0 gmodule-2.0` -lgfortran -lm -ldl

MainProg        = Gui/MainWindow.c

Main        	= monitor

include $(MAKEFILEDIR)/project
