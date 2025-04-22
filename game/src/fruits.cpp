#include <iostream>
#include <sstream>

#include "fruits.hpp"

#define FRUIT_MAX_SIZE 150.0f
#define FRUIT_COLLISION_RADIUS 42.0f
#define FRUIT_TIME_INTERVAL 1.0f
#define GRAVITY 982.0f

Fruits::Fruits(const ConfigData& configData, const std::array<FruitLevelData, GAME_LEVELS_NUMBER> levelData) {
    SetConfig(configData);
    for (const auto& fruitData : static_FruitDataMap) {
        try {
            const int spriteKey = static_cast<int>(fruitData.first);
            const char* spriteUri = fruitData.second.uri.c_str();
            sprites[spriteKey] = LoadTexture(spriteUri);
        } catch (const std::exception& error) {
            std::cerr << error.what() << '\n';
            CloseWindow();
        }
    }
    SetFruitLevelData(levelData);
    Reset();
}

Fruits::~Fruits() {
    for (const auto& sprite : sprites) {
        UnloadTexture(sprite);
    }
}

void Fruits::SetConfig(const ConfigData& configData) {
    displayDebug = configData.debug.displayDebug;
    showCollisions = configData.debug.showCollisions;
}

void Fruits::SetFruitLevelData(const std::array<FruitLevelData, GAME_LEVELS_NUMBER> levelData) {
    fruitLevelData = levelData;
}

void Fruits::ResetFruit(Fruit &fruit) {
    fruit.active = false;
}

void Fruits::Reset(void){
    for(auto& fruit : fruits){
        ResetFruit(fruit);
    }
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
    
    FruitData fruitData = static_FruitDataMap.at(fruit.type);
    const int longSide = fruitWidth > fruitHeight ? fruitWidth : fruitHeight;
    const float ratio = longSide / FRUIT_MAX_SIZE;
    
    fruit.collision = { 
        offset: fruitData.offset,
        radius: FRUIT_COLLISION_RADIUS * ratio 
    };

    fruit.mass = ratio;
    fruit.collided = false;
    fruit.debounce = false;
}

void Fruits::SpawnFruit(Fruit &fruit) {
    const std::array<FruitType, 10> fruitRatio = fruitLevelData[currentLevel].fruitRatio;
    const int index = GetRandomValue(0, fruitRatio.size()-1);
    const int fruitIndex = static_cast<int>(fruitRatio[index]);
    MakeFruit(fruit, fruitIndex);
    
    const float posX = GetRandomValue(fruit.width, SCREEN_WIDTH - fruit.height);
    fruit.position = { posX, -fruit.height };
    
    float forceX = 0.0f;
    if(fruit.position.x > SCREEN_WIDTH/2) {
        forceX = GetRandomValue(-500, 0);
    } else {
        forceX = GetRandomValue(0, 500);
    }
    fruit.force = { forceX, GRAVITY };

    fruit.velocity = { 0.0f, 0.0f };
    fruit.rotation = 0.0f;
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

    if(fruitTimeInterval <= 0) {
        fruitTimeInterval = FRUIT_TIME_INTERVAL;
        Spawn();
    } else {
        fruitTimeInterval -= GetFrameTime();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
        Fruit &fruit = fruits[i];

        if(!fruit.active){
            continue;
        }
        // fruit hits bottom
        if(fruit.position.y > SCREEN_HEIGHT) {
            lives--;
            ResetFruit(fruit);
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
                ResetFruit(fruit);
                continue;
            }
        }
        
        UpdateMovementFruit(fruit);
    }

    return std::make_tuple(lives, score);
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

void Fruits::RenderFruit(const Fruit& fruit) const {
    int spriteKey = static_cast<int>(fruit.type);
    Texture2D fruitSprite = sprites[spriteKey];
    DrawTexturePro(fruitSprite, {0, 0, fruit.width, fruit.height}, {fruit.position.x, fruit.position.y, fruit.width, fruit.height}, fruit.origin, fruit.rotation, WHITE);
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

void Fruits::Render(void) const {
    for (const Fruit& fruit : fruits) {
        if(!fruit.active) continue;

        RenderFruit(fruit);
    }
}
