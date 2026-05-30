#include "super_mario3.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace PY {
    void clear_map(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        for (int i = 0; i < MAP_WIDTH; i++) {
            map[0][i] = ' ';
        }
        map[0][MAP_WIDTH] = '\0';
        
        for (int j = 1; j < MAP_HEIGHT; j++) {
            memcpy(map[j], map[0], MAP_WIDTH + 1);
        }
    }
    
    void show_map_fast(const char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        static char buffer[MAP_HEIGHT * (MAP_WIDTH + 2)];
        int pos = 0;
        
        for (int j = 0; j < MAP_HEIGHT; j++) {
            memcpy(buffer + pos, map[j], MAP_WIDTH);
            pos += MAP_WIDTH;
            buffer[pos++] = '\n';
        }
        
        DWORD written;
        WriteConsole(hConsole, buffer, pos, &written, NULL);
    }
    
    void set_cursor_pos(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    
    void put_score_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], int score) {
        char text[30];
        sprintf(text, "Score: %d", score);
        int len = strlen(text);
        
        for (int i = 0; i < len; i++) {
            map[1][i + 5] = text[i];
        }
    }
    
    void put_object_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], const Object& obj) {
        int ix = (int)(obj.x + 0.5f);
        int iy = (int)(obj.y + 0.5f);
        int iw = (int)(obj.width + 0.5f);
        int ih = (int)(obj.height + 0.5f);
        
        for (int i = ix; i < ix + iw; i++) {
            for (int j = iy; j < iy + ih; j++) {
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                    map[j][i] = obj.type;
                }
            }
        }
    }
    
    void init_object(Object* obj, float x, float y, float w, float h, char t) {
        obj->x = x;
        obj->y = y;
        obj->width = w;
        obj->height = h;
        obj->vert_speed = 0;
        obj->horizon_speed = 0.2f;
        obj->is_fly = false;
        obj->type = t;
    }
    
    bool check_collision(const Object& o1, const Object& o2) {
        return o1.x + o1.width > o2.x && 
			o1.x < o2.x + o2.width &&
            o1.y + o1.height > o2.y && 
			o1.y < o2.y + o2.height;
    }
    
    void add_brick(GameState& game, const Object& brick) {
        Object* new_bricks = new Object[game.bricks_count + 1];
        for (int i = 0; i < game.bricks_count; i++) {
            new_bricks[i] = game.bricks[i];
        }
        new_bricks[game.bricks_count] = brick;
        delete[] game.bricks;
        game.bricks = new_bricks;
        game.bricks_count++;
    }
    
    void add_moving(GameState& game, const Object& moving) {
        Object* new_movings = new Object[game.movings_count + 1];
        for (int i = 0; i < game.movings_count; i++) {
            new_movings[i] = game.movings[i];
        }
        new_movings[game.movings_count] = moving;
        delete[] game.movings;
        game.movings = new_movings;
        game.movings_count++;
    }
    
    void delete_moving(GameState& game, int index) {
        for (int i = index; i < game.movings_count - 1; i++) {
            game.movings[i] = game.movings[i + 1];
        }
        game.movings_count--;
        
        if (game.movings_count > 0) {
            Object* new_movings = new Object[game.movings_count];
            for (int i = 0; i < game.movings_count; i++) {
                new_movings[i] = game.movings[i];
            }
            delete[] game.movings;
            game.movings = new_movings;
        } else {
            delete[] game.movings;
            game.movings = nullptr;
        }
    }
    
    void move_object(GameState& game, Object& obj) {
        obj.is_fly = true;
        obj.vert_speed += game.gravity;
        obj.y += obj.vert_speed;
        
        for (int i = 0; i < game.bricks_count; i++) {
            if (check_collision(obj, game.bricks[i])) {
                if (obj.vert_speed > 0) {
                    obj.is_fly = false;
                }
                
                if (game.bricks[i].type == '?' && 
					obj.vert_speed < 0 && 
					&obj == &game.mario) 
				{
                    game.bricks[i].type = '-';
                    Object coin;
                    init_object(&coin, game.bricks[i].x, game.bricks[i].y - 3, 3, 2, '$');
                    coin.vert_speed = game.coin_vert_speed;
                    add_moving(game, coin);
                }
                
                if (game.bricks[i].type == '+') {
                    game.level++;
                    if (game.level > game.max_lvl) {
                        game.level = 1;
                    }
                    system("color 2F");
                    Sleep(500);
                    game.need_reload = true;
                }
                
                obj.y -= obj.vert_speed;
                obj.vert_speed = 0;
                obj.is_fly = false;
                break;
            }
        }
    }
    
    void move_horizon(GameState& game, Object& obj) {
        obj.x += obj.horizon_speed;
        
        for (int i = 0; i < game.bricks_count; i++) {
            if (check_collision(obj, game.bricks[i])) {
                obj.x -= obj.horizon_speed;
                obj.horizon_speed = -obj.horizon_speed;
                return;
            }
        }
        
        if (obj.type == 'o') {
            Object temp = obj;
            move_object(game, temp);
            if (temp.is_fly) {
                obj.x -= obj.horizon_speed;
                obj.horizon_speed = -obj.horizon_speed;
            }
        }
    }
    
    void move_map(GameState& game, float dx) {
        float oldX = game.mario.x;
        game.mario.x -= dx;
        
        bool collision = false;
        for (int i = 0; i < game.bricks_count; i++) {
            if (check_collision(game.mario, game.bricks[i])) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            game.mario.x = oldX;
        } else {
            for (int i = 0; i < game.bricks_count; i++) {
                game.bricks[i].x += dx;
            }
            for (int i = 0; i < game.movings_count; i++) {
                game.movings[i].x += dx;
            }
        }
    }
    
    void create_level(GameState& game, int lvl) {
        system("color 9F");
        
        delete[] game.bricks;
        delete[] game.movings;
        
        game.bricks_count = 0;
        game.movings_count = 0;
        game.bricks = nullptr;
        game.movings = nullptr;
        
        init_object(&game.mario, 39, 10, 3, 3, '@');
        game.score = 0;
        game.need_reload = false;
        
        switch (lvl) {
            case 1:
                add_brick(game, Object{20, 20, 40, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{30, 10, 5, 3, 0, 0.2f, false, '?'});
                add_brick(game, Object{50, 10, 5, 3, 0, 0.2f, false, '?'});
                add_brick(game, Object{60, 15, 40, 10, 0, 0.2f, false, '#'});
                add_brick(game, Object{60, 5, 5, 3, 0, 0.2f, false, '-'});
                add_brick(game, Object{70, 5, 5, 3, 0, 0.2f, false, '?'});
                add_brick(game, Object{75, 5, 5, 3, 0, 0.2f, false, '-'});
                add_brick(game, Object{80, 5, 5, 3, 0, 0.2f, false, '?'});
                add_brick(game, Object{85, 5, 10, 3, 0, 0.2f, false, '-'});
                add_brick(game, Object{100, 20, 20, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{120, 15, 10, 10, 0, 0.2f, false, '#'});
                add_brick(game, Object{150, 20, 40, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{210, 15, 10, 10, 0, 0.2f, false, '+'});
                add_moving(game, Object{25, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{80, 10, 3, 2, 0, 0.2f, false, 'o'});
                break;
                
            case 2:
                add_brick(game, Object{20, 20, 40, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{60, 15, 10, 10, 0, 0.2f, false, '#'});
                add_brick(game, Object{80, 20, 20, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{120, 15, 10, 10, 0, 0.2f, false, '#'});
                add_brick(game, Object{150, 20, 40, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{210, 15, 10, 10, 0, 0.2f, false, '+'});
                add_moving(game, Object{25, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{80, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{65, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{120, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{160, 10, 3, 2, 0, 0.2f, false, 'o'});
                add_moving(game, Object{175, 10, 3, 2, 0, 0.2f, false, 'o'});
                break;
                
            case 3:
                add_brick(game, Object{20, 20, 40, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{80, 20, 15, 5, 0, 0.2f, false, '#'});
                add_brick(game, Object{120, 15, 15, 10, 0, 0.2f, false, '#'});
                add_brick(game, Object{160, 10, 15, 15, 0, 0.2f, false, '+'});
                break;
        }
    }
    
    void check_collisions(GameState& game) {
        for (int i = 0; i < game.movings_count; i++) {
            if (check_collision(game.mario, game.movings[i])) {
                if (game.movings[i].type == 'o') {
                    if (game.mario.is_fly && game.mario.vert_speed > 0 && 
                        game.mario.y + game.mario.height < game.movings[i].y + game.movings[i].height * 0.5) {
                        game.score += game.score_monster;
                        delete_moving(game, i);
                        i--;
                    } else {
                        player_dead(game);
                        return;
                    }
                }
                else if (game.movings[i].type == '$') {
                    game.score += game.score_coin;
                    delete_moving(game, i);
                    i--;
                }
            }
        }
    }
    
    void player_dead(GameState& game) {
        system("color 4F");
        Sleep(500);
        game.need_reload = true;
    }
}