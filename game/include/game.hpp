#pragma once

#include "lib.h"
#include "fruits.hpp"
#include "basket.hpp"
#include "display.hpp"
#include "stage.hpp"

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
    Stage stage;

    public:
        Game() :
        fruits(),
        basket(),
        display(),
        stage()
        {}
        ~Game()
        {};
        
        void Load(void);
        void Update(void);
        void Render(void);
        void Over(void);
        void Unload(void);
};
