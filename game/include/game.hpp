#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "basket.hpp"
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
    Basket &basket;
    Display &display;
    Stage &stage;
    State state;
    float timeEnd;
    float timeStart;
    int lives;
    int score;

    public:
        Game(Fruits &_fruits, Basket &_basket, Display &_display, Stage &_stage) :
        fruits(_fruits),
        basket(_basket),
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
