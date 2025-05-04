#include "game.hpp"

#define GAME_LEVEL_READY_TIME 1.25f

const bool Game::isDebug() const {
    return debug;
}

const bool Game::isRunning() const {
    return state != END;
}

void Game::Update() {
    if(state == END) return;

    stage.Update();

    if(state == PAUSE) {
        if(IsKeyPressed(KEY_ESCAPE)){
            state = PLAY;
            HideCursor();
            return;
        }
    }
    
    if(IsKeyPressed(KEY_ESCAPE)) {
        if(state == WIN){
            state = END;
        } else {
            state = PAUSE;
            ShowCursor();
        }
        return;
    }

    mousePosition = GetMousePosition();

    if(state == PLAY) {
        // Process fruits
        const Rectangle bucketCollision = bucket.GetCollision();
        const FruitResult result = fruits.Update(bucketCollision);
        lives -= result.isMiss;
        score += result.score;
        // Process jar
        bucket.Update(mousePosition, result.bounced, result.isCatch, result.color);
        // Calculate time
        const int duration = level.GetCurrentLevel().duration;
        const int currentLevel = level.GetCurrentLevel().id;
        timeCount += GetFrameTime();
        timeLeft = duration - timeCount;
        // Update UI and HUD effects
        const DisplayStats stats = { lives, score, timeLeft, currentLevel };
        const ScorePopup popup = { result.location, result.score, result.isCatch };
        display.Update(stats, popup);
        // Game Over
        if(lives <= 0) {
            state = OVER;
            timeEnd = GetTime();
            level.Reset();
            fruits.SetLevel(0);
            display.UpdateOnce(score, timeEnd, timeStart);
            ShowCursor();
            return;
        }
        // Progress
        if(timeCount >= duration){
            timeCount = 0.0f;
            if(currentLevel == LEVEL_COUNT - 1){
                state = WIN;
                timeEnd = GetTime();
                level.Reset();
                fruits.SetLevel(0);
                display.UpdateOnce(score, timeEnd, timeStart);
                ShowCursor();
                return;                
            } else {
                state = READY;
                level.NextLevel();
                fruits.Reset();
                fruits.SetLevel(level.GetCurrentLevel().id);
                return;
            }
        }
    }

    if(state == READY){
        // Input
        bucket.Update(mousePosition, false, false);
        // HUD
        timeLeft = level.GetCurrentLevel().duration;
        display.Update({ lives, score, timeLeft, level.GetCurrentLevel().id }, { { 0, 0 }, 0, false });
        // Countdown
        timeReady += GetFrameTime();
        if(timeReady >= GAME_LEVEL_READY_TIME){
            timeReady = 0.0f;
            state = PLAY;
            return;
        }

    }

    // UI
    if(state == OVER || state == PAUSE || state == START || state == WIN) {
        display.UpdateStartMenu(mousePosition);
        // Start or Restart action
        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            fruits.Reset();
            bucket.Reset();
            bucket.Update(mousePosition, false, false);
            HideCursor();
            timeStart = GetTime();
            timeReady = 0.0f;
            timeCount = 0.0f;
            points = 0;
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

    if(state == WIN){
        fruits.UpdateWin();
    }
}

void Game::Render() const {
    if(state == END) return;
    
    stage.Render();

    if(state == PLAY || state == PAUSE || state == OVER || state == WIN) {
        bucket.Render();
        fruits.Render();
        display.Render();
    }

    if(state == OVER) {
        display.RenderGameOver();
    }

    if(state == WIN) {
        display.RenderWin();
    }

    if(state == START || state == PAUSE) {
        display.RenderStartMenu();
    }

    if(state == READY) {
        bucket.Render();
        display.Render();
        display.RenderReady();
    }
}

void Game::UpdateDebug() {
    if(state != END){
        stage.Update();

        fruits.UpdateDebug();
        bucket.UpdateDebug();
    }
}

void Game::RenderDebug() const {
    if(state != END){
        stage.Render();
        fruits.RenderDebug();
        bucket.RenderDebug();
    }
}
