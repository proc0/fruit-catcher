#include "display.hpp"

const char *textScore = TEXT_SCORE;
const char *textLives = TEXT_LIVES;
const char *gameOver = TEXT_GAME_OVER;
const char *restartMessage = TEXT_RESTART;
const char *textTimePlayed = TEXT_TIME_PLAYED;

const int TITLE_FONT_SIZE = 40;
const int SUBTITLE_FONT_SIZE = 20;
const int SCORE_FONT_SIZE = 32;

void Display::Ending(int lives, int score, float timeEnd, float timeStart) {
    const int screenWidthHalf = SCREEN_WIDTH/2;
    const int centerGameOver = screenWidthHalf - MeasureText(gameOver, TITLE_FONT_SIZE)/2;
    const int centerRestartMessage = screenWidthHalf - MeasureText(restartMessage, SUBTITLE_FONT_SIZE)/2;
    const int verticalAlignTitle = SCREEN_HEIGHT/2 - 10;
    const int verticalAlignSubtitle = SCREEN_HEIGHT * 0.75f;
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(WHITE, 0.8f));
    DrawText(gameOver, centerGameOver, verticalAlignTitle, TITLE_FONT_SIZE, BLACK);
    DrawText(restartMessage, centerRestartMessage, verticalAlignSubtitle, SUBTITLE_FONT_SIZE, DARKGRAY);
    
    const char *scoreText = TextFormat(textScore, score);
    const int centerScoreText = screenWidthHalf - MeasureText(scoreText, SCORE_FONT_SIZE)/2;
    const int verticalAlignScore = SCREEN_HEIGHT * 0.25f;
    DrawText(scoreText, centerScoreText, verticalAlignScore, SCORE_FONT_SIZE, GetTextColor());
    
    int totalMinutes = (int)(timeEnd - timeStart)/60;
    int totalSeconds = (int)(timeEnd - timeStart) % 60;
    const char *totalTime = TextFormat(textTimePlayed, totalMinutes, totalSeconds);
    DrawText(totalTime, SUBTITLE_FONT_SIZE, SCREEN_HEIGHT - 40, SUBTITLE_FONT_SIZE, DARKGRAY);
}

void Display::Playing(int lives, int score) {
    const char *scoreText = TextFormat(textScore, score);
    const char *livesText = TextFormat(textLives, lives);
    DrawText(scoreText, 10, 10, 32, BLACK);
    DrawText(livesText, 10, 35, 24, DARKGRAY);
}
