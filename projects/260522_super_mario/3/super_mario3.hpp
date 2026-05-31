#pragma once
#include <windows.h>

namespace PY {
    class Object {
    public:
        float x, y;
        float width, height;
        char type;

        Object();
        Object(float x, float y, float w, float h, char t);
        void draw(char map[35][121]);
        bool check_collision(const Object& other);
    };
	
    class Moving {
    public:
        float x, y;
        float width, height;
        float vert_speed;
        float horizon_speed;
        bool is_fly;
        char type;

        Moving();
        Moving(float x, float y, float w, float h, char t);
        void draw(char map[35][121]);
        bool check_collision(const Object& other);
        bool check_collision(const Moving& other);
    };

    class Map {
    public:
        char data[35][121];
        
        void clear();
        void show();
        void put_score(int score);
    };

    class ObjectList {
    public:
        Object* bricks;
        Moving* movings;
        int bricks_count;
        int movings_count;
        int bricks_capacity;
        int movings_capacity;
        
        ObjectList();
        ~ObjectList();
        void add_brick(const Object& brick);
        void add_moving(const Moving& moving);
        void delete_moving(int index);
        void clear_all();
    };

    class Game {
    public:
        Map game_map;
        ObjectList objects;
        Moving mario;
        int level;
        int score;
        bool need_reload;
        
        int MAP_WIDTH;
        int MAP_HEIGHT;
        int MAX_LVL;
        int SCORE_MONSTER;
        int SCORE_COIN;
        float GRAVITY;
        float JUMP_SPEED;
        float MARIO_SPEED;
        float MONSTER_SPEED;
        float COIN_VERT_SPEED;
        
        Game(int map_width, int map_height, int max_lvl, int score_monster, 
             int score_coin, float gravity, float jump_speed, float mario_speed,
             float monster_speed, float coin_vert_speed);
        ~Game();
        void create_level(int lvl);
        void move_object(Moving& obj);
        void move_horizon(Moving& obj);
        void check_collisions();
        void player_dead();
        void scroll_world(float deltaX);
    };
}