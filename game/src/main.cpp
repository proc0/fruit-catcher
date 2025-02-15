#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "lib.h"

typedef struct Apple {
    Vector2 position;
    int fallSpeed;
    bool _active;
} Apple;

typedef enum GameState {
    PLAYING,
    END
} GameState;

float _timeGameStarted;
float _timeGameEnded;
float _nextAppleTimer;
float _basketXPosition;
int _score;
int _lives;

GameState _state;
Apple _apples[APPLE_MAX_COUNT];

Texture2D _atlasApples;
Texture2D _atlasBasket;

void GameInit(void);
void UpdateDrawFrame(void);
void GameEnd(void);

void SetAppleAt(int, Vector2, int);
void UnsetAppleAt(int);

void UpdateApples(void);
void SpawnApple(void);
void DrawApples(void);

Vector2 GetBasketPosition(void);
void DrawBasket(void);

int main() {
    
  srand(time(0));
  
  InitWindow(screenWidth, screenHeight, "Fruit Catcher");
  SetTargetFPS(60);
  
  _atlasApples = LoadTexture("resources/applesprites.png");
  _atlasBasket = LoadTexture("resources/basket.png");
  
  GameInit();
  
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }
  
  CloseWindow();
  
  return 0;
}

void SetAppleAt(int i, Vector2 position, int fallSpeed) {
    if(i < 0 || i > APPLE_MAX_COUNT){
        return;
    }
    _apples[i].position = position;
    _apples[i].fallSpeed = fallSpeed;
    _apples[i]._active = true;
}

void UnsetAppleAt(int i) {
    if(i < 0 || i > APPLE_MAX_COUNT){
        return;
    }
    
    _apples[i]._active = false;
}

void GameInit(void) {
    _state = PLAYING;
    _timeGameStarted = GetTime();
    _nextAppleTimer = TIME_BETWEEN_APPLES;
    _lives = INITIAL_LIVES;
    _score = 0;
    
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        UnsetAppleAt(i);
    }
}

void DrawApples(void) {
    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!_apples[i]._active){
           continue;
       }
       Vector2 position = _apples[i].position;
       position.x -= APPLE_SOURCE_WIDTH / 2;
       position.y -= APPLE_SOURCE_HEIGHT / 2;
       DrawTextureRec(_atlasApples, APPLE_SOURCE_RECTANGLE, position, WHITE);
    }
}

Vector2 GetBasketPosition(void) {
    
    if(_state == PLAYING) {
        _basketXPosition = GetMousePosition().x;
    }
    
    return { _basketXPosition, screenHeight - (BASKET_SOURCE_HEIGHT * 0.75) };
}

void DrawBasket(void) {
    Vector2 position = GetBasketPosition();
    
    position.x -= _atlasBasket.width / 2;
    position.y -= _atlasBasket.height / 2;
    
    DrawTextureRec(_atlasBasket, BASKET_SOURCE_RECTANGLE, position, WHITE);
}

void UpdateApples(void) {
    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!_apples[i]._active){
           continue;
       }
        
       if(_apples[i].position.y > screenHeight) {
            UnsetAppleAt(i);
            _lives--;
            if(_lives <= 0) {
                GameEnd();
                return;
            }
            continue;
       }
       
       Vector2 basketPosition = GetBasketPosition();
       if(fabsf(_apples[i].position.x - basketPosition.x) < BASKET_SOURCE_WIDTH && fabsf(_apples[i].position.y - basketPosition.y) < BASKET_SOURCE_HEIGHT / 4 ) {
           UnsetAppleAt(i);
           _score++;
           continue;
       }
       
       float offset = _apples[i].fallSpeed * GetFrameTime();
       _apples[i].position.y += offset;
    }
}

void SpawnApple(void) {
    int availableIndex = -1;
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        if(!_apples[i]._active){
           availableIndex = i;
           break;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }
    
    int fallSpeed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
    int posX = GetRandomValue(APPLE_SOURCE_WIDTH / 2, screenWidth - (APPLE_SOURCE_WIDTH / 2));
    
    SetAppleAt(availableIndex, { float(posX), -APPLE_SOURCE_HEIGHT}, fallSpeed);
}

void GameEnd(void) {
    _state = END;
    _timeGameEnded = GetTime();
}

void UpdateDrawFrame(void) {
    
    if(_state == END) {
        if(IsKeyPressed(KEY_R)){
            GameInit();
        }
    }
    
    if(_state == PLAYING) {
        UpdateApples();
        
        _nextAppleTimer -= GetFrameTime();
        if(_nextAppleTimer <= 0) {
            _nextAppleTimer = TIME_BETWEEN_APPLES;
            SpawnApple();
        }
    }
    
    //if (win condition){
    //    GameEnd();
    //}

    // Draw Game
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawBasket();
    DrawApples();

    if(_state == END) {
        const char* gameOver = GAME_OVER_TEXT;
        const char* restartMessage = RESTART_MESSAGE;
        
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
        DrawText(gameOver, screenWidth / 2 - MeasureText(gameOver, 40) / 2, screenHeight / 2 - 10, 40, BLACK);
        DrawText(restartMessage, screenWidth / 2 - MeasureText(restartMessage, 20) / 2, screenHeight * 0.75f, 20, DARKGRAY);
        
        const char* scoreText = TextFormat("SCORE: %d", _score);
        DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 32) / 2, screenHeight * 0.25f, 32, BLACK);
        
        int totalMinutes = (int)(_timeGameEnded - _timeGameStarted) / 60;
        int totalSeconds = (int)(_timeGameEnded - _timeGameStarted) % 60;
        DrawText(TextFormat("Time Played: %d minutes and %d seconds", totalMinutes, totalSeconds), 20, screenHeight - 40, 20, DARKGRAY);
    } else {
        const char* scoreText = TextFormat("SCORE: %d", _score);
        const char* livesText = TextFormat("LIVES: %d", _lives);
        DrawText(scoreText, 10, 10, 32, BLACK);
        DrawText(livesText, 10, 35, 24, DARKGRAY);
    }
    
    EndDrawing();
}