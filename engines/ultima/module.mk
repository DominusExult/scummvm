MODULE := engines/ultima

MODULE_OBJS = \
	core/base_object.o \
	core/file.o \
	core/lzw.o \
	core/message_target.o \
	core/mouse_cursor.o \
	core/named_item.o \
	core/resources.o \
	core/tree_item.o \
	gfx/bitmap.o \
	gfx/font.o \
	gfx/screen.o \
	gfx/sprites.o \
	gfx/visual_container.o \
	gfx/visual_item.o \
	gfx/visual_surface.o \
	debugger.o \
	detection.o \
	events.o \
	game_base.o \
	game_state.o \
	input_handler.o \
	input_translator.o \
	messages.o \
	utils.o \
	ultima.o \
	games/shared/game.o \
	games/shared/actions/action.o \
	games/shared/actions/huh.o \
	games/shared/actions/pass.o \
	games/shared/core/character.o \
	games/shared/core/game_state.o \
	games/shared/core/map.o \
	games/shared/core/resources.o \
	games/shared/core/widgets.o \
	games/shared/gfx/dungeon_surface.o \
	games/shared/gfx/info.o \
	games/shared/gfx/viewport_dungeon.o \
	games/shared/gfx/viewport_map.o \
	games/ultima0/game.o \
	games/ultima0/core/resources.o \
	games/ultima1/game.o \
	games/ultima1/core/game_state.o \
	games/ultima1/core/resources.o \
	games/ultima1/core/transports.o \
	games/ultima1/map/map.o \
	games/ultima1/map/map_city_castle.o \
	games/ultima1/map/map_dungeon.o \
	games/ultima1/map/map_overworld.o \
	games/ultima1/u1gfx/game_view.o \
	games/ultima1/u1gfx/drawing_support.o \
	games/ultima1/u1gfx/info.o \
	games/ultima1/u1gfx/sprites.o \
	games/ultima1/u1gfx/status.o \
	games/ultima1/u1gfx/text_cursor.o \
	games/ultima1/u1gfx/viewport_dungeon.o \
	games/ultima1/u1gfx/viewport_map.o \
	games/ultima1/u6gfx/game_view.o \
	games/ultima1/actions/action.o \
	games/ultima1/actions/climb.o \
	games/ultima1/actions/enter.o \
	games/ultima1/actions/move.o \
	games/ultima1/widgets/person.o \
	games/ultima1/widgets/bard.o \
	games/ultima1/widgets/guard.o \
	games/ultima1/widgets/king.o \
	games/ultima1/widgets/princess.o \
	games/ultima1/widgets/wench.o \
	games/ultima1/widgets/dungeon_player.o \
	games/ultima1/widgets/dungeon_widget.o \
	games/ultima1/widgets/dungeon_monster.o \
	games/ultima1/widgets/transport.o \
	games/ultima1/widgets/overworld_monster.o


# This module can be built as a plugin
ifeq ($(ENABLE_ULTIMA), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
