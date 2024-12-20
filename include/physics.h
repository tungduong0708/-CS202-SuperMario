#ifndef PHYSICS_H
#define PHYSICS_H

#include "renderer.h"
#include <vector>
#include <box2d/b2_world.h>
#include <raylib.h>
#include <box2d/b2_draw.h>
#include <box2d/b2_prismatic_joint.h>

class b2DrawRayLib : public b2Draw
{
public:
    explicit b2DrawRayLib(float scale = 1.0f) noexcept;
    void SetAllFlags() noexcept;
    void SetScale(float scale) noexcept;
    float GetScale() noexcept;
    void DrawPolygon(b2Vec2 const* vertices, int32 vertexCount, b2Color const& color) noexcept;
    void DrawSolidPolygon(b2Vec2 const* vertices, int32 vertexCount, b2Color const& color) noexcept;
    void DrawCircle(b2Vec2 const& center, float radius, b2Color const& color) noexcept;
    void DrawSolidCircle(b2Vec2 const& center, float radius, b2Vec2 const& axis, b2Color const& color) noexcept;
    void DrawSegment(b2Vec2 const& p1, b2Vec2 const& p2, b2Color const& color) noexcept;
    void DrawTransform(b2Transform const& xf) noexcept;
    void DrawPoint(b2Vec2 const& p, float size, b2Color const& color) noexcept;

private:
    /// PRIV:
    float M_ToPixels(float f) const noexcept;
    /// PRIV:
    Vector2 M_ToPixels(b2Vec2 const& p) const noexcept;
    /// PRIV:
    Color M_ConvertColor(b2Color const& color) const noexcept;
    /// PRIV:
    Color M_ConvertColor(b2Color const& color, float newAlpha) const noexcept;
    float m_scale;
};

class Physics 
{
public:
    static b2World world;
    static std::vector<b2Body*> bodiesToDestroy;
    static b2DrawRayLib* debugDraw;
    static std::vector<b2Body*> bodiesToDestroyNextFrame;

    static void Init();
    static void Update(float deltaTime);
    static void DebugDraw();
};

#endif // PHYSICS_H