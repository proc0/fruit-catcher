#include "bucket.hpp"

#define BUCKET_IMAGE_URI "resources/bucket.png"
#define BUCKET_SOURCE_WIDTH 131
#define BUCKET_SOURCE_HEIGHT 160
#define BUCKET_COLLISION_SIZE 110
#define BUCKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}

using namespace std;

Bucket::Bucket(){
    texture = LoadTexture(BUCKET_IMAGE_URI);
    Vector2 mousePosition = GetMousePosition();
    position = { mousePosition.x - texture.width/2, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT * 1.18 };
    collision = { mousePosition.x - BUCKET_COLLISION_SIZE/2, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT, BUCKET_COLLISION_SIZE, BUCKET_COLLISION_SIZE };
}

Bucket::~Bucket() {
    UnloadTexture(texture);
}

const Rectangle Bucket::GetCollision() const {
    return collision;
}

void Bucket::Update(Vector2 mousePosition) {
    position.x = mousePosition.x - texture.width/2;
    collision.x = mousePosition.x - BUCKET_COLLISION_SIZE/2;
}

void Bucket::Render(void) const {
    
    DrawTextureRec(texture, BUCKET_SOURCE_RECTANGLE, position, WHITE);
    // DrawRectangleRec(collision, GREEN);
}
