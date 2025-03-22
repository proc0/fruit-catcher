#pragma once

#include <tuple>

#include "lib.h"
#include "bucket.hpp"

using namespace std;

typedef struct Fruit {
    Rectangle collision;
    Vector2 position;
    Vector2 lastPos;
    Vector2 acceleration;
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
        Fruits();
        ~Fruits();
        void Remove(Fruit&);
        void Add(Fruit&);
        void Spawn();
        void UpdateMovement(Fruit&, Rectangle collision);
        const tuple<int, int> Update(Bucket&);
        void Render() const;
};