#pragma once

#include "raylib.h"

#define GAME_OVER_TEXT "Game Over"
#define RESTART_MESSAGE "Press R to restart"
#define TEXT_TIME_PLAYED "Time Played: %d minutes and %d seconds"

#define INITIAL_LIVES 3
#define APPLE_SCORE 100
#define APPLE_MAX_COUNT 8

static constexpr int screenWidth = 800;
static constexpr int screenHeight = 600;

Color GetTextColor();
