#pragma once

#include <tuple>

#include "lib.h"
#include "basket.hpp"

using namespace std;

typedef struct Movable {
    Vector2 position;
    bool active;
    int speed;
    float atlasPosition;
} Movable;

class Fruits {
    float timeNextFruit;
    Texture2D atlasFruit;
    Movable fruits[GAME_FRUITS_MAX];

    public:
        void Init(void);
        void Remove(Movable*);
        void Add(Movable*, Vector2, int);
        void Spawn(void);
        tuple<int, int> Update(Basket &);
        void Render(void);
};