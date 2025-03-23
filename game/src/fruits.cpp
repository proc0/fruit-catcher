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
    // ball = { 0.0f, 0.0f, 982.0f, 1.0f };
    // ballRect = Rectangle({ 0, 0, 100, 100 });
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
    fruit.position = { float(posX), -FRUIT_ATLAS_HEIGHT };

    fruit.collision = { fruit.position.x, fruit.position.y, FRUIT_ATLAS_WIDTH, FRUIT_ATLAS_HEIGHT };

    fruit.mass = 1.0f;
    fruit.velocity = { 0.0f, 0.0f };
    fruit.collided = false;
    fruit.impactCollision = {0};
    // int velY = GetRandomValue(FRUIT_FALL_SPEED_MIN, FRUIT_FALL_SPEED_MAX);
    // int velX = GetRandomValue(0, 75);

    float forceX = GetRandomValue(0, 500);
    if(fruit.position.x > SCREEN_WIDTH/2) {
        forceX = GetRandomValue(-500, 0);
    }
    // fruit.velocity = { velX, 0.0f };
    fruit.force = { forceX, 982.0f };

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


void Fruits::UpdateMovement(Fruit &fruit) {
    
    if(fruit.impactCollision.x > 0 && !fruit.collided) {
        fruit.velocity.x = -fruit.velocity.x/fruit.impactCollision.width;
        fruit.velocity.y = -fruit.velocity.y/fruit.impactCollision.height;
        fruit.force.x = -fruit.force.x;
        fruit.force.y = -fruit.force.y;
        fruit.collided = true;
    } 

    float deltaTime = GetFrameTime();

    float accelerationX = fruit.force.x/fruit.mass;
    fruit.position.x = fruit.position.x + fruit.velocity.x * deltaTime + accelerationX * deltaTime * deltaTime * 0.5f;
    fruit.collision.x = fruit.position.x;

    float accelerationY = fruit.force.y/fruit.mass;
    fruit.position.y = fruit.position.y + fruit.velocity.y * deltaTime + accelerationY * deltaTime * deltaTime * 0.5f;
    fruit.collision.y = fruit.position.y;

    float nextAccelerationY = fruit.force.y/fruit.mass;
    fruit.velocity.y = fruit.velocity.y + (accelerationY + nextAccelerationY) * deltaTime * 0.5f;

    float nextAccelerationX = fruit.force.x/fruit.mass;
    fruit.velocity.x = fruit.velocity.x + (accelerationX + nextAccelerationX) * deltaTime * 0.5f;
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
            // lives--;
            Remove(fruit);
            continue;
        }

        const Rectangle bucketCollision = bucket.GetCollision();
        if(CheckCollisionRecs(fruit.collision, bucketCollision)) {
            const Rectangle collision = GetCollisionRec(fruit.collision, bucketCollision);

            if(collision.width > bucketCollision.width/4 && collision.height < 20){
                score++;
                Remove(fruit);
                continue;
            }

            fruit.impactCollision = collision;
        } 
        
        UpdateMovement(fruit);
    }

    return make_tuple(lives, score);
}

void Fruits::Render(void) const {
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
            continue;
        }

        DrawTextureRec(atlasFruit, FRUIT_ATLAS_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), fruits[i].position, WHITE);
        // DrawRectangleRec(ballRect, RED);
        //DrawRectangleRec(fruits[i].collision, BLUE);
    }
}
