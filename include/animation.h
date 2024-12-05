#ifndef ANIMATION_H
#define ANIMATION_H

#include "include.h"

struct Frame {
    float time = 0.0f;
    Texture texture;

    Frame(float time, Texture texture) : time(time), texture(texture) {}
};

class Animation {
private:
    vector<Frame> frames;
public: 
    Animation();
    Animation(const vector<Frame> &frames);
    Animation(const Animation &a);
    ~Animation();
    void AddFrame(float time, Texture texture);
    void AddFrame(const Frame &frame);
    int size();
};

#endif // ANIMATION_H