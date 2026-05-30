#include "super_mario3.hpp"

int main() {
    using namespace PY;
    
    GameState game = {0};
    game.bricks = nullptr;
    game.movings = nullptr;
    game.bricks_count = 0;
    game.movings_count = 0;
    game.level = 1;
    game.need_reload = false;
    
    game.max_lvl = 3;
    game.score_monster = 50;
    game.score_coin = 100;
    game.gravity = 0.05f;
    game.jump_speed = -1.0f;
    game.mario_speed = 0.8f;
    game.monster_speed = 0.2f;
    game.coin_vert_speed = -0.7f;
    
    create_level(game, game.level);
    system("mode con cols=121 lines=36");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    do {
        if (game.need_reload) {
            create_level(game, game.level);
            continue;
        }
        
        clear_map(game.map);
        
        if (!game.mario.is_fly && (GetKeyState(VK_SPACE) < 0)) {
            game.mario.vert_speed = game.jump_speed;
        }
        
        if (GetKeyState('A') < 0) {
            move_map(game, game.mario_speed);
        }
        if (GetKeyState('D') < 0) {
            move_map(game, -game.mario_speed);
        }
        
        if (game.mario.y > MAP_HEIGHT) {
            player_dead(game);
            continue;
        }
        
        move_object(game, game.mario);
        check_collisions(game);
        
        for (int i = 0; i < game.bricks_count; i++) {
            put_object_on_map(game.map, game.bricks[i]);
        }
        
        for (int i = 0; i < game.movings_count; i++) {
            move_object(game, game.movings[i]);
            move_horizon(game, game.movings[i]);
            if (game.movings[i].y > MAP_HEIGHT) {
                delete_moving(game, i);
                i--;
                continue;
            }
            put_object_on_map(game.map, game.movings[i]);
        }
        
        put_object_on_map(game.map, game.mario);
        put_score_on_map(game.map, game.score);
        
        set_cursor_pos(0, 0);
        show_map_fast(game.map);
        
        Sleep(16);
        
    } while (GetKeyState(VK_ESCAPE) >= 0);
    
    delete[] game.bricks;
    delete[] game.movings;
    
    return 0;
}