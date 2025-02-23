#include "game.hpp"

using namespace std;

void Game::Over() {
    state = END;
    timeEnd = GetTime();
}

void Game::Load() {
    score = 0;
    lives = GAME_LIVES;
    state = PLAYING;
    timeStart = GetTime();

    stage.Load();
    fruits.Load();
    basket.Load();
}

void Game::Update() {
    stage.Update();

    if(state == END) {
        if(IsKeyPressed(KEY_R)){
            Load();
        }
    }
    
    if(state == PLAYING) {
        basket.Update();

        tuple<int, int> result = fruits.Update(basket);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            Over();
        }
    }
}

void Game::Render() {
    stage.Render();
    basket.Render();
    fruits.Render();

    if(state == END) {
        display.Ending(lives, score, timeEnd, timeStart);
    }

    if(state == PLAYING) {
        display.Playing(lives, score);
    }
}

void Game::Unload() {
    fruits.Unload();
    basket.Unload();
    stage.Unload();
}
