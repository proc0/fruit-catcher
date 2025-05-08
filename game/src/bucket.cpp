#include "bucket.hpp"
#include <iostream>
#include <cstdio>

#define BUCKET_IMAGE_URI "resources/bucket.png"
#define BUCKET_JAM_TOP_URI "resources/jam-top.png"
#define BUCKET_JAM_MIDDLE_URI "resources/jam-middle.png"
#define BUCKET_JAM_BOTTOM_URI "resources/jam-bottom.png"
#define BUCKET_FRUIT_CATCH_EFFECT "resources/fruit-catch.png"
#define BUCKET_SOUND_FRUIT_CLINK(buf, idx) sprintf(buf, "resources/clink%d.wav", idx)
#define BUCKET_SOURCE_WIDTH 131
#define BUCKET_SOURCE_HEIGHT 160
#define BUCKET_COLLISION_SIZE 120
#define BUCKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}
#define BUCKET_POS_Y SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT

#define JAM_OFFSET_X 12
#define JAM_OFFSET_Y 120
#define JAM_TOP_SOURCE_WIDTH 105
#define JAM_TOP_SOURCE_HEIGHT 30
#define JAM_MIDDLE_SOURCE_WIDTH 105
#define JAM_MIDDLE_SOURCE_HEIGHT 1
#define JAM_BOTTOM_SOURCE_WIDTH 105
#define JAM_BOTTOM_SOURCE_HEIGHT 30

#define JAM_TOP_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, JAM_TOP_SOURCE_WIDTH, JAM_TOP_SOURCE_HEIGHT}
#define JAM_MIDDLE_SOURCE_RECTANGLE(jamHeight) CLITERAL(Rectangle){0, 0, JAM_MIDDLE_SOURCE_WIDTH, jamHeight}
#define JAM_BOTTOM_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, JAM_BOTTOM_SOURCE_WIDTH, JAM_BOTTOM_SOURCE_HEIGHT}

#define JAM_TOP_POS_Y(jamHeight) BUCKET_POS_Y + JAM_OFFSET_Y - JAM_TOP_SOURCE_HEIGHT - jamHeight
#define JAM_MIDDLE_POS_Y(jamHeight) BUCKET_POS_Y + JAM_OFFSET_Y - jamHeight
#define JAM_BOTTOM_POS_Y BUCKET_POS_Y + JAM_OFFSET_Y

#define JAM_TOP_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - JAM_TOP_SOURCE_HEIGHT - jamHeight
#define JAM_MIDDLE_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - jamHeight
#define JAM_BOTTOM_MOUSE_POS_Y JAM_OFFSET_Y

Bucket::Bucket(){
    texture = LoadTexture(BUCKET_IMAGE_URI);
    textureJamTop = LoadTexture(BUCKET_JAM_TOP_URI);
    textureJamMiddle = LoadTexture(BUCKET_JAM_MIDDLE_URI);
    textureJamBottom = LoadTexture(BUCKET_JAM_BOTTOM_URI);
    textureCatchEffect = LoadTexture(BUCKET_FRUIT_CATCH_EFFECT);


    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        const int _idx = j + 1;
        char _uri[20];
        BUCKET_SOUND_FRUIT_CLINK(_uri, _idx);
        soundClinks[j] = LoadSound(_uri);
    }

    Vector2 mousePosition = GetMousePosition();
    const float bucketPosX = mousePosition.x - texture.width/2;
    const float jamPosX = bucketPosX + JAM_OFFSET_X;
    position = { bucketPosX, BUCKET_POS_Y * 1.18 };
    jamTopPosition = { jamPosX, JAM_TOP_POS_Y(0) };
    jamMiddlePosition = { jamPosX, JAM_MIDDLE_POS_Y(0) };
    jamBottomPosition = { jamPosX, JAM_BOTTOM_POS_Y };
    collision = { mousePosition.x - BUCKET_COLLISION_SIZE/2, BUCKET_POS_Y, BUCKET_COLLISION_SIZE, BUCKET_COLLISION_SIZE };
}

Bucket::~Bucket(void) {
    UnloadTexture(texture);
    UnloadTexture(textureJamTop);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureJamBottom);
    UnloadTexture(textureCatchEffect);
    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        UnloadSound(soundClinks[j]);
    }
}

void Bucket::Reset(void) {
    jamHeight = 0;
}

const Rectangle Bucket::GetCollision(void) const {
    return collision;
}

void Bucket::UpdateOnCatch(const Color color, const int bucketPosY) {
    jamColor = ColorLerp(jamColor, color, 0.1f);
    jamHeight++;
    jamTopPosition.y = JAM_TOP_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamMiddlePosition.y = JAM_MIDDLE_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamBottomPosition.y = JAM_BOTTOM_MOUSE_POS_Y + bucketPosY;
    catchEffectAnimationIdx++;
}

// old params: const bool bounced, const bool isCatch, const bool isSpike, const Color color
// note: processes single result item, instead of a list of results, works for now
void Bucket::Update(const Vector2 mousePosition, const BucketDisplayResult result) {
    const float bucketPosX = mousePosition.x - texture.width/2;
    position.x = bucketPosX;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    collision.x = mousePosition.x - BUCKET_COLLISION_SIZE/2;

    int bucketPosY = mousePosition.y;
    if(bucketPosY < BUCKET_POS_Y - 100){
        bucketPosY = BUCKET_POS_Y - 100;
    }

    if(bucketPosY < 0){
        bucketPosY = 0;
    }

    position.y = bucketPosY;
    jamTopPosition.y = JAM_TOP_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamMiddlePosition.y = JAM_MIDDLE_MOUSE_POS_Y(jamHeight) + bucketPosY;
    jamBottomPosition.y = JAM_BOTTOM_MOUSE_POS_Y + bucketPosY;
    collision.y = bucketPosY;

    if(result.isCatch){
        isCatching = true;
        UpdateOnCatch(result.color, bucketPosY);
    }
    
    if(result.isSpike){
        isStunned = true;
    } else if(result.isBounce){
        const int randomSoundIdx = GetRandomValue(0, SOUND_CLINK_LENGTH-1);
        PlaySound(soundClinks[randomSoundIdx]);
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
        collision.x += displaceX;
        stunLockIdx++;
    } else {
        isStunned = false;
        stunLockIdx = 0;
    }
}

void Bucket::UpdateDebug(void) {
    const float bucketPosX = SCREEN_WIDTH/2 - texture.width/2;
    position.x = bucketPosX;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    collision.x = SCREEN_WIDTH/2 - BUCKET_COLLISION_SIZE/2;
}

void Bucket::Render(void) const {
    if(jamHeight > 0){
        DrawTextureRec(textureJamTop, JAM_TOP_SOURCE_RECTANGLE, jamTopPosition, jamColor);
        DrawTextureRec(textureJamMiddle, JAM_MIDDLE_SOURCE_RECTANGLE(float(jamHeight)), jamMiddlePosition, jamColor);
        DrawTextureRec(textureJamBottom, JAM_BOTTOM_SOURCE_RECTANGLE, jamBottomPosition, jamColor);
    }

    DrawTextureRec(texture, BUCKET_SOURCE_RECTANGLE, position, WHITE);

    if(isCatching){
        DrawTexturePro(textureCatchEffect, {float(113*catchEffectAnimationIdx), 0, 113, 100 }, { position.x, position.y-55, 113, 100 }, {0, 0}, 0.0f, WHITE);
    }
}

void Bucket::RenderDebug(void) const {
    Render();

    DrawRectangleLinesEx(collision, 2, GREEN);
}
