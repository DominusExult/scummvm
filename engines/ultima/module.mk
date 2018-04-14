MODULE := engines/ultima

MODULE_OBJS = \
	core/base_object.o \
	core/file.o \
	core/game.o \
	core/message_target.o \
	core/mouse_cursor.o \
	core/named_item.o \
	core/resources.o \
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
	games/shared/ultima_game.o \
	games/shared/core/character.o \
	games/shared/core/game_state.o \
	games/shared/core/map.o \
	games/shared/core/resources.o \
	games/shared/gfx/game_view.o \
	games/shared/gfx/info.o \
	games/shared/gfx/status.o \
	games/shared/gfx/viewport_dungeon.o \
	games/shared/gfx/viewport_map.o \
	games/ultima0/project_item.o \
	games/ultima0/core/resources.o \
	games/ultima1/game.o \
	games/ultima1/core/map.o \
	games/ultima1/core/resources.o

# This module can be built as a plugin
ifeq ($(ENABLE_ULTIMA), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
