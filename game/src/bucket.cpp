#include "bucket.hpp"
#include <iostream>
#include <cstdio>

#define URI_IMAGE_JAR "resources/bucket.png"
#define BUCKET_JAM_TOP_URI "resources/jam-top.png"
#define BUCKET_JAM_MIDDLE_URI "resources/jam-middle.png"
#define BUCKET_JAM_BOTTOM_URI "resources/jam-bottom.png"
#define BUCKET_FRUIT_CATCH_EFFECT "resources/fruit-catch.png"
#define BUCKET_SOUND_FRUIT_CLINK(buf, idx) sprintf(buf, "resources/clink%d.wav", (idx))
#define BUCKET_SOURCE_WIDTH 131
#define BUCKET_SOURCE_HEIGHT 160
#define WIDTH_TEXTURE_BIG_JAR 144
#define HEIGHT_TEXTURE_BIG_JAR 176
#define SIZE_COLLISION_JAR 120
#define SIZE_COLLISION_BIG_JAR 160
#define RECTANGLE_JAR CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}
#define RECTANGLE_BIG_JAR CLITERAL(Rectangle){0, 0, WIDTH_TEXTURE_BIG_JAR, HEIGHT_TEXTURE_BIG_JAR}
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

#define JAM_TOP_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - JAM_TOP_SOURCE_HEIGHT - jamHeight
#define JAM_MIDDLE_MOUSE_POS_Y(jamHeight) JAM_OFFSET_Y - jamHeight
#define JAM_BOTTOM_MOUSE_POS_Y JAM_OFFSET_Y

#define SIZE_SCALE_BIG_JAR 1.1f

void Bucket::Load(){
    Image jarImage = LoadImage(URI_IMAGE_JAR);
    textureJar = LoadTextureFromImage(jarImage);
    Image imageJamTop = LoadImage(BUCKET_JAM_TOP_URI);
    textureJamTop = LoadTextureFromImage(imageJamTop);
    Image imageJamMiddle = LoadImage(BUCKET_JAM_MIDDLE_URI);
    textureJamMiddle = LoadTextureFromImage(imageJamMiddle);
    Image imageJamBottom = LoadImage(BUCKET_JAM_BOTTOM_URI);
    textureJamBottom = LoadTextureFromImage(imageJamBottom);

    ImageResize(&jarImage, jarImage.width*SIZE_SCALE_BIG_JAR, jarImage.height*SIZE_SCALE_BIG_JAR);
    textureBigJar = LoadTextureFromImage(jarImage);
    ImageResize(&imageJamTop, imageJamTop.width*SIZE_SCALE_BIG_JAR, imageJamTop.height*SIZE_SCALE_BIG_JAR);
    textureBigJamTop = LoadTextureFromImage(imageJamTop);
    ImageResize(&imageJamMiddle, imageJamMiddle.width*SIZE_SCALE_BIG_JAR, imageJamMiddle.height*SIZE_SCALE_BIG_JAR);
    textureBigJamMiddle = LoadTextureFromImage(imageJamMiddle);
    ImageResize(&imageJamBottom, imageJamBottom.width*SIZE_SCALE_BIG_JAR, imageJamBottom.height*SIZE_SCALE_BIG_JAR);
    textureBigJamBottom = LoadTextureFromImage(imageJamBottom);

    UnloadImage(jarImage);
    UnloadImage(imageJamTop);
    UnloadImage(imageJamMiddle);
    UnloadImage(imageJamBottom);

    textureCatchEffect = LoadTexture(BUCKET_FRUIT_CATCH_EFFECT);

    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        const int _idx = j + 1;
        char _uri[22];
        BUCKET_SOUND_FRUIT_CLINK(_uri, _idx);
        soundClinks[j] = LoadSound(_uri);
    }

    // only place where collision width/height is set
    collisionJar = { SCREEN_WIDTH/2 - SIZE_COLLISION_JAR/2, BUCKET_POS_Y, SIZE_COLLISION_JAR, SIZE_COLLISION_JAR };
}

void Bucket::Unload() {
    UnloadTexture(textureJar);
    UnloadTexture(textureBigJar);
    UnloadTexture(textureJamTop);
    UnloadTexture(textureBigJamTop);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureBigJamMiddle);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureBigJamBottom);
    UnloadTexture(textureJamBottom);
    UnloadTexture(textureCatchEffect);
    for(int j = 0; j < SOUND_CLINK_LENGTH; j++){
        UnloadSound(soundClinks[j]);
    }
}

void Bucket::Reset(void) {
    jamHeight = 0;
    jarState = SMALL;
    jamColor = WHITE;
}

const Rectangle Bucket::GetCollision(void) const {
    return collisionJar;
}

// old params: const bool bounced, const bool isCatch, const bool isSpike, const Color color
// note: processes single result item, instead of a list of results, works for now
void Bucket::Update(const Vector2 mousePosition, const BucketDisplayResult result) {
    //////////
    float bucketPosX = mousePosition.x - textureJar.width/2;
    if(jarState == BIG){
        bucketPosX = mousePosition.x - textureBigJar.width/2;
    }

    position.x = bucketPosX;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    if(jarState == BIG){
        collisionJar.x = mousePosition.x - SIZE_COLLISION_BIG_JAR/2;
    } else {
        collisionJar.x = mousePosition.x - SIZE_COLLISION_JAR/2;
    }

    int bucketPosY = mousePosition.y;
    if(bucketPosY < BUCKET_POS_Y - 100){
        bucketPosY = BUCKET_POS_Y - 100;
    }

    if(bucketPosY < 0){
        bucketPosY = 0;
    }

    if(result.isSpike){
        isStunned = true;
        jamHeight *= 0.3f;
        jarState = SMALL;
    } else if(result.isBounce){
        const int randomSoundIdx = GetRandomValue(0, SOUND_CLINK_LENGTH-1);
        PlaySound(soundClinks[randomSoundIdx]);
    }

    position.y = bucketPosY;

    int jarPosY = bucketPosY;
    if(jarState == BIG){
        jarPosY += 18;
    } 
    jamTopPosition.y = JAM_TOP_MOUSE_POS_Y(jamHeight) + jarPosY;
    jamMiddlePosition.y = JAM_MIDDLE_MOUSE_POS_Y(jamHeight) + jarPosY;
    jamBottomPosition.y = JAM_BOTTOM_MOUSE_POS_Y + jarPosY;
    collisionJar.y = jarPosY;

    if(result.isCatch){
        isCatching = true;
        catchEffectAnimationIdx++;

        jamColor = ColorLerp(jamColor, result.color, 0.1f);
        jamHeight++;
        if(jamHeight > 80){
            jarState = BIG;
            jamHeight = 0;
            return;
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
    if(jarState == BIG) {
        if(jamHeight > 0){
            DrawTextureRec(textureBigJamTop, JAM_TOP_SOURCE_RECTANGLE, jamTopPosition, jamColor);
            DrawTextureRec(textureBigJamMiddle, JAM_MIDDLE_SOURCE_RECTANGLE(float(jamHeight)), jamMiddlePosition, jamColor);
            DrawTextureRec(textureBigJamBottom, JAM_BOTTOM_SOURCE_RECTANGLE, jamBottomPosition, jamColor);
        }

        DrawTextureRec(textureBigJar, RECTANGLE_BIG_JAR, position, WHITE);
    } else {
        if(jamHeight > 0){
            DrawTextureRec(textureJamTop, JAM_TOP_SOURCE_RECTANGLE, jamTopPosition, jamColor);
            DrawTextureRec(textureJamMiddle, JAM_MIDDLE_SOURCE_RECTANGLE(float(jamHeight)), jamMiddlePosition, jamColor);
            DrawTextureRec(textureJamBottom, JAM_BOTTOM_SOURCE_RECTANGLE, jamBottomPosition, jamColor);
        }

        DrawTextureRec(textureJar, RECTANGLE_JAR, position, WHITE);
    }

    if(isCatching){
        DrawTexturePro(textureCatchEffect, {float(113*catchEffectAnimationIdx), 0, 113, 100 }, { position.x, position.y-55, 113, 100 }, {0, 0}, 0.0f, WHITE);
    }
}

void Bucket::RenderDebug(void) const {
    Render();

    DrawRectangleLinesEx(collisionJar, 2, GREEN);
}
