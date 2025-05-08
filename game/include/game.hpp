#pragma once

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
            display(config.GetData()),
            level(config.GetData()),
            fruits(config.GetData(), level.GetFruitLevelData()) {
                musicLevel = LoadMusicStream(MUSIC_LEVEL_URI);
                musicIntro = LoadMusicStream(MUSIC_INTRO_URI);
                SetMusicVolume(musicLevel, 0.5f);
                SetMusicVolume(musicIntro, 0.5f);
                PlayMusicStream(musicIntro);
            };
        ~Game() {
            UnloadMusicStream(musicLevel);
            UnloadMusicStream(musicIntro);
        };
        const bool isRunning() const;
        const bool isDebug() const;
        void Update();
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
