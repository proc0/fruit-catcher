using namespace std;

#include "game.h"

void FruitCatcher::EndGame() {
    this->_state = END;
    this->_endTime = GetTime();
}

void FruitCatcher::InitGame() {
    this->_state = PLAYING;
    this->_startTime = GetTime();
    this->_lives = INITIAL_LIVES;
    this->_score = 0;
    this->_fruit.Init();
    this->_basket.Init();
}

void FruitCatcher::Update() {
    if(this->_state == END) {
        if(IsKeyPressed(KEY_R)){
            this->InitGame();
        }
    }
    
    if(this->_state == PLAYING) {
        this->_basket.Update();

        tuple<int, int> result = this->_fruit.Update(this->_basket);
        this->_lives += get<0>(result);
        this->_score += get<1>(result);

        if(this->_lives <= 0) {
            this->EndGame();
            return;
        }
    }
}

void FruitCatcher::Render() {
    this->_basket.Render();
    this->_fruit.Render();

    if(this->_state == END) {
        const char* gameOver = GAME_OVER_TEXT;
        const char* restartMessage = RESTART_MESSAGE;
        
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
        DrawText(gameOver, screenWidth / 2 - MeasureText(gameOver, 40) / 2, screenHeight / 2 - 10, 40, BLACK);
        DrawText(restartMessage, screenWidth / 2 - MeasureText(restartMessage, 20) / 2, screenHeight * 0.75f, 20, DARKGRAY);
        
        const char* scoreText = TextFormat("SCORE: %d", _score);
        DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 32) / 2, screenHeight * 0.25f, 32, BLACK);
        
        int totalMinutes = (int)(this->_endTime - this->_startTime) / 60;
        int totalSeconds = (int)(this->_endTime - this->_startTime) % 60;
        DrawText(TextFormat("Time Played: %d minutes and %d seconds", totalMinutes, totalSeconds), 20, screenHeight - 40, 20, DARKGRAY);
    } else {
        const char* scoreText = TextFormat("SCORE: %d", _score);
        const char* livesText = TextFormat("LIVES: %d", _lives);
        DrawText(scoreText, 10, 10, 32, BLACK);
        DrawText(livesText, 10, 35, 24, DARKGRAY);
    }
}