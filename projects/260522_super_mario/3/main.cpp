#include "super_mario3.hpp"
#include <stdio.h>
#include <stdlib.h>

int main() {
    using namespace PY;
    
    Game game;
    game.create_level(game.level);
    
    system("color 9F");
    system("mode con cols=121 lines=36");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    do {
        if (game.need_reload) {
            game.create_level(game.level);
            continue;
        }
        
        game.clear_map();
        
        if (!game.mario.is_fly && GetKeyState(VK_SPACE) < 0) {
            game.mario.vert_speed = JUMP_SPEED;
        }
        
        float oldX = game.mario.x;
        if (GetKeyState('A') < 0) {
            game.mario.x -= MARIO_SPEED;
        }
        if (GetKeyState('D') < 0) {
            game.mario.x += MARIO_SPEED;
        }
        
        bool collision = false;
        for (int i = 0; i < game.bricks_count; i++) {
            if (game.mario.check_collision(game.bricks[i])) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            game.mario.x = oldX;
        } else {
            for (int i = 0; i < game.bricks_count; i++) {
                game.bricks[i].x -= (game.mario.x - oldX);
            }
            for (int i = 0; i < game.movings_count; i++) {
                game.movings[i].x -= (game.mario.x - oldX);
            }
        }
        
        if (game.mario.y > MAP_HEIGHT) {
            game.player_dead();
            continue;
        }
        
        game.move_object(game.mario);
        game.check_collisions();
        
        for (int i = 0; i < game.bricks_count; i++) {
            game.bricks[i].draw(game.map);
        }
        
        for (int i = 0; i < game.movings_count; i++) {
            game.move_object(game.movings[i]);
            game.move_horizon(game.movings[i]);
            if (game.movings[i].y > MAP_HEIGHT) {
                game.delete_moving(i);
                i--;
                continue;
            }
            game.movings[i].draw(game.map);
        }
        
        game.mario.draw(game.map);
        game.put_score();
        
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        game.show_map();
        
        Sleep(16);
        
    } while (GetKeyState(VK_ESCAPE) >= 0);
    
    return 0;
}