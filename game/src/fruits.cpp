#include <iostream>
#include <sstream>

#include "fruits.hpp"

#define FRUIT_MAX_SIZE 150.0f
#define FRUIT_COLLISION_RADIUS 42.0f
#define FRUIT_TIME_INTERVAL 1.0f
#define GRAVITY 982.0f

Fruits::Fruits(const ConfigData& configData) {
    std::cout << "Loading Fruits ..." << std::endl;
    std::cout << "showCollisions: " << configData.debug.showCollisions << std::endl;
    std::cout << "showFPS: " << configData.debug.displayDebug << std::endl;
    if(configData.debug.displayDebug){
        displayDebug = true;
    }
    if (configData.debug.showCollisions) {
        showCollisions = true;
    }
    for (const auto& pair : fruitSpriteDetails) {
        try {
            int spriteKey = static_cast<int>(pair.first);
            sprites[spriteKey] = LoadTexture(pair.second.uri.c_str());
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            CloseWindow();
        }
    }
    fruitTimeInterval = FRUIT_TIME_INTERVAL;
    Reset();
}

Fruits::~Fruits() {
    for (const auto& pair : fruitSpriteDetails) {
        try {
            int spriteKey = static_cast<int>(pair.first);
            UnloadTexture(sprites[spriteKey]);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void Fruits::Reset(void){
    for(int i=0; i < GAME_FRUITS_MAX; i++){
        RemoveFruit(fruits[i]);
    }
}

void Fruits::RemoveFruit(Fruit &fruit) {
    fruit.active = false;
}

void Fruits::MakeFruit(Fruit &fruit, int index) {
    fruit.active = true;
    fruit.type = (FruitType)index;

    Texture2D fruitSprite = sprites[index];
    const float fruitWidth = (float)fruitSprite.width;
    const float fruitHeight = (float)fruitSprite.height;
    fruit.origin = { fruitWidth/2, fruitHeight/2 };
    fruit.width = fruitWidth;
    fruit.height = fruitHeight;
    
    FruitSprite fruitDetails = fruitSpriteDetails.at(fruit.type);
    const int longSide = fruitWidth > fruitHeight ? fruitWidth : fruitHeight;
    const float ratio = longSide / FRUIT_MAX_SIZE;
    
    fruit.collision = { 
        offset: fruitDetails.offset,
        radius: FRUIT_COLLISION_RADIUS * ratio 
    };

    fruit.mass = ratio;
}

void Fruits::SpawnFruit(Fruit &fruit) {
    const int typeIndex = GetRandomValue(0, fruitSpriteDetails.size()-1);
    MakeFruit(fruit, typeIndex);

    fruit.velocity = { 0.0f, 0.0f };
    fruit.rotation = 0.0f;
    fruit.collided = false;
    fruit.debounce = false;

    const float posX = GetRandomValue(fruit.width, SCREEN_WIDTH - fruit.height);
    fruit.position = { posX, -fruit.height };

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
    
    SpawnFruit(fruits[availableIndex]);
}

void Fruits::UpdateMovementFruit(Fruit &fruit) {
    // Velocity Verlet Integration
    // (i)   x(t+Δt) = x(t) + v(t)Δt + 1/2a(t)Δt^2
    // (ii)  a(t+Δt) = f(x(t+Δt))
    // (iii) v(t+Δt) = v(t) + 1/2(a(t)+a(t+Δt))Δt

    // Δt and Δt * 1/2
    const float deltaTime = GetFrameTime();
    const float halfTime = deltaTime * 0.5f;
    // [acceleration] using F=ma
    const Vector2 acceleration = fruit.force/fruit.mass;
    // (i) [next position] expanding first equation to match order
    // x(t+Δt) = x(t) + v(t) * Δt + a(t) * Δt * (Δt * 1/2)
    fruit.position = fruit.position + fruit.velocity * deltaTime + acceleration * deltaTime * halfTime;
    // (iii) [next velocity] without force change (skips (ii))
    // v(t+Δt) = v(t) + a(t)Δt
    fruit.velocity = fruit.velocity + acceleration * deltaTime;

    // [rotation] basic simple rotation effect
    fruit.rotation += acceleration.x * halfTime;
    
    // on collision
    if(fruit.collided && !fruit.debounce) {
        // reverse direction
        fruit.velocity *= -1;
        fruit.collided = false;
        fruit.debounce = true;
    }

    if(fruit.velocity.y < 0) {
        // add gravity on bounce
        fruit.velocity.y += GRAVITY * deltaTime;
    } else {
        // reset debounce on fall
        fruit.debounce = false;
    }
}

const std::tuple<int, int> Fruits::Update(Bucket &bucket) {
    int lives = 0;
    int score = 0;

    fruitTimeInterval -= GetFrameTime();
    if(fruitTimeInterval <= 0) {
        fruitTimeInterval = FRUIT_TIME_INTERVAL;
        Spawn();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
        Fruit &fruit = fruits[i];

        if(!fruit.active){
            continue;
        }
        // fruit hits bottom
        if(fruit.position.y > SCREEN_HEIGHT) {
            lives--;
            RemoveFruit(fruit);
            continue;
        }
        // fruit hits bucket
        const Rectangle bucketCollision = bucket.GetCollision();
        const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
        if(!fruit.debounce && CheckCollisionCircleRec(fruitCenter, fruit.collision.radius, bucketCollision)) {
            fruit.collided = true;
            // when fruit is above bucket, and fruit is centered with bucket
            if(fruitCenter.y - fruit.collision.radius < bucketCollision.y && fruitCenter.x - fruit.collision.radius > bucketCollision.x && fruitCenter.x + fruit.collision.radius < bucketCollision.x + bucketCollision.width){
                score++;
                RemoveFruit(fruit);
                continue;
            }
        }
        
        UpdateMovementFruit(fruit);
    }

    return std::make_tuple(lives, score);
}

void Fruits::RenderFruit(const Fruit& fruit) const {
    int spriteKey = static_cast<int>(fruit.type);
    Texture2D fruitSprite = sprites[spriteKey];
    DrawTexturePro(fruitSprite, {0, 0, fruit.width, fruit.height}, {fruit.position.x, fruit.position.y, fruit.width, fruit.height}, fruit.origin, fruit.rotation, WHITE);
}

void Fruits::Render(void) const {
    for (const Fruit& fruit : fruits) {
        if(!fruit.active) continue;

        RenderFruit(fruit);
    }
}

void Fruits::UpdateDebug(void){
    int colNum = 0;
    int rowNum = 0;
    for(int i=0; i<GAME_FRUIT_TYPES; i++){
        Fruit& fruit = fruitsDebug[i];

        MakeFruit(fruit, i);
    
        fruit.position.x = 150*colNum + 100;
        fruit.position.y = 150*rowNum + 100;
        if(colNum == 4){
            colNum = 0;
            rowNum++;
        } else {
            colNum++; 
        }
    }
}

void Fruits::RenderDebug(void) const {
    for (const Fruit& fruit : fruitsDebug) {
        RenderFruit(fruit);

        if(showCollisions){
            const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
            DrawCircleV(fruitCenter, fruit.collision.radius, BLUE);
        }

        DrawRectangleLines(fruit.position.x - fruit.origin.x, fruit.position.y - fruit.origin.y, fruit.width, fruit.height, RED);
    }
}
