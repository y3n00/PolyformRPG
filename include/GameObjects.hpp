#pragma once

#include <raylib-cpp.hpp>

#include "ShapeType.hpp"

struct StaticObject {
    static StaticObject init(Color color, float size,
                             ShapeType shape_type = ShapeType::Rectangle,
                             RVector2 pos = Vector2Zero());

    inline StaticObject() noexcept = default;
    StaticObject(Color color, float size,
                 ShapeType shape_type = ShapeType::Rectangle,
                 RVector2 pos = Vector2Zero()) noexcept;

    StaticObject(StaticObject&& other) noexcept;
    StaticObject& operator=(StaticObject&& other) noexcept;
    virtual ~StaticObject() noexcept = default;

    inline void set_position(Vector2 new_pos) noexcept { position = new_pos; }
    inline void set_color(Color new_color) noexcept { color = new_color; }
    inline void set_shape_size(float new_size) noexcept { shape_size = new_size; }
    inline void set_shape_type(ShapeType new_shape_type) noexcept { shape_type = new_shape_type; }
    inline void set_interaction_distance(uint32_t new_distance) noexcept { interaction_distance = new_distance; }

    [[nodiscard]] RRectangle get_bounding_box() const noexcept;
    [[nodiscard]] bool check_collision(const StaticObject& another) const noexcept;

    [[nodiscard]] inline auto get_color() const noexcept { return color; }
    [[nodiscard]] inline auto get_position() const noexcept { return position; }
    [[nodiscard]] inline auto get_size() const noexcept { return shape_size; }
    [[nodiscard]] inline auto get_shape_type() const noexcept { return shape_type; }
    [[nodiscard]] inline auto get_interaction_distance() const noexcept { return interaction_distance; }

    inline virtual void update() noexcept {}
    inline virtual void render() const noexcept { draw_shape<DrawMode::Filled>(shape_type, position, shape_size, color); }

   protected:
    ShapeType shape_type;
    uint16_t interaction_distance;
    RColor color;
    ShapeSize shape_size;
    RVector2 position;
};

struct DynamicObject : virtual public StaticObject {
    static DynamicObject init(Color color, float speed, float size,
                              ShapeType shape_type = ShapeType::Rectangle,
                              RVector2 pos = Vector2Zero(),
                              Vector2 vel = Vector2Zero());

    inline DynamicObject() noexcept = default;
    DynamicObject(Color color, float speed, float size,
                  ShapeType shape_type = ShapeType::Rectangle,
                  Vector2 pos = Vector2Zero(),
                  Vector2 vel = Vector2Zero()) noexcept;

    DynamicObject(DynamicObject&& other) noexcept;
    DynamicObject& operator=(DynamicObject&& other) noexcept;
    ~DynamicObject() noexcept override = default;

    inline void set_velocity(Vector2 new_vel) noexcept { velocity = new_vel; }
    inline void set_speed(float new_speed) noexcept { speed = new_speed; }

    [[nodiscard]] inline auto get_velocity() const noexcept { return velocity; }

    virtual void update() noexcept;

   protected:
    double speed;
    RVector2 velocity;
    inline virtual float get_speed_multiplier() const noexcept { return 1.0f; }
};

struct UpgradableObject : virtual public StaticObject {
    static UpgradableObject init(Color color, float size,
                                 ShapeType shape_type = ShapeType::Rectangle,
                                 RVector2 pos = Vector2Zero(),
                                 uint8_t level = 0);

    inline UpgradableObject() noexcept = default;
    UpgradableObject(Color color, float size,
                     ShapeType shape_type = ShapeType::Rectangle,
                     Vector2 pos = Vector2Zero(),
                     uint8_t level = 0) noexcept;

    UpgradableObject(UpgradableObject&& other) noexcept;
    UpgradableObject& operator=(UpgradableObject&& other) noexcept;
    ~UpgradableObject() noexcept override = default;

    inline void set_level(uint8_t new_level) noexcept { current_level = new_level; }
    inline void set_current_exp(uint16_t new_exp) noexcept { current_exp = new_exp; }
    inline void set_max_exp(uint16_t new_max_exp) noexcept { max_exp = new_max_exp; }
    inline void set_upgrade_multiplier(double new_multiplier) noexcept { upgrade_multiplier = new_multiplier; }

    [[nodiscard]] inline auto get_level() const noexcept { return current_level; }
    [[nodiscard]] inline auto get_max_level() const noexcept { return max_level; }
    [[nodiscard]] inline auto get_upgrade_multiplier() const noexcept { return upgrade_multiplier; }

    inline void update() noexcept override {}
    virtual void upgrade() noexcept;

   protected:
    constexpr static uint8_t max_level = 100;
    uint16_t current_exp = 0, max_exp = 100;
    uint32_t current_level = 0;
    double upgrade_multiplier = 1.2f;
};