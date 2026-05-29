#include "super_mario2.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

namespace PY {

    void clear_map(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        for (int i = 0; i < MAP_WIDTH; i++)
            map[0][i] = ' ';
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

    void set_object_pos(SObject* obj, float xPos, float yPos) {
        obj->x = xPos;
        obj->y = yPos;
    }

    void init_object(SObject* obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {
        obj->x = xPos;
        obj->y = yPos;
        obj->width = oWidth;
        obj->height = oHeight;
        obj->vertSpeed = 0;
        obj->horizonSpeed = MONSTER_SPEED;
        obj->isFly = FALSE;
        obj->cType = inType;
    }

    BOOL is_collision(const SObject& o1, const SObject& o2) {
        return o1.x + o1.width > o2.x &&
               o1.x < o2.x + o2.width &&
               o1.y + o1.height > o2.y &&
               o1.y < o2.y + o2.height;
    }

    void delete_moving(SObject*& moving, int& movingLength, int i) {
        movingLength--;
        moving[i] = moving[movingLength];
        moving = (SObject*)realloc(moving, sizeof(*moving) * movingLength);
    }

    void mario_collision(GameState& game) {
        for (int i = 0; i < game.movingLength; i++) {
            if (is_collision(game.mario, game.moving[i])) {
                if (game.moving[i].cType == 'o') {
                    if (game.mario.isFly == TRUE &&
                        game.mario.vertSpeed > 0 &&
                        game.mario.y + game.mario.height < game.moving[i].y + game.moving[i].height * 0.5) 
					{
                        game.score += SCORE_MONSTER;
                        delete_moving(game.moving, game.movingLength, i);
                        i--;
                        continue;
                    } 
					else {
                        player_dead(game);
                        return;
                    }
                }

                if (game.moving[i].cType == '$') {
                    game.score += SCORE_COIN;
                    delete_moving(game.moving, game.movingLength, i);
                    i--;
                    continue;
                }
            }
        }
    }

    SObject* get_new_brick(SObject*& brick, int& brickLength) {
        brickLength++;
        brick = (SObject*)realloc(brick, sizeof(*brick) * brickLength);
        return brick + brickLength - 1;
    }

    SObject* get_new_moving(SObject*& moving, int& movingLength) {
        movingLength++;
        moving = (SObject*)realloc(moving, sizeof(*moving) * movingLength);
        return moving + movingLength - 1;
    }

    void put_score_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], int score) {
        char c[30];
        sprintf(c, "Score: %d", score);
        int len = strlen(c);
        for (int i = 0; i < len; i++) {
            map[1][i + 5] = c[i];
        }
    }

    void vert_move_object(GameState& game, SObject* obj) {
        obj->isFly = TRUE;
        obj->vertSpeed += GRAVITY;
        obj->y += obj->vertSpeed;

        for (int i = 0; i < game.brickLength; i++) {
            if (is_collision(*obj, game.brick[i])) {
                if (obj->vertSpeed > 0) {
                    obj->isFly = FALSE;
                }

                if (game.brick[i].cType == '?' &&
                    obj->vertSpeed < 0 &&
                    obj == &game.mario) 
				{
                    game.brick[i].cType = '-';
                    SObject* newMoving = get_new_moving(game.moving, game.movingLength);
                    init_object(newMoving, game.brick[i].x, game.brick[i].y - 3, 3, 2, '$');
                    game.moving[game.movingLength - 1].vertSpeed = COIN_VERT_SPEED;
                }

                obj->y -= obj->vertSpeed;
                obj->vertSpeed = 0;
                obj->isFly = FALSE;

                if (game.brick[i].cType == '+') {
                    game.level++;
                    if (game.level > MAX_LVL) {
                        game.level = 1;
                    }
                    system("color 2F");
                    Sleep(500);
                    game.needReload = 1;
                }
                break;
            }
        }
    }

    void horizon_move_object(GameState& game, SObject* obj) {
        obj->x += obj->horizonSpeed;

        for (int i = 0; i < game.brickLength; i++) {
            if (is_collision(obj[0], game.brick[i])) {
                obj->x -= obj->horizonSpeed;
                obj->horizonSpeed = -obj->horizonSpeed;
                return;
            }
        }

        if (obj->cType == 'o') {
            SObject tmp = *obj;
            vert_move_object(game, &tmp);
            if (tmp.isFly == TRUE) {
                obj->x -= obj->horizonSpeed;
                obj->horizonSpeed = -obj->horizonSpeed;
            }
        }
    }

    BOOL is_pos_in_map(int x, int y) {
        return x >= 0 && x < MAP_WIDTH &&
               y >= 0 && y < MAP_HEIGHT;
    }

    void put_object_on_map(char map[MAP_HEIGHT][MAP_WIDTH + 1], const SObject& obj) {
        int ix = (int)(obj.x + 0.5f);
        int iy = (int)(obj.y + 0.5f);
        int iWidth = (int)(obj.width + 0.5f);
        int iHeight = (int)(obj.height + 0.5f);

        for (int i = ix; i < ix + iWidth; i++) {
            for (int j = iy; j < iy + iHeight; j++) {
                if (is_pos_in_map(i, j)) {
                    map[j][i] = obj.cType;
                }
            }
        }
    }

    void set_cur(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void horizon_move_map(GameState& game, float dx) {
        float oldX = game.mario.x;
        game.mario.x -= dx;

        BOOL collision = FALSE;
        for (int i = 0; i < game.brickLength; i++) {
            if (is_collision(game.mario, game.brick[i])) {
                collision = TRUE;
                break;
            }
        }

        if (collision) {
            game.mario.x = oldX;
        } 
		else {
            for (int i = 0; i < game.brickLength; i++) {
                game.brick[i].x += dx;
            }
            for (int i = 0; i < game.movingLength; i++) {
                game.moving[i].x += dx;
            }
        }
    }

    void player_dead(GameState& game) {
        system("color 4F");
        Sleep(500);
        game.needReload = 1;
    }

    void create_level(GameState& game, int lvl) {
        system("color 9F");

        free(game.brick);
        free(game.moving);

        game.brickLength = 0;
        game.movingLength = 0;
        game.brick = NULL;
        game.moving = NULL;

        init_object(&game.mario, 39, 10, 3, 3, '@');
        game.score = 0;
        game.needReload = 0;

        switch (lvl) {
            case 1:
                init_object(get_new_brick(game.brick, game.brickLength), 20, 20, 40, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 30, 10, 5, 3, '?');
                init_object(get_new_brick(game.brick, game.brickLength), 50, 10, 5, 3, '?');
                init_object(get_new_brick(game.brick, game.brickLength), 60, 15, 40, 10, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 60, 5, 5, 3, '-');
                init_object(get_new_brick(game.brick, game.brickLength), 70, 5, 5, 3, '?');
                init_object(get_new_brick(game.brick, game.brickLength), 75, 5, 5, 3, '-');
                init_object(get_new_brick(game.brick, game.brickLength), 80, 5, 5, 3, '?');
                init_object(get_new_brick(game.brick, game.brickLength), 85, 5, 10, 3, '-');
                init_object(get_new_brick(game.brick, game.brickLength), 100, 20, 20, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 120, 15, 10, 10, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 150, 20, 40, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 210, 15, 10, 10, '+');
                init_object(get_new_moving(game.moving, game.movingLength), 25, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 80, 10, 3, 2, 'o');
                break;

            case 2:
                init_object(get_new_brick(game.brick, game.brickLength), 20, 20, 40, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 60, 15, 10, 10, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 80, 20, 20, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 120, 15, 10, 10, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 150, 20, 40, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 210, 15, 10, 10, '+');
                init_object(get_new_moving(game.moving, game.movingLength), 25, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 80, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 65, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 120, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 160, 10, 3, 2, 'o');
                init_object(get_new_moving(game.moving, game.movingLength), 175, 10, 3, 2, 'o');
                break;

            case 3:
                init_object(get_new_brick(game.brick, game.brickLength), 20, 20, 40, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 80, 20, 15, 5, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 120, 15, 15, 10, '#');
                init_object(get_new_brick(game.brick, game.brickLength), 160, 10, 15, 15, '+');
                break;
        }
    }
}