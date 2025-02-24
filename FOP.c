#include <ncursesw/ncurses.h>
#include <sqlite3.h>
#include <wchar.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include "menus.h"
#include "game.h"


int game();

int main() 
{
    sqlite3 *db;
    int rc = sqlite3_open("players.db", &db);

    const char *sql_create_table = 
        "CREATE TABLE IF NOT EXISTS players ("
        "username TEXT PRIMARY KEY, "
        "email TEXT, "
        "password TEXT, "
        "score INTEGER, "
        "gold INTEGER, "
        "finished_games INTEGER, "
        "last_game_exists INTEGER, "
        "last_game_last_level INTEGER);";
    
    execute_sql(db, sql_create_table);

    start_color();
    if (has_colors() == 0) 
    {
        endwin();
        printf("Your terminal does not support color\n");
    }
    setlocale(LC_ALL, "");
    initializeRandom();
    initscr();    
    cbreak();         
    keypad(stdscr, TRUE);    
    noecho();                
    curs_set(0);  

    if (!init_audio()) 
    {
        printf("Failed to initialize audio!\n");
    }

    Mix_Music *menu_music = Mix_LoadMUS("musics/menu_music.mp3");
    Mix_PlayMusic(menu_music, -1);

    if (!start_menu()) game();

    game();

    save_changes();
    endwin();
    return 1;
}


int game()
{
    close_audio();
    napms(200);
    init_audio();
    Mix_Music *menu_music = Mix_LoadMUS("musics/menu_music.mp3");
    Mix_PlayMusic(menu_music, -1);
    if (game_menu())
    {
        load_players();
        clear();
        Mix_FreeMusic(menu_music);
        close_audio();
        preparing(1, 1, 1);
        if (!guest) save_changes();
        load_players();
        return game();
    }
    else
    {
        load_players();
        clear();
        Mix_FreeMusic(menu_music);
        close_audio();
        preparing(-1, 0, 0);
        if (!guest) save_changes();
        load_players();
        return game();
    }
}