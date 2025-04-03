#include <iostream>

#include "fruits.hpp"

#define FRUIT_MAX_SIZE 150.0f
#define FRUIT_COLLISION_RADIUS 45.0f
#define FRUIT_TIME_INTERVAL 1.0f
#define GRAVITY 982.0f

Fruits::Fruits() {
    for (const auto& pair : fruit_sprite_uris) {
        try {
            sprites[pair.first] = LoadTexture(pair.second.c_str());
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            CloseWindow();
        }
    }
    timeNextFruit = FRUIT_TIME_INTERVAL;
    Reset();
}

Fruits::~Fruits() {
    for (const auto& pair : fruit_sprite_uris) {
        try {
            UnloadTexture(sprites[pair.first]);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void Fruits::Reset(void){
    for(int i=0; i < GAME_FRUITS_MAX; i++){
        Remove(fruits[i]);
    }
}

void Fruits::Remove(Fruit &fruit) {
    fruit.active = false;
}

void Fruits::Add(Fruit &fruit) {
    fruit.active = true;
    fruit.type = (FruitType)GetRandomValue(0, GAME_FRUIT_TYPES-1);

    Texture2D fruitSprite = sprites[fruit.type];
    const float fruitWidth = (float)fruitSprite.width;
    const float fruitHeight = (float)fruitSprite.height;

    const int posX = GetRandomValue(fruitWidth, SCREEN_WIDTH - fruitHeight);
    fruit.position = { float(posX), -fruitHeight };
    fruit.origin = { fruitWidth/2, fruitHeight/2 };

    const int longSide = fruitWidth > fruitHeight ? fruitWidth : fruitHeight;
    const float ratio = longSide / FRUIT_MAX_SIZE;
    
    fruit.collision = { 
        offset: { 0, 0 },
        radius: FRUIT_COLLISION_RADIUS * ratio 
    };

    fruit.mass = 1.0f + ratio;
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
        fruit.debounce = true;
    } 

    float deltaTime = GetFrameTime();

    float accelerationX = fruit.force.x/fruit.mass;
    fruit.position.x = fruit.position.x + fruit.velocity.x * deltaTime + accelerationX * deltaTime * deltaTime * 0.5f;

    float accelerationY = fruit.force.y/fruit.mass;
    fruit.position.y = fruit.position.y + fruit.velocity.y * deltaTime + accelerationY * deltaTime * deltaTime * 0.5f;

    fruit.velocity.y = fruit.velocity.y + accelerationY * deltaTime;
    fruit.velocity.x = fruit.velocity.x + accelerationX * deltaTime;

    if(fruit.velocity.y < 0) {
        fruit.velocity.y += GRAVITY * deltaTime;
    } else {
        fruit.debounce = false;
    }

    fruit.rotation += accelerationX/2 * deltaTime;
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
        const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
        if(!fruit.debounce && CheckCollisionCircleRec(fruitCenter, fruit.collision.radius, bucketCollision)) {

            if(fruitCenter.y - fruit.collision.radius < bucketCollision.y && fruitCenter.x - fruit.collision.radius > bucketCollision.x && fruitCenter.x + fruit.collision.radius < bucketCollision.x + bucketCollision.width){
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

        Texture2D fruitSprite = sprites[fruits[i].type];
        const float fruitWidth = (float)fruitSprite.width;
        const float fruitHeight = (float)fruitSprite.height;
        DrawTexturePro(fruitSprite, {0, 0, fruitWidth, fruitHeight}, {fruits[i].position.x, fruits[i].position.y, fruitWidth, fruitHeight}, fruits[i].origin, fruits[i].rotation, WHITE);
        
        // const Vector2 fruitCenter = { fruits[i].position.x + fruits[i].collision.offset.x, fruits[i].position.y + fruits[i].collision.offset.y };
        // DrawCircleV(fruitCenter, fruits[i].collision.radius, BLUE);
    }
}
