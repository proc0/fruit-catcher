#pragma once

#include <tuple>

#include "lib.h"
#include "pot.hpp"

using namespace std;

typedef struct Fruit {
    Rectangle collision;
    Vector2 position;
    Vector2 velocity;
    Vector2 force;
    float atlasXPos;
    float atlasYPos;
    bool active;
} Fruit;

class Fruits {
    float timeNextFruit;
    Texture2D atlasFruit;
    Fruit fruits[GAME_FRUITS_MAX];

    public:
        Fruits(void);
        ~Fruits(void);
        void Remove(Fruit&);
        void Add(Fruit&, Vector2, Vector2);
        void Spawn(void);
        tuple<int, int> Update(Pot&);
        void Render(void);
};