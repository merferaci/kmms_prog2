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

    class Object {
    public:
        float x, y;
        float width, height;
        float vertSpeed;
        float horizonSpeed;
        BOOL isFly;
        char type;

        Object();
        Object(float x, float y, float w, float h, char t);
        void draw(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
        bool collision(const Object& other);
    };

    class Game {
    private:
        char map[MAP_HEIGHT][MAP_WIDTH + 1];
        Object* bricks;
        int bricksCount;
        Object* movings;
        int movingsCount;
        Object mario;
        int level;
        int score;
        int needReload;

        void clearMap();
        void showMap();
        void putScore();
        void addBrick(Object brick);
        void addMoving(Object moving);
        void deleteMoving(int index);
        void moveObject(Object* obj);
        void moveHorizon(Object* obj);
        
    public:
        Game();
        ~Game();
        void run();
        void createLevel(int lvl);
        void checkCollisions();
        void playerDead();
    };
}