#include "super_mario3.hpp"
#include <stdio.h>
#include <stdlib.h>

int main() {
    using namespace PY;
	
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
    
    Game game(MAP_WIDTH, MAP_HEIGHT, MAX_LVL, SCORE_MONSTER, SCORE_COIN,
              GRAVITY, JUMP_SPEED, MARIO_SPEED, MONSTER_SPEED, COIN_VERT_SPEED);
    game.create_level(game.level);
    
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
        
        game.game_map.clear();
        
        if (!game.mario.is_fly && GetKeyState(VK_SPACE) < 0) {
            game.mario.vert_speed = game.JUMP_SPEED;
        }
        
        float oldX = game.mario.x;
        if (GetKeyState('A') < 0) {
            game.mario.x -= game.MARIO_SPEED;
        }
        if (GetKeyState('D') < 0) {
            game.mario.x += game.MARIO_SPEED;
        }
        
        bool collision = false;
        for (int i = 0; i < game.objects.bricks_count; i++) {
            if (game.mario.check_collision(game.objects.bricks[i])) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            game.mario.x = oldX;
        } else {
            game.scroll_world(game.mario.x - oldX);
        }
        
        if (game.mario.y > game.MAP_HEIGHT) {
            game.player_dead();
            continue;
        }
        
        game.move_object(game.mario);
        game.check_collisions();
        
        for (int i = 0; i < game.objects.bricks_count; i++) {
            game.objects.bricks[i].draw(game.game_map.data);
        }
        
        for (int i = 0; i < game.objects.movings_count; i++) {
            game.move_object(game.objects.movings[i]);
            game.move_horizon(game.objects.movings[i]);
            if (game.objects.movings[i].y > game.MAP_HEIGHT) {
                game.objects.delete_moving(i);
                i--;
                continue;
            }
            game.objects.movings[i].draw(game.game_map.data);
        }
        
        game.mario.draw(game.game_map.data);
        game.game_map.put_score(game.score);
        
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        game.game_map.show();
        
        Sleep(16);
        
    } while (GetKeyState(VK_ESCAPE) >= 0);
    
    return 0;
}