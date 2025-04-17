#pragma once

#include <unordered_map>
#include <string>
#include "lib.h"

#define STARTMENU_OPTIONS_LENGTH 3

typedef struct TextParams {
    const char *text;
    int x;
    int y;
    int fontSize;
    Color color;
} TextParams;

typedef struct TextureParams {
    Texture2D texture;
    int x;
    int y;
    Color color;
} TextureParams;

class Display {
    char textTime[80];
    char textScore[20];
    const int hudAnimation[15] = {0,1,2,6,10,12,14,18,18,14,10,8,4,1,0};
    Texture2D panelStartMenu;
    Texture2D panelGameOver;
    Texture2D fruitIcon;
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
    std::unordered_map<std::string, TextParams> startMenuTextParams;
    std::unordered_map<std::string, TextParams> gameOverTextParams;
    std::unordered_map<std::string, TextureParams> panelTextureParams;
    int score;
    int lives;
    int hudScoreFrameIdx = 0;
    int hudLivesFrameIdx = 0;
    bool scoreChanged = false;
    bool livesChanged = false;

    public:
        Display();
        ~Display();
        const bool isStartButtonClicked() const;
        const bool isQuitButtonClicked() const;
        void UpdateStartMenu(Vector2 mousePosition);
        void RenderStartMenu() const;
        void Update(int lives, int score);
        void Render() const;
        void UpdateGameOver(int score, float timeEnd, float timeStart);
        void RenderGameOver() const;
};
