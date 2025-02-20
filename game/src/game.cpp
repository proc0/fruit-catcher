#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
    timeEnd = GetTime();
}

void Game::Begin() {
    score = 0;
    lives = GAME_LIVES;
    state = PLAYING;
    timeStart = GetTime();

    fruits.Init();
    basket.Init();
}

void Game::Update() {
    if(state == END) {
        if(IsKeyPressed(KEY_R)){
            Begin();
        }
    }
    
    if(state == PLAYING) {
        basket.Update();

        tuple<int, int> result = fruits.Update(basket);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            End();
        }
    }
}

void Game::Render() {
    basket.Render();
    fruits.Render();

    if(state == END) {
        display.Ending(lives, score, timeEnd, timeStart);
    }

    if(state == PLAYING) {
        display.Playing(lives, score);
    }
}