#pragma once

#include <map>
#include <array>
#include <string>
#include <tuple>
#include <iostream>

#include "lib.h"
#include "config.hpp"
#include "bucket.hpp"

#define FRUITS \
    X("APPLE", APPLE, "resources/fruits/red-apple.png", 0.0f, 15.0f, 0.8f) \
    X("BANANA", BANANA, "resources/fruits/banana.png", 23.0f, 28.0f, 0.9f) \
    X("BLACKBERRY", BLACKBERRY, "resources/fruits/black-berry-light.png", 0.0f, 9.0f, 0.3f) \
    X("CHERRY", CHERRY, "resources/fruits/red-cherry.png", -4.0f, 9.0f, 0.3f) \
    X("COCONUT", COCONUT, "resources/fruits/coconut.png", -20.0f, 10.0f, 1.0f) \
    X("GRAPE", GRAPE, "resources/fruits/red-grape.png", 3.0f, -8.0f, 0.8f) \
    X("LEMON", LEMON, "resources/fruits/lemon.png", 0.0f, 0.0f, 0.5f) \
    X("LIMA", LIMA, "resources/fruits/lime.png", 0.0f, 0.0f, 0.5f) \
    X("NARANJA", NARANJA, "resources/fruits/orange.png", 0.0f, 5.0f, 0.8f) \
    X("PEACH", PEACH, "resources/fruits/peach.png", 0.0f, 0.0f, 0.8f) \
    X("PEAR", PEAR, "resources/fruits/pear.png", 0.0f, 25.0f, 0.65f) \
    X("PLUM", PLUM, "resources/fruits/plum.png", 4.0f, 10.0f, 0.8f) \
    X("RASPBERRY", RASPBERRY, "resources/fruits/raspberry.png", 0.0f, 9.0f, 0.3f) \
    X("STRAWBERRY", STRAWBERRY, "resources/fruits/strawberry.png", -8.0f, 12.0f, 0.4f) \
    X("WATERMELON", WATERMELON, "resources/fruits/watermelon.png", 0.0f, 0.0f, 1.0f)

typedef enum class FruitType {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, RATING) ENUM,
    FRUITS
#undef X
} FruitType;

typedef struct FruitData {
    const FruitType type;
    const std::string uri;
    const Vector2 offset;
    const float rating;
} FruitData;

static const std::map<FruitType, FruitData> static_FruitDataMap = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, RATING) { FruitType::ENUM, \
    { type: FruitType::ENUM, uri: URI, offset: { OFFSETX, OFFSETY }, rating: RATING }},
    FRUITS
#undef X
};

static const FruitType static_StringToFruit(const std::string& fruitString) {
    static const std::map<std::string, FruitType> fruitMap = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, RATING) { STRING, FruitType::ENUM },
    FRUITS
#undef X
    };
    
    //TODO: replace with asserts
    try {
        return fruitMap.at(fruitString);
    } catch (const std::exception& e) {
        std::cerr << "ERROR at static_StringToFruit:\n" << e.what() << '\n';
        return FruitType::APPLE;
    }
}

static const char* static_FruitToString(const FruitType fruitType) {
    static const char* fruitArray[] = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, RATING) STRING,
    FRUITS
#undef X
    };

    //TODO: replace with asserts
    try {
        return fruitArray[static_cast<int>(fruitType)];
    } catch (const std::exception& e) {
        std::cerr << "ERROR at static_FruitToString:\n" << e.what() << '\n';
        return "BANANA";
    }
}

#undef FRUITS

typedef struct FruitLevelData {
    int levelId;
    std::map<FruitType, float> fruitFrequencies;
    std::array<FruitType, 10> fruitRatio;
    int dropFrequencyMin;
    int dropFrequencyMax;
} FruitLevelData;

typedef std::array<FruitLevelData, GAME_LEVELS_NUMBER> FruitLevels;

typedef struct FruitCollision {
    Vector2 offset;
    float radius;
} FruitCollision;

typedef struct Fruit {
    FruitCollision collision;
    Vector2 position = { 0.0f, 0.0f };
    Vector2 origin;
    Vector2 velocity;
    Vector2 force;
    float mass = 0.0f;
    float rotation = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    FruitType type = FruitType::APPLE;
    bool active = false;
    bool created = false;
    bool collided = false;
    bool debounce = false;
} Fruit;

class Fruits {
    Texture2D sprites[GAME_FRUIT_TYPES];
    Fruit fruits[GAME_FRUITS_MAX];
    Fruit fruitsDebug[GAME_FRUIT_TYPES];
    FruitLevels fruitLevels;
    float fruitTimeInterval = 0.0f;
    int currentLevel = 0;
    bool displayDebug = false;
    bool showCollisions = false;

    void CreateFruit(Fruit&, int index);
    void SpawnFruit(Fruit&);
    void UpdateMovementFruit(Fruit&);
    void RenderFruit(const Fruit&) const;
    
    public:
        Fruits(const ConfigData&, const FruitLevels&);
        ~Fruits();
        void Reset();
        void Spawn();
        const std::tuple<int, int> Update(Bucket&);
        void Render() const;
        void UpdateDebug();
        void RenderDebug() const;
};
