#include "../include/GameObjects.hpp"

StaticObject StaticObject::init(Color color, float size, ShapeType shape_type, RVector2 pos) {
    return StaticObject(color, size, shape_type, pos);
}

StaticObject::StaticObject(Color color, float size, ShapeType shape_type, RVector2 pos) noexcept
    : color(color),
      shape_size(size),
      shape_type(shape_type),
      interaction_distance(0),
      position(pos) {}

StaticObject::StaticObject(StaticObject&& other) noexcept
    : color(other.color),
      shape_size(other.shape_size),
      shape_type(other.shape_type),
      interaction_distance(other.interaction_distance),
      position(other.position) {}

StaticObject& StaticObject::operator=(StaticObject&& other) noexcept {
    color = other.color;
    shape_size = other.shape_size;
    shape_type = other.shape_type;
    interaction_distance = other.interaction_distance;
    position = other.position;
    return *this;
}

RRectangle StaticObject::get_bounding_box() const noexcept {
    return RRectangle(
        position.x - shape_size.width / 2,
        position.y - shape_size.height / 2,
        shape_size.width,
        shape_size.height);
}

bool StaticObject::check_collision(const StaticObject& another) const noexcept {
    return get_bounding_box().CheckCollision(another.get_bounding_box());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

DynamicObject DynamicObject::init(Color color, float speed, float size, ShapeType shape_type, RVector2 pos, Vector2 vel) {
    return DynamicObject(color, speed, size, shape_type, pos, vel);
}

DynamicObject::DynamicObject(Color color, float speed, float size, ShapeType shape_type, Vector2 pos, Vector2 vel) noexcept
    : StaticObject(color, size, shape_type, pos),
      speed(speed),
      velocity(vel) {}

DynamicObject::DynamicObject(DynamicObject&& other) noexcept
    : StaticObject(std::move(other)),
      speed(other.speed),
      velocity(other.velocity) {}

DynamicObject& DynamicObject::operator=(DynamicObject&& other) noexcept {
    StaticObject::operator=(std::move(other));
    speed = other.speed;
    velocity = other.velocity;
    return *this;
}

void DynamicObject::update() noexcept {
    velocity = velocity.Normalize().Scale(speed * get_speed_multiplier());
    position += velocity;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
UpgradableObject::UpgradableObject(Color color, float size, ShapeType shape_type, Vector2 pos, uint8_t level) noexcept
    : StaticObject(color, size, shape_type, pos),
      current_level(level),
      current_exp(0),
      max_exp(100),
      upgrade_multiplier(1.2f) {}

UpgradableObject::UpgradableObject(UpgradableObject&& other) noexcept
    : StaticObject(std::move(other)),
      current_level(other.current_level),
      current_exp(other.current_exp),
      max_exp(other.max_exp),
      upgrade_multiplier(other.upgrade_multiplier) {}

UpgradableObject& UpgradableObject::operator=(UpgradableObject&& other) noexcept {
    StaticObject::operator=(std::move(other));
    current_level = other.current_level;
    current_exp = other.current_exp;
    max_exp = other.max_exp;
    upgrade_multiplier = other.upgrade_multiplier;
    return *this;
}

UpgradableObject UpgradableObject::init(Color color, float size, ShapeType shape_type, RVector2 pos, uint8_t level) {
    return UpgradableObject(color, size, shape_type, pos, level);
}

void UpgradableObject::upgrade() noexcept {
    if (const auto exp_diff = current_exp - max_exp; exp_diff > 0) {
        current_exp = exp_diff;
        upgrade();
    }
}