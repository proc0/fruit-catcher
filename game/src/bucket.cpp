#include "bucket.hpp"

#define BUCKET_IMAGE_URI "resources/bucket.png"
#define BUCKET_JAM_TOP_URI "resources/jam-top.png"
#define BUCKET_JAM_MIDDLE_URI "resources/jam-middle.png"
#define BUCKET_JAM_BOTTOM_URI "resources/jam-bottom.png"
#define BUCKET_SOURCE_WIDTH 131
#define BUCKET_SOURCE_HEIGHT 160
#define BUCKET_COLLISION_SIZE 110
#define BUCKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}
#define BUCKET_POS_Y SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT

#define JAM_OFFSET_X 12
#define JAM_OFFSET_Y 90
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

Bucket::Bucket(){
    texture = LoadTexture(BUCKET_IMAGE_URI);
    textureJamTop = LoadTexture(BUCKET_JAM_TOP_URI);
    textureJamMiddle = LoadTexture(BUCKET_JAM_MIDDLE_URI);
    textureJamBottom = LoadTexture(BUCKET_JAM_BOTTOM_URI);
    
    Vector2 mousePosition = GetMousePosition();
    const float bucketPosX = mousePosition.x - texture.width/2;
    const float jamPosX = bucketPosX + JAM_OFFSET_X;
    position = { bucketPosX, BUCKET_POS_Y * 1.18 };
    jamTopPosition = { jamPosX, JAM_TOP_POS_Y(0) };
    jamMiddlePosition = { jamPosX, JAM_MIDDLE_POS_Y(0) };
    jamBottomPosition = { jamPosX, JAM_BOTTOM_POS_Y };
    collision = { mousePosition.x - BUCKET_COLLISION_SIZE/2, BUCKET_POS_Y, BUCKET_COLLISION_SIZE, BUCKET_COLLISION_SIZE };
}

Bucket::~Bucket() {
    UnloadTexture(texture);
    UnloadTexture(textureJamTop);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureJamBottom);
}

void Bucket::Reset() {
    jamHeight = 0;
}

const Rectangle Bucket::GetCollision() const {
    return collision;
}

void Bucket::UpdateJam(const Color color) {
    jamColor = color;
    jamHeight++;
    jamTopPosition.y = JAM_TOP_POS_Y(jamHeight);
    jamMiddlePosition.y = JAM_MIDDLE_POS_Y(jamHeight);
    jamBottomPosition.y = JAM_BOTTOM_POS_Y;
}

void Bucket::Update(const Vector2 mousePosition) {
    const float bucketPosX = mousePosition.x - texture.width/2;
    position.x = bucketPosX;
    jamTopPosition.x = bucketPosX + JAM_OFFSET_X;
    jamMiddlePosition.x = bucketPosX + JAM_OFFSET_X;
    jamBottomPosition.x = bucketPosX + JAM_OFFSET_X;
    collision.x = mousePosition.x - BUCKET_COLLISION_SIZE/2;
}

void Bucket::UpdateDebug() {
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
}

void Bucket::RenderDebug(void) const {
    Render();

    DrawRectangleLinesEx(collision, 2, GREEN);
}
