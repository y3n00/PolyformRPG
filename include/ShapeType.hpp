#pragma once

#include <raylib-cpp.hpp>

enum class ShapeType : uint16_t {
    Rectangle,
    Circle,
    Triangle,
};

enum class DrawMode : bool {
    Filled,
    Lines
};

union ShapeSize {
    float size;
    struct {
        float width;
        float height;
    };

    constexpr ShapeSize() noexcept : width(0), height(0) {}
    constexpr ShapeSize(float s) noexcept : width(s), height(s) {}
    constexpr ShapeSize(float w, float h) noexcept : width(w), height(h) {}
    [[nodiscard]] inline constexpr operator float() const { return size; }
    [[nodiscard]] inline constexpr operator Vector2() const { return {width, height}; }
};

template <DrawMode mode>
void draw_shape(ShapeType shape, const Vector2& position, ShapeSize size, Color color, float rotation = 0) {
    constexpr static auto filled_mode = (mode == DrawMode::Filled);

    const auto& [x, y] = position;
    const auto& half_size = RVector2(size.operator Vector2()) / 2;
    const auto& [half_w, half_h] = half_size;

    switch (shape) {
        case ShapeType::Rectangle: {
            if (filled_mode || rotation != 0)
                DrawRectanglePro({x, y, size.width, size.height}, half_size, rotation, color);
            else
                DrawRectangleLinesEx({x - half_w, y - half_h, size.width, size.height}, 1.f, color);
            break;
        }
        case ShapeType::Circle: {
            static const auto draw_f = filled_mode ? DrawCircleV : DrawCircleLinesV;
            draw_f(position, half_w, color);
            break;
        }
        case ShapeType::Triangle: {
            Vector2 vertices[] = {
                {x - half_w, y + half_h},
                {x + half_w, y + half_h},
                {x, y - half_h},
            };

            if (rotation != 0) {
                const auto radians = rotation * DEG2RAD;
                const auto cos_angle = cosf(radians), sin_angle = sinf(radians);

                for (auto& [vert_x, vert_y] : vertices) {
                    const auto dx = vert_x - x, dy = vert_y - y;
                    vert_x = x + dx * cos_angle - dy * sin_angle;
                    vert_y = y + dx * sin_angle + dy * cos_angle;
                }
            }

            static const auto draw_f = filled_mode ? DrawTriangle : DrawTriangleLines;
            draw_f(vertices[0], vertices[1], vertices[2], color);
            break;
        }
    }
}