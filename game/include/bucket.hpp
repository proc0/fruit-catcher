#pragma once

#include "lib.h"

#define SOUND_THUMP_LENGTH 5
#define SOUND_CLINK_LENGTH 9

class Bucket {
    Texture2D texture;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Texture2D textureCatchEffect;
    Sound soundThump[SOUND_THUMP_LENGTH];
    Sound soundClinks[SOUND_CLINK_LENGTH];
    Rectangle collision;
    Color jamColor = WHITE;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    int jamHeight = 0;
    int catchEffectAnimationIdx = 0;
    int catchEffectAnimationLength = 40;
    bool isCatching = false;

    void UpdateOnCatch(const Color color);
    
    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Reset();
        void Update(const Vector2 mousePosition, const bool bounced, const bool isCatch, const Color color = WHITE);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
