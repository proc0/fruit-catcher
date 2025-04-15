#include "game.hpp"

using namespace std;

const bool Game::isDebug() const {
    return debug;
}

const bool Game::isRunning() const {
    return state != END;
}

void Game::Update() {
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();
    }

    if(state == OVER || state == PAUSE || state == START) {
        display.UpdateStartMenu(mousePosition);

        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            fruits.Reset();
            state = PLAY;
            score = 0;
            lives = GAME_LIVES;
            timeStart = GetTime();
        }

        if(display.isQuitButtonClicked()){
            state = END;
        }
    }

    if(state == PLAY) {
        bucket.Update(mousePosition);

        const tuple<int, int> result = fruits.Update(bucket);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            state = OVER;
            timeEnd = GetTime();
        }

        if(IsKeyPressed(KEY_ESCAPE)){
            state = PAUSE;
        }
    } else if(state == PAUSE) {
        if(IsKeyPressed(KEY_ESCAPE)){
            state = PLAY;
        }
    }
}

void Game::Render() const {
    if(state != END){
        stage.Render();
    }

    if(state == PLAY || state == PAUSE || state == OVER) {
        bucket.Render();
        fruits.Render();
        display.Render(lives, score);
    }

    if(state == OVER) {
        display.RenderGameOver(lives, score, timeEnd, timeStart);
    }

    if(state == START || state == PAUSE) {
        display.RenderStartMenu();
    }
}

void Game::UpdateDebug() {
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();

        fruits.UpdateDebug();
    }
}

void Game::RenderDebug() const {
    if(state != END){
        stage.Render();
        fruits.RenderDebug();
    }
}
