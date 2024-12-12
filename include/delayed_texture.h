#ifndef DELAYED_TEXTURE_H
#define DELAYED_TEXTURE_H

#include <raylib.h>
#include <string>
#include <vector>

class DelayedTexture {
private:
    Texture texture;
    float timer;
    float span;
    bool flag; // indicate that the texture is active or not
    Vector2 position;
public:
    DelayedTexture();
    DelayedTexture(Texture texture, float span, Vector2 position);
    DelayedTexture(const DelayedTexture &dt);
    ~DelayedTexture();
    
    void setPosition(Vector2 position);
    void setTexture(Texture texture);
    void setSpan(float span);
    void setFlag(bool flag);
    Texture getTexture();
    float getSpan();
    bool isActive();
    Vector2 getPosition();

    void Update(float deltaTime);
    void Draw();
};

#endif