#ifndef ANIMATION_H
#define ANIMATION_H

#include "include.h"

struct Frame {
    float time = 0.0f;
    Texture texture;

    Frame(float time, Texture texture) : time(time), texture(texture) {}

    Texture getTexture() const{
        return texture;
    }
};

class Animation {
private:
    float curTime;
    float timer;
    vector<Frame> frames;
    int curIndex;
    // curTime = current time of the animation
    // frames = vector of frames in the animation
    // ------------------------------------------
    // | frame1 | frame2 | frame3 | ... | frameN |
    //              ^
    //              |
    //           curTime => GetFrame() = frame2
    float width, height;
public: 
    Animation();
    Animation(const vector<Frame> &frames, float curTime = 0.0f);
    Animation(const Animation &a);
    ~Animation();
    void AddFrame(float time, Texture texture);
    void AddFrame(const Frame &frame);
    void Update(float deltaTime);
    void setTimer(float timer = 0.0f);
    Texture GetFrame();
    int size();
    int getCurrentIndex();
    float getTimer();
    float getCurrentTime();
    float getFrameTime(int index); // get the time of the frame at index
    float getWidth();
    float getHeight();
    float getCurTime();
};

#endif // ANIMATION_H