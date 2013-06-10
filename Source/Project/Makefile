Empty                  =
Space                  = $(Empty) $(Empty)
temproot			   = $(subst /Source,$(Space),$(CURDIR))
DEV_ROOT			   = $(word 1,$(temproot))

Source 				   = \
						GetStateName.c \
						GetAlarmLevel.c \

include $(MAKEFILEDIR)/gtken
include $(DEV_ROOT)/Makefile
