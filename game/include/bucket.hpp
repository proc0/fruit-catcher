#pragma once

#include "lib.h"

#define SOUND_CLINK_LENGTH 9

typedef struct BucketDisplayResult {
    const bool isCatch;
    const bool isSpike;
    const bool isBounce;
    const Color color;
} BucketDisplayResult;

class Bucket {
    Texture2D texture;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Texture2D textureCatchEffect;
    Sound soundClinks[SOUND_CLINK_LENGTH];
    Rectangle collision;
    Color jamColor = WHITE;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    int stunLock[30] = {0,1,2,6,10,12,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,14,10,8,4,1,0};
    int stunLockIdx = 0;
    int jamHeight = 0;
    int catchEffectAnimationIdx = 0;
    int catchEffectAnimationLength = 40;
    bool isCatching = false;
    bool isStunned = false;
    
    void UpdateOnCatch(const Color color, const int bucketPosY);
    
    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Reset();
        void Update(const Vector2 mousePosition, const BucketDisplayResult results);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
