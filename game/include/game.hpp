#pragma once

#include "lib.h"
#include "config.hpp"
#include "fruits.hpp"
#include "bucket.hpp"
#include "display.hpp"
#include "stage.hpp"

class Game {
    Config config;
    Fruits fruits;
    Bucket bucket;
    Display display;
    Stage stage;
    Vector2 mousePosition;
    enum State {
        START,
        PLAY,
        OVER,
        END
    };
    State state = START;
    float timeEnd;
    float timeStart;
    int lives;
    int score;
    bool debug = false;

    public:
        Game(const std::string& configFile): 
            config(configFile),
            debug(config.GetData().debug.displayDebug),
            fruits(config.GetData()) {};
        const bool isRunning() const;
        const bool isDebug() const;
        void Begin();
        void Update();
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
        void Over();
        void End();

};
