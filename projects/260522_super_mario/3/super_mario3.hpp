#pragma once
#include <windows.h>
// разделить гейм на классы : board; 
// object на object и moving 
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

    class Object {
    public:
        float x, y;
        float width, height;
        float vert_speed;
        float horizon_speed;
        bool is_fly;
        char type;

        Object();
        Object(float x, float y, float w, float h, char t);
        void draw(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
        bool check_collision(const Object& other);
    };

    class Game {
    private:
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Object* bricks;
        int bricks_count;
        int bricks_capacity;
        Object* movings;
        int movings_count;
        int movings_capacity;
        Object mario;
        int level;
        int score;
        bool need_reload;

        void clear_map();
        void show_map();
        void put_score();
        void add_brick(const Object& brick);
        void add_moving(const Object& moving);
        void delete_moving(int index);
        void move_object(Object& obj);
        void move_horizon(Object& obj);
        
    public:
        Game();
        ~Game();
        void run();
        void create_level(int lvl);
        void check_collisions();
        void player_dead();
    };
}