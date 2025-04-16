#include "game.hpp"

const bool Game::isDebug() const {
    return debug;
}

const bool Game::isRunning() const {
    return state != END;
}

void Game::Update() {
    // always 
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();
    }
    // only on state change
    if(lastState != state){
        if(state == OVER){
            displayScore = score * GAME_SCORE_UNIT;
            display.UpdateGameOver(displayScore, timeEnd, timeStart);
        }
    }
    // UI update
    if(state == OVER || state == PAUSE || state == START) {
        display.UpdateStartMenu(mousePosition);

        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            fruits.Reset();
            state = PLAY;
            score = 0;
            lives = GAME_LIVES;
            timeStart = GetTime();
            return;
        }

        if(display.isQuitButtonClicked()){
            state = END;
            return;
        }
    }
    // Game update
    if(state == PLAY) {
        bucket.Update(mousePosition);

        const std::tuple<int, int> result = fruits.Update(bucket);
        lives += std::get<0>(result);
        score += std::get<1>(result);

        if(lives <= 0) {
            state = OVER;
            timeEnd = GetTime();
            return;
        }

        if(IsKeyPressed(KEY_ESCAPE)){
            state = PAUSE;
            return;
        }
    } else if(state == PAUSE) {
        if(IsKeyPressed(KEY_ESCAPE)){
            state = PLAY;
            return;
        }
    }
    
    lastState = state;
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
        display.RenderGameOver();
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
