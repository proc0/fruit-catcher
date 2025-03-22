#include "display.hpp"

#define START_MENU_IMAGE "resources/start_menu_panel.png"

const char *textGameTitle = TEXT_GAME_TITLE;
const char *textStart = TEXT_GAME_START;
const char *textQuit = TEXT_GAME_QUIT;
const char *textScore = TEXT_SCORE;
const char *textLives = TEXT_LIVES;
const char *gameOver = TEXT_GAME_OVER;
const char *restartMessage = TEXT_RESTART;
const char *textTimePlayed = TEXT_TIME_PLAYED;

static constexpr int TITLE_FONT_SIZE = 40;
static constexpr int SUBTITLE_FONT_SIZE = 20;
static constexpr int SCORE_FONT_SIZE = 32;
static constexpr int SCREEN_HALF_WIDTH = SCREEN_WIDTH/2;

Display::Display(void) {
    textureStartMenu = LoadTexture(START_MENU_IMAGE);

    const float centerGameStart = SCREEN_HALF_WIDTH - MeasureText(textStart, TITLE_FONT_SIZE)/2;
    const float verticalAlignGameStart = SCREEN_HEIGHT/2 + 30;
    startButtonCollision = { centerGameStart, verticalAlignGameStart, float(MeasureText(textStart, TITLE_FONT_SIZE)), TITLE_FONT_SIZE };

    const int verticalAlignGameQuit = SCREEN_HEIGHT/2 + 80;
    quitButtonCollision = { centerGameStart, verticalAlignGameQuit, float(MeasureText(textQuit, TITLE_FONT_SIZE)), TITLE_FONT_SIZE };
}

Display::~Display(void) {
    UnloadTexture(textureStartMenu);
}

void Display::UpdateStartMenu(Vector2 mousePosition){
    if (CheckCollisionPointRec(mousePosition, startButtonCollision)) {
        startButtonState = HOVER;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            startButtonState = CLICKED;
        }
    } else {
        startButtonState = NONE;
    }

    if(CheckCollisionPointRec(mousePosition, quitButtonCollision)) {
        quitButtonState = HOVER;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            quitButtonState = CLICKED;
        }
    } else {
        quitButtonState = NONE;
    }

    if(startButtonState == HOVER || quitButtonState == HOVER) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    }
}

void Display::DisplayStartMenu(void) {
    const int centerPanel = SCREEN_HALF_WIDTH - textureStartMenu.width/2;
    const int verticalAlignPanel = SCREEN_HEIGHT/2 - textureStartMenu.height/2;
    DrawTexture(textureStartMenu, centerPanel, verticalAlignPanel, WHITE);

    const int centerGameTitle = SCREEN_HALF_WIDTH - MeasureText(textGameTitle, TITLE_FONT_SIZE)/2;
    const int verticalAlignGameTitle = SCREEN_HEIGHT/2 - 100;
    DrawText(textGameTitle, centerGameTitle, verticalAlignGameTitle, TITLE_FONT_SIZE, BLACK);

    const int centerGameStart = SCREEN_HALF_WIDTH - MeasureText(textStart, TITLE_FONT_SIZE)/2;
    const int verticalAlignGameStart = SCREEN_HEIGHT/2 + 30;
    DrawText(textStart, centerGameStart, verticalAlignGameStart, TITLE_FONT_SIZE, BLACK);
    //DrawRectangleRec(startButtonCollision, RED);

    const int verticalAlignGameQuit = SCREEN_HEIGHT/2 + 80;
    DrawText(textQuit, centerGameStart, verticalAlignGameQuit, TITLE_FONT_SIZE, BLACK);
}

void Display::DisplayGameOver(int lives, int score, float timeEnd, float timeStart) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(WHITE, 0.8f));

    const char *scoreText = TextFormat(textScore, score);
    const int centerScoreText = SCREEN_HALF_WIDTH - MeasureText(scoreText, SCORE_FONT_SIZE)/2;
    const int verticalAlignScore = SCREEN_HEIGHT * 0.25f;
    DrawText(scoreText, centerScoreText, verticalAlignScore, SCORE_FONT_SIZE, GetTextColor());
    
    const int centerGameOver = SCREEN_HALF_WIDTH - MeasureText(gameOver, TITLE_FONT_SIZE)/2;
    const int verticalAlignTitle = SCREEN_HEIGHT/2 - 10;
    DrawText(gameOver, centerGameOver, verticalAlignTitle, TITLE_FONT_SIZE, BLACK);

    const int centerRestartMessage = SCREEN_HALF_WIDTH - MeasureText(restartMessage, SUBTITLE_FONT_SIZE)/2;
    const int verticalAlignSubtitle = SCREEN_HEIGHT * 0.75f;
    DrawText(restartMessage, centerRestartMessage, verticalAlignSubtitle, SUBTITLE_FONT_SIZE, DARKGRAY);

    int totalMinutes = (int)(timeEnd - timeStart)/60;
    int totalSeconds = (int)(timeEnd - timeStart) % 60;
    const char *totalTime = TextFormat(textTimePlayed, totalMinutes, totalSeconds);
    DrawText(totalTime, SUBTITLE_FONT_SIZE, SCREEN_HEIGHT - 40, SUBTITLE_FONT_SIZE, DARKGRAY);
}

void Display::DisplayHUD(int lives, int score) {
    const char *scoreText = TextFormat(textScore, score);
    const char *livesText = TextFormat(textLives, lives);
    DrawText(scoreText, 10, 10, 32, BLACK);
    DrawText(livesText, 10, 35, 24, DARKGRAY);
}
