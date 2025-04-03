#pragma once

#include <tuple>
#include <unordered_map>
#include <string>

#include "lib.h"
#include "bucket.hpp"

typedef enum FruitType {
    APPLE,
    BANANA,
    BLACKBERRY,
    CHERRY,
    COCONUT,
    GRAPE,
    LEMON,
    LIMA,
    NARANJA,
    STRAWBERRY,
    PEACH,
    PEAR,
    PLUM,
    RASPBERRY,
    WATERMELON
} FruitType;

typedef struct FruitCollision {
    Vector2 offset;
    float radius;
} FruitCollision;

typedef struct Fruit {
    FruitCollision collision;
    Vector2 position;
    Vector2 origin;
    Vector2 velocity;
    Vector2 force;
    float mass;
    float rotation;
    FruitType type;
    bool active;
    bool collided;
    bool debounce;
} Fruit;

static const std::unordered_map<FruitType, std::string> fruit_sprite_uris = {
    {FruitType::APPLE, "resources/fruits/red-apple.png"},
    {FruitType::BANANA, "resources/fruits/banana.png"},
    {FruitType::BLACKBERRY, "resources/fruits/black-berry-light.png"},
    {FruitType::CHERRY, "resources/fruits/red-cherry.png"},
    {FruitType::COCONUT, "resources/fruits/coconut.png"},
    {FruitType::GRAPE, "resources/fruits/red-grape.png"},
    {FruitType::LEMON, "resources/fruits/lemon.png"},
    {FruitType::LIMA, "resources/fruits/lime.png"},
    {FruitType::NARANJA, "resources/fruits/orange.png"},
    {FruitType::PEACH, "resources/fruits/peach.png"},
    {FruitType::PEAR, "resources/fruits/pear.png"},
    {FruitType::PLUM, "resources/fruits/plum.png"},
    {FruitType::RASPBERRY, "resources/fruits/raspberry.png"},
    {FruitType::STRAWBERRY, "resources/fruits/strawberry.png"},
    {FruitType::WATERMELON, "resources/fruits/watermelon.png"},
};

class Fruits {
    float timeNextFruit;
    Texture2D sprites[GAME_FRUIT_TYPES];
    Fruit fruits[GAME_FRUITS_MAX];

    public:
        Fruits();
        ~Fruits();
        void Remove(Fruit&);
        void Add(Fruit&);
        void Reset();
        void Spawn();
        void UpdateMovement(Fruit&);
        const std::tuple<int, int> Update(Bucket&);
        void Render() const;
};