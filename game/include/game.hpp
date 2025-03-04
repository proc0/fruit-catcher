#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "pot.hpp"
#include "display.hpp"
#include "stage.hpp"

typedef enum State {
    START,
    PLAY,
    OVER,
    END
} State;

class Game {
    Fruits &fruits;
    Pot &pot;
    Display &display;
    Stage &stage;
    State state;
    float timeEnd;
    float timeStart;
    int lives;
    int score;

    public:
        Game(Fruits &_fruits, Pot &_pot, Display &_display, Stage &_stage) :
        fruits(_fruits),
        pot(_pot),
        display(_display),
        stage(_stage) 
        { 
            state = START; 
        }

        void Start(void);
        void Update(void);
        void Render(void);
        void Over(void);
};
