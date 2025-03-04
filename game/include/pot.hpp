#pragma once

#include <tuple>

#include "lib.h"

using namespace std;

class Pot {
    Texture2D atlasPot;
    Rectangle collision;
    float x;
    
    public:
        Pot();
        ~Pot();
        Vector2 GetPosition(void);
        Rectangle GetCollision(void);
        tuple<int, int> GetDimensions(void);
        void SetPosition(void);
        void Update(void);
        void Render(void);
};
