#pragma once

#include "lib.h"
#include "config.hpp"
#include "fruits.hpp"
#include "bucket.hpp"
#include "display.hpp"
#include "level.hpp"
#include "stage.hpp"

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
        END
    };
    State state = START;
    float timeEnd = 0.0f;
    float timeLevel = 0.0f;
    float timeStart = 0.0f;
    float timeReady = 0.0f;
    int displayScore = 0;
    int displayTime = 0;
    int lives = GAME_LIVES;
    int score = 0;
    bool debug = false;

    public:
        Game(const std::string& configFile): 
            config(configFile),
            debug(config.GetData().debug.displayDebug),
            display(config.GetData()),
            level(config.GetData()),
            fruits(config.GetData(), level.GetFruitLevelData()) {};
        const bool isRunning() const;
        const bool isDebug() const;
        void Update();
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
