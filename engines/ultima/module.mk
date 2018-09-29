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
	gfx/character_input.o \
	gfx/font.o \
	gfx/popup.o \
	gfx/screen.o \
	gfx/sprites.o \
	gfx/text_cursor.o \
	gfx/text_input.o \
	gfx/visual_container.o \
	gfx/visual_item.o \
	gfx/visual_surface.o \
	debugger.o \
	detection.o \
	events.o \
	game_base.o \
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
	games/shared/core/party.o \
	games/shared/core/resources.o \
	games/shared/gfx/dungeon_surface.o \
	games/shared/gfx/info.o \
	games/shared/gfx/viewport_dungeon.o \
	games/shared/gfx/viewport_map.o \
	games/shared/maps/map.o \
	games/shared/maps/map_base.o \
	games/shared/maps/map_tile.o \
	games/shared/maps/map_widget.o \
	games/shared/maps/creature.o \
	games/ultima0/game.o \
	games/ultima0/core/resources.o \
	games/ultima1/game.o \
	games/ultima1/core/party.o \
	games/ultima1/core/resources.o \
	games/ultima1/maps/map.o \
	games/ultima1/maps/map_base.o \
	games/ultima1/maps/map_city_castle.o \
	games/ultima1/maps/map_dungeon.o \
	games/ultima1/maps/map_overworld.o \
	games/ultima1/maps/map_tile.o \
	games/ultima1/u1dialogs/buy_sell_dialog.o \
	games/ultima1/u1dialogs/combat.o \
	games/ultima1/u1dialogs/dialog.o \
	games/ultima1/u1dialogs/full_screen_dialog.o \
	games/ultima1/u1dialogs/drop.o \
	games/ultima1/u1dialogs/grocery.o \
	games/ultima1/u1dialogs/ready.o \
	games/ultima1/u1dialogs/stats.o \
	games/ultima1/u1gfx/drawing_support.o \
	games/ultima1/u1gfx/info.o \
	games/ultima1/u1gfx/sprites.o \
	games/ultima1/u1gfx/status.o \
	games/ultima1/u1gfx/text_cursor.o \
	games/ultima1/u1gfx/view_game.o \
	games/ultima1/u1gfx/view_char_gen.o \
	games/ultima1/u1gfx/view_title.o \
	games/ultima1/u1gfx/viewport_dungeon.o \
	games/ultima1/u1gfx/viewport_map.o \
	games/ultima1/u6gfx/game_view.o \
	games/ultima1/actions/action.o \
	games/ultima1/actions/attack.o \
	games/ultima1/actions/move.o \
	games/ultima1/actions/quit.o \
	games/ultima1/actions/ready.o \
	games/ultima1/actions/stats.o \
	games/ultima1/spells/spell.o \
	games/ultima1/spells/blink.o \
	games/ultima1/spells/create.o \
	games/ultima1/spells/destroy.o \
	games/ultima1/spells/kill_magic_missile.o \
	games/ultima1/spells/ladder_down.o \
	games/ultima1/spells/ladder_up.o \
	games/ultima1/spells/open_unlock.o \
	games/ultima1/spells/prayer.o \
	games/ultima1/spells/steal.o \
	games/ultima1/widgets/attack_effect.o \
	games/ultima1/widgets/person.o \
	games/ultima1/widgets/bard.o \
	games/ultima1/widgets/guard.o \
	games/ultima1/widgets/king.o \
	games/ultima1/widgets/merchant.o \
	games/ultima1/widgets/merchant_armor.o \
	games/ultima1/widgets/merchant_grocer.o \
	games/ultima1/widgets/merchant_magic.o \
	games/ultima1/widgets/merchant_tavern.o \
	games/ultima1/widgets/merchant_transport.o \
	games/ultima1/widgets/merchant_weapons.o \
	games/ultima1/widgets/princess.o \
	games/ultima1/widgets/wench.o \
	games/ultima1/widgets/dungeon_chest.o \
	games/ultima1/widgets/dungeon_coffin.o \
	games/ultima1/widgets/dungeon_item.o \
	games/ultima1/widgets/dungeon_player.o \
	games/ultima1/widgets/dungeon_widget.o \
	games/ultima1/widgets/dungeon_monster.o \
	games/ultima1/widgets/transport.o \
	games/ultima1/widgets/overworld_widget.o \
	games/ultima1/widgets/overworld_monster.o \
	games/ultima1/widgets/urban_player.o \
	games/ultima1/widgets/urban_widget.o


# This module can be built as a plugin
ifeq ($(ENABLE_ULTIMA), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
