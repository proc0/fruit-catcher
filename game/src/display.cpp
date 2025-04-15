#include "display.hpp"

#define START_MENU_IMAGE "resources/start_menu_panel.png"
#define PANEL_GAME_OVER "resources/game_over_panel.png"
#define HUD_PANEL "resources/hud_panel.png"
#define FRUIT_ICON_URI "resources/fruit_icon.png"

const char *textGameTitle = TEXT_START_MENU_TITLE;
const char *textStart = TEXT_GAME_START;
const char *textQuit = TEXT_GAME_QUIT;
const char *textScore = TEXT_SCORE;
const char *textLives = TEXT_LIVES;
const char *textRestart = TEXT_RESTART;
const char *gameOver = TEXT_GAME_OVER;
const char *restartMessage = TEXT_PRESS_R;
const char *textTimePlayed = TEXT_TIME_PLAYED;

static const int TITLE_FONT_SIZE = 58;
static const int TEXT_MENU_FONT_SIZE = 44;
static const int SUBTITLE_FONT_SIZE = 20;
static const int SCORE_FONT_SIZE = 52;
static constexpr int SCREEN_HALF_WIDTH = SCREEN_WIDTH/2;

//TODO: refactor and optimize the layout variables. Needs to be initialized and stored in some structure.
Display::Display(void) {
    textureStartMenu = LoadTexture(START_MENU_IMAGE);
    fruitIcon = LoadTexture(FRUIT_ICON_URI);
    gameOverPanel = LoadTexture(PANEL_GAME_OVER);

    const float centerGameStart = SCREEN_HALF_WIDTH - MeasureText(textStart, TEXT_MENU_FONT_SIZE)/2;
    const float verticalAlignGameStart = SCREEN_HEIGHT/2 + 30;
    startButtonCollision = { centerGameStart, verticalAlignGameStart, float(MeasureText(textStart, TEXT_MENU_FONT_SIZE)), TEXT_MENU_FONT_SIZE };

    const int verticalAlignGameQuit = SCREEN_HEIGHT/2 + 80;
    quitButtonCollision = { centerGameStart, verticalAlignGameQuit, float(MeasureText(textQuit, TEXT_MENU_FONT_SIZE)), TEXT_MENU_FONT_SIZE };
}

Display::~Display(void) {
    UnloadTexture(textureStartMenu);
    UnloadTexture(fruitIcon);
    UnloadTexture(gameOverPanel);
}

const bool Display::isStartButtonClicked(void) const {
    return startButtonState == CLICKED;
}

const bool Display::isQuitButtonClicked(void) const {
    return quitButtonState == CLICKED;
}

void Display::UpdateStartMenu(Vector2 mousePosition) {
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

void Display::RenderStartMenu(void) const {
    const int centerPanel = SCREEN_HALF_WIDTH - textureStartMenu.width/2;
    const int verticalAlignPanel = SCREEN_HEIGHT/2 - textureStartMenu.height/2;
    DrawTexture(textureStartMenu, centerPanel, verticalAlignPanel, WHITE);

    const int centerGameTitle = SCREEN_HALF_WIDTH - MeasureText(textGameTitle, TITLE_FONT_SIZE)/2;
    const int verticalAlignGameTitle = SCREEN_HEIGHT/2 - 140;
    DrawText(textGameTitle, centerGameTitle, verticalAlignGameTitle, TITLE_FONT_SIZE, BLACK);

    const int centerGameStart = SCREEN_HALF_WIDTH - MeasureText(textStart, TEXT_MENU_FONT_SIZE)/2;
    const int verticalAlignGameStart = SCREEN_HEIGHT/2 + 30;
    const Color startColor = startButtonState == HOVER ? RED : BLACK;
    DrawText(textStart, centerGameStart, verticalAlignGameStart, TEXT_MENU_FONT_SIZE, startColor);
    //DrawRectangleRec(startButtonCollision, RED);

    const int verticalAlignGameQuit = SCREEN_HEIGHT/2 + 80;
    const Color quitColor = quitButtonState == HOVER ? RED : BLACK;
    DrawText(textQuit, centerGameStart, verticalAlignGameQuit, TEXT_MENU_FONT_SIZE, quitColor);
}

void Display::RenderGameOver(int lives, int score, float timeEnd, float timeStart) const {
    const int centerPanel = SCREEN_HALF_WIDTH - gameOverPanel.width/2;
    const int verticalAlignPanel = SCREEN_HEIGHT/2 - gameOverPanel.height/2;
    DrawTexture(gameOverPanel, centerPanel, verticalAlignPanel, WHITE);
   
    const char *scoreText = TextFormat(textScore, score);
    const int centerScoreText = SCREEN_HALF_WIDTH - MeasureText(scoreText, SCORE_FONT_SIZE)/2;
    const int verticalAlignScore = SCREEN_HEIGHT*0.22f;
    DrawText(scoreText, centerScoreText, verticalAlignScore, SCORE_FONT_SIZE, GOLD);

    int totalMinutes = (int)(timeEnd - timeStart)/60;
    int totalSeconds = (int)(timeEnd - timeStart)%60;
    const char *totalTime = TextFormat(textTimePlayed, totalMinutes, totalSeconds);
    const int centerTotalTime = SCREEN_HALF_WIDTH - MeasureText(totalTime, SUBTITLE_FONT_SIZE)/2;
    DrawText(totalTime, centerTotalTime, verticalAlignScore + 60, SUBTITLE_FONT_SIZE, DARKGRAY);
 
    const int centerGameOver = SCREEN_HALF_WIDTH - MeasureText(gameOver, TEXT_MENU_FONT_SIZE)/2;
    const int verticalAlignTitle = SCREEN_HEIGHT*0.5f - 100;
    DrawText(gameOver, centerGameOver, verticalAlignTitle, TEXT_MENU_FONT_SIZE, BLACK);

    const int centerGameRestart = SCREEN_HALF_WIDTH - MeasureText(textRestart, TEXT_MENU_FONT_SIZE)/2;
    const int verticalAlignGameStart = SCREEN_HEIGHT*0.5f + 30;
    const Color startColor = startButtonState == HOVER ? RED : BLACK;
    DrawText(textRestart, centerGameRestart, verticalAlignGameStart, TEXT_MENU_FONT_SIZE, startColor);

    const int verticalAlignGameQuit = SCREEN_HEIGHT*0.5f + 80;
    const Color quitColor = quitButtonState == HOVER ? RED : BLACK;
    DrawText(textQuit, centerGameRestart, verticalAlignGameQuit, TEXT_MENU_FONT_SIZE, quitColor);

    const int centerRestartMessage = SCREEN_HALF_WIDTH - MeasureText(restartMessage, SUBTITLE_FONT_SIZE)*0.5f;
    const int verticalAlignSubtitle = SCREEN_HEIGHT*0.75f;
    DrawText(restartMessage, centerRestartMessage, verticalAlignSubtitle, SUBTITLE_FONT_SIZE, DARKGRAY);
}

void Display::Render(int lives, int score) const {
    const char *scoreNum = TextFormat("%d", score * 50);
    const char *livesNum = TextFormat("%d", lives);
    DrawText(textLives, 28, 32, 18, WHITE);
    DrawText(livesNum, 28, 55, 72, WHITE);

    DrawTexture(fruitIcon, 100, 32, WHITE);
    DrawText("x", 100, 85, 18, WHITE);
    DrawText(scoreNum, 115, 75, 32, WHITE);
}
