#include "camera.h"
#include <iostream>

MyCamera::MyCamera(float initialZoom, Vector2 cameraTarget, Vector2 mapSize, int screenWidth, int screenHeight)
    : mapSize(mapSize), screenWidth(screenWidth), screenHeight(screenHeight) 
{
    float halfScreenWidth = (screenWidth / 2.0f) / initialZoom;
    float halfScreenHeight = (screenHeight / 2.0f) / initialZoom;
    float targetX = Clamp(cameraTarget.x, halfScreenWidth, mapSize.x - halfScreenWidth);
    float targetY = Clamp(cameraTarget.y, halfScreenHeight, mapSize.y - halfScreenHeight);
    camera.target = Vector2{targetX, targetY};                
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f}; 
    camera.rotation = 0.0f;                
    camera.zoom = initialZoom;              
}

void MyCamera::Update(Vector2 targetPosition) {
    float halfScreenWidth = (screenWidth / 2.0f) / camera.zoom;
    float halfScreenHeight = (screenHeight / 2.0f) / camera.zoom;
    float targetX = Clamp(targetPosition.x, halfScreenWidth, mapSize.x - halfScreenWidth);
    float targetY = Clamp(targetPosition.y, halfScreenHeight, mapSize.y - halfScreenHeight);
    
    camera.target.x = targetX;
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

Vector2 MyCamera::GetCameraTarget() const
{
    return camera.target;
}

float MyCamera::GetLeftEdge() const
{
    return camera.target.x - screenWidth / 2.0f / camera.zoom;
}