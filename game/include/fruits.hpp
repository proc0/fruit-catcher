#pragma once

#include <tuple>

#include "lib.h"
#include "pot.hpp"

using namespace std;

typedef struct Fruit {
    Vector2 position;
    Rectangle collision;
    float atlasXPos;
    float atlasYPos;
    int speed;
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
        void Add(Fruit&, Vector2, int);
        void Spawn(void);
        tuple<int, int> Update(Pot&);
        void Render(void);
};