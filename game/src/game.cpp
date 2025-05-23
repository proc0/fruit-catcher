#include "game.hpp"

#define GAME_LEVEL_READY_TIME 1.25f

void Game::Load() {
    musicLevel = LoadMusicStream(MUSIC_LEVEL_URI);
    musicIntro = LoadMusicStream(MUSIC_INTRO_URI);

    SetMusicVolume(musicLevel, 0.4f);
    SetMusicVolume(musicIntro, 0.4f);
    PlayMusicStream(musicIntro);

    soundCheers = LoadSound(URI_SOUND_CHEERS);

    stage.Load();
    fruits.Load(config.GetData(), level.GetFruitLevelData());
    bucket.Load();
    display.Load(config.GetData());
}

void Game::Unload(){
    UnloadSound(soundCheers);
    UnloadMusicStream(musicLevel);
    UnloadMusicStream(musicIntro);

    display.Unload();
    fruits.Unload();
    bucket.Unload();
    stage.Unload();
}

const bool Game::isDebug() const {
    return debug;
}

const bool Game::isRunning() const {
    return state != END;
}

const FruitDisplayResult Game::DisplayFruitResult(const FruitResult& result) const {
    const FruitDisplayResult displayResult = (FruitDisplayResult){
        .location = result.location,
        .color = result.color,
        .id = result.id,
        .score = result.score,
        .lives = result.lives,
        .bounces = result.bounces,
        .hudAnimationIdx = 0,
        .isCatch = result.isCatch,
        .bounced = result.bounced,
        .isSpike = result.isSpike,
        .discard = false,
    };
    return displayResult;
}

const FruitDisplayResults Game::DisplayFruitResults(const FruitResults& results) const {
    FruitDisplayResults displayResults;
    for(const FruitResult& result : results){
        displayResults.push_back(DisplayFruitResult(result));
    }
    return displayResults;
}

// reducing the fruit results into one result struct for bucket...
// might need to mirror how display processes a list of results as well
const BucketDisplayResult Game::DisplayBucketResult(const FruitResults& results) const {
    bool isCatch = false;
    bool isSpike = false;
    bool isBounce = false;
    Color color = WHITE;

    // if any are true, set flag as true
    for(const FruitResult& result : results){
        if(result.isCatch){
            isCatch = true;
            color = result.color;
        } else if(result.isSpike){
            isSpike = true;
        }

        if(result.bounced){
            isBounce = true;
        }
    }

    return (BucketDisplayResult){
        .isCatch = isCatch,
        .isSpike = isSpike,
        .isBounce = isBounce,
        .color = color,
    };
}

const int Game::GetScore(const FruitResults& results) const {
    int score = 0;
    for(const FruitResult& result : results){
        score += result.score;
    }
    return score;
}

const int Game::GetLives(const FruitResults& results) const {
    int lives = 0;
    for(const FruitResult& result : results){
        lives += result.lives;
    }
    return lives;
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

    // using P key for pause because browsers set ESC key to unlock the mouse (when DisableCursor is called)
    // if Hide/ShowCursor is used instead, ESC can be used, but that is blocked (see below)
    if(IsKeyPressed(KEY_P)){
        if(state == PAUSE) {
            state = PLAY;
            DisableCursor();
            return;
        }
        
        if(state == PLAY || state == READY) {
            state = PAUSE;
            EnableCursor();
            return;
        }
    }

    // using Enable/DisableCursor instead of Hide/ShowCursor because Raylib and/or Emsripten is locking mouse with Hide/Show
    // working around this by using GetMouseDelta when mouse locked (disabled). Raylib ticket: https://github.com/raysan5/raylib/issues/4940
    if(IsCursorHidden()){
        mousePosition += GetMouseDelta();
    } else {
        mousePosition = GetMousePosition();
    }

    if(state == PLAY) {
        // Process fruits
        const Rectangle bucketCollision = bucket.GetCollision();
        const std::vector<Vector2> projectiles = bucket.GetProjectiles();
        const FruitResults results = fruits.Update(bucketCollision, projectiles);
        const FruitDisplayResults displayResults = DisplayFruitResults(results);
        const BucketDisplayResult bucketResults = DisplayBucketResult(results);

        lives += GetLives(results);
        score += GetScore(results);
        // Process jar
        bucket.Update(mousePosition, bucketResults);
        // Calculate time
        const int duration = level.GetCurrentLevel().duration;
        const int currentLevel = level.GetCurrentLevel().id;
        timeCount += GetFrameTime();
        timeLeft = duration - timeCount;
        const int projectilesNumber = bucket.GetProjectileNumber();
        // Update UI and HUD effects
        const DisplayStats stats = { lives, score, timeLeft, currentLevel, projectiles: projectilesNumber };
        display.Update(stats, displayResults);
        // Game Over
        if(lives <= 0) {
            state = OVER;
            timeEnd = GetTime();
            display.UpdateOnce(score, timeEnd, timeStart);
            EnableCursor();
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
                EnableCursor();
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
        const std::vector<Vector2> projectiles = bucket.GetProjectiles();
        const FruitResults results = fruits.Update(bucketCollision, projectiles);
        const FruitDisplayResults displayResults = DisplayFruitResults(results);
        const BucketDisplayResult bucketResults = DisplayBucketResult(results);
        
        lives += GetLives(results);
        score += GetScore(results);
        // Process jar
        bucket.Update(mousePosition, bucketResults);
        // HUD
        timeLeft = level.GetCurrentLevel().duration;
        display.Update({ lives, score, timeLeft, level.GetCurrentLevel().id }, displayResults);
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
        display.UpdateMenu(mousePosition);
        // Start or Restart action
        if(display.isStartButtonClicked() || IsKeyPressed(KEY_R)){
            state = READY;
            fruits.Reset();
            fruits.SetLevel(0);
            fruits.Unmute();
            level.Reset();
            bucket.Reset();
            bucket.Update(mousePosition, {false, false, false, WHITE});
            DisableCursor();
            timeStart = GetTime();
            timeReady = 0.0f;
            timeCount = 0.0f;
            points = 0;
            score = 0;
            lives = GAME_LIVES;
            hasCheersPlayed = false;
            if(IsMusicStreamPlaying(musicIntro)){
                StopMusicStream(musicIntro);
            }
            if(IsMusicStreamPlaying(musicLevel)){
                StopMusicStream(musicLevel);
            }
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
        // intro fruits
        fruits.Update({ 0, 0, 0, 0 }, std::vector<Vector2>());
    }

    if(state == WIN){
        if(!hasCheersPlayed){
            PlaySound(soundCheers);
            hasCheersPlayed = true;
        }
        fruits.UpdateWin();
    }
}

void Game::Render() const {
    if(state == END) return;
    
    stage.Render();

    // PLAY || PAUSE || OVER || READY || WIN
    if(state != START && state != END) {
        fruits.Render();
        bucket.Render();
        display.Render();
    }

    if(state == OVER) {
        display.RenderOverMenu();
        return;
    }

    if(state == READY) {
        display.RenderReady();
        return;
    }

    if(state == START){
        fruits.Render();
    }

    if(state == START || state == PAUSE) {
        display.RenderStartMenu();
        return;
    }
    
    if(state == WIN) {
        display.RenderWinMenu();
    }
}

void Game::Loop() {
    Update();
    BeginDrawing();
    Render();
    EndDrawing();
}

void Game::UpdateDebug() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        debugCoordinates = GetMousePosition();
    }
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
    const char* coords = TextFormat("(%d,%d)", (int)debugCoordinates.x, (int)debugCoordinates.y);
    DrawText(coords, (int)debugCoordinates.x, (int)debugCoordinates.y, 33, BLACK);
}

void Game::LoopDebug() {
    UpdateDebug();
    BeginDrawing();
    RenderDebug();
    EndDrawing();
}
