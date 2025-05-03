#pragma once

#include <map>
#include <array>
#include <string>
#include <tuple>
#include <iostream>

#include "lib.h"
#include "config.hpp"

#define FRUIT_TYPE_COUNT 15
#define FRUITS \
    X("APPLE", APPLE, "resources/fruits/red-apple.png", 0.0f, 15.0f, RED, 0.8f) \
    X("BANANA", BANANA, "resources/fruits/banana.png", 23.0f, 28.0f, YELLOW, 0.85f) \
    X("BLACKBERRY", BLACKBERRY, "resources/fruits/black-berry-light.png", 0.0f, 9.0f, PURPLE, 0.3f) \
    X("CHERRY", CHERRY, "resources/fruits/red-cherry.png", -4.0f, 9.0f, RED, 0.3f) \
    X("COCONUT", COCONUT, "resources/fruits/coconut.png", -20.0f, 10.0f, WHITE, 1.0f) \
    X("GRAPE", GRAPE, "resources/fruits/red-grape.png", 3.0f, -8.0f, GREEN, 0.8f) \
    X("LEMON", LEMON, "resources/fruits/lemon.png", 0.0f, 0.0f, YELLOW, 0.5f) \
    X("LIMA", LIMA, "resources/fruits/lime.png", 0.0f, 0.0f, LIME, 0.5f) \
    X("NARANJA", NARANJA, "resources/fruits/orange.png", 0.0f, 5.0f, ORANGE, 0.8f) \
    X("PEACH", PEACH, "resources/fruits/peach.png", 0.0f, 0.0f, BEIGE, 0.8f) \
    X("PEAR", PEAR, "resources/fruits/pear.png", 0.0f, 25.0f, GREEN, 0.65f) \
    X("PLUM", PLUM, "resources/fruits/plum.png", 4.0f, 10.0f, PURPLE, 0.8f) \
    X("RASPBERRY", RASPBERRY, "resources/fruits/raspberry.png", 0.0f, 9.0f, RED, 0.3f) \
    X("STRAWBERRY", STRAWBERRY, "resources/fruits/strawberry.png", -8.0f, 12.0f, RED, 0.4f) \
    X("WATERMELON", WATERMELON, "resources/fruits/watermelon.png", 0.0f, 0.0f, RED, 1.0f)

typedef enum class FruitType {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) ENUM,
    FRUITS
#undef X
} FruitType;

typedef struct FruitData {
    const FruitType type;
    const std::string uri;
    const Vector2 offset;
    const Color color;
    const float rating;
} FruitData;

static const std::map<FruitType, FruitData> static_FruitDataMap = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { FruitType::ENUM, \
    { type: FruitType::ENUM, uri: URI, offset: { OFFSETX, OFFSETY }, color: COLOR, rating: RATING }},
    FRUITS
#undef X
};

static const FruitType static_StringToFruit(const std::string& fruitString) {
    static const std::map<std::string, FruitType> fruitMap = {
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { STRING, FruitType::ENUM },
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
#define X(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) STRING,
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

typedef std::array<FruitType, FRUIT_TYPE_COUNT> FruitSample;

typedef struct FruitLevelData {
    int levelId;
    std::map<FruitType, float> fruitFrequencies;
    FruitSample fruitSample;
    int dropFrequencyMin;
    int dropFrequencyMax;
    int density;
} FruitLevelData;

typedef std::array<FruitLevelData, LEVEL_COUNT> FruitLevels;

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

typedef struct FruitCatch {
    Vector2 fruitCenter;
    Color fruitColor;
    int fruitScore;
    bool isCatch;
} FruitCatch;

typedef std::tuple<int, FruitCatch> FruitResult;

class Fruits {
    Texture2D sprites[FRUIT_TYPE_COUNT];
    Fruit fruits[FRUIT_TYPE_COUNT];
    Fruit fruitsDebug[FRUIT_TYPE_COUNT];
    FruitLevels fruitLevels;
    float fruitTimeInterval = 0.0f;
    int currentLevel = 0;
    int currentFruits = 0;
    bool displayDebug = false;
    bool showCollisions = false;

    void CreateFruit(Fruit&, int index);
    void SpawnFruit(Fruit&);
    void Spawn();
    void UpdateMovementFruit(Fruit&);
    void RenderFruit(const Fruit&) const;
    
    public:
        Fruits(const ConfigData&, const FruitLevels&);
        ~Fruits();
        void Reset();
        void SetLevel(int level);
        const FruitResult Update(Rectangle bucketCollision);
        void Render() const;
        void UpdateDebug();
        void RenderDebug() const;
        void UpdateWin();
};
