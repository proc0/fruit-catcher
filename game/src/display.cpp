#include "display.hpp"

#define START_MENU_IMAGE "resources/start_menu_panel.png"
#define PANEL_GAME_OVER "resources/game_over_panel.png"
#define HUD_PANEL "resources/hud_panel.png"
#define FRUIT_ICON_URI "resources/fruit_icon.png"

static const char *textGameOver = TEXT_GAME_OVER;
static const char *textLevel = TEXT_LEVEL;
static const char *textLives = TEXT_LIVES;
static const char *textQuit = TEXT_GAME_QUIT;
static const char *textRestart = TEXT_RESTART;
static const char *textRestartMessage = TEXT_PRESS_R;
static const char *textStart = TEXT_GAME_START;
static const char *textStartMenuTitle = TEXT_START_MENU_TITLE;
static const char *textTimePlayed = TEXT_TIME_PLAYED;
static const char *textTotalScore = TEXT_SCORE;
static const char *textGameWin = TEXT_GAME_WIN;

static const int FONTSIZE_TITLE = 100;
static const int FONTSIZE_SUBTITLE = 20;
static const int FONTSIZE_MENUTEXT = 44;
static const int FONTSIZE_SCORETEXT = 52;
static constexpr int SCREEN_HALFWIDTH = SCREEN_WIDTH*0.5f;
static constexpr int SCREEN_HALFHEIGHT = SCREEN_HEIGHT*0.5f;

Display::Display(const ConfigData& configData) {
    panelStartMenu = LoadTexture(START_MENU_IMAGE);
    panelGameOver = LoadTexture(PANEL_GAME_OVER);
    fruitIcon = LoadTexture(FRUIT_ICON_URI);
    
    mainFont = LoadFontEx("resources/Lacquer-Regular.ttf", FONTSIZE_TITLE, 0, 0);
    subFont = LoadFontEx("resources/Jua-Regular.ttf", FONTSIZE_TITLE, 0, 0);
    SetTextureFilter(mainFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(subFont.texture, TEXTURE_FILTER_BILINEAR);
    // config
    displayFPS = configData.debug.showFPS;

    // panel params
    const int startMenuPanelX = SCREEN_HALFWIDTH - panelStartMenu.width/2;
    const int startMenuPanelY = SCREEN_HALFHEIGHT - panelStartMenu.height/2;
    panelTextureParams["startMenuPanel"] = {
        texture: panelStartMenu,
        color: WHITE,
        x: startMenuPanelX,
        y: startMenuPanelY,
    };

    const int gameOverPanelX = SCREEN_HALFWIDTH - panelGameOver.width/2;
    const int gameOverPanelY = SCREEN_HEIGHT/2 - panelGameOver.height/2;
    panelTextureParams["gameOverPanel"] = {
        texture: panelGameOver,
        color: WHITE,
        x: gameOverPanelX,
        y: gameOverPanelY,
    };

    // Start Menu
    const Color titleColor = { 200, 40, 40, 255 };
    const Vector2 fontTitleXY = MeasureTextEx(mainFont, textStartMenuTitle, FONTSIZE_TITLE, 1.0f);
    const int startMenuTitleX = SCREEN_HALFWIDTH - fontTitleXY.x*0.5f;
    const int startMenuTitleY = SCREEN_HALFHEIGHT - fontTitleXY.y + 10;
    startMenuTextParams["startMenuTitle"] = {
        text: textStartMenuTitle,
        color: titleColor,
        fontSize: FONTSIZE_TITLE,
        x: startMenuTitleX,
        y: startMenuTitleY,
    };

    // Button Collisions - using the same buttons for Start, Pause, and Game Over screens
    const float startButtonX = SCREEN_HALFWIDTH - MeasureText(textStart, FONTSIZE_MENUTEXT)*0.5f;
    const float startButtonY = SCREEN_HALFHEIGHT + 30.0f;
    const float startButtonWidth = MeasureText(textStart, FONTSIZE_MENUTEXT);
    startButtonCollision = { startButtonX, startButtonY, startButtonWidth, FONTSIZE_MENUTEXT };

    startMenuTextParams["startMenuStartButton"] = {
        text: textStart,
        color: BLACK,
        fontSize: FONTSIZE_MENUTEXT,
        x: int(startButtonX),
        y: int(startButtonY),
    };

    const float quitButtonY = SCREEN_HALFHEIGHT + 80.0f;
    const float quitButtonWidth = MeasureText(textQuit, FONTSIZE_MENUTEXT);
    quitButtonCollision = { startButtonX, quitButtonY, quitButtonWidth, FONTSIZE_MENUTEXT };

    startMenuTextParams["startMenuQuitButton"] = {
        text: textQuit,
        color: BLACK,
        fontSize: FONTSIZE_MENUTEXT,
        x: int(startButtonX),
        y: int(quitButtonY),
    };

    // Level Ready
    const char *labelLevel = TextFormat(textLevel, 1);
    const Vector2 textReadyXY = MeasureTextEx(subFont, labelLevel, FONTSIZE_TITLE, 1.0f);
    const int textReadyX = SCREEN_HALFWIDTH - textReadyXY.x*0.5f;
    const int textReadyY = SCREEN_HALFHEIGHT - textReadyXY.y;
    gamePlayTextParams["startLevelReady"] = {
        text: textLevel,
        color: WHITE,
        fontSize: FONTSIZE_TITLE,
        x: textReadyX,
        y: textReadyY,
    };

    // Game End
    const int gameOverTitleX = SCREEN_HALFWIDTH - MeasureText(textGameOver, FONTSIZE_MENUTEXT)*0.5f;
    const int gameOverTitleY = SCREEN_HALFHEIGHT - 100;
    gameOverTextParams["gameOverTitle"] = {
        text: textGameOver,
        color: BLACK,
        fontSize: FONTSIZE_MENUTEXT,
        x: gameOverTitleX,
        y: gameOverTitleY,
    };

    const int restartButtonX = SCREEN_HALFWIDTH - MeasureText(textRestart, FONTSIZE_MENUTEXT)*0.5f;
    gameEndTextParams["gameEndRestartButton"] = {
        text: textRestart,
        color: BLACK,
        fontSize: FONTSIZE_MENUTEXT,
        x: restartButtonX,
        y: int(startButtonY),
    };

    gameEndTextParams["gameEndQuitButton"] = {
        text: textQuit,
        color: BLACK,
        fontSize: FONTSIZE_MENUTEXT,
        x: restartButtonX,
        y: int(quitButtonY),
    };

    const int restartMessageX = SCREEN_HALFWIDTH - MeasureText(textRestartMessage, FONTSIZE_SUBTITLE)*0.5f;
    const int restartMessageY = SCREEN_HEIGHT*0.75f;
    gameEndTextParams["gameEndRestartMessage"] = {
        text: textRestartMessage,
        color: DARKGRAY,
        fontSize: FONTSIZE_SUBTITLE,
        x: restartMessageX,
        y: int(restartMessageY),
    };

    // Game Win
    const Vector2 gameWinXY = MeasureTextEx(mainFont, textGameWin, FONTSIZE_TITLE, 1.0f);
    const int gameWinTitleX = SCREEN_HALFWIDTH - gameWinXY.x*0.5f;
    const int gameWinTitleY = SCREEN_HALFHEIGHT - gameWinXY.y;
    gameWinTextParams["gameWinTitle"] = {
        text: textGameWin,
        color: titleColor,
        fontSize: FONTSIZE_TITLE,
        x: gameWinTitleX,
        y: gameWinTitleY,
    };
}

Display::~Display(void) {
    UnloadTexture(panelStartMenu);
    UnloadTexture(fruitIcon);
    UnloadTexture(panelGameOver);
    UnloadFont(mainFont);
    UnloadFont(subFont);
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
        gameEndTextParams["gameEndRestartButton"].color = RED;
    } else if (startButtonLastState != startButtonState) {
        startMenuTextParams["startMenuStartButton"].color = BLACK;
        gameEndTextParams["gameEndRestartButton"].color = BLACK;
    }

    if(quitButtonState == HOVER && quitButtonLastState != quitButtonState){
        startMenuTextParams["startMenuQuitButton"].color = RED;
        gameEndTextParams["gameEndQuitButton"].color = RED;
    } else if(quitButtonLastState != quitButtonState){
        startMenuTextParams["startMenuQuitButton"].color = BLACK;
        gameEndTextParams["gameEndQuitButton"].color = BLACK;
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
        if(textParams.first == "startMenuTitle"){
            DrawTextEx(mainFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
        } else {
            DrawTextEx(subFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
        }
    }
}

void Display::UpdateOnce(int score, float timeEnd, float timeStart) {
    // Using snprintf because this function is being called once only (in game update)
    // If function is called multiple times in update, change to using Raylib TextFormat
    // const char *textScore = TextFormat(textTotalScore, score);
    snprintf(textScore, sizeof(textScore), textTotalScore, score);
    const int scoreTextX = SCREEN_HALFWIDTH - MeasureText(textScore, FONTSIZE_SCORETEXT)/2;
    const int scoreTextY = SCREEN_HEIGHT*0.22f;
    gameEndTextParams["gameEndScore"] = {
        text: textScore,
        color: GOLD,
        fontSize: FONTSIZE_SCORETEXT,
        x: scoreTextX,
        y: scoreTextY,
    };

    const int totalMinutes = (int)(timeEnd - timeStart)/60;
    const int totalSeconds = (int)(timeEnd - timeStart)%60;
    snprintf(textTime, sizeof(textTime), textTimePlayed, totalMinutes, totalSeconds);
    // const char *textTime = TextFormat(textTimePlayed, totalMinutes, totalSeconds);
    const int timeTextX = SCREEN_HALFWIDTH - MeasureText(textTime, FONTSIZE_SUBTITLE)/2;
    gameEndTextParams["gameEndTime"] = {
        text: textTime,
        color: DARKGRAY,
        fontSize: FONTSIZE_SUBTITLE,
        x: timeTextX,
        y: scoreTextY + 60,
    };
    // reset HUD
    livesChanged = false;
    scoreChanged = false;
    hudLivesFrameIdx = 0;
    hudScoreFrameIdx = 0;
}

void Display::RenderGameOver() const {
    const TextureParams& panel = panelTextureParams.at("gameOverPanel");
    DrawTexture(panel.texture, panel.x, panel.y, panel.color);

    for(const auto& textParams : gameEndTextParams) {
        const TextParams& entry = textParams.second;
        DrawTextEx(subFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
    }

    for(const auto& textParams : gameOverTextParams) {
        const TextParams& entry = textParams.second;
        DrawTextEx(subFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
    }
}

void Display::RenderWin() const {
    for(const auto& textParams : gameEndTextParams) {
        const TextParams& entry = textParams.second;
        DrawTextEx(subFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
    }

    for(const auto& textParams : gameWinTextParams) {
        const TextParams& entry = textParams.second;
        if(textParams.first == "gameWinTitle"){
            DrawTextEx(mainFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
        } else {
            DrawTextEx(subFont, entry.text, { (float)entry.x, (float)entry.y }, entry.fontSize, 1.0f, entry.color);
        }
    }
}

void Display::MergeFruitResults(const FruitDisplayResults &results) {
    if(fruitDisplayResults.size() == 0) {
        fruitDisplayResults = results;
        return;
    }

    if(results.size() == 0){
        // when no results, cleanup fruit cache after animation is done
        const int hudAnimation2Size = std::size(hudAnimation2) - 1;
        for(FruitDisplayResult& result : fruitDisplayResults){
            if(result.hudAnimationIdx >= hudAnimation2Size) {
                result.discard = true;
            }
        }
        return;
    }

    for(const auto &remoteResult : results){
        bool updated = false;
        for(int i=0; i < fruitDisplayResults.size(); i++){
            FruitDisplayResult &localResult = fruitDisplayResults[i];
            // cleanup tagged cache
            if(localResult.discard){
                fruitDisplayResults.erase(fruitDisplayResults.begin() + i);
                continue;
            }
            // copy everything except hudAnimationIdx
            if(localResult.id == remoteResult.id){
                updated = true;
                localResult.bounced = remoteResult.bounced;
                localResult.isCatch = remoteResult.isCatch;
                localResult.isSpike = remoteResult.isSpike;
                localResult.location = remoteResult.location;
                localResult.score = remoteResult.score;
                localResult.lives = remoteResult.lives;
            }
        }

        if(!updated){
            fruitDisplayResults.push_back(remoteResult);
        }
    }
}

void Display::Update(const DisplayStats stats, const FruitDisplayResults results) {
    livesChanged = stats.lives != lives;
    scoreChanged = stats.score != score;
    time = stats.time;
    level = stats.level + 1;

    if(livesChanged || scoreChanged){
        score = stats.score;
        lives = stats.lives;
    }

    MergeFruitResults(results);

    const int hudAnimation2Size = std::size(hudAnimation2) - 1;
    for(FruitDisplayResult& result : fruitDisplayResults){    
        if(!result.isSpike && result.hudAnimationIdx < hudAnimation2Size && (result.isCatch || result.bounced)) {
            result.hudAnimationIdx++;
        }
        
        if(result.hudAnimationIdx >= hudAnimation2Size) {
            result.discard = true;
        }
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

    const char *levelTime = TextFormat("%d", time);
    const static constexpr int levelTimePosY = SCREEN_HALFWIDTH-20;
    DrawText(levelTime, levelTimePosY, 20, 52, WHITE);

    const char *levelNumber = TextFormat(TEXT_LEVEL, level);
    const static constexpr int levelNumberPosY = SCREEN_HALFWIDTH-48;
    DrawText(levelNumber, levelNumberPosY, 80, 32, WHITE);

    if(fruitDisplayResults.size() > 0){
        for(const FruitDisplayResult& result : fruitDisplayResults){    
            // if one of the fruit cache results has an animation in progress
            if(result.hudAnimationIdx > 0 && result.hudAnimationIdx < 29) {
                const char* formatString;
                int formatNumber;
                if(result.bounced){
                    formatString = "x%d";
                    formatNumber = result.bounces;
                } else if(result.isCatch){
                    formatString = "%d";
                    formatNumber = result.score;
                }
                
                const char *popupText = TextFormat(formatString, formatNumber);
                const int popupSize = 24 + hudAnimation2[result.hudAnimationIdx];
                const int popupY = result.location.y - 50;
                const int popupX = result.location.x;
                // text stroke
                DrawText(popupText, popupX, popupY + 2, popupSize, BLACK);
                DrawText(popupText, popupX, popupY - 2, popupSize, BLACK);
                DrawText(popupText, popupX + 2, popupY, popupSize, BLACK);
                DrawText(popupText, popupX - 2, popupY, popupSize, BLACK);
                // popup text, either score or bounce multiplier
                DrawText(popupText, popupX, popupY, popupSize, WHITE);
            }
        }
    }

    if(displayFPS){
        const int fps = GetFPS();
        const char *fpsNum = TextFormat("%d", fps);
        DrawText(fpsNum, 10, 10, 20, BLACK);
    }
}

void Display::RenderReady() const {
    const TextParams &labelReady = gamePlayTextParams.at("startLevelReady");

    const char *labelLevel = TextFormat(labelReady.text, level);
    DrawTextEx(subFont, labelLevel, { (float)labelReady.x, (float)labelReady.y}, labelReady.fontSize, 1.0f, labelReady.color);
}
