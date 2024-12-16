#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

class MyCamera {
private:
    Camera2D camera; 
    Vector2 mapSize; 
    int screenWidth;
    int screenHeight;
    template <typename T>
    T Clamp(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

public:
    MyCamera() = default;
    MyCamera(float initialZoom, Vector2 cameraTarget, Vector2 mapSize, int screenWidth, int screenHeight);
    void Update(Vector2 targetPosition);
    void SetZoom(float zoom);
    float GetZoom() const;
    Camera2D GetCamera() const;
    Vector2 GetCameraTarget() const;
    ~MyCamera() = default;
};

#endif