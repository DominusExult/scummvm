MODULE := engines/ultima

MODULE_OBJS = \
	core/file.o \
	core/message_target.o \
	core/mouse_cursor.o \
	core/named_item.o \
	core/project_item.o \
	core/resources.o \
	core/saveable_object.o \
	core/tree_item.o \
	gfx/font.o \
	gfx/screen.o \
	gfx/visual_container.o \
	gfx/visual_item.o \
	gfx/visual_surface.o \
	debugger.o \
	detection.o \
	events.o \
	game_manager.o \
	game_state.o \
	input_handler.o \
	input_translator.o \
	main_game_window.o \
	messages.o \
	ultima.o \
	games/shared/core/resources.o \
	games/ultima0/project_item.o \
	games/ultima0/core/resources.o \
	games/ultima1/project_item.o

# This module can be built as a plugin
ifeq ($(ENABLE_ULTIMA), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
