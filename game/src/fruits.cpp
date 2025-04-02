#include "fruits.hpp"

#define FRUIT_ATLAS_URI "resources/atlas_fruits.png"
#define FRUIT_ATLAS_WIDTH 150
#define FRUIT_ATLAS_HEIGHT 150
#define FRUIT_ATLAS_TYPES 4
#define FRUIT_COLLISION_RADIUS 40
#define FRUIT_ATLAS_RECT(xPos, yPos) CLITERAL(Rectangle){xPos, yPos, FRUIT_ATLAS_WIDTH, FRUIT_ATLAS_HEIGHT}

#define FRUIT_TIME_INTERVAL 1.0f
#define GRAVITY 982.0f

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
    fruit.position = { float(posX), -FRUIT_ATLAS_HEIGHT };

    fruit.collision = { fruit.position.x + FRUIT_ATLAS_HEIGHT/2, fruit.position.y + FRUIT_ATLAS_HEIGHT/2 };

    fruit.mass = 1.0f;
    fruit.velocity = { 0.0f, 0.0f };
    fruit.collided = false;
    fruit.debounce = false;
    fruit.rotation = 0.0f;

    float forceX = GetRandomValue(0, 500);
    if(fruit.position.x > SCREEN_WIDTH/2) {
        forceX = GetRandomValue(-500, 0);
    }

    fruit.force = { forceX, GRAVITY };
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
    
    if(fruit.collided && !fruit.debounce) {
        fruit.velocity.x = -fruit.velocity.x;
        fruit.velocity.y = -fruit.velocity.y;
        fruit.collided = false;
    } 

    float deltaTime = GetFrameTime();

    float accelerationX = fruit.force.x/fruit.mass;
    fruit.position.x = fruit.position.x + fruit.velocity.x * deltaTime + accelerationX * deltaTime * deltaTime * 0.5f;
    fruit.collision.x = fruit.position.x;

    float accelerationY = fruit.force.y/fruit.mass;
    fruit.position.y = fruit.position.y + fruit.velocity.y * deltaTime + accelerationY * deltaTime * deltaTime * 0.5f;
    fruit.collision.y = fruit.position.y;

    fruit.velocity.y = fruit.velocity.y + accelerationY * deltaTime;
    fruit.velocity.x = fruit.velocity.x + accelerationX * deltaTime;

    if(fruit.velocity.y < 0) {
        if(!fruit.debounce){
            fruit.debounce = true;
        }
        fruit.velocity.y += GRAVITY * deltaTime;
    }

    fruit.rotation += fruit.force.x * deltaTime;
}

const std::tuple<int, int> Fruits::Update(Bucket &bucket) {
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
        fruit.debounce = fruit.debounce && GetFrameTime() == 0.0f ? false : fruit.debounce;
        if(fruit.collision.y + FRUIT_COLLISION_RADIUS >= bucketCollision.y && CheckCollisionCircleRec(fruit.collision, FRUIT_COLLISION_RADIUS, bucketCollision)) {

            if(fruit.collision.x - FRUIT_COLLISION_RADIUS > bucketCollision.x && fruit.collision.x + FRUIT_COLLISION_RADIUS < bucketCollision.x + bucketCollision.width){
                score++;
                Remove(fruit);
                continue;
            }
            
            fruit.collided = true;
        } 
        
        UpdateMovement(fruit);
    }

    return std::make_tuple(lives, score);
}

void Fruits::Render(void) const {
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
            continue;
        }

        // DrawTextureRec(atlasFruit, FRUIT_ATLAS_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), fruits[i].position, WHITE);
        DrawTexturePro(atlasFruit, FRUIT_ATLAS_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), FRUIT_ATLAS_RECT(fruits[i].position.x, fruits[i].position.y), {75, 75}, fruits[i].rotation, WHITE);
        // DrawCircleV(fruits[i].collision, FRUIT_COLLISION_RADIUS, BLUE);
    }
}
