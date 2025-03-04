#include "lib.h"

class Stage {
    Texture2D backgroundAnimated;
    Image backgroundImage;
    int frameTracker;
    static const int frameDelay = 8;
    unsigned int frameOffset;
    int frameCurrent;
    int frameCounter;

    public:
        Stage(void);
        ~Stage(void);
        void Update(void);
        void Render(void);
};
