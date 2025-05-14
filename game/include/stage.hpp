#pragma once

#include "lib.h"

class Stage {
    Texture2D cloudsBackground;
    Texture2D cloudsForeground;
    Texture2D mountains;
    Texture2D sun;
    RenderTexture2D cloudTexture;
    Rectangle cloudsBackgroundRect;
    Rectangle cloudsForegroundRect;
    Rectangle cloudsTextureRect;

    public:
        Stage() = default;
        ~Stage() = default;
        void Load();
        void Unload();
        void Update();
        void Render() const;
};
