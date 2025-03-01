#include "fruits.hpp"

#define ATLAS_FRUIT_URI "resources/atlas_fruits.png"
#define ATLAS_FRUIT_WIDTH 150
#define ATLAS_FRUIT_HEIGHT 150
#define ATLAS_FRUIT_TYPES 4
#define ATLAS_FRUIT_RECT(xPos, yPos) CLITERAL(Rectangle){xPos, yPos, ATLAS_FRUIT_WIDTH, ATLAS_FRUIT_HEIGHT}

#define TIME_BETWEEN_APPLES 1.0f
#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

using namespace std;

Fruits::Fruits() {
    atlasFruit = LoadTexture(ATLAS_FRUIT_URI);
    timeNextFruit = TIME_BETWEEN_APPLES;
    for(int i=0; i < GAME_FRUITS_MAX; i++){
        Remove(&fruits[i]);
    }
}

Fruits::~Fruits() {
    UnloadTexture(atlasFruit);
}

void Fruits::Remove(Fruit *fruit) {
    fruit->active = false;
}

void Fruits::Add(Fruit *fruit, Vector2 position, int speed) {
    fruit->active = true;
    fruit->position = position;
    fruit->speed = speed;
    float xPos = GetRandomValue(0, ATLAS_FRUIT_TYPES-1) * ATLAS_FRUIT_WIDTH;
    float yPos = GetRandomValue(0, ATLAS_FRUIT_TYPES-1) * ATLAS_FRUIT_HEIGHT;
    fruit->atlasXPos = xPos;
    fruit->atlasYPos = yPos;
    fruit->collision = { position.x, position.y, ATLAS_FRUIT_WIDTH/2, ATLAS_FRUIT_HEIGHT/2 };
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
    
    int speed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
    int posX = GetRandomValue(ATLAS_FRUIT_WIDTH/2, SCREEN_WIDTH - (ATLAS_FRUIT_WIDTH/2));
    
    Add(&fruits[availableIndex], { float(posX), -ATLAS_FRUIT_HEIGHT}, speed);
}

tuple<int, int> Fruits::Update(Basket &basket) {
    int lives = 0;
    int score = 0;

    timeNextFruit -= GetFrameTime();
    if(timeNextFruit <= 0) {
        timeNextFruit = TIME_BETWEEN_APPLES;
        Spawn();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
       if(!fruits[i].active){
            continue;
       }
        
       if(fruits[i].position.y > SCREEN_HEIGHT) {
            lives--;
            Remove(&fruits[i]);
            continue;
       }

       Rectangle basketColl = basket.GetCollision();
       if(CheckCollisionRecs(fruits[i].collision, basketColl)) {
            tuple<int, int> basketDimensions = basket.GetDimensions();
            int basketWidth = get<0>(basketDimensions);
            Rectangle collRect = GetCollisionRec(fruits[i].collision, basketColl);
            if(collRect.width > basketWidth/4 && collRect.height < 20){
                score++;
                Remove(&fruits[i]);
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

        DrawTextureRec(atlasFruit, ATLAS_FRUIT_RECT(fruits[i].atlasXPos, fruits[i].atlasYPos), fruits[i].position, WHITE);
        // DrawRectangleRec(fruits[i].collision, BLUE);
    }
}
