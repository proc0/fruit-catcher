#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "basket.hpp"
#include "display.hpp"

typedef enum State {
    PLAYING,
    END
} State;

class Game {
    int score;
    int lives;
    float timeStart;
    float timeEnd;
    State state;
    Fruits fruits;
    Basket basket;
    Display display;

    public:
        Game(Fruits& _fruits, Basket& _basket, Display& _display) :
        fruits(_fruits),
        basket(_basket),
        display(_display)
        {}

        void End(void);
        void Begin(void);
        void Update(void);
        void Render(void);
};
