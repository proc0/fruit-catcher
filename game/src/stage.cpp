#include "stage.hpp"

void Stage::Load() {
    backgroundImage = LoadImageAnim("resources/bg_anim.gif", &frameTracker);
    backgroundAnimated = LoadTextureFromImage(backgroundImage);

    frameOffset = 0; 
    frameCurrent = 0;
    frameCounter = 0;
}

void Stage::Update() {
    frameCounter++;
    if (frameCounter >= frameDelay) {
        frameCurrent++;
        if (frameCurrent >= frameTracker) frameCurrent = 0;

        // Get memory offset position for next frame data in image.data
        frameOffset = backgroundImage.width * backgroundImage.height * 4 * frameCurrent;

        // WARNING: Data size (frame size) and pixel format must match already created texture
        UpdateTexture(backgroundAnimated, ((unsigned char *)backgroundImage.data) + frameOffset);

        frameCounter = 0;
    }
}

void Stage::Render() {
    DrawTexturePro(backgroundAnimated, {0, 0, (float)backgroundImage.width, (float)backgroundImage.height}, {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0, 0}, 0, RAYWHITE);
}

void Stage::Unload() {
    UnloadTexture(backgroundAnimated);
    UnloadImage(backgroundImage);
}
