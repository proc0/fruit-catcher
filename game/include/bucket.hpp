#pragma once

#include <tuple>

#include "lib.h"

class Bucket {
    Texture2D texture;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Rectangle collision;
    Color jamColor = PINK;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    int jamHeight = 0;

    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Reset();
        void Update(const Vector2 mousePosition);
        void UpdateJam(const Color color);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
