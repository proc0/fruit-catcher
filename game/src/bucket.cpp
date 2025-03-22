#include "bucket.hpp"

#define BUCKET_IMAGE_URI "resources/pot.png"
#define BUCKET_SOURCE_WIDTH 150
#define BUCKET_SOURCE_HEIGHT 150
#define BUCKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}

using namespace std;

Bucket::Bucket(){
    texture = LoadTexture(BUCKET_IMAGE_URI);
    Vector2 mousePosition = GetMousePosition();
    collision = { mousePosition.x, SCREEN_HEIGHT - (BUCKET_SOURCE_HEIGHT * 1.25), BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT };
}

Bucket::~Bucket() {
    UnloadTexture(texture);
}

const Rectangle Bucket::GetCollision() const {
    return collision;
}

const Vector2 Bucket::GetPosition(void) const {
    return { collision.x, collision.y };
}

// const tuple<int, int> Bucket::GetDimensions(void) const {
//     return make_tuple(texture.width, texture.height);
// }

void Bucket::Update(Vector2 mousePosition) {
    collision.x = mousePosition.x - texture.width/2;
}

void Bucket::Render(void) const {
    Vector2 position = GetPosition();
    
    // position.x -= texture.width/2;
    // position.y -= texture.height/2;
    
    DrawTextureRec(texture, BUCKET_SOURCE_RECTANGLE, position, WHITE);
    // DrawRectangleRec(collision, GREEN);
}
