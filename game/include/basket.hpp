#pragma once

#include <tuple>

#include "lib.h"

using namespace std;

class Basket {
    Texture2D atlasBasket;
    Rectangle collision;
    float x;
    
    public:
        Basket();
        ~Basket();
        Vector2 GetPosition(void);
        Rectangle GetCollision(void);
        tuple<int, int> GetDimensions(void);
        void SetPosition(void);
        void Update(void);
        void Render(void);
};
