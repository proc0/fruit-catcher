#include "fruits.hpp"

#define FRUIT_ATLAS_URI "resources/atlas_fruits.png"
#define FRUIT_ATLAS_WIDTH 150
#define FRUIT_ATLAS_HEIGHT 150
#define FRUIT_ATLAS_TYPES 4
#define FRUIT_ATLAS_RECT(xPos, yPos) CLITERAL(Rectangle){xPos, yPos, FRUIT_ATLAS_WIDTH, FRUIT_ATLAS_HEIGHT}

#define FRUIT_TIME_INTERVAL 1.0f
#define FRUIT_FALL_SPEED_MIN 150
#define FRUIT_FALL_SPEED_MAX 250

using namespace std;

Fruits::Fruits() {
    atlasFruit = LoadTexture(FRUIT_ATLAS_URI);
    timeNextFruit = FRUIT_TIME_INTERVAL;
    for(int i=0; i < GAME_FRUITS_MAX; i++){
        Remove(fruits[i]);
    }
}

Fruits::~Fruits() {
    UnloadTexture(atlasFruit);
}

void Fruits::Remove(Fruit &fruit) {
    fruit.active = false;
}

void Fruits::Add(Fruit &fruit) {
    fruit.active = true;
        
    fruit.atlasXPos = GetRandomValue(0, FRUIT_ATLAS_TYPES-1) * FRUIT_ATLAS_WIDTH;
    fruit.atlasYPos = GetRandomValue(0, FRUIT_ATLAS_TYPES-1) * FRUIT_ATLAS_HEIGHT;

    int posX = GetRandomValue(FRUIT_ATLAS_WIDTH, SCREEN_WIDTH - FRUIT_ATLAS_WIDTH);
    fruit.position = fruit.lastPos = { float(posX), -FRUIT_ATLAS_HEIGHT };

    fruit.collision = { fruit.position.x, fruit.position.y, FRUIT_ATLAS_WIDTH/2, FRUIT_ATLAS_HEIGHT/2 };

    fruit.acceleration = { 0.0f, 0.0f };
    fruit.force = { -2.0f, 982.0f };
    // int velY = GetRandomValue(FRUIT_FALL_SPEED_MIN, FRUIT_FALL_SPEED_MAX);
    // int velX = GetRandomValue(0, 75);

    float velX = GetRandomValue(0, 500);
    if(fruit.position.x > SCREEN_WIDTH/2) {
        velX = GetRandomValue(-500, 0);
    }
    fruit.velocity = { velX, 0.0f };
}

void Fruits::Spawn(void) {
    int availableIndex = -1;
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
           availableIndex = i;
           break;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }
    
    Add(fruits[availableIndex]);
}

void Fruits::UpdateMovement(Fruit &fruit, Rectangle collision) {
    float deltaTime = GetFrameTime();
    float deltaHalf = deltaTime * 0.5f;
    float deltaSquare = deltaTime * deltaHalf;
    // calculate next position using Velocity Verlet integration
    fruit.position = fruit.position + fruit.velocity * deltaTime + fruit.acceleration * deltaSquare;
    // copy to collision position
    fruit.collision.x = fruit.position.x;
    fruit.collision.y = fruit.position.y;
    // calculate new velocity by applying forces
    // Vector2 force = { -2.0f, 982.0f };
    if(collision.x) {
        fruit.force.x -= collision.width*15.0f;
        fruit.force.y -= collision.height*20.0f;
    } else {
        fruit.force.x = 2.0f;
        fruit.force.y = 982.0f;
    }
    fruit.velocity = fruit.velocity + (fruit.acceleration + fruit.force) * deltaHalf;
    // set last acceleration and last position
    fruit.acceleration = fruit.force;
    fruit.lastPos = fruit.position;
}

const tuple<int, int> Fruits::Update(Bucket &bucket) {
    int lives = 0;
    int score = 0;

    timeNextFruit -= GetFrameTime();
    if(timeNextFruit <= 0) {
        timeNextFruit = FRUIT_TIME_INTERVAL;
        Spawn();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
        Fruit &fruit = fruits[i];

        if(!fruit.active){
            continue;
        }
            
        if(fruit.position.y > SCREEN_HEIGHT) {
            lives--;
            Remove(fruit);
            continue;
        }

        const Rectangle bucketCollision = bucket.GetCollision();
        if(CheckCollisionRecs(fruit.collision, bucketCollision)) {
            const Rectangle collision = GetCollisionRec(fruit.collision, bucketCollision);
            // TODO: calculate if force is from left or right by comparing collision.x to bucketWidth or collision.width??
            // then pass that into UpdateMovement, and make a struct with isColliding, bool fromLeft, and collision info
            // tuple<int, int> bucketDimensions = bucket.GetDimensions();
            // int bucketWidth = get<0>(bucketDimensions);

            if(collision.width > bucketCollision.width/4 && collision.height < 20){
                score++;
                Remove(fruit);
                continue;
            } else {
                UpdateMovement(fruit, collision);
            }
        } else {
            UpdateMovement(fruit, {});
        }
    }

    return make_tuple(lives, score);
}

void Fruits::Render(void) const {
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
            continue;
        }

        DrawTextureRec(atlasFruit, FRUIT_ATLAS_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), fruits[i].position, WHITE);
        // DrawRectangleRec(fruits[i].collision, BLUE);
    }
}
