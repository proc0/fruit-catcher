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

    public:
        Game() = default;
        void Start(void);
        void Play(void);
        void Update(void);
        void Render(void);
        void Over(void);
        void End(void);
        enum State {
            NONE,
            START,
            PLAY,
            OVER,
            END
        };
        State state = NONE;
};
