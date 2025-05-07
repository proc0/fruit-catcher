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

    if(IsMusicStreamPlaying(musicLevel)){
        UpdateMusicStream(musicLevel);
    }

    if(IsMusicStreamPlaying(musicIntro)){
        UpdateMusicStream(musicIntro);
    }

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
        lives += result.lives;
        score += result.score;
        // Process jar
        bucket.Update(mousePosition, result.bounced, result.isCatch, result.isSpike, result.color);
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
                level.NextLevel();
                fruits.SetLevel(level.GetCurrentLevel().id);
                state = READY;
                return;
            }
        }
    }

    if(state == READY){
        // Input
        const Rectangle bucketCollision = bucket.GetCollision();
        const FruitResult result = fruits.Update(bucketCollision);
        lives += result.lives;
        score += result.score;
        bucket.Update(mousePosition, result.bounced, result.isCatch, result.isSpike, result.color);
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
            state = READY;
            fruits.Reset();
            fruits.SetLevel(0);
            level.Reset();
            bucket.Reset();
            bucket.Update(mousePosition, false, false, false);
            HideCursor();
            timeStart = GetTime();
            timeReady = 0.0f;
            timeCount = 0.0f;
            points = 0;
            score = 0;
            lives = GAME_LIVES;
            StopMusicStream(musicIntro);
            PlayMusicStream(musicLevel);
            return;
        }
        // Quit action
        if(display.isQuitButtonClicked()){
            state = END;
            return;
        }
    }

    if(state == START){
        fruits.Update({ 0, 0, 0, 0 });
    }

    if(state == WIN){
        fruits.UpdateWin();
    }
}

void Game::Render() const {
    if(state == END) return;
    
    stage.Render();

    if(state == PLAY || state == PAUSE || state == OVER || state == WIN) {
        fruits.Render();
        bucket.Render();
        display.Render();
    }

    if(state == OVER) {
        display.RenderGameOver();
    }

    if(state == WIN) {
        display.RenderWin();
    }

    if(state == START || state == PAUSE) {
        fruits.Render();
        display.RenderStartMenu();
    }

    if(state == READY) {
        fruits.Render();
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
