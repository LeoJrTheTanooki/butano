/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_log.h"

#include "bn_sprite_items_ninja.h"
#include "bn_sprite_items_ninja_npc.h"
#include "bn_regular_bg_items_map.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

int main()
{
    bn::core::init();

    constexpr bn::string_view info_text_lines[] = {
        "PAD: move ninja"};

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 32> text_sprites;
    common::info info("Map collision", info_text_lines, text_generator);

    bn::regular_bg_ptr map_bg = bn::regular_bg_items::map.create_bg(0, 0);

    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_ptr ninja_npc_sprite = bn::sprite_items::ninja_npc.create_sprite(16, 16);

    const bn::regular_bg_map_item &map_item = bn::regular_bg_items::map.map_item();

    int valid_tile_array[] = {
        bn::regular_bg_map_cell_info(map_item.cell(0, 0)).tile_index(),
        bn::regular_bg_map_cell_info(map_item.cell(1, 0)).tile_index(),
        bn::regular_bg_map_cell_info(map_item.cell(2, 0)).tile_index(),
        bn::regular_bg_map_cell_info(map_item.cell(3, 0)).tile_index(),
    };
    bool text_active = false;

    bn::point ninja_map_position(16, 16);
    // bn::point ninja_npc_map_position(16, 16);

    while (true)
    {
        bn::point new_ninja_map_position = ninja_map_position;

        if (bn::keypad::left_pressed())
        {
            new_ninja_map_position.set_x(new_ninja_map_position.x() - 1);
            ninja_sprite.set_horizontal_flip(true);
        }
        else if (bn::keypad::right_pressed())
        {
            new_ninja_map_position.set_x(new_ninja_map_position.x() + 1);
            ninja_sprite.set_horizontal_flip(false);
        }

        if (bn::keypad::up_pressed())
        {
            new_ninja_map_position.set_y(new_ninja_map_position.y() - 1);
        }
        else if (bn::keypad::down_pressed())
        {
            new_ninja_map_position.set_y(new_ninja_map_position.y() + 1);
        }

        if (bn::keypad::a_pressed())
        {
            if (!text_active)
            {
                if (
                    (ninja_sprite.x() - ninja_npc_sprite.x() == 0 && ninja_sprite.y() - ninja_npc_sprite.y() == -16) ||
                    (ninja_sprite.x() - ninja_npc_sprite.x() == -16 && ninja_sprite.y() - ninja_npc_sprite.y() == 0) ||
                    (ninja_sprite.x() - ninja_npc_sprite.x() == 0 && ninja_sprite.y() - ninja_npc_sprite.y() == 16) ||
                    (ninja_sprite.x() - ninja_npc_sprite.x() == 16 && ninja_sprite.y() - ninja_npc_sprite.y() == 0))
                {
                    text_generator.generate(0, 32, "Ah, hello there", text_sprites);
                    text_active = true;
                }
            }
            else
            {
                text_sprites.clear();
                text_active = false;
            }
        }

        // if (bn::keypad::b_pressed())
        // {
        //     // BN_LOG(ninja_map_position.x(), " ", ninja_map_position.y());
        //     // BN_LOG(ninja_npc_map_position.x(), " ", ninja_npc_map_position.y());
        //     // BN_LOG("Ninja Coords X:", ninja_sprite.x(), " Y:", ninja_sprite.y());
        //     // BN_LOG("NPC Coords X:", ninja_npc_sprite.x(), " Y:", ninja_npc_sprite.y());
        //     BN_LOG("Ninja Coords X:", ninja_sprite.x() - ninja_npc_sprite.x(), " Y:", ninja_sprite.y() - ninja_npc_sprite.y());
        //     // BN_LOG("NPC Coords X:", ninja_npc_sprite.x() - ninja_npc_sprite.x(), " Y:", ninja_npc_sprite.y() - ninja_npc_sprite.y());
        //     BN_LOG(" ");
        // }

        bn::regular_bg_map_cell ninja_map_cell = map_item.cell(new_ninja_map_position);
        int ninja_tile_index = bn::regular_bg_map_cell_info(ninja_map_cell).tile_index();

        for (int i = 0; i < sizeof(valid_tile_array) / sizeof(int); i++)
        {
            if (ninja_tile_index == valid_tile_array[i])
            {
                ninja_map_position = new_ninja_map_position;
            }
        }

        bn::fixed ninja_sprite_x = (ninja_map_position.x() * 8) - (map_item.dimensions().width() * 4);
        bn::fixed ninja_sprite_y = (ninja_map_position.y() * 8) - (map_item.dimensions().height() * 4);
        ninja_sprite.set_position(ninja_sprite_x, ninja_sprite_y);
        // ninja_npc_sprite.set_position()

        info.update();
        bn::core::update();
    }
}
