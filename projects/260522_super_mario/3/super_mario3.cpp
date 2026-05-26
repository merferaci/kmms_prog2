#include "super_mario3.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace PY {
    Object::Object() : x(0), y(0), width(0), height(0), 
                       vertSpeed(0), horizonSpeed(MONSTER_SPEED), 
                       isFly(FALSE), type(' ') {}
    
    Object::Object(float x, float y, float w, float h, char t) {
        this->x = x; this->y = y; this->width = w; this->height = h;
        vertSpeed = 0; horizonSpeed = MONSTER_SPEED;
        isFly = FALSE; type = t;
    }
    
    void Object::draw(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
        int ix = (int)(x + 0.5f), iy = (int)(y + 0.5f);
        int iw = (int)(width + 0.5f), ih = (int)(height + 0.5f);
        
        for (int i = ix; i < ix + iw; i++)
            for (int j = iy; j < iy + ih; j++)
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT)
                    map[j][i] = type;
    }
    
    bool Object::collision(const Object& other) {
        return (x + width > other.x) && (x < other.x + other.width) &&
               (y + height > other.y) && (y < other.y + other.height);
    }
    
    Game::Game() {
        bricks = NULL;
        movings = NULL;
        bricksCount = 0;
        movingsCount = 0;
        level = 1;
        score = 0;
        needReload = 0;
    }
    
    Game::~Game() {
        free(bricks);
        free(movings);
    }
    
    void Game::clearMap() {
        for (int i = 0; i < MAP_WIDTH; i++) map[0][i] = ' ';
        map[0][MAP_WIDTH] = '\0';
        for (int j = 1; j < MAP_HEIGHT; j++)
            memcpy(map[j], map[0], MAP_WIDTH + 1);
    }
    
    void Game::showMap() {
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
    
    void Game::putScore() {
        char text[30];
        sprintf(text, "Score: %d", score);
        int len = strlen(text);
        for (int i = 0; i < len; i++)
            map[1][i + 5] = text[i];
    }
    
    void Game::addBrick(Object brick) {
        bricksCount++;
        bricks = (Object*)realloc(bricks, sizeof(Object) * bricksCount);
        bricks[bricksCount - 1] = brick;
    }
    
    void Game::addMoving(Object moving) {
        movingsCount++;
        movings = (Object*)realloc(movings, sizeof(Object) * movingsCount);
        movings[movingsCount - 1] = moving;
    }
    
    void Game::deleteMoving(int index) {
        movingsCount--;
        movings[index] = movings[movingsCount];
        movings = (Object*)realloc(movings, sizeof(Object) * movingsCount);
    }
    
    void Game::moveObject(Object* obj) {
        obj->isFly = TRUE;
        obj->vertSpeed += GRAVITY;
        obj->y += obj->vertSpeed;
        
        for (int i = 0; i < bricksCount; i++) {
            if (obj->collision(bricks[i])) {
                if (obj->vertSpeed > 0) obj->isFly = FALSE;
                
                if (bricks[i].type == '?' && obj->vertSpeed < 0 && obj == &mario) {
                    bricks[i].type = '-';
                    Object coin(bricks[i].x, bricks[i].y - 3, 3, 2, '$');
                    coin.vertSpeed = COIN_VERT_SPEED;
                    addMoving(coin);
                }
                
                if (bricks[i].type == '+') {
                    level++;
                    if (level > MAX_LVL) level = 1;
                    system("color 2F");
                    Sleep(500);
                    needReload = 1;
                }
                
                obj->y -= obj->vertSpeed;
                obj->vertSpeed = 0;
                obj->isFly = FALSE;
                break;
            }
        }
    }
    
    void Game::moveHorizon(Object* obj) {
        obj->x += obj->horizonSpeed;
        
        for (int i = 0; i < bricksCount; i++) {
            if (obj->collision(bricks[i])) {
                obj->x -= obj->horizonSpeed;
                obj->horizonSpeed = -obj->horizonSpeed;
                return;
            }
        }
        
        if (obj->type == 'o') {
            Object temp = *obj;
            moveObject(&temp);
            if (temp.isFly == TRUE) {
                obj->x -= obj->horizonSpeed;
                obj->horizonSpeed = -obj->horizonSpeed;
            }
        }
    }
    
    void Game::createLevel(int lvl) {
        system("color 9F");
        
        free(bricks);
        free(movings);
        bricksCount = 0;
        movingsCount = 0;
        bricks = NULL;
        movings = NULL;
        
        mario = Object(39, 10, 3, 3, '@');
        score = 0;
        needReload = 0;
        
        if (lvl == 1) {
            addBrick(Object(20, 20, 40, 5, '#'));
            addBrick(Object(30, 10, 5, 3, '?'));
            addBrick(Object(50, 10, 5, 3, '?'));
            addBrick(Object(60, 15, 40, 10, '#'));
            addBrick(Object(60, 5, 5, 3, '-'));
            addBrick(Object(70, 5, 5, 3, '?'));
            addBrick(Object(75, 5, 5, 3, '-'));
            addBrick(Object(80, 5, 5, 3, '?'));
            addBrick(Object(85, 5, 10, 3, '-'));
            addBrick(Object(100, 20, 20, 5, '#'));
            addBrick(Object(120, 15, 10, 10, '#'));
            addBrick(Object(150, 20, 40, 5, '#'));
            addBrick(Object(210, 15, 10, 10, '+'));
            addMoving(Object(25, 10, 3, 2, 'o'));
            addMoving(Object(80, 10, 3, 2, 'o'));
        }
        else if (lvl == 2) {
            addBrick(Object(20, 20, 40, 5, '#'));
            addBrick(Object(60, 15, 10, 10, '#'));
            addBrick(Object(80, 20, 20, 5, '#'));
            addBrick(Object(120, 15, 10, 10, '#'));
            addBrick(Object(150, 20, 40, 5, '#'));
            addBrick(Object(210, 15, 10, 10, '+'));
            addMoving(Object(25, 10, 3, 2, 'o'));
            addMoving(Object(80, 10, 3, 2, 'o'));
            addMoving(Object(65, 10, 3, 2, 'o'));
            addMoving(Object(120, 10, 3, 2, 'o'));
            addMoving(Object(160, 10, 3, 2, 'o'));
            addMoving(Object(175, 10, 3, 2, 'o'));
        }
        else if (lvl == 3) {
            addBrick(Object(20, 20, 40, 5, '#'));
            addBrick(Object(80, 20, 15, 5, '#'));
            addBrick(Object(120, 15, 15, 10, '#'));
            addBrick(Object(160, 10, 15, 15, '+'));
        }
    }
    
    void Game::checkCollisions() {
        for (int i = 0; i < movingsCount; i++) {
            if (mario.collision(movings[i])) {
                if (movings[i].type == 'o') {
                    if (mario.isFly && mario.vertSpeed > 0 && 
                        mario.y + mario.height < movings[i].y + movings[i].height * 0.5) {
                        score += SCORE_MONSTER;
                        deleteMoving(i);
                        i--;
                    } else {
                        playerDead();
                        return;
                    }
                }
                else if (movings[i].type == '$') {
                    score += SCORE_COIN;
                    deleteMoving(i);
                    i--;
                }
            }
        }
    }
    
    void Game::playerDead() {
        system("color 4F");
        Sleep(500);
        needReload = 1;
    }
    
    void Game::run() {
        system("mode con cols=121 lines=36");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
        
        createLevel(level);
        
        do {
            if (needReload) {
                createLevel(level);
                continue;
            }
            
            clearMap();
            
            if (!mario.isFly && GetKeyState(VK_SPACE) < 0)
                mario.vertSpeed = JUMP_SPEED;
            
            float oldX = mario.x;
            if (GetKeyState('A') < 0) mario.x -= MARIO_SPEED;
            if (GetKeyState('D') < 0) mario.x += MARIO_SPEED;
            
            bool collision = false;
            for (int i = 0; i < bricksCount; i++) {
                if (mario.collision(bricks[i])) {
                    collision = true;
                    break;
                }
            }
            
            if (collision) {
                mario.x = oldX;
            } else {
                for (int i = 0; i < bricksCount; i++) bricks[i].x -= (mario.x - oldX);
                for (int i = 0; i < movingsCount; i++) movings[i].x -= (mario.x - oldX);
            }
            
            if (mario.y > MAP_HEIGHT) {
                playerDead();
                continue;
            }
            
            moveObject(&mario);
            checkCollisions();
            
            for (int i = 0; i < bricksCount; i++)
                bricks[i].draw(map);
            
            for (int i = 0; i < movingsCount; i++) {
                moveObject(&movings[i]);
                moveHorizon(&movings[i]);
                if (movings[i].y > MAP_HEIGHT) {
                    deleteMoving(i);
                    i--;
                    continue;
                }
                movings[i].draw(map);
            }
            
            mario.draw(map);
            putScore();
            
            COORD coord = {0, 0};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            showMap();
            
            Sleep(16);
            
        } while (GetKeyState(VK_ESCAPE) >= 0);
    }
}