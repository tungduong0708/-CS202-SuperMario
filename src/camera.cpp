#include "camera.h"
#include <iostream>

MyCamera::MyCamera(float initialZoom, Vector2 mapSize, int screenWidth, int screenHeight)
    : mapSize(mapSize), screenWidth(screenWidth), screenHeight(screenHeight) {
    camera.target = {0.0f, 0.0f};                
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f}; 
    camera.rotation = 0.0f;                
    camera.zoom = initialZoom;              
}

void MyCamera::Update(Vector2 targetPosition) {
    float halfScreenWidth = (screenWidth / 2.0f) / camera.zoom;
    float halfScreenHeight = (screenHeight / 2.0f) / camera.zoom;
    
    camera.target.x = Clamp(targetPosition.x, halfScreenWidth, mapSize.x - halfScreenWidth);
    camera.target.y = Clamp(targetPosition.y, halfScreenHeight, mapSize.y - halfScreenHeight);
}

void MyCamera::SetZoom(float zoom) {
    // camera.zoom = Clamp(zoom, 0.25f, 3.0f); 
    camera.zoom = zoom;
}

float MyCamera::GetZoom() const {
    return camera.zoom;
}

Camera2D MyCamera::GetCamera() const {
    return camera;
}

