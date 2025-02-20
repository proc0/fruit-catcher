#pragma once

#include <tuple>
#include "raylib.h"
#include "raymath.h"

#include "lib.h"
#include "basket.hpp"

using namespace std;

typedef struct Apple {
    Vector2 position;
    int fallSpeed;
    bool _active;
} Apple;

class Fruits {
    float _nextFruitTimer;
    Apple fruits[APPLE_MAX_COUNT];
    Texture2D _atlasFruits;

    public:
        void Init(void);
        void RemoveFruit(Apple*);
        void AddFruit(Apple*, Vector2, int);
        void Spawn(void);
        tuple<int, int> Update(Basket &);
        void Render(void);
};