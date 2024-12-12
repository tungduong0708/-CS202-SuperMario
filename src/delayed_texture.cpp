#include "delayed_texture.h"
#include "renderer.h"
#include <iostream>

DelayedTexture::DelayedTexture() {
    texture = LoadTexture("resources/images/object/active.png");
    timer = 0.0f;
    span = 0.0f;
    flag = false;
    position = {0, 0};
}

DelayedTexture::DelayedTexture(Texture texture, float span, Vector2 position): 
    texture(texture), 
    span(span), 
    position(position) 
{
    timer = 0.0f;
    flag = false;
}

DelayedTexture::DelayedTexture(const DelayedTexture &dt): 
    texture(dt.texture), 
    span(dt.span), 
    timer(dt.timer), 
    flag(dt.flag) 
{}

DelayedTexture::~DelayedTexture() {
    UnloadTexture(texture);
    timer = 0.0f;
    span = 0.0f;
    flag = false;
}

void DelayedTexture::setPosition(Vector2 position) {
    this->position = position;
}

void DelayedTexture::setTexture(Texture texture) {
    this->texture = texture;
}

void DelayedTexture::setSpan(float span) {
    this->span = span;
}

void DelayedTexture::setFlag(bool flag) {
    this->flag = flag;
}

Texture DelayedTexture::getTexture() {
    return texture;
}

float DelayedTexture::getSpan() {
    return span;
}

bool DelayedTexture::isActive() {
    return flag;
}

Vector2 DelayedTexture::getPosition() {
    return position;
}

void DelayedTexture::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= span) {
        flag = true;
    }
}

void DelayedTexture::Draw() {
    //std::cout << texture.id << std::endl;
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    Renderer::DrawPro(texture, sourceRect, position, Vector2{1, 1}, false, 0.0f);
}