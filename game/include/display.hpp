#pragma once

#include <unordered_map>

#include "lib.h"

typedef struct TextParams {
    const char *text;
    Color color;
    int fontSize;
    int x;
    int y;
} TextParams;

typedef struct TextureParams {
    Texture2D texture;
    Color color;
    int x;
    int y;
} TextureParams;

typedef struct DisplayStats {
    const int lives;
    const int score;
    const int time;
    const int level;
} DisplayStats;

typedef struct ScorePopup {
    const Vector2 location;
    const int score;
    const bool isCatch;
} ScorePopup;

typedef struct FruitDisplayResult {
    Vector2 location;
    Color color;
    int id;
    int score;
    int lives;
    int bounces;
    int hudAnimationIdx;
    bool isCatch;
    bool bounced;
    bool isSpike;
    bool discard;
} FruitDisplayResult;

typedef std::vector<FruitDisplayResult> FruitDisplayResults;

class Display {
    char textTime[80];
    char textScore[20];
    const int hudAnimation[15] = {0,1,2,6,10,12,14,18,18,14,10,8,4,1,0};
    const int hudAnimation2[30] = {0,1,2,6,10,12,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,14,10,8,4,1,0};
    Texture2D panelStartMenu;
    Texture2D panelGameOver;
    Texture2D fruitIcon;
    Rectangle startButtonCollision;
    Rectangle quitButtonCollision;
    Font mainFont;
    Font subFont;
    enum ButtonState {
        NONE,
        HOVER,
        CLICKED
    };
    ButtonState startButtonState = NONE;
    ButtonState quitButtonState = NONE;
    ButtonState startButtonLastState = NONE;
    ButtonState quitButtonLastState = NONE;
    std::unordered_map<std::string, TextParams> startMenuTextParams;
    std::unordered_map<std::string, TextParams> gameEndTextParams;
    std::unordered_map<std::string, TextParams> gameOverTextParams;
    std::unordered_map<std::string, TextParams> gameWinTextParams;
    std::unordered_map<std::string, TextParams> gamePlayTextParams;
    std::unordered_map<std::string, TextureParams> panelTextureParams;
    FruitDisplayResults fruitDisplayResults;
    Vector2 fruitCenter;
    int score = 0;
    int level = 0;
    int lives = 0;
    int time = 0;
    int fruitScore = 0;
    int hudScoreFrameIdx = 0;
    int hudScoreFrameIdx2 = 0;
    int hudLivesFrameIdx = 0;
    int delayFruitResultsClearFrames = 360;
    bool scoreChanged = false;
    bool livesChanged = false;
    bool displayFPS = false;

    void MergeFruitResults(const FruitDisplayResults &results);

    public:
        Display(const ConfigData& configData);
        ~Display();
        const bool isStartButtonClicked() const;
        const bool isQuitButtonClicked() const;
        void UpdateStartMenu(Vector2 mousePosition);
        void RenderStartMenu() const;
        void Update(const DisplayStats stats, const FruitDisplayResults results);
        void Render() const;
        void RenderReady() const;
        void UpdateOnce(int score, float timeEnd, float timeStart);
        void RenderGameOver() const;
        void RenderWin() const;
};
