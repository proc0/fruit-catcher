#include "game.hpp"

using namespace std;

void Game::Over() {
    state = END;
    timeEnd = GetTime();
}

void Game::Start() {
    score = 0;
    lives = GAME_LIVES;
    state = PLAY;
    timeStart = GetTime();
}

void Game::Update() {
    stage.Update();

    if((state == END && IsKeyPressed(KEY_R)) || state == START) {
        Start();
    }
    
    if(state == PLAY) {
        pot.Update();

        tuple<int, int> result = fruits.Update(pot);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            Over();
        }
    }
}

void Game::Render() {
    stage.Render();
    pot.Render();
    fruits.Render();

    if(state == END) {
        display.Ending(lives, score, timeEnd, timeStart);
    }

    if(state == PLAY) {
        display.Playing(lives, score);
    }
}

