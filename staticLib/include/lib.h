#pragma once

#include "raylib.h"
#include "raymath.h"

#define TEXT_GAME_TITLE "Fruit Catcher"
#define TEXT_START_MENU_TITLE "Fruit\nCatcher"
#define TEXT_GAME_START "New Game"
#define TEXT_GAME_QUIT "Quit"
#define TEXT_GAME_OVER "Game Over"
#define TEXT_RESTART "Press R to restart"
#define TEXT_TIME_PLAYED "Time Played: %d minutes and %d seconds"
#define TEXT_SCORE "SCORE"
#define TEXT_LIVES "LIVES"

#define GAME_LIVES 10
#define GAME_SCORE_UNIT 100
#define GAME_FRUITS_MAX 8

static constexpr int SCREEN_WIDTH = 1280;
static constexpr int SCREEN_HEIGHT = 720;

Color GetTextColor();
