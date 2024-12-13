#include "physics.h"
#include "renderer.h"
#include "contactlistener.h"

#include <box2d/b2_draw.h>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include <raylib.h>
#include <vector>
#include <iostream>

b2World Physics::world{ b2Vec2(0.0f, 50.0f) };
std::vector<b2Body*> Physics::bodiesToDestroy;
b2DrawRayLib* Physics::debugDraw{ nullptr };

b2DrawRayLib::b2DrawRayLib(float scale) noexcept
    : m_scale { scale }
{}

void b2DrawRayLib::SetAllFlags() noexcept
{
    SetFlags(
        b2Draw::e_shapeBit |
        b2Draw::e_jointBit |
        b2Draw::e_aabbBit |
        b2Draw::e_pairBit |
        b2Draw::e_centerOfMassBit
    );
}

void b2DrawRayLib::SetScale(float scale) noexcept
{
    m_scale = scale;
}

float b2DrawRayLib::GetScale() noexcept
{
    return m_scale;
}

void b2DrawRayLib::DrawPolygon(b2Vec2 const* vertices, int32 vertexCount, b2Color const& color) noexcept
{
    auto const count = static_cast<size_t>(vertexCount);

    for (size_t i = 0; i < count - 1; ++i)
    {
        DrawSegment(vertices[i], vertices[i + 1], color);
    }

    DrawSegment(vertices[count - 1], vertices[0], color);
}

void b2DrawRayLib::DrawSolidPolygon(b2Vec2 const* vertices, int32 vertexCount, b2Color const& color) noexcept
{
    auto const count = static_cast<size_t>(vertexCount);

    auto convertedVertices = std::vector<Vector2>{ count };

    for (size_t i = 0; i < count; ++i)
    {
        convertedVertices[i] = M_ToPixels(vertices[count - i - 1]);
    }

    DrawTriangleFan(convertedVertices.data(), count, M_ConvertColor(color, 0.1f));
}

void b2DrawRayLib::DrawCircle(b2Vec2 const& center, float radius, b2Color const& color) noexcept
{
    auto const convertedCenter = M_ToPixels(center);

    DrawCircleLines(convertedCenter.x, convertedCenter.y, M_ToPixels(radius), M_ConvertColor(color));
}

void b2DrawRayLib::DrawSolidCircle(b2Vec2 const& center, float radius, b2Vec2 const& /* axis */, b2Color const& color) noexcept
{
    DrawCircleV(M_ToPixels(center), M_ToPixels(radius), M_ConvertColor(color, 0.1f));
}

void b2DrawRayLib::DrawSegment(b2Vec2 const& p1, b2Vec2 const& p2, b2Color const& color) noexcept
{
    DrawLineV(M_ToPixels(p1), M_ToPixels(p2), M_ConvertColor(color));
}

void b2DrawRayLib::DrawTransform(b2Transform const& xf) noexcept
{
    auto const axisScale = 0.4f;

    DrawSegment(xf.p, xf.p + axisScale * xf.q.GetXAxis(), b2Color{ 1.0f, 0.0f, 0.0f });
    DrawSegment(xf.p, xf.p + axisScale * xf.q.GetYAxis(), b2Color{ 0.0f, 1.0f, 0.0f });
}

void b2DrawRayLib::DrawPoint(b2Vec2 const& p, float size, b2Color const& color) noexcept
{
    DrawSolidCircle(p, size, {}, color);
}

float b2DrawRayLib::M_ToPixels(float f) const noexcept
{
    return f * m_scale;
}

Vector2 b2DrawRayLib::M_ToPixels(b2Vec2 const& p) const noexcept
{
    return { p.x * m_scale, p.y * m_scale };
}

Color b2DrawRayLib::M_ConvertColor(b2Color const& color) const noexcept 
{
    return M_ConvertColor(color, color.a);
}

Color b2DrawRayLib::M_ConvertColor(b2Color const& color, float newAlpha) const noexcept
{
    return {
        static_cast<unsigned char>(color.r  * 255.0f),
        static_cast<unsigned char>(color.g  * 255.0f),
        static_cast<unsigned char>(color.b  * 255.0f),
        static_cast<unsigned char>(newAlpha * 50.0f)
    };
}

void Physics::Init() {
    // Initialize physics world or any necessary settings
}

void Physics::Update(float deltaTime) {
    world.SetContactListener(new ContactListener());
    world.Step(deltaTime, 6, 2);
    if (!bodiesToDestroy.empty()) {
        for (auto& body : bodiesToDestroy) {
            world.DestroyBody(body);
            body = nullptr;
        }
        bodiesToDestroy.clear();
    }
}

void Physics::DebugDraw() {
    if (!debugDraw) {
        debugDraw = new b2DrawRayLib();
        debugDraw->SetFlags(b2Draw::e_aabbBit);
        world.SetDebugDraw(debugDraw);
    }
    world.DebugDraw();
}
