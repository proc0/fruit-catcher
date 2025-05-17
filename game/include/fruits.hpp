#pragma once

#include <array>

#include "lib.h"

#define URI_SOUND_SPIKE_HIT "resources/spike.wav"
#define URI_SOUND_EGG_CATCH "resources/eggpop.wav"
#define LENGTH_SOUND_SPLATS 5
#define URI_SOUND_SPLAT(buf, idx) sprintf(buf, "resources/splat%d.wav", (idx))
#define LENGTH_SOUND_THUMPS 5
#define URI_SOUND_THUMP(buf, idx) sprintf(buf, "resources/thump%d.wav", (idx))

#define GRAVITY 982.0f

#define LENGTH_FRUIT_TYPES 17
#define LENGTH_FRUIT_SAMPLE 34
#define SIZE_FRUIT_TEXTURE 150.0f
#define SIZE_FRUIT_COLLISION 42.0f

#ifndef FRUITS
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
#define FRUIT(NAME, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) ENUM,
    FRUITS
#undef FRUIT
} FruitType;

typedef struct FruitData {
    const std::string uri;
    const Color color;
    const Vector2 offset;
    const float rating;
    const FruitType type;
} FruitData;

static inline const std::unordered_map<FruitType, FruitData> static_FruitDataMap = {
#define FRUIT(NAME, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { FruitType::ENUM, \
    { .uri = URI, .color = COLOR, .offset = { OFFSETX, OFFSETY }, .rating = RATING, .type = FruitType::ENUM }},
    FRUITS
#undef FRUIT
};

static inline const FruitType static_StringToFruit(const std::string& fruitString) {
    static const std::unordered_map<std::string, FruitType> fruitMap = {
#define FRUIT(NAME, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) { NAME, FruitType::ENUM },
    FRUITS
#undef FRUIT
    };
    
    return fruitMap.at(fruitString);
}

static inline const char* static_FruitToString(const FruitType fruitType) {
    static const char* fruitArray[] = {
#define FRUIT(NAME, ENUM, URI, OFFSETX, OFFSETY, COLOR, RATING) NAME,
    FRUITS
#undef FRUIT
    };

    return fruitArray[static_cast<int>(fruitType)];
}

#undef FRUITS
#endif

typedef std::unordered_map<FruitType, float> MapFruitSample;
typedef std::array<FruitType, LENGTH_FRUIT_SAMPLE> LevelSample;

typedef struct FruitLevel {
    MapFruitSample fruitSample;
    LevelSample levelSample;
    int levelId;
    int minDropTime;
    int maxDropTime;
    int density;
} FruitLevel;

typedef std::array<FruitLevel, LEVEL_COUNT> FruitLevels;

typedef struct FruitCollision {
    Vector2 offset {};
    float radius = 0.0f;
} FruitCollision;

typedef struct Fruit {
    FruitCollision collision {};
    Vector2 force {};
    Vector2 origin {};
    Vector2 position {};
    Vector2 velocity {};
    float mass = 0.0f;
    float rotation = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    FruitType type = FruitType::APPLE;
    int id = 0;
    int bounces = 0;
    bool active = false;
    bool created = false;
    bool collided = false;
    bool debounce = false;
} Fruit;

typedef struct FruitResult {
    Color color {};
    Vector2 location {};
    int id = 0;
    int score = 0;
    int lives = 0;
    int bounces = 0;
    bool isCatch = false;
    bool bounced = false;
    bool isSpike = false;
} FruitResult;

typedef std::vector<FruitResult> FruitResults;

class Fruits {
    Sound soundSplat[LENGTH_SOUND_SPLATS];
    Sound soundThump[LENGTH_SOUND_THUMPS];
    Sound soundSpike;
    Sound soundEgg;

    Fruit fruits[LENGTH_FRUIT_TYPES];
    Fruit fruitsDebug[LENGTH_FRUIT_TYPES];
    Texture2D sprites[LENGTH_FRUIT_TYPES];
    FruitLevels fruitLevels;

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
        Fruits() = default;
        ~Fruits() = default;
        void Load(const ConfigData&, const FruitLevels&);
        void Unload();        
        void Reset();
        void SetLevel(int level);
        const FruitResults Update(const Rectangle bucketCollision);
        void Mute();
        const bool IsMute() const;
        void Unmute();
        void Render() const;
        void UpdateDebug();
        void RenderDebug() const;
        void UpdateWin();
};
