#pragma once

#include <tuple>

#include "lib.h"

class Bucket {
    Texture2D texture;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Rectangle collision;
    Color jamColor;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;

    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Update(const Vector2 mousePosition);
        void UpdateColor(const Color color);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
