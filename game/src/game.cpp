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

    // UI update
    if(state == OVER || state == PAUSE || state == START) {
        display.UpdateStartMenu(mousePosition);

        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            fruits.Reset();
            timeStart = GetTime();
            score = 0;
            lives = GAME_LIVES;
            state = PLAY;
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
        displayScore = score * GAME_SCORE_UNIT;

        if(lives <= 0) {
            timeEnd = GetTime();
            // fires once to update score and time - for now
            display.UpdateGameOver(displayScore, timeEnd, timeStart);
            state = OVER;
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
}

void Game::Render() const {
    if(state != END){
        stage.Render();
    }

    if(state == PLAY || state == PAUSE || state == OVER) {
        bucket.Render();
        fruits.Render();
        display.Render(lives, displayScore);
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
