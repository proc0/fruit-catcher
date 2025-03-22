#pragma once

#include "lib.h"

class Display {
    Texture2D textureStartMenu;
    Rectangle startButtonCollision;
    Rectangle quitButtonCollision;
    enum ButtonState {
        NONE,
        HOVER,
        CLICKED
    };
    ButtonState startButtonState = NONE;
    ButtonState quitButtonState = NONE;

    public:
        Display();
        ~Display();
        void DisplayGameOver(int, int, float, float);
        void DisplayHUD(int, int);
        void UpdateStartMenu(Vector2 mousePosition);
        void DisplayStartMenu();
        bool isStartButtonClicked();
        bool isQuitButtonClicked();
};
