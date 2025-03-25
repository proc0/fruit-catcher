#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
}

const bool Game::isRunning() const {
    return state != END;
}

void Game::Over() {
    state = OVER;
    timeEnd = GetTime();
}

void Game::Begin() {
    state = PLAY;
    score = 0;
    lives = GAME_LIVES;
    timeStart = GetTime();
}

void Game::Update() {
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();
    }

    if(state == OVER) {
        if(IsKeyPressed(KEY_R)){
            Begin();
        }
    }

    if(state == START){
        display.UpdateStartMenu(mousePosition);

        if(display.isStartButtonClicked()){
            Begin();
        }

        if(display.isQuitButtonClicked()){
            End();
        }
    }

    if(state == PLAY) {
        bucket.Update(mousePosition);

        const tuple<int, int> result = fruits.Update(bucket);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            Over();
        }
    }
}

void Game::Render() const {
    if(state != END){
        stage.Render();
    }

    if(state == OVER) {
        display.RenderGameOver(lives, score, timeEnd, timeStart);
    }

    if(state == START) {
        display.RenderStartMenu();
    }

    if(state == PLAY) {
        bucket.Render();
        fruits.Render();
        display.Render(lives, score);
    }
}

