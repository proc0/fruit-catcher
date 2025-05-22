#include "bucket.hpp"
#include <iostream>
#include <cstdio>

#define URI_IMAGE_JAR "resources/jar.png"
#define BUCKET_JAM_TOP_URI "resources/jam-top-atlas-v2.png"
#define BUCKET_JAM_MIDDLE_URI "resources/jam-middle-v2.png"
#define BUCKET_JAM_BOTTOM_URI "resources/jam-bottom-v2.png"
#define BUCKET_FRUIT_CATCH_EFFECT "resources/fruit-catch.png"
#define SOUND_POP "resources/pop.wav"
#define URI_PROJECTILE "resources/projectile.png"
#define BUCKET_SOUND_FRUIT_CLINK(buf, idx) sprintf(buf, "resources/clink%d.wav", (idx))
#define BUCKET_SOURCE_WIDTH 130
#define BUCKET_SOURCE_HEIGHT 215
#define WIDTH_TEXTURE_BIG_JAR 144
#define HEIGHT_TEXTURE_BIG_JAR 176
#define SIZE_COLLISION_JAR 120
#define SIZE_COLLISION_BIG_JAR 150
#define RECTANGLE_JAR CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}
#define RECTANGLE_BIG_JAR CLITERAL(Rectangle){0, 0, WIDTH_TEXTURE_BIG_JAR, HEIGHT_TEXTURE_BIG_JAR}
#define BUCKET_POS_Y SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT - 100
#define BUCKET_COLLISION_Y BUCKET_POS_Y + 15

#define JAM_OFFSET_X 7
#define JAM_OFFSET_Y 173
#define JAM_TOP_ATLAS_WIDTH 345
#define JAM_TOP_ATLAS_HEIGHT 75
#define JAM_TOP_SOURCE_WIDTH 115
#define JAM_TOP_SOURCE_HEIGHT 74
#define JAM_MIDDLE_SOURCE_WIDTH 115
#define JAM_MIDDLE_SOURCE_HEIGHT 1
#define JAM_BOTTOM_SOURCE_WIDTH 115
#define JAM_BOTTOM_SOURCE_HEIGHT 33

#define JAM_TOP_SOURCE_RECTANGLE(offset) CLITERAL(Rectangle){(float)(offset), 0, JAM_TOP_SOURCE_WIDTH, JAM_TOP_SOURCE_HEIGHT}
#define JAM_MIDDLE_SOURCE_RECTANGLE(jamHeight) CLITERAL(Rectangle){0, 0, JAM_MIDDLE_SOURCE_WIDTH, jamHeight}
#define JAM_BOTTOM_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, JAM_BOTTOM_SOURCE_WIDTH, JAM_BOTTOM_SOURCE_HEIGHT}

#define JAM_TOP_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - JAM_TOP_SOURCE_HEIGHT - jamHeight
#define JAM_MIDDLE_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - jamHeight
#define JAM_BOTTOM_MOUSE_POS_Y JAM_OFFSET_Y

#define SIZE_SCALE_BIG_JAR 1.1f

void Bucket::Load(){
    textureJar = LoadTexture(URI_IMAGE_JAR);
    textureJamTop = LoadTexture(BUCKET_JAM_TOP_URI);
    textureJamMiddle = LoadTexture(BUCKET_JAM_MIDDLE_URI);
    textureJamBottom = LoadTexture(BUCKET_JAM_BOTTOM_URI);
    textureCatchEffect = LoadTexture(BUCKET_FRUIT_CATCH_EFFECT);
    textureProjectile = LoadTexture(URI_PROJECTILE);

    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        const int _idx = j + 1;
        char _uri[22];
        BUCKET_SOUND_FRUIT_CLINK(_uri, _idx);
        soundClinks[j] = LoadSound(_uri);
    }
    soundPop = LoadSound(SOUND_POP);

    // only place where collision width/height is set
    collisionJar = { SCREEN_WIDTH/2 - SIZE_COLLISION_JAR/2, BUCKET_COLLISION_Y, SIZE_COLLISION_JAR, SIZE_COLLISION_JAR+60 };
}

void Bucket::Unload() {
    UnloadTexture(textureJar);
    UnloadTexture(textureJamTop);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureJamBottom);
    UnloadTexture(textureCatchEffect);
    UnloadTexture(textureProjectile);
    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        UnloadSound(soundClinks[j]);
    }
    UnloadSound(soundPop);
}

void Bucket::Reset(void) {
    jamHeight = 0;
    collisionJar.width = SIZE_COLLISION_JAR;
    jamColor = WHITE;
    for( auto &projectile : projectiles ){
        projectile.active = false;
    }
}

const Rectangle Bucket::GetCollision(void) const {
    return collisionJar;
}

const std::vector<Vector2> Bucket::GetProjectiles() const {
    std::vector<Vector2> projectilePos;

    for( auto &projectile : projectiles ){
        if(projectile.active){
            projectilePos.push_back(projectile.position);
        }
    }

    return projectilePos;
}

const int Bucket::GetProjectileNumber() const {
    return (int)jamHeight/5;
}

void Bucket::ProjectileUpdate(){
    for( auto &projectile : projectiles ){
        if(!projectile.active){
            continue;
        }

        if(projectile.position.y < -10){
            projectile.active = false;
            break;
        }
        projectile.position.x += projectile.velocity.x;
        projectile.position.y += projectile.velocity.y + 982.0f * GetFrameTime() * GetFrameTime();
    }
}

void Bucket::ProjectileShoot(){
    for( auto &projectile : projectiles ){
        if(!projectile.active){
            projectile.active = true;
            projectile.position = { position.x + textureJar.width/2, position.y };
            projectile.velocity = { 0, -10.0f };
            PlaySound(soundPop);
            break;
        }
    }
}

// note: processes single result item, instead of a list of results, works for now
void Bucket::Update(const Vector2 mousePosition, const BucketDisplayResult result) {
    float bucketPosX = mousePosition.x - textureJar.width/2;

    position.x = bucketPosX;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    collisionJar.x = mousePosition.x - SIZE_COLLISION_JAR/2;

    int bucketPosY = mousePosition.y;
    int collisionPosY = mousePosition.y;

    if(mousePosition.y < 0){
        bucketPosY = 0;
        collisionPosY = 0;
    }

    if(mousePosition.y < BUCKET_POS_Y){
        bucketPosY = BUCKET_POS_Y;
    }

    if(mousePosition.y < BUCKET_COLLISION_Y){
        collisionPosY = BUCKET_COLLISION_Y;
    }
    
    position.y = bucketPosY;
    jamTopPosition.y = JAM_TOP_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamMiddlePosition.y = JAM_MIDDLE_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamBottomPosition.y = JAM_BOTTOM_MOUSE_POS_Y + bucketPosY;
    collisionJar.y = collisionPosY;

    if(result.isSpike){
        isStunned = true;
        jamHeight *= 0.3f;
        collisionJar.width = SIZE_COLLISION_JAR;
    } else if(result.isBounce){
        const int randomSoundIdx = GetRandomValue(0, SOUND_CLINK_LENGTH-1);
        PlaySound(soundClinks[randomSoundIdx]);
    }

    if(result.isCatch){
        isCatching = true;
        catchEffectAnimationIdx++;

        if(ColorIsEqual(jamColor, WHITE) || jamHeight == 0){ 
            jamColor = result.color;
        } else {
            jamColor = ColorLerp(jamColor, result.color, 0.1f);
        }

        if(jamHeight < 100){
            jamHeight++;
        }
    }

    if(isCatching && catchEffectAnimationIdx < 5 && catchEffectAnimationLength > 0){
        catchEffectAnimationLength--;
        if(catchEffectAnimationLength%8 == 0){
            catchEffectAnimationIdx++;
        }
    } else {
        isCatching = false;
        catchEffectAnimationIdx = 0;
        catchEffectAnimationLength = 40;
    }

    if(isStunned && stunLockIdx < 30){
        const int negate = GetRandomValue(0, 1) ? -1 : 1;
        const float displaceX = stunLock[stunLockIdx] * negate;
        position.x += displaceX;
        jamTopPosition.x += displaceX;
        jamMiddlePosition.x += displaceX;
        jamBottomPosition.x += displaceX;
        collisionJar.x += displaceX;
        stunLockIdx++;
    } else {
        isStunned = false;
        stunLockIdx = 0;
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(jamHeight >= 5){
            jamHeight -= 5;
            ProjectileShoot();
        }

        if(animFrameShotIdx == 0){
            animFrameShotIdx = 1;
        }
    }

    if(animFramesShot[animFrameShotIdx] > 0){
        animFrameShotIdx++;
    } else {
        animFrameShotIdx = 0;
    }
    
    ProjectileUpdate();
}

void Bucket::UpdateDebug(void) {
    const float bucketPosX = SCREEN_WIDTH/2 - textureJar.width/2;
    position.x = bucketPosX;
    position.y = BUCKET_POS_Y;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    collisionJar.x = SCREEN_WIDTH/2 - SIZE_COLLISION_JAR/2;
}

void Bucket::Render(void) const {
    if(jamHeight > 0){
        DrawTextureRec(textureJamTop, JAM_TOP_SOURCE_RECTANGLE(animFramesShot[animFrameShotIdx]*JAM_TOP_SOURCE_WIDTH), jamTopPosition, jamColor);
        DrawTextureRec(textureJamMiddle, JAM_MIDDLE_SOURCE_RECTANGLE(float(jamHeight)), jamMiddlePosition, jamColor);
        DrawTextureRec(textureJamBottom, JAM_BOTTOM_SOURCE_RECTANGLE, jamBottomPosition, jamColor);
    }

    DrawTextureRec(textureJar, RECTANGLE_JAR, position, WHITE);

    if(isCatching){
        DrawTexturePro(textureCatchEffect, {float(113*catchEffectAnimationIdx), 0, 113, 100 }, { position.x, position.y-55, 113, 100 }, {0, 0}, 0.0f, WHITE);
    }

    for( auto &projectile : projectiles){
        if(projectile.active){
            DrawTexture(textureProjectile, projectile.position.x, projectile.position.y, jamColor);
        }
    }
}

void Bucket::RenderDebug(void) const {
    Render();

    DrawRectangleLinesEx(collisionJar, 2, GREEN);
}
