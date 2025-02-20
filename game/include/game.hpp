#pragma once

#include "fruits.hpp"
#include "basket.hpp"

#define INITIAL_LIVES 3
#define APPLE_SCORE 100

#define GAME_OVER_TEXT "Game Over"
#define RESTART_MESSAGE "Press R to restart"

typedef enum GameState {
    PLAYING,
    END
} GameState;

class FruitCatcher {
    int score;
    int lives;
    float timeStart;
    float timeEnd;
    GameState state;
    Fruits fruits;
    Basket basket;

    public:
        FruitCatcher(Fruits& _fruits, Basket& _basket) :
        score(0), 
        lives(INITIAL_LIVES),
        timeStart(0.),
        timeEnd(0.),
        state(END),
        fruits(_fruits),
        basket(_basket)
        {}

        void End(void);
        void Begin(void);
        void Update(void);
        void Render(void);
};
