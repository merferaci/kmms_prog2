#include "super_mario2.hpp"
#include <stdio.h>

int main() {
    using namespace PY;
    
    GameState game = {0};
    game.brick = NULL;
    game.moving = NULL;
    game.level = 1;
    game.needReload = 0;
    
    game.max_lvl = 3;
    game.score_monster = 50;
    game.score_coin = 100;
    game.gravity = 0.05f;
    game.jump_speed = -1.0f;
    game.mario_speed = 0.8f;
    game.monster_speed = 0.2f;
    game.coin_vert_speed = -0.7f;
    
    create_level(game, game.level);
    system("color 9F");
    system("mode con cols=121 lines=36");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    do {
        if (game.needReload) {
            create_level(game, game.level);
            continue;
        }
        
        clear_map(game.map);
        
        if ((game.mario.isFly == FALSE) && (GetKeyState(VK_SPACE) < 0))
            game.mario.vertSpeed = game.jump_speed;
        
        if (GetKeyState('A') < 0)
            horizon_move_map(game, game.mario_speed);
        if (GetKeyState('D') < 0)
            horizon_move_map(game, -game.mario_speed);
        
        if (game.mario.y > MAP_HEIGHT) {
            player_dead(game);
            continue;
        }
        
        vert_move_object(game, &game.mario);
        mario_collision(game);
        
        for (int i = 0; i < game.brickLength; i++) {
            put_object_on_map(game.map, game.brick[i]);
        }
        
        for (int i = 0; i < game.movingLength; i++) {
            vert_move_object(game, &game.moving[i]);
            horizon_move_object(game, &game.moving[i]);
            if (game.moving[i].y > MAP_HEIGHT) {
                delete_moving(game.moving, game.movingLength, i);
                i--;
                continue;
            }
            put_object_on_map(game.map, game.moving[i]);
        }
        
        put_object_on_map(game.map, game.mario);
        put_score_on_map(game.map, game.score);
        
        set_cur(0, 0);
        show_map_fast(game.map);
        
        Sleep(16);
        
    } while (GetKeyState(VK_ESCAPE) >= 0);
    
    free(game.brick);
    free(game.moving);
    return 0;
}