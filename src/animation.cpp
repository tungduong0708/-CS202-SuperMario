#include "animation.h"

Animation::Animation() {
    frames = {};
}

Animation::Animation(const vector<Frame> &frames) {
    this->frames = frames;
}

Animation::Animation(const Animation &a) {
    frames = a.frames;
}

Animation::~Animation() {
    frames.clear();
}

void Animation::AddFrame(float time, Texture texture) {
    Frame frame(time, texture);
    frames.push_back(frame);
}

void Animation::AddFrame(const Frame &frame) {
    frames.push_back(frame);
}

int Animation::size() {
    return frames.size();
}