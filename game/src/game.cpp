#include "game.hpp"

#define GAME_LEVEL_READY_TIME 2.0f

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

    if(state == PAUSE) {
        if(IsKeyPressed(KEY_ESCAPE)){
            HideCursor();
            state = PLAY;
            return;
        }
    }
    
    if(IsKeyPressed(KEY_ESCAPE)){
        ShowCursor();
        state = PAUSE;
    }

    // UI update
    if(state == OVER || state == PAUSE || state == START) {
        display.UpdateStartMenu(mousePosition);
        // Start or Restart action
        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            fruits.Reset();
            bucket.Update(mousePosition);
            HideCursor();
            timeStart = GetTime();
            levelTimeEnd = 0.0f;
            levelTimeStart = 0.0f;
            score = 0;
            lives = GAME_LIVES;
            state = READY;
            return;
        }
        // Quit action
        if(display.isQuitButtonClicked()){
            state = END;
            return;
        }
    }
    
    // Game update
    if(state == READY){
        bucket.Update(mousePosition);
        display.Update(lives, displayScore);

        levelTimeStart += GetFrameTime();
        if(levelTimeStart >= GAME_LEVEL_READY_TIME){
            state = PLAY;
            return;
        }

    }

    if(state == PLAY) {
        bucket.Update(mousePosition);

        const std::tuple<int, int> result = fruits.Update(bucket);
        lives += std::get<0>(result);
        score += std::get<1>(result);
        displayScore = score * GAME_SCORE_UNIT;
        display.Update(lives, displayScore);

        if(lives <= 0) {
            timeEnd = GetTime();
            // fires once to update score and time
            display.UpdateGameOver(displayScore, timeEnd, timeStart);
            ShowCursor();
            state = OVER;
            return;
        }

        levelTimeStart += GetFrameTime();
        if(levelTimeStart >= level.GetCurrentLevel().duration){
            levelTimeEnd = GetTime();
            levelTimeStart = 0.0f;
            if(level.GetCurrentLevel().id == GAME_LEVELS_NUMBER - 1){
                //TODO: add a game ending screen
                timeEnd = GetTime();
                level.Reset();
                fruits.SetLevel(0);
                // fires once to update score and time
                display.UpdateGameOver(displayScore, timeEnd, timeStart);
                ShowCursor();
                state = OVER;
                return;                
            } else {
                level.NextLevel();
                fruits.Reset();
                fruits.SetLevel(level.GetCurrentLevel().id);
                state = READY;
                return;
            }
        }
    }
}

void Game::Render() const {
    if(state != END){
        stage.Render();
    }

    if(state == READY){
        bucket.Render();
        display.Render();
        display.RenderReady();
    }

    if(state == PLAY || state == PAUSE || state == OVER) {
        bucket.Render();
        fruits.Render();
        display.Render();
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
