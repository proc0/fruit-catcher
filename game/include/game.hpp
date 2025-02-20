#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "basket.hpp"

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

    public:
        Game(Fruits& _fruits, Basket& _basket) :
        fruits(_fruits),
        basket(_basket)
        {}

        void End(void);
        void Begin(void);
        void Update(void);
        void Render(void);
};
