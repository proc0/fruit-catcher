#include "lib.h"

class Stage {
    int frameTracker;
    static const int frameDelay = 8;
    unsigned int frameOffset;
    int frameCurrent;
    int frameCounter;
    Image backgroundImage;
    Texture2D backgroundAnimated;

    public:
        Stage(void);
        ~Stage(void);
        void Update(void);
        void Render(void);
};
