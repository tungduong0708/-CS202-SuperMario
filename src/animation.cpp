#include "animation.h"

Animation::Animation() {
    timer = 0.0f;
    curTime = 0.0f;
    curIndex = 0;
    width = 0.0f;
    height = 0.0f;
}


Animation::Animation(const vector<Frame> &frames, float curTime) {
    this->frames = frames;
    this->curTime = curTime;
    timer = curTime;
    curIndex = 0;
    width = frames[0].texture.width;
    height = frames[0].texture.height;
}

Animation::Animation(const Animation &a) {
    timer = a.timer;
    frames = a.frames;
    curTime = a.curTime;
    curIndex = a.curIndex;
    width = a.width;
    height = a.height;
}

Animation::~Animation() {
    frames.clear();
    curTime = 0.0f;
    timer = 0.0f;
}

void Animation::AddFrame(float time, Texture texture) {
    Frame frame(time, texture);
    frames.push_back(frame);
}

void Animation::AddFrame(const Frame &frame) {
    frames.push_back(frame);
}

void Animation::Update(float deltaTime) {
    timer += deltaTime;
    curTime += deltaTime;
    if (frames.size() == 0) {
        return;
    }
    if (curTime >= frames[curIndex].time) {
        curIndex++;
        curIndex %= frames.size();
        curTime = 0.0f;
    }
}

void Animation::setTimer(float timer) {
    this->timer = timer;
}

Texture Animation::GetFrame() {
    if (frames.size() == 0) {
        return {};
    }
    return frames[curIndex].texture;
}

int Animation::size() {
    return frames.size();
}

int Animation::getCurrentIndex() {
    return curIndex;
}

float Animation::getTimer() {
    return timer;
}

float Animation::getCurrentTime() {
    return curTime;
}

float Animation::getFrameTime(int index) {
    return frames[index].time;
}

float Animation::getWidth() {
    return width;
}

float Animation::getHeight() {
    return height;
}