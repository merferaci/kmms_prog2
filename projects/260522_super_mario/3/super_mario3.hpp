#pragma once
#include <windows.h>

namespace PY {
    const int MAP_WIDTH = 120;
    const int MAP_HEIGHT = 35;

    struct Object {
        float x, y;
        float width, height;
        float vert_speed;
        float horizon_speed;
        bool is_fly;
        char type;
    };

    struct GameState {
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Object mario;
        Object* bricks;
        int bricks_count;
        Object* movings;
        int movings_count;
        int level;
        int score;
        bool need_reload;

        int max_lvl;
        int score_monster;
        int score_coin;
        float gravity;
        float jump_speed;
        float mario_speed;
        float monster_speed;
        float coin_vert_speed;
    };

    void add_brick(GameState& game, const Object& brick);
    void add_moving(GameState& game, const Object& moving);
    bool check_collision(const Object& o1, const Object& o2);
    void check_collisions(GameState& game);
    void clear_map(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
    void create_level(GameState& game, int lvl);
    void delete_moving(GameState& game, int index);
    void init_object(Object* obj, float x, float y, float w, float h, char t);
    void move_horizon(GameState& game, Object& obj);
    void move_map(GameState& game, float dx);
    void move_object(GameState& game, Object& obj);
    void player_dead(GameState& game);
    void put_object_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], const Object& obj);
    void put_score_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], int score);
    void set_cursor_pos(int x, int y);
    void show_map_fast(const char map[MAP_HEIGHT][MAP_WIDTH + 1]);
}