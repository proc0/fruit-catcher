#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "bucket.hpp"
#include "display.hpp"
#include "stage.hpp"

class Game {
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
        Game() = default;
        void Play();
        void Update();
        void Render();
        void Over();
        void End();
        bool isRunning();

};
