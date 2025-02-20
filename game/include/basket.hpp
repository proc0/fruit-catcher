#pragma once

#include "raylib.h"

#define BASKET_SOURCE_WIDTH 100
#define BASKET_SOURCE_HEIGHT 100
#define BASKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT}

class Basket {
    Texture2D _atlasBasket;
    float _x;

    public:
        void Init(void);
        Vector2 GetPosition(void);
        void SetPosition(void);
        void Update(void);
        void Render(void);
};
