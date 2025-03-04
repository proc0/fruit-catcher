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

void Fruits::Add(Fruit &fruit, Vector2 position, int speed) {
    fruit.active = true;
    fruit.position = position;
    fruit.speed = speed;
    
    float xPos = GetRandomValue(0, FRUIT_ATLAS_TYPES-1) * FRUIT_ATLAS_WIDTH;
    float yPos = GetRandomValue(0, FRUIT_ATLAS_TYPES-1) * FRUIT_ATLAS_HEIGHT;
    fruit.atlasXPos = xPos;
    fruit.atlasYPos = yPos;
    fruit.collision = { position.x, position.y, FRUIT_ATLAS_WIDTH/2, FRUIT_ATLAS_HEIGHT/2 };
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
    
    int speed = GetRandomValue(FRUIT_FALL_SPEED_MIN, FRUIT_FALL_SPEED_MAX);
    int posX = GetRandomValue(FRUIT_ATLAS_WIDTH/2, SCREEN_WIDTH - (FRUIT_ATLAS_WIDTH/2));
    
    Add(fruits[availableIndex], { float(posX), -FRUIT_ATLAS_HEIGHT}, speed);
}

tuple<int, int> Fruits::Update(Basket &basket) {
    int lives = 0;
    int score = 0;

    timeNextFruit -= GetFrameTime();
    if(timeNextFruit <= 0) {
        timeNextFruit = FRUIT_TIME_INTERVAL;
        Spawn();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
       if(!fruits[i].active){
            continue;
       }
        
       if(fruits[i].position.y > SCREEN_HEIGHT) {
            lives--;
            Remove(fruits[i]);
            continue;
       }

       Rectangle basketColl = basket.GetCollision();
       if(CheckCollisionRecs(fruits[i].collision, basketColl)) {
            tuple<int, int> basketDimensions = basket.GetDimensions();
            int basketWidth = get<0>(basketDimensions);
            Rectangle collRect = GetCollisionRec(fruits[i].collision, basketColl);
            if(collRect.width > basketWidth/4 && collRect.height < 20){
                score++;
                Remove(fruits[i]);
                continue;
            }
        }

       float fruitY = fruits[i].speed * GetFrameTime();
       fruits[i].position.y += fruitY;
       fruits[i].collision.y += fruitY;
    }

    return make_tuple(lives, score);
}

void Fruits::Render(void) {
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
            continue;
        }

        DrawTextureRec(atlasFruit, FRUIT_ATLAS_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), fruits[i].position, WHITE);
        // DrawRectangleRec(fruits[i].collision, BLUE);
    }
}
