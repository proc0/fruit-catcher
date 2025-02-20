#pragma once

#include <tuple>
#include "raylib.h"
#include "raymath.h"
#include "lib.h"

#include "basket.hpp"

#define APPLE_SOURCE_WIDTH 75
#define APPLE_SOURCE_HEIGHT 75
#define APPLE_MAX_COUNT 8
#define APPLE_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, APPLE_SOURCE_WIDTH, APPLE_SOURCE_HEIGHT}

#define TIME_BETWEEN_APPLES 1.0f

#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

typedef struct Apple {
    Vector2 position;
    int fallSpeed;
    bool _active;
} Apple;

class Fruit {
    float _nextFruitTimer;
    Apple _fruits[APPLE_MAX_COUNT];
    Texture2D _atlasFruits;

    public:
        void Init(void);
        void RemoveFruit(Apple*);
        void AddFruit(Apple*, Vector2, int);
        void Spawn(void);
        std::tuple<int, int> Update(Basket &);
        void Render(void);
};