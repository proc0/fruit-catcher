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
    float timeEnd;
    float timeStart;
    int lives;
    int score;
    enum State {
        START,
        PLAY,
        OVER,
        END
    };
    State state = START;

    public:
        Game(const std::string& configFile): 
            config(configFile),
            fruits(config.GetData()) {};
        const bool isRunning() const;
        void Begin();
        void Update();
        void Render() const;
        void Over();
        void End();

};
