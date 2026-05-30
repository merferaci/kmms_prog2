#pragma once

#include <windows.h>

namespace PY {

    struct SObject {
        float x, y;
        float width, height;
        float vertSpeed;
        float horizonSpeed;
        BOOL isFly;
        char cType;
    };

    struct GameState {
        char map[35][121];
        SObject mario;
        SObject* brick;
        int brickLength;
        SObject* moving;
        int movingLength;
        int level;
        int score;
        int needReload;

        int max_lvl;
        int score_monster;
        int score_coin;
        float gravity;
        float jump_speed;
        float mario_speed;
        float monster_speed;
        float coin_vert_speed;
    };

    void clear_map(char map[35][121]);
    void create_level(GameState& game, int lvl);
    void delete_moving(SObject*& moving, int& movingLength, int i);
    SObject* get_new_brick(SObject*& brick, int& brickLength);
    SObject* get_new_moving(SObject*& moving, int& movingLength);
    void horizon_move_map(GameState& game, float dx);
    void horizon_move_object(GameState& game, SObject* obj);
    void init_object(SObject* obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
    BOOL is_collision(const SObject& o1, const SObject& o2);
    BOOL is_pos_in_map(int x, int y);
    void mario_collision(GameState& game);
    void player_dead(GameState& game);
    void put_object_on_map(char map[35][121], const SObject& obj);
    void put_score_on_map(char map[35][121], int score);
    void set_cur(int x, int y);
    void set_object_pos(SObject* obj, float xPos, float yPos);
    void show_map_fast(const char map[35][121]);
    void vert_move_object(GameState& game, SObject* obj);

    const int MAP_WIDTH = 120;
    const int MAP_HEIGHT = 35;
}