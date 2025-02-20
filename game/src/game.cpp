#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
    timeEnd = GetTime();
}

void Game::Begin() {
    score = 0;
    lives = INITIAL_LIVES;
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

    const char *textScore = TEXT_SCORE;
    const char *textLives = TEXT_LIVES;

    if(state == END) {
        const char *gameOver = GAME_OVER_TEXT;
        const char *restartMessage = RESTART_MESSAGE;
        const char *textTimePlayed = TEXT_TIME_PLAYED;

        int screenWidthHalf = screenWidth/2;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
        DrawText(gameOver, screenWidthHalf - MeasureText(gameOver, 40)/2, screenHeight/2 - 10, 40, BLACK);
        DrawText(restartMessage, screenWidthHalf - MeasureText(restartMessage, 20)/2, screenHeight * 0.75f, 20, DARKGRAY);
        
        const char *scoreText = TextFormat(textScore, score);
        DrawText(scoreText, screenWidthHalf - MeasureText(scoreText, 32)/2, screenHeight * 0.25f, 32, GetTextColor());
        
        int totalMinutes = (int)(timeEnd - timeStart)/60;
        int totalSeconds = (int)(timeEnd - timeStart) % 60;
        DrawText(TextFormat(textTimePlayed, totalMinutes, totalSeconds), 20, screenHeight - 40, 20, DARKGRAY);
    } else {
        const char *scoreText = TextFormat(textScore, score);
        const char *livesText = TextFormat(textLives, lives);
        DrawText(scoreText, 10, 10, 32, BLACK);
        DrawText(livesText, 10, 35, 24, DARKGRAY);
    }
}