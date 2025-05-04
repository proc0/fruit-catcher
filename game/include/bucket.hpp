#pragma once

#include "lib.h"

class Bucket {
    Texture2D texture;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Texture2D textureCatchEffect;
    Sound soundSplats[5];
    Sound soundClinks[9];
    Rectangle collision;
    Color jamColor = PINK;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    int jamHeight = 0;
    int catchEffectAnimationIdx = 0;
    int catchEffectAnimationLength = 40;
    bool isCatching = false;

    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Reset();
        void Update(const Vector2 mousePosition, bool collided);
        void UpdateOnCatch(const Color color);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
