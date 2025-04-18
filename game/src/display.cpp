#include <iterator>
#include "display.hpp"

#define START_MENU_IMAGE "resources/start_menu_panel.png"
#define PANEL_GAME_OVER "resources/game_over_panel.png"
#define HUD_PANEL "resources/hud_panel.png"
#define FRUIT_ICON_URI "resources/fruit_icon.png"

static const char *textGameOver = TEXT_GAME_OVER;
static const char *textLives = TEXT_LIVES;
static const char *textQuit = TEXT_GAME_QUIT;
static const char *textRestart = TEXT_RESTART;
static const char *textRestartMessage = TEXT_PRESS_R;
static const char *textStart = TEXT_GAME_START;
static const char *textStartMenuTitle = TEXT_START_MENU_TITLE;
static const char *textTimePlayed = TEXT_TIME_PLAYED;
static const char *textTotalScore = TEXT_SCORE;

static const int FONTSIZE_TITLE = 58;
static const int FONTSIZE_SUBTITLE = 20;
static const int FONTSIZE_MENUTEXT = 44;
static const int FONTSIZE_SCORETEXT = 52;
static constexpr int SCREEN_HALFWIDTH = SCREEN_WIDTH*0.5f;
static constexpr int SCREEN_HALFHEIGHT = SCREEN_HEIGHT*0.5f;

Display::Display(void) {
    panelStartMenu = LoadTexture(START_MENU_IMAGE);
    panelGameOver = LoadTexture(PANEL_GAME_OVER);
    fruitIcon = LoadTexture(FRUIT_ICON_URI);

    const int startMenuPanelX = SCREEN_HALFWIDTH - panelStartMenu.width/2;
    const int startMenuPanelY = SCREEN_HALFHEIGHT - panelStartMenu.height/2;
    panelTextureParams["startMenuPanel"] = {
        texture: panelStartMenu,
        x: startMenuPanelX,
        y: startMenuPanelY,
        color: WHITE,
    };

    const int gameOverPanelX = SCREEN_HALFWIDTH - panelGameOver.width/2;
    const int gameOverPanelY = SCREEN_HEIGHT/2 - panelGameOver.height/2;
    panelTextureParams["gameOverPanel"] = {
        texture: panelGameOver,
        x: gameOverPanelX,
        y: gameOverPanelY,
        color: WHITE,
    };

    // Button Collisions - using the same buttons for Start, Pause, and Game Over screens
    const float startButtonX = SCREEN_HALFWIDTH - MeasureText(textStart, FONTSIZE_MENUTEXT)*0.5f;
    const float startButtonY = SCREEN_HALFHEIGHT + 30.0f;
    const float startButtonWidth = MeasureText(textStart, FONTSIZE_MENUTEXT);
    startButtonCollision = { startButtonX, startButtonY, startButtonWidth, FONTSIZE_MENUTEXT };

    const float quitButtonY = SCREEN_HALFHEIGHT + 80.0f;
    const float quitButtonWidth = MeasureText(textQuit, FONTSIZE_MENUTEXT);
    quitButtonCollision = { startButtonX, quitButtonY, quitButtonWidth, FONTSIZE_MENUTEXT };

    // Start Menu
    const int startMenuTitleX = SCREEN_HALFWIDTH - MeasureText(textStartMenuTitle, FONTSIZE_TITLE)*0.5f;
    const int startMenuTitleY = SCREEN_HALFHEIGHT - 140.0f;
    startMenuTextParams["startMenuTitle"] = {
        text: textStartMenuTitle,
        x: startMenuTitleX,
        y: startMenuTitleY,
        fontSize: FONTSIZE_TITLE,
        color: BLACK,
    };

    startMenuTextParams["startMenuStartButton"] = {
        text: textStart,
        x: int(startButtonX),
        y: int(startButtonY),
        fontSize: FONTSIZE_MENUTEXT,
        color: BLACK,
    };

    startMenuTextParams["startMenuQuitButton"] = {
        text: textQuit,
        x: int(startButtonX),
        y: int(quitButtonY),
        fontSize: FONTSIZE_MENUTEXT,
        color: BLACK,
    };

    // Game Over
    const int gameOverTitleX = SCREEN_HALFWIDTH - MeasureText(textGameOver, FONTSIZE_MENUTEXT)/2;
    const int gameOverTitleY = SCREEN_HALFHEIGHT - 100;
    gameOverTextParams["gameOverTitle"] = {
        text: textGameOver,
        x: gameOverTitleX,
        y: gameOverTitleY,
        fontSize: FONTSIZE_MENUTEXT,
        color: BLACK,
    };

    const int restartButtonX = SCREEN_HALFWIDTH - MeasureText(textRestart, FONTSIZE_MENUTEXT)/2;
    gameOverTextParams["gameOverRestartButton"] = {
        text: textRestart,
        x: restartButtonX,
        y: int(startButtonY),
        fontSize: FONTSIZE_MENUTEXT,
        color: BLACK,
    };

    gameOverTextParams["gameOverQuitButton"] = {
        text: textQuit,
        x: restartButtonX,
        y: int(quitButtonY),
        fontSize: FONTSIZE_MENUTEXT,
        color: BLACK,
    };

    const int restartMessageX = SCREEN_HALFWIDTH - MeasureText(textRestartMessage, FONTSIZE_SUBTITLE)*0.5f;
    const int restartMessageY = SCREEN_HEIGHT*0.75f;
    gameOverTextParams["gameOverRestartMessage"] = {
        text: textRestartMessage,
        x: restartMessageX,
        y: int(restartMessageY),
        fontSize: FONTSIZE_SUBTITLE,
        color: DARKGRAY,
    };
}

Display::~Display(void) {
    UnloadTexture(panelStartMenu);
    UnloadTexture(fruitIcon);
    UnloadTexture(panelGameOver);
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

    if(startButtonState == HOVER && startButtonLastState != startButtonState){
        startMenuTextParams["startMenuStartButton"].color = RED;
        gameOverTextParams["gameOverRestartButton"].color = RED;
    } else if (startButtonLastState != startButtonState) {
        startMenuTextParams["startMenuStartButton"].color = BLACK;
        gameOverTextParams["gameOverRestartButton"].color = BLACK;
    }

    if(quitButtonState == HOVER && quitButtonLastState != quitButtonState){
        startMenuTextParams["startMenuQuitButton"].color = RED;
        gameOverTextParams["gameOverQuitButton"].color = RED;
    } else if(quitButtonLastState != quitButtonState){
        startMenuTextParams["startMenuQuitButton"].color = BLACK;
        gameOverTextParams["gameOverQuitButton"].color = BLACK;
    }

    if(startButtonState == HOVER || quitButtonState == HOVER) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    }

    startButtonLastState = startButtonState;
    quitButtonLastState = quitButtonState;
}


void Display::RenderStartMenu(void) const {
    const TextureParams panel = panelTextureParams.at("startMenuPanel");
    DrawTexture(panel.texture, panel.x, panel.y, panel.color);

    for(const auto& textParams : startMenuTextParams) {
        const TextParams& entry = textParams.second;
        DrawText(entry.text, entry.x, entry.y, entry.fontSize, entry.color);
    }
}

void Display::UpdateGameOver(int score, float timeEnd, float timeStart) {
    // Using snprintf because this function is being called once only (in game update)
    // If function is called multiple times in update, change to using Raylib TextFormat
    // const char *textScore = TextFormat(textTotalScore, score);
    snprintf(textScore, sizeof(textScore), textTotalScore, score);
    const int scoreTextX = SCREEN_HALFWIDTH - MeasureText(textScore, FONTSIZE_SCORETEXT)/2;
    const int scoreTextY = SCREEN_HEIGHT*0.22f;
    gameOverTextParams["gameOverScore"] = {
        text: textScore,
        x: scoreTextX,
        y: scoreTextY,
        fontSize: FONTSIZE_SCORETEXT,
        color: GOLD,
    };

    const int totalMinutes = (int)(timeEnd - timeStart)/60;
    const int totalSeconds = (int)(timeEnd - timeStart)%60;
    snprintf(textTime, sizeof(textTime), textTimePlayed, totalMinutes, totalSeconds);
    // const char *textTime = TextFormat(textTimePlayed, totalMinutes, totalSeconds);
    const int timeTextX = SCREEN_HALFWIDTH - MeasureText(textTime, FONTSIZE_SUBTITLE)/2;
    gameOverTextParams["gameOverTime"] = {
        text: textTime,
        x: timeTextX,
        y: scoreTextY + 60,
        fontSize: FONTSIZE_SUBTITLE,
        color: DARKGRAY,
    };
    // reset HUD
    livesChanged = false;
    scoreChanged = false;
    hudLivesFrameIdx = 0;
    hudScoreFrameIdx = 0;
}

void Display::RenderGameOver() const {
    const TextureParams panel = panelTextureParams.at("gameOverPanel");
    DrawTexture(panel.texture, panel.x, panel.y, panel.color);

    for(const auto& textParams : gameOverTextParams) {
        const TextParams& entry = textParams.second;
        DrawText(entry.text, entry.x, entry.y, entry.fontSize, entry.color);
    }
}

void Display::Update(int _lives, int _score) {
    livesChanged = _lives != lives;
    scoreChanged = _score != score;

    if(livesChanged || scoreChanged){
        score = _score;
        lives = _lives;
    }

    const int hudAnimationSize = std::size(hudAnimation) - 1;
    if(hudLivesFrameIdx < hudAnimationSize && (livesChanged || hudLivesFrameIdx != 0)) {
        hudLivesFrameIdx++;
    } else {
        hudLivesFrameIdx = 0;
    }

    if(hudScoreFrameIdx < hudAnimationSize && (scoreChanged || hudScoreFrameIdx != 0)) {
        hudScoreFrameIdx++;
    } else {
        hudScoreFrameIdx = 0;
    }
}

void Display::Render() const {
    // Lives
    DrawText(textLives, 28, 32, 18, WHITE);
    const char *livesNum = TextFormat("%d", lives);
    const int livesFontsize = 72 + hudAnimation[hudLivesFrameIdx];
    const Color livesColor = hudLivesFrameIdx != 0 ? RED : WHITE;
    DrawText(livesNum, 28, 55, livesFontsize, livesColor);
    
    // Score
    DrawTexture(fruitIcon, 100, 32, WHITE);
    DrawText("x", 100, 85, 18, WHITE);
    const char *scoreNum = TextFormat("%d", score);
    const int scoreFontsize = 32 + hudAnimation[hudScoreFrameIdx];
    const Color scoreColor = hudScoreFrameIdx != 0 ? GOLD : WHITE;
    DrawText(scoreNum, 115, 75, scoreFontsize, scoreColor);
}
