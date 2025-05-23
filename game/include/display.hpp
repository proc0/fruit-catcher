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
    const int projectiles;
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
    Texture2D panelStartMenu;
    Texture2D panelGameOver;
    Texture2D fruitIcon;
    Texture2D heartIcon;
    Texture2D projectileIcon;
    Font mainFont;
    Font subFont;
    std::unordered_map<std::string, TextParams> startMenuTextParams;
    std::unordered_map<std::string, TextParams> gameEndTextParams;
    std::unordered_map<std::string, TextParams> gameOverTextParams;
    std::unordered_map<std::string, TextParams> gameWinTextParams;
    std::unordered_map<std::string, TextParams> gamePlayTextParams;
    std::unordered_map<std::string, TextureParams> panelTextureParams;
    const int hudAnimation2[30] = {0,1,2,6,10,12,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,14,10,8,4,1,0};
    const int hudAnimation[15] = {0,1,2,6,10,12,14,18,18,14,10,8,4,1,0};
    char textTime[80];
    char textScore[20];
    Rectangle startButtonCollision;
    Rectangle quitButtonCollision;
    enum ButtonState {
        NONE,
        HOVER,
        CLICKED
    };
    ButtonState startButtonState = NONE;
    ButtonState quitButtonState = NONE;
    ButtonState startButtonLastState = NONE;
    ButtonState quitButtonLastState = NONE;
    FruitDisplayResults fruitDisplayResults;
    int score = 0;
    int level = 0;
    int lives = 0;
    int time = 0;
    int projectiles = 0;
    int hudScoreFrameIdx = 0;
    int hudLivesFrameIdx = 0;
    bool scoreChanged = false;
    bool livesChanged = false;
    bool displayFPS = false;

    void MergeFruitResults(const FruitDisplayResults &results);

    public:
        Display() = default;
        ~Display() = default;
        void Load(const ConfigData& configData);
        void Unload();
        const bool isStartButtonClicked() const;
        const bool isQuitButtonClicked() const;
        void UpdateMenu(Vector2 mousePosition);
        void Update(const DisplayStats stats, const FruitDisplayResults results);
        void Render() const;
        void RenderReady() const;
        void UpdateOnce(int score, float timeEnd, float timeStart);
        void RenderStartMenu() const;
        void RenderOverMenu() const;
        void RenderWinMenu() const;
};
