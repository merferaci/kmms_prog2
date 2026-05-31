#include "super_mario3.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace PY {
    
    Object::Object() : x(0), y(0), width(0), height(0), type(' ') {}
    
    Object::Object(float x, float y, float w, float h, char t) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->type = t;
    }
    
    void Object::draw(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        int ix = (int)(x + 0.5f);
        int iy = (int)(y + 0.5f);
        int iw = (int)(width + 0.5f);
        int ih = (int)(height + 0.5f);
        
        for (int i = ix; i < ix + iw; i++) {
            for (int j = iy; j < iy + ih; j++) {
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                    map[j][i] = type;
                }
            }
        }
    }
    
    bool Object::check_collision(const Object& other) {
        return (x + width > other.x) && (x < other.x + other.width) &&
               (y + height > other.y) && (y < other.y + other.height);
    }
    
    Moving::Moving() : x(0), y(0), width(0), height(0), 
                       vert_speed(0), horizon_speed(MONSTER_SPEED), 
                       is_fly(false), type(' ') {}
    
    Moving::Moving(float x, float y, float w, float h, char t) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        vert_speed = 0;
        horizon_speed = MONSTER_SPEED;
        is_fly = false;
        type = t;
    }
    
    void Moving::draw(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        int ix = (int)(x + 0.5f);
        int iy = (int)(y + 0.5f);
        int iw = (int)(width + 0.5f);
        int ih = (int)(height + 0.5f);
        
        for (int i = ix; i < ix + iw; i++) {
            for (int j = iy; j < iy + ih; j++) {
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                    map[j][i] = type;
                }
            }
        }
    }
    
    bool Moving::check_collision(const Object& other) {
        return (x + width > other.x) && (x < other.x + other.width) &&
               (y + height > other.y) && (y < other.y + other.height);
    }
    
    bool Moving::check_collision(const Moving& other) {
        return (x + width > other.x) && (x < other.x + other.width) &&
               (y + height > other.y) && (y < other.y + other.height);
    }
    
    void Map::clear() {
        for (int i = 0; i < MAP_WIDTH; i++) {
            data[0][i] = ' ';
        }
        data[0][MAP_WIDTH] = '\0';
        
        for (int j = 1; j < MAP_HEIGHT; j++) {
            memcpy(data[j], data[0], MAP_WIDTH + 1);
        }
    }
    
    void Map::show() {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        static char buffer[MAP_HEIGHT * (MAP_WIDTH + 2)];
        int pos = 0;
        
        for (int j = 0; j < MAP_HEIGHT; j++) {
            memcpy(buffer + pos, data[j], MAP_WIDTH);
            pos += MAP_WIDTH;
            buffer[pos++] = '\n';
        }
        
        DWORD written;
        WriteConsole(hConsole, buffer, pos, &written, NULL);
    }
    
    void Map::put_score(int score) {
        char text[30];
        sprintf(text, "Score: %d", score);
        int len = strlen(text);
        
        for (int i = 0; i < len; i++) {
            data[1][i + 5] = text[i];
        }
    }
    
    ObjectList::ObjectList() {
        bricks = nullptr;
        movings = nullptr;
        bricks_count = 0;
        bricks_capacity = 10;
        movings_count = 0;
        movings_capacity = 10;
        
        bricks = new Object[bricks_capacity];
        movings = new Moving[movings_capacity];
    }
    
    ObjectList::~ObjectList() {
        delete[] bricks;
        delete[] movings;
    }
    
    void ObjectList::add_brick(const Object& brick) {
        if (bricks_count >= bricks_capacity) {
            bricks_capacity *= 2;
            Object* new_bricks = new Object[bricks_capacity];
            for (int i = 0; i < bricks_count; i++) {
                new_bricks[i] = bricks[i];
            }
            delete[] bricks;
            bricks = new_bricks;
        }
        bricks[bricks_count] = brick;
        bricks_count++;
    }
    
    void ObjectList::add_moving(const Moving& moving) {
        if (movings_count >= movings_capacity) {
            movings_capacity *= 2;
            Moving* new_movings = new Moving[movings_capacity];
            for (int i = 0; i < movings_count; i++) {
                new_movings[i] = movings[i];
            }
            delete[] movings;
            movings = new_movings;
        }
        movings[movings_count] = moving;
        movings_count++;
    }
    
    void ObjectList::delete_moving(int index) {
        for (int i = index; i < movings_count - 1; i++) {
            movings[i] = movings[i + 1];
        }
        movings_count--;
    }
    
    void ObjectList::clear_all() {
        delete[] bricks;
        delete[] movings;
        
        bricks_count = 0;
        bricks_capacity = 10;
        movings_count = 0;
        movings_capacity = 10;
        
        bricks = new Object[bricks_capacity];
        movings = new Moving[movings_capacity];
    }
    
    Game::Game() {
        level = 1;
        score = 0;
        need_reload = false;
        mario = Moving(39, 10, 3, 3, '@');
    }
    
    Game::~Game() {}
    
    void Game::create_level(int lvl) {
        system("color 9F");
        
        objects.clear_all();
        score = 0;
        need_reload = false;
        mario = Moving(39, 10, 3, 3, '@');
        
        switch (lvl) {
            case 1:
                objects.add_brick(Object(20, 20, 40, 5, '#'));
                objects.add_brick(Object(30, 10, 5, 3, '?'));
                objects.add_brick(Object(50, 10, 5, 3, '?'));
                objects.add_brick(Object(60, 15, 40, 10, '#'));
                objects.add_brick(Object(60, 5, 5, 3, '-'));
                objects.add_brick(Object(70, 5, 5, 3, '?'));
                objects.add_brick(Object(75, 5, 5, 3, '-'));
                objects.add_brick(Object(80, 5, 5, 3, '?'));
                objects.add_brick(Object(85, 5, 10, 3, '-'));
                objects.add_brick(Object(100, 20, 20, 5, '#'));
                objects.add_brick(Object(120, 15, 10, 10, '#'));
                objects.add_brick(Object(150, 20, 40, 5, '#'));
                objects.add_brick(Object(210, 15, 10, 10, '+'));
                objects.add_moving(Moving(25, 10, 3, 2, 'o'));
                objects.add_moving(Moving(80, 10, 3, 2, 'o'));
                break;
                
            case 2:
                objects.add_brick(Object(20, 20, 40, 5, '#'));
                objects.add_brick(Object(60, 15, 10, 10, '#'));
                objects.add_brick(Object(80, 20, 20, 5, '#'));
                objects.add_brick(Object(120, 15, 10, 10, '#'));
                objects.add_brick(Object(150, 20, 40, 5, '#'));
                objects.add_brick(Object(210, 15, 10, 10, '+'));
                objects.add_moving(Moving(25, 10, 3, 2, 'o'));
                objects.add_moving(Moving(80, 10, 3, 2, 'o'));
                objects.add_moving(Moving(65, 10, 3, 2, 'o'));
                objects.add_moving(Moving(120, 10, 3, 2, 'o'));
                objects.add_moving(Moving(160, 10, 3, 2, 'o'));
                objects.add_moving(Moving(175, 10, 3, 2, 'o'));
                break;
                
            case 3:
                objects.add_brick(Object(20, 20, 40, 5, '#'));
                objects.add_brick(Object(80, 20, 15, 5, '#'));
                objects.add_brick(Object(120, 15, 15, 10, '#'));
                objects.add_brick(Object(160, 10, 15, 15, '+'));
                break;
        }
    }
    
    void Game::move_object(Moving& obj) {
        obj.is_fly = true;
        obj.vert_speed += GRAVITY;
        obj.y += obj.vert_speed;
        
        for (int i = 0; i < objects.bricks_count; i++) {
            if (obj.check_collision(objects.bricks[i])) {
                if (obj.vert_speed > 0) {
                    obj.is_fly = false;
                }
                
                if (objects.bricks[i].type == '?' && obj.vert_speed < 0 && &obj == &mario) {
                    objects.bricks[i].type = '-';
                    Moving coin(objects.bricks[i].x, objects.bricks[i].y - 3, 3, 2, '$');
                    coin.vert_speed = COIN_VERT_SPEED;
                    objects.add_moving(coin);
                }
                
                if (objects.bricks[i].type == '+') {
                    level++;
                    if (level > MAX_LVL) {
                        level = 1;
                    }
                    system("color 2F");
                    Sleep(500);
                    need_reload = true;
                }
                
                obj.y -= obj.vert_speed;
                obj.vert_speed = 0;
                obj.is_fly = false;
                break;
            }
        }
    }
    
    void Game::move_horizon(Moving& obj) {
        obj.x += obj.horizon_speed;
        
        for (int i = 0; i < objects.bricks_count; i++) {
            if (obj.check_collision(objects.bricks[i])) {
                obj.x -= obj.horizon_speed;
                obj.horizon_speed = -obj.horizon_speed;
                return;
            }
        }
        
        if (obj.type == 'o') {
            Moving temp = obj;
            move_object(temp);
            if (temp.is_fly) {
                obj.x -= obj.horizon_speed;
                obj.horizon_speed = -obj.horizon_speed;
            }
        }
    }
    
    void Game::check_collisions() {
        for (int i = 0; i < objects.movings_count; i++) {
            if (mario.check_collision(objects.movings[i])) {
                if (objects.movings[i].type == 'o') {
                    if (mario.is_fly && mario.vert_speed > 0 && 
                        mario.y + mario.height < objects.movings[i].y + objects.movings[i].height * 0.5) {
                        score += SCORE_MONSTER;
                        objects.delete_moving(i);
                        i--;
                    } else {
                        player_dead();
                        return;
                    }
                }
                else if (objects.movings[i].type == '$') {
                    score += SCORE_COIN;
                    objects.delete_moving(i);
                    i--;
                }
            }
        }
    }
    
    void Game::player_dead() {
        system("color 4F");
        Sleep(500);
        need_reload = true;
    }
    
    void Game::scroll_world(float deltaX) {
        for (int i = 0; i < objects.bricks_count; i++) {
            objects.bricks[i].x -= deltaX;
        }
        for (int i = 0; i < objects.movings_count; i++) {
            objects.movings[i].x -= deltaX;
        }
    }
}