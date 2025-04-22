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
        PAUSE,
        OVER,
        END
    };
    State state = START;
    float timeEnd;
    float timeStart;
    int lives;
    int score;
    int displayScore;
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
