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
        const bool isStartButtonClicked() const;
        const bool isQuitButtonClicked() const;
        void UpdateStartMenu(Vector2 mousePosition);
        void RenderStartMenu() const;
        void Render(int lives, int score) const;
        void RenderGameOver(int lives, int score, float timeEnd, float timeStart) const;
};
