#pragma once
#include <SFML/Window.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct timer
{
    sfClock *clock;
    sfTime time;
    sfSprite *timer_sprite;
    sfTexture *timer_texture;
    sfIntRect timer_sprite_rect;
    sfVector2f timer_sprite_pos;
    int *digits;
} timer;
