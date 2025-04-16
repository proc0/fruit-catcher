#pragma once

#include <unordered_map>
#include <string>
#include "lib.h"

#define STARTMENU_OPTIONS_LENGTH 3

typedef struct TextParams {
    const char* text;
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

    public:
        Display();
        ~Display();
        const bool isStartButtonClicked() const;
        const bool isQuitButtonClicked() const;
        void UpdateStartMenu(Vector2 mousePosition);
        void RenderStartMenu() const;
        void Render(int lives, int score) const;
        void UpdateGameOver(int score, float timeEnd, float timeStart);
        void RenderGameOver() const;
};
