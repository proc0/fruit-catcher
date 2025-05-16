#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <html5.h>
#include <functional>
#endif

#include "bucket.hpp"
#include "display.hpp"
#include "level.hpp"
#include "stage.hpp"

#define MUSIC_LEVEL_URI "resources/music-level.mp3"
#define MUSIC_INTRO_URI "resources/music-intro.mp3"

class Game {
    Config config;
    Level level;
    Fruits fruits;
    Bucket bucket;
    Display display;
    Stage stage;
    Vector2 mousePosition;
    Vector2 debugCoordinates;
    enum State {
        START,
        PLAY,
        READY,
        PAUSE,
        OVER,
        WIN,
        END
    };
    State state = START;
    Music musicLevel;
    Music musicIntro;
    float timeEnd = 0.0f;
    float timeCount = 0.0f;
    float timeStart = 0.0f;
    float timeReady = 0.0f;
    int lives = GAME_LIVES;
    int points = 0;
    int score = 0;
    int timeLeft = 0;
    bool debug = false;
    
    const FruitDisplayResult DisplayFruitResult(const FruitResult& results) const;
    const FruitDisplayResults DisplayFruitResults(const FruitResults& results) const;
    const BucketDisplayResult DisplayBucketResult(const FruitResults& results) const;

    const int GetScore(const FruitResults& results) const;
    const int GetLives(const FruitResults& results) const;

    public:
        Game(const std::string& filepath): 
            config(filepath),
            debug(config.GetData().debug.modeDebug),
            level(config.GetData()) {};
        ~Game() = default;
        void Load();
        void Unload();
        const bool isRunning() const;
        const bool isDebug() const;
        void Loop();
        void Pause();
        void Update();
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
