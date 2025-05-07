#pragma once

#include <array>

#include "lib.h"

#define SOUND_SPLAT_LENGTH 5
#define SOUND_THUMP_LENGTH 5
#define BUCKET_SOUND_FRUIT_THUMP(buf, idx) sprintf(buf, "resources/thump%d.wav", idx)

#define FRUIT_TYPE_COUNT 17
#define FRUIT_SAMPLE_COUNT 34
#define FRUITS \
    FRUIT("LEMON", LEMON, "resources/fruits/lemon.png", 0.0f, 0.0f, YELLOW, 0.3f) \
    FRUIT("CHERRY", CHERRY, "resources/fruits/red-cherry.png", -4.0f, 9.0f, RED, 0.3f) \
    FRUIT("BLACKBERRY", BLACKBERRY, "resources/fruits/black-berry-light.png", 0.0f, 9.0f, PURPLE, 0.3f) \
    FRUIT("RASPBERRY", RASPBERRY, "resources/fruits/raspberry.png", 0.0f, 9.0f, RED, 0.3f) \
    FRUIT("STRAWBERRY", STRAWBERRY, "resources/fruits/strawberry.png", -8.0f, 12.0f, RED, 0.4f) \
    FRUIT("LIMA", LIMA, "resources/fruits/lime.png", 0.0f, 0.0f, LIME, 0.5f) \
    FRUIT("PEAR", PEAR, "resources/fruits/pear.png", 0.0f, 25.0f, GREEN, 0.6f) \
    FRUIT("GRAPE", GRAPE, "resources/fruits/red-grape.png", 3.0f, -8.0f, GREEN, 0.7f) \
    FRUIT("PLUM", PLUM, "resources/fruits/plum.png", 4.0f, 10.0f, PURPLE, 0.7f) \
    FRUIT("PEACH", PEACH, "resources/fruits/peach.png", 0.0f, 0.0f, BEIGE, 0.7f) \
    FRUIT("APPLE", APPLE, "resources/fruits/red-apple.png", 0.0f, 15.0f, RED, 0.8f) \
    FRUIT("NARANJA", NARANJA, "resources/fruits/orange.png", 0.0f, 5.0f, ORANGE, 0.8f) \
    FRUIT("BANANA", BANANA, "resources/fruits/banana.png", 23.0f, 28.0f, YELLOW, 0.9f) \
    FRUIT("WATERMELON", WATERMELON, "resources/fruits/watermelon.png", 0.0f, 0.0f, RED, 1.0f) \
    FRUIT("COCONUT", COCONUT, "resources/fruits/coconut.png", -20.0f, 10.0f, WHITE, 1.0f) \
    FRUIT("EGG", EGG, "resources/fruits/egg.png", 0.0f, 10.0f, GREEN, 1.0f) \
    FRUIT("SPIKE", SPIKE, "resources/fruits/spike.png", 0.0f, 0.0f, BLACK, 1.0f) 

typedef enum class FruitType {
#define FRUIT(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) ENUM,
    FRUITS
#undef FRUIT
} FruitType;

typedef struct FruitData {
    const FruitType type;
    const std::string uri;
    const Vector2 offset;
    const Color color;
    const float rating;
} FruitData;

static const std::map<FruitType, FruitData> static_FruitDataMap = {
#define FRUIT(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { FruitType::ENUM, \
    { type: FruitType::ENUM, uri: URI, offset: { OFFSETX, OFFSETY }, color: COLOR, rating: RATING }},
    FRUITS
#undef FRUIT
};

static const FruitType static_StringToFruit(const std::string& fruitString) {
    static const std::map<std::string, FruitType> fruitMap = {
#define FRUIT(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { STRING, FruitType::ENUM },
    FRUITS
#undef FRUIT
    };
    
    return fruitMap.at(fruitString);
}

static const char* static_FruitToString(const FruitType fruitType) {
    static const char* fruitArray[] = {
#define FRUIT(STRING, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) STRING,
    FRUITS
#undef FRUIT
    };

    return fruitArray[static_cast<int>(fruitType)];
}

#undef FRUITS

typedef std::array<FruitType, FRUIT_SAMPLE_COUNT> FruitSample;

typedef struct FruitLevelData {
    std::map<FruitType, float> fruitFrequencies;
    FruitSample fruitSample;
    int levelId;
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
    Vector2 origin = { 0.0f, 0.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    Vector2 force = { 0.0f, 0.0f };
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

typedef struct FruitResult {
    Vector2 location;
    Color color;
    int score;
    int lives;
    bool isCatch;
    bool bounced;
    bool isSpike;
} FruitResult;

class Fruits {
    Texture2D sprites[FRUIT_TYPE_COUNT];
    Fruit fruits[FRUIT_TYPE_COUNT];
    Fruit fruitsDebug[FRUIT_TYPE_COUNT];
    FruitLevels fruitLevels;
    Sound soundSplat[SOUND_SPLAT_LENGTH];
    Sound soundThump[SOUND_THUMP_LENGTH];

    Sound soundSpike;
    Sound soundEgg;
    float fruitTimeInterval = 0.0f;
    int currentLevel = 0;
    int currentFruits = 0;
    bool displayDebug = false;
    bool showCollisions = false;
    bool isMute = false;

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
        void Mute();
        const bool IsMute() const;
        void Unmute();
        void Render() const;
        void UpdateDebug();
        void RenderDebug() const;
        void UpdateWin();
};
