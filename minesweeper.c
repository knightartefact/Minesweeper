#include <SFML/Window.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TILE_SIZE 32
#define WIDTH 512
#define HEIGHT 576

int nb_of_flagged_bombs(int map[16][16], int showed_map[16][16])
{
    int flagged_bombs = 0;
    for (int rows = 0; rows < 16; rows++)
        for (int cols = 0; cols < 16; cols++)
        {
            if (map[rows][cols] == 2 && showed_map[rows][cols] == 1)
            {
                flagged_bombs++;
            }
        }
    return (flagged_bombs);
}

int check_victory(int map[16][16], int showed_map[16][16], int bomb_nb, int remaining_bombs)
{
    int flagged_bombs = nb_of_flagged_bombs(map, showed_map);
    
    if (remaining_bombs == 0 && flagged_bombs == bomb_nb)
        return (1);
    else
        return 0;
}

void flood_fill(int map[16][16], int showed_map[16][16], int pos_x, int pos_y)
{
    if (map[pos_y][pos_x] > 3)
    {
        showed_map[pos_y][pos_x] = map[pos_y][pos_x];
        return;
    }
    else
    {
        map[pos_y][pos_x] = 12;
        if (map[pos_y][pos_x + 1] >= 3 && pos_x != 15)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y);
        }
        if (map[pos_y][pos_x - 1] >= 3 && pos_x != 0)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y);
        }
        if (map[pos_y + 1][pos_x] >= 3 && pos_y != 15)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x, pos_y + 1);
        }
        if (map[pos_y - 1][pos_x] >= 3 && pos_y != 0)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x, pos_y - 1);
        }
        if (map[pos_y + 1][pos_x + 1] >= 3 && pos_y != 15 && pos_x != 15)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y + 1);
        }
        if (map[pos_y + 1][pos_x - 1] >= 3 && pos_y != 15 && pos_x != 0)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y + 1);
        }
        if (map[pos_y - 1][pos_x + 1] >= 3 && pos_y != 0 && pos_x != 15)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x + 1, pos_y - 1);
        }
        if (map[pos_y - 1][pos_x - 1] >= 3 && pos_y != 0 && pos_x != 0)
        {
            showed_map[pos_y][pos_x] = map[pos_y][pos_x];
            flood_fill(map, showed_map, pos_x - 1, pos_y - 1);
        }
    }
    for (int rows = 0; rows < 16; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            if (map[rows][cols] > 11)
            {
                showed_map[rows][cols] = 3;
            }
        }
    }
}

int main(void)
{
    srand(time(0));
    sfRenderWindow *window_render;
    sfVideoMode video_mode = {WIDTH, HEIGHT, 32};
    sfEvent event;
    sfTexture *tile;
    sfSprite *sprite;
    sfVector2f sprite_pos;
    sfVector2i mouse_pos;
    sfIntRect sprite_rect;

    int bomb_nb = 0;
    int map[16][16];
    int showed_map[16][16];
    int remaining_bombs;
    int counter = 0;

    sprite = sfSprite_create();
    tile = sfTexture_createFromFile("minesweeper_tiles.png", NULL);
    window_render = sfRenderWindow_create(video_mode, "Minesweeper !", sfDefaultStyle, NULL);
    sfSprite_setTexture(sprite, tile, sfFalse);

    for (int rows = 0; rows < 16; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            if (rand() % 8 == 0)
            {
                map[rows][cols] = 2;
                bomb_nb++;
            }
            else
            {
                map[rows][cols] = 3;
            }
        }
    }
    remaining_bombs = bomb_nb;
    printf("nb of bombs : %d\n", bomb_nb);

    for (int rows = 0; rows < 16; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            if (map[rows][cols] != 2)
            {

                if (map[rows][cols + 1] == 2 && cols != 15)
                {
                    counter++;
                }
                if (map[rows][cols - 1] == 2 && cols != 0)
                {
                    counter++;
                }
                if (map[rows + 1][cols] == 2 && rows != 15)
                {
                    counter++;
                }
                if (map[rows - 1][cols] == 2 && rows != 0)
                {
                    counter++;
                }
                if (map[rows + 1][cols + 1] == 2 && rows != 15 && cols != 15)
                {
                    counter++;
                }
                if (map[rows - 1][cols - 1] == 2 && rows != 0 && cols != 0)
                {
                    counter++;
                }
                if (map[rows + 1][cols - 1] == 2 && rows != 15 && cols != 0)
                {
                    counter++;
                }
                if (map[rows - 1][cols + 1] == 2 && rows != 0 && cols != 15)
                {
                    counter++;
                }
                map[rows][cols] = counter + 3;
                counter = 0;
            }
        }
    }
    for (int rows = 0; rows < 16; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            showed_map[rows][cols] = 0;
        }
    }
    for (int rows = 0; rows < 16; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            printf("[%d] ", map[rows][cols]);
        }
        printf("\n");
    }

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
                int temp_map[16][16];
                for (int rows = 0; rows < 16; rows++)
                {
                    for (int cols = 0; cols < 16; cols++)
                    {
                        temp_map[rows][cols] = showed_map[rows][cols];
                        showed_map[rows][cols] = map[rows][cols];
                        map[rows][cols] = temp_map[rows][cols];
                    }
                }
            }
            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft && showed_map[pos_y][pos_x] != 1)
            {
                printf("pos_x %d, pos_y %d\n", pos_x, pos_y);
                if (map[pos_y][pos_x] == 2)
                {
                    for (int rows = 0; rows < 16; rows++)
                    {
                        for (int cols = 0; cols < 16; cols++)
                        {
                            showed_map[rows][cols] = map[rows][cols];
                        }
                    }
                }
                else
                {
                    flood_fill(map, showed_map, pos_x, pos_y);
                }
            }
            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseRight && showed_map[pos_y][pos_x] == 0)
            {
                printf("flagged\n");
                showed_map[pos_y][pos_x] = 1;
                remaining_bombs--;
                printf("nb of bombs : %d\n", remaining_bombs);
            }
            else if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseRight && showed_map[pos_y][pos_x] == 1)
            {
                printf("unflagged\n");
                showed_map[pos_y][pos_x] = 0;
                remaining_bombs++;
                printf("nb of bombs : %d\n", remaining_bombs);
            }
        }
        if (check_victory(map, showed_map, bomb_nb, remaining_bombs) == 1)
            return(0);

            sfRenderWindow_clear(window_render, sfBlack);

        for (int rows = 0; rows < 16; rows++)
        {
            for (int cols = 0; cols < 16; cols++)
            {
                sprite_rect.top = ((showed_map[rows][cols] / 4)) * TILE_SIZE;
                sprite_rect.left = (showed_map[rows][cols] % 4) * TILE_SIZE;
                sprite_rect.width = TILE_SIZE;
                sprite_rect.height = TILE_SIZE;
                sprite_pos.x = cols * TILE_SIZE;
                sprite_pos.y = rows * TILE_SIZE + 64;
                sfSprite_setTextureRect(sprite, sprite_rect);
                sfSprite_setPosition(sprite, sprite_pos);
                sfRenderWindow_drawSprite(window_render, sprite, NULL);
            }
        }
        sfRenderWindow_display(window_render);
    }
    return (0);
}