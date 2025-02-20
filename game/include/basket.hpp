#pragma once

#include <tuple>
#include "raylib.h"

#include "lib.h"

using namespace std;

class Basket {
    Texture2D _atlasBasket;
    float _x;

    public:
        void Init(void);
        Vector2 GetPosition(void);
        void SetPosition(void);
        tuple<int, int> GetDimensions(void);
        void Update(void);
        void Render(void);
};
