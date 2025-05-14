#pragma once

#include "lib.h"

class Stage {
    Texture2D cloudsBackground;
    Texture2D cloudsForeground;
    Texture2D mountains;
    Texture2D sun;
    Rectangle cloudsBackgroundRect;
    Rectangle cloudsForegroundRect;

    public:
        Stage() = default;
        ~Stage() = default;
        void Load();
        void Unload();
        void Update();
        void Render() const;
};
