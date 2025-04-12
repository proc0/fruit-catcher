#pragma once

#include <map>
#include <string>
#include <tuple>

#include "lib.h"
#include "config.hpp"
#include "bucket.hpp"

#define GAME_FRUITS_MAX 8
#define GAME_FRUIT_TYPES 15
#define FRUITS \
    X("APPLE", APPLE, "resources/fruits/red-apple.png", 0.0f, 15.0f) \
    X("BANANA", BANANA, "resources/fruits/banana.png", 23.0f, 28.0f) \
    X("BLACKBERRY", BLACKBERRY, "resources/fruits/black-berry-light.png", 0.0f, 9.0f) \
    X("CHERRY", CHERRY, "resources/fruits/red-cherry.png", -4.0f, 9.0f) \
    X("COCONUT", COCONUT, "resources/fruits/coconut.png", -20.0f, 10.0f) \
    X("GRAPE", GRAPE, "resources/fruits/red-grape.png", 3.0f, -8.0f) \
    X("LEMON", LEMON, "resources/fruits/lemon.png", 0.0f, 0.0f) \
    X("LIMA", LIMA, "resources/fruits/lime.png", 0.0f, 0.0f) \
    X("NARANJA", NARANJA, "resources/fruits/orange.png", 0.0f, 5.0f) \
    X("PEACH", PEACH, "resources/fruits/peach.png", 0.0f, 0.0f) \
    X("PEAR", PEAR, "resources/fruits/pear.png", 0.0f, 25.0f) \
    X("PLUM", PLUM, "resources/fruits/plum.png", 4.0f, 10.0f) \
    X("RASPBERRY", RASPBERRY, "resources/fruits/raspberry.png", 0.0f, 9.0f) \
    X("STRAWBERRY", STRAWBERRY, "resources/fruits/strawberry.png", -8.0f, 12.0f) \
    X("WATERMELON", WATERMELON, "resources/fruits/watermelon.png", 0.0f, 0.0f)

typedef enum class FruitType {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY) ENUM,
    FRUITS
#undef X
} FruitType;

typedef struct FruitData {
    FruitType type;
    std::string uri;
    Vector2 offset;
} FruitData;

static const std::map<FruitType, FruitData> static_FruitDataMap = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY) { FruitType::ENUM, \
    { type: FruitType::ENUM, uri: URI, offset: { OFFSETX, OFFSETY } }},
    FRUITS
#undef X
};

#undef FRUITS

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
    float width;
    float height;
    FruitType type;
    bool active;
    bool collided;
    bool debounce;
} Fruit;

class Fruits {
    Texture2D sprites[GAME_FRUIT_TYPES];
    Fruit fruits[GAME_FRUITS_MAX];
    Fruit fruitsDebug[GAME_FRUIT_TYPES];
    float fruitTimeInterval = 0.0f;
    bool displayDebug = false;
    bool showCollisions = false;

    void SetConfig(const ConfigData&);
    void ResetFruit(Fruit&);
    void MakeFruit(Fruit&, int index);
    void SpawnFruit(Fruit&);
    void UpdateMovementFruit(Fruit&);
    void RenderFruit(const Fruit&) const;
    
    public:
    Fruits(const ConfigData&);
    ~Fruits();
    void Reset();
    void Spawn();
    const std::tuple<int, int> Update(Bucket&);
    void Render() const;
    void UpdateDebug();
    void RenderDebug() const;
};
