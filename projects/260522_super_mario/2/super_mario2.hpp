#pragma once

#include <windows.h>

namespace PY {

    const int MAP_WIDTH = 120;
    const int MAP_HEIGHT = 35;
    const int MAX_LVL = 3;
    const int SCORE_MONSTER = 50;
    const int SCORE_COIN = 100;
    const float GRAVITY = 0.05f;
    const float JUMP_SPEED = -1.0f;
    const float MARIO_SPEED = 0.8f;
    const float MONSTER_SPEED = 0.2f;
    const float COIN_VERT_SPEED = -0.7f;

    struct SObject {
        float x, y;
        float width, height;
        float vertSpeed;
        float horizonSpeed;
        BOOL isFly;
        char cType;
    };

    struct GameState {
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        SObject mario;
        SObject* brick;
        int brickLength;
        SObject* moving;
        int movingLength;
        int level;
        int score;
        int needReload;  
    };

    void clear_map(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
    void show_map_fast(const char map[MAP_HEIGHT][MAP_WIDTH + 1]);
    void set_object_pos(SObject* obj, float xPos, float yPos);
    void init_object(SObject* obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
    BOOL is_collision(const SObject& o1, const SObject& o2);
    void delete_moving(SObject*& moving, int& movingLength, int i);
    void mario_collision(GameState& game);
    SObject* get_new_brick(SObject*& brick, int& brickLength);
    SObject* get_new_moving(SObject*& moving, int& movingLength);
    void put_score_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], int score);
    void vert_move_object(GameState& game, SObject* obj);
    void horizon_move_object(GameState& game, SObject* obj);
    BOOL is_pos_in_map(int x, int y);
    void put_object_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], const SObject& obj);
    void set_cur(int x, int y);
    void horizon_move_map(GameState& game, float dx);
    void create_level(GameState& game, int lvl);
    void player_dead(GameState& game);

}
