#pragma once

#include <assert.h>
#include <raylib.h>
#include <raymath.h>

#include "config.hpp"

#define TEXT_GAME_TITLE "Jam Slam"
#define TEXT_START_MENU_TITLE "Jam\n   Slam"
#define TEXT_GAME_START "New Game"
#define TEXT_GAME_QUIT "Quit"
#define TEXT_GAME_OVER "Game Over"
#define TEXT_GAME_WIN "YOU WIN"
#define TEXT_LEVEL "Level %d"
#define TEXT_PRESS_R "Press R to restart"
#define TEXT_RESTART "Restart"
#define TEXT_TIME_PLAYED "Time Played: %d minutes and %d seconds"
#define TEXT_SCORE "SCORE %d"
#define TEXT_LIVES "LIVES"

#define GAME_LIVES 3

static constexpr int SCREEN_WIDTH = 1280;
static constexpr int SCREEN_HEIGHT = 720;

Color GetTextColor();
