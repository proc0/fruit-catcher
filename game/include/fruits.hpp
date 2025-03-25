#pragma once

#include <tuple>

#include "lib.h"
#include "bucket.hpp"

typedef struct Fruit {
    Vector2 collision;
    Vector2 position;
    Vector2 velocity;
    Vector2 force;
    float mass;
    float atlasXPos;
    float atlasYPos;
    bool active;
    bool collided;
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
        void UpdateMovement(Fruit&);
        const std::tuple<int, int> Update(Bucket&);
        void Render() const;
};