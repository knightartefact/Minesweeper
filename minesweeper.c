#include <SFML/Window.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "timer.h"

#define TILE_SIZE 32
#define SMILEY_SIZE 64
#define TIMER_WIDTH 32
#define TIMER_HEIGHT 64

const int grid_size = 16;
const int width = TILE_SIZE * grid_size;
const int height = TILE_SIZE * grid_size + TILE_SIZE * 2;
int map_size = width / TILE_SIZE;
int difficulty = 8;

void show_map(int map[map_size][map_size])
{
    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            printf("[%d] ", map[rows][cols]);
        }
        printf("\n");
    }
}

void initialize_player_view(int showed_map[map_size][map_size])
{
    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            showed_map[rows][cols] = 9;
        }
    }
}

int generate_grid(int map[map_size][map_size])
{
    int bomb_nb = 0;

    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            if (rand() % difficulty == 0)
            {
                map[rows][cols] = 12;
                bomb_nb++;
            }
            else
            {
                map[rows][cols] = 9;
            }
        }
    }
    return (bomb_nb);
}

void check_neighbors(int map[map_size][map_size])
{
    int counter = 0;

    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            if (map[rows][cols] != 12)
            {

                if (cols != (map_size - 1) && map[rows][cols + 1] == 12)
                {
                    counter++;
                }
                if (cols != 0 && map[rows][cols - 1] == 12)
                {
                    counter++;
                }
                if (rows != (map_size - 1) && map[rows + 1][cols] == 12)
                {
                    counter++;
                }
                if (rows != 0 && map[rows - 1][cols] == 12)
                {
                    counter++;
                }
                if (rows != (map_size - 1) && cols != (map_size - 1) && map[rows + 1][cols + 1] == 12)
                {
                    counter++;
                }
                if (rows != 0 && cols != 0 && map[rows - 1][cols - 1] == 12)
                {
                    counter++;
                }
                if (rows != (map_size - 1) && cols != 0 && map[rows + 1][cols - 1] == 12)
                {
                    counter++;
                }
                if (rows != 0 && cols != (map_size - 1) && map[rows - 1][cols + 1] == 12)
                {
                    counter++;
                }
                map[rows][cols] = counter;
                counter = 0;
            }
        }
    }
}

int nb_of_flagged_bombs(int map[map_size][map_size], int showed_map[map_size][map_size])
{
    int flagged_bombs = 0;
    for (int rows = 0; rows < map_size; rows++)
        for (int cols = 0; cols < map_size; cols++)
        {
            if (map[rows][cols] == 12 && showed_map[rows][cols] == 10)
            {
                flagged_bombs++;
            }
        }
    return (flagged_bombs);
}

int check_victory(int map[map_size][map_size], int showed_map[map_size][map_size], int bomb_nb, int remaining_bombs)
{
    int flagged_bombs = nb_of_flagged_bombs(map, showed_map);

    if (remaining_bombs == 0 && flagged_bombs == bomb_nb)
        return (1);
    else
        return 0;
}

void flood_fill(int map[map_size][map_size], int showed_map[map_size][map_size], int pos_x, int pos_y)
{
    if (map[pos_y][pos_x] < 9 && map[pos_y][pos_x] != 0)
    {
        showed_map[pos_y][pos_x] = map[pos_y][pos_x];
        return;
    }
    else
    {
        map[pos_y][pos_x] = 64;
        if (pos_x != (map_size - 1) && showed_map[pos_y][pos_x + 1] != 10 && map[pos_y][pos_x + 1] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y);
        }
        if (pos_x != 0 && showed_map[pos_y][pos_x - 1] != 10 && map[pos_y][pos_x - 1] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y);
        }
        if (pos_y != (map_size - 1) && showed_map[pos_y + 1][pos_x] != 10 && map[pos_y + 1][pos_x] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x, pos_y + 1);
        }
        if (pos_y != 0 && showed_map[pos_y - 1][pos_x] != 10 && map[pos_y - 1][pos_x] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x, pos_y - 1);
        }
        if (pos_y != (map_size - 1) && pos_x != (map_size - 1) && showed_map[pos_y + 1][pos_x + 1] != 10 && map[pos_y + 1][pos_x + 1] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y + 1);
        }
        if (pos_y != (map_size - 1) && pos_x != 0 && showed_map[pos_y + 1][pos_x - 1] != 10 && map[pos_y + 1][pos_x - 1] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y + 1);
        }
        if (pos_x != (map_size - 1) && showed_map[pos_y - 1][pos_x + 1] != 10 && map[pos_y - 1][pos_x + 1] <= 8 && pos_y != 0)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y - 1);
        }
        if (pos_y != 0 && pos_x != 0 && showed_map[pos_y - 1][pos_x - 1] != 10 && map[pos_y - 1][pos_x - 1] <= 8)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y - 1);
        }
    }
    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            if (map[rows][cols] == 64)
            {
                showed_map[rows][cols] = 0;
            }
        }
    }
}

int reveal_tiles(int map[map_size][map_size], int showed_map[map_size][map_size], int pos_x, int pos_y)

{
    int loose = 0;

    if (map[pos_y][pos_x] == 12)
    {
        loose = 1;
        map[pos_y][pos_x] = 11;
        for (int rows = 0; rows < map_size; rows++)
        {
            for (int cols = 0; cols < map_size; cols++)
            {
                showed_map[rows][cols] = map[rows][cols];
            }
        }
    }
    else
    {
        flood_fill(map, showed_map, pos_x, pos_y);
    }
    return (loose);
}

void switch_maps(int map[map_size][map_size], int showed_map[map_size][map_size])
{
    int temp_map[map_size][map_size];
    for (int rows = 0; rows < map_size; rows++)
    {
        for (int cols = 0; cols < map_size; cols++)
        {
            temp_map[rows][cols] = showed_map[rows][cols];
            showed_map[rows][cols] = map[rows][cols];
            map[rows][cols] = temp_map[rows][cols];
        }
    }
}

int update_smiley(int smiley_state, int state)
{
    smiley_state = state;
    return (smiley_state);
}

void end_screen(int victory, int map[map_size][map_size], int showed_map[map_size][map_size])
{
}

int *time_to_timer(int *timer_array, int elapsed_time)
{
    timer_array[2] = elapsed_time % 10;
    timer_array[1] = elapsed_time / 10 % 10;
    timer_array[0] = elapsed_time / 100 % 10;
}

//USE A STRUCTURE TO INIT TIMER THEN MODIFY THE VALUES OF IT IN DISPLAY TIMER

timer init_timer(timer timer)
{
    timer.clock = sfClock_create();
    timer.timer_sprite = sfSprite_create();
    timer.digits = malloc(sizeof(int) * 10);
    timer.digits[0] = 0;
    timer.digits[1] = 0;
    timer.digits[2] = 0;
    timer.timer_sprite_rect.left = 0;
    timer.timer_sprite_rect.top = 0;
    timer.timer_sprite_rect.height = TIMER_HEIGHT;
    timer.timer_sprite_rect.width = TIMER_WIDTH;
    timer.timer_sprite_pos.x = 0;
    timer.timer_sprite_pos.y = 0;
    timer.timer_texture = sfTexture_createFromFile("assets/minesweeper_full_tileset.png", NULL);
    sfSprite_setTexture(timer.timer_sprite, timer.timer_texture, sfFalse);

    return (timer);
}

void display_timer(sfRenderWindow *window_render, timer timer)
{
    int elapsed_time_seconds;

    timer.time = sfClock_getElapsedTime(timer.clock);
    elapsed_time_seconds = timer.time.microseconds / 1000000;
    timer.digits = time_to_timer(timer.digits, elapsed_time_seconds);
    timer.timer_sprite_rect.height = TIMER_HEIGHT;
    timer.timer_sprite_rect.width = TIMER_WIDTH;
    timer.timer_sprite_pos.y = 0;

    for (int i = 0; i < 3; i++)
    {
        timer.timer_sprite_pos.x = TIMER_WIDTH * i + (TILE_SIZE * map_size - TIMER_WIDTH * 3);
        if (timer.digits[i] <= 2)
        {
            timer.timer_sprite_rect.left = 160 + (TIMER_WIDTH * timer.digits[i]);
            timer.timer_sprite_rect.top = TIMER_HEIGHT * 2;
        }
        else
        {
            timer.timer_sprite_rect.left = TIMER_WIDTH * (timer.digits[i]) - TIMER_WIDTH * 3;
            timer.timer_sprite_rect.top = TIMER_HEIGHT * 3;
        }
        sfSprite_setTextureRect(timer.timer_sprite, timer.timer_sprite_rect);
        sfSprite_setPosition(timer.timer_sprite, timer.timer_sprite_pos);
        sfRenderWindow_drawSprite(window_render, timer.timer_sprite, NULL);
    }
}

int main(void)
{
    srand(time(0));
    sfRenderWindow *window_render;
    sfVideoMode video_mode = {width, height, map_size};
    sfEvent event;
    sfTexture *tile;
    sfSprite *tiles_sprite;
    sfVector2f tiles_sprite_pos;
    sfIntRect tiles_sprite_rect;
    sfTexture *smiley;
    sfSprite *smiley_sprite;
    sfVector2f smiley_sprite_pos;
    sfIntRect smiley_sprite_rect;
    sfVector2i mouse_pos;
    timer timer;

    int map[map_size][map_size];
    int showed_map[map_size][map_size];
    int bomb_nb = generate_grid(map);
    int remaining_bombs = bomb_nb;
    int smiley_state = 0;

    tiles_sprite = sfSprite_create();
    tile = sfTexture_createFromFile("assets/minesweeper_full_tileset.png", NULL);
    smiley_sprite = sfSprite_create();
    smiley = sfTexture_createFromFile("assets/minesweeper_full_tileset.png", NULL);

    sfSprite_setTexture(tiles_sprite, tile, sfFalse);
    sfSprite_setTexture(smiley_sprite, tile, sfFalse);

    window_render = sfRenderWindow_create(video_mode, "Minesweeper !", sfClose, NULL);

    initialize_player_view(showed_map);
    show_map(map);
    check_neighbors(map);
    timer = init_timer(timer);

    while (sfRenderWindow_isOpen(window_render))
    {
        mouse_pos = sfMouse_getPositionRenderWindow(window_render);
        int pos_x = mouse_pos.x / TILE_SIZE;
        int pos_y = mouse_pos.y / TILE_SIZE - 2;

        while (sfRenderWindow_pollEvent(window_render, &event))
        {
            if (event.type == sfEvtClosed || (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
            {
                sfRenderWindow_close(window_render);
            }
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyC)
            {
                switch_maps(map, showed_map);
            }
            if (sfMouse_isButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                if (showed_map[pos_y][pos_x] == 9)
                    smiley_state = 2;
                if (event.type == sfEvtMouseButtonReleased && event.mouseButton.button == sfMouseLeft && showed_map[pos_y][pos_x] == 9)
                {
                    smiley_state = 0;
                    if (reveal_tiles(map, showed_map, pos_x, pos_y) == 1)
                    {
                        smiley_state = 3;
                    }
                }
            }
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyX && showed_map[pos_y][pos_x] == 9)
            {
                showed_map[pos_y][pos_x] = 10;
                remaining_bombs--;
            }
            else if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseRight && showed_map[pos_y][pos_x] == 10)
            {
                showed_map[pos_y][pos_x] = 9;
                remaining_bombs++;
            }
            if (check_victory(map, showed_map, bomb_nb, remaining_bombs) && event.type == sfEvtKeyPressed && event.key.code == sfKeyT)
            {
                sfRenderWindow_destroy(window_render);
                main();
                exit(0);
            }
        }

        if (check_victory(map, showed_map, bomb_nb, remaining_bombs))
        {
            smiley_state = 4;
        }

        sfRenderWindow_clear(window_render, sfBlack);

        for (int rows = 0; rows < map_size; rows++)
        {
            for (int cols = 0; cols < map_size; cols++)
            {
                tiles_sprite_rect.top = (showed_map[rows][cols] / 8 % 2) * TILE_SIZE;
                tiles_sprite_rect.left = (showed_map[rows][cols] % 8) * TILE_SIZE;
                tiles_sprite_rect.width = TILE_SIZE;
                tiles_sprite_rect.height = TILE_SIZE;
                tiles_sprite_pos.x = cols * TILE_SIZE;
                tiles_sprite_pos.y = rows * TILE_SIZE + TILE_SIZE * 2;
                sfSprite_setTextureRect(tiles_sprite, tiles_sprite_rect);
                sfSprite_setPosition(tiles_sprite, tiles_sprite_pos);
                sfRenderWindow_drawSprite(window_render, tiles_sprite, NULL);
            }
        }

        smiley_sprite_rect.left = SMILEY_SIZE * (smiley_state % 4);
        smiley_sprite_rect.top = SMILEY_SIZE * (smiley_state / 4 % 2) + SMILEY_SIZE;
        smiley_sprite_rect.height = smiley_sprite_rect.width = SMILEY_SIZE;
        smiley_sprite_pos.x = map_size * (TILE_SIZE / 2) - 32;
        smiley_sprite_pos.y = 0;
        sfSprite_setTextureRect(smiley_sprite, smiley_sprite_rect);
        sfSprite_setPosition(smiley_sprite, smiley_sprite_pos);
        sfRenderWindow_drawSprite(window_render, smiley_sprite, NULL);

        display_timer(window_render, timer);

        sfRenderWindow_display(window_render);
    }
    return (0);
}