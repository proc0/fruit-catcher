#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "pot.hpp"
#include "display.hpp"
#include "stage.hpp"

class Game {
    Fruits fruits;
    Pot pot;
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
