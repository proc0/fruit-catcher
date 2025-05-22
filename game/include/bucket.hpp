#pragma once

#include "lib.h"

#define SOUND_CLINK_LENGTH 8
#define MAX_PROJECTILES 20

typedef struct Projectile {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Projectile;


typedef struct BucketDisplayResult {
    const bool isCatch;
    const bool isSpike;
    const bool isBounce;
    const Color color;
} BucketDisplayResult;

class Bucket {
    Texture2D textureJar;
    Texture2D textureJamTop;
    Texture2D textureJamMiddle;
    Texture2D textureJamBottom;
    Texture2D textureCatchEffect;
    Texture2D textureProjectile;
    Rectangle collisionJar;
    Color jamColor = WHITE;
    Vector2 position;
    Vector2 jamTopPosition;
    Vector2 jamMiddlePosition;
    Vector2 jamBottomPosition;
    Sound soundPop;
    Sound soundClinks[SOUND_CLINK_LENGTH];
    Projectile projectiles[MAX_PROJECTILES];
    
    int animFramesShot[30] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,0};
    int animFrameShotIdx = 0;
    int stunLock[30] = {0,1,2,6,10,12,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,14,10,8,4,1,0};
    int stunLockIdx = 0;
    int jamHeight = 0;
    int catchEffectAnimationIdx = 0;
    int catchEffectAnimationLength = 40;
    bool isCatching = false;
    bool isStunned = false;
    
    void ProjectileShoot();
    void ProjectileUpdate();

    public:
        Bucket() = default;
        ~Bucket() = default;
        const Rectangle GetCollision() const;
        const std::vector<Vector2> GetProjectiles() const;
        const int GetProjectileNumber() const;
        void Load();
        void Unload();
        void Reset();
        void Update(const Vector2 mousePosition, const BucketDisplayResult results);
        void UpdateDebug();
        void Render() const;
        void RenderDebug() const;
};
