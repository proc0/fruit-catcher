#pragma once

#define APPLE_SOURCE_WIDTH 75
#define APPLE_SOURCE_HEIGHT 75
#define APPLE_MAX_COUNT 8
#define APPLE_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, APPLE_SOURCE_WIDTH, APPLE_SOURCE_HEIGHT}

#define BASKET_SOURCE_WIDTH 100
#define BASKET_SOURCE_HEIGHT 100
#define BASKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT}

#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

#define TIME_BETWEEN_APPLES 1.0f
#define INITIAL_LIVES 3
#define APPLE_SCORE 100

#define GAME_OVER_TEXT "Game Over"
#define RESTART_MESSAGE "Press R to restart"

static constexpr int screenWidth = 800;
static constexpr int screenHeight = 600;
static constexpr Vector2 screenSize = { screenWidth, screenHeight };
