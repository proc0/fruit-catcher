#pragma once

#include "lib.h"

#define SOUND_CLINK_LENGTH 8

typedef struct BucketDisplayResult {
    const bool isCatch;
    const bool isSpike;
    const bool isBounce;
    const Color color;
} BucketDisplayResult;

class Bucket {
    Texture2D textureJar;
    Texture2D textureBigJar;
    Texture2D textureJamTop;
    Texture2D textureBigJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureBigJamMiddle;
    Texture2D textureJamBottom;
    Texture2D textureBigJamBottom;
    Texture2D textureCatchEffect;
    Sound soundClinks[SOUND_CLINK_LENGTH];
    Rectangle collisionJar;
    Color jamColor = WHITE;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    enum JarState {
        SMALL,
        BIG
    };
    JarState jarState = SMALL;
    int stunLock[30] = {0,1,2,6,10,12,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,14,10,8,4,1,0};
    int stunLockIdx = 0;
    int jamHeight = 0;
    int catchEffectAnimationIdx = 0;
    int catchEffectAnimationLength = 40;
    bool isCatching = false;
    bool isStunned = false;
        
    public:
        Bucket() = default;
        ~Bucket() = default;
        const Rectangle GetCollision() const;
        void Load();
        void Unload();
        void Reset();
        void Update(const Vector2 mousePosition, const BucketDisplayResult results);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
