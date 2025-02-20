#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
    timeEnd = GetTime();
}

void Game::Begin() {
    state = PLAYING;
    timeStart = GetTime();

    fruits.Init();
    basket.Init();
}

void Game::Update() {
    if(state == END) {
        if(IsKeyPressed(KEY_R)){
            Begin();
        }
    }
    
    if(state == PLAYING) {
        basket.Update();

        tuple<int, int> result = fruits.Update(basket);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            End();
        }
    }
}

void Game::Render() {
    basket.Render();
    fruits.Render();

    if(state == END) {
        const char *gameOver = GAME_OVER_TEXT;
        const char *restartMessage = RESTART_MESSAGE;
        
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
        DrawText(gameOver, screenWidth / 2 - MeasureText(gameOver, 40) / 2, screenHeight / 2 - 10, 40, BLACK);
        DrawText(restartMessage, screenWidth / 2 - MeasureText(restartMessage, 20) / 2, screenHeight * 0.75f, 20, DARKGRAY);
        
        const char *scoreText = TextFormat("SCORE: %d", score);
        DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 32) / 2, screenHeight * 0.25f, 32, GetTextColor());
        
        int totalMinutes = (int)(timeEnd - timeStart) / 60;
        int totalSeconds = (int)(timeEnd - timeStart) % 60;
        DrawText(TextFormat(TEXT_TIME_PLAYED, totalMinutes, totalSeconds), 20, screenHeight - 40, 20, DARKGRAY);
    } else {
        const char *scoreText = TextFormat("SCORE: %d", score);
        const char *livesText = TextFormat("LIVES: %d", lives);
        DrawText(scoreText, 10, 10, 32, BLACK);
        DrawText(livesText, 10, 35, 24, DARKGRAY);
    }
}