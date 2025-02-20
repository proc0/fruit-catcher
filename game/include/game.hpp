#pragma once

#include "fruit.hpp"
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
    int _score;
    int _lives;
    float _startTime;
    float _endTime;
    GameState _state;
    Fruit _fruit;
    Basket _basket;

    public:
        FruitCatcher(Fruit& fruit, Basket& basket) :
        _score(0), 
        _lives(INITIAL_LIVES),
        _startTime(0.),
        _endTime(0.),
        _state(END),
        _fruit(fruit),
        _basket(basket)
        {}

        void EndGame(void);
        void InitGame(void);
        void Update(void);
        void Render(void);
};
