#include "../include/Player.hpp"

#include <ranges>
#include <raylib-cpp.hpp>
#include <set>

#include "../include/Utils.hpp"

constexpr static auto PLAYER_COLOR = GREEN;
constexpr static auto PLAYER_SIZE = 20.f, PLAYER_SPEED = 5.f;
constexpr static auto PLAYER_HP = 200;
constexpr static auto PLAYER_SHAPE = ShapeType::Rectangle;

Player::Player() noexcept
    : StaticObject(PLAYER_COLOR, PLAYER_SIZE, PLAYER_SHAPE),
      Character(PLAYER_COLOR, PLAYER_SPEED, PLAYER_SIZE, PLAYER_HP, 1, PLAYER_SHAPE) {}

void Player::update() noexcept {
    constexpr static auto DIRECTION = 1.f;
    auto& [hor_move, vert_move] = (velocity = Vector2Zero());

    if (RKeyboard::IsKeyDown(KEY_W)) vert_move -= DIRECTION;
    if (RKeyboard::IsKeyDown(KEY_A)) hor_move -= DIRECTION;
    if (RKeyboard::IsKeyDown(KEY_S)) vert_move += DIRECTION;
    if (RKeyboard::IsKeyDown(KEY_D)) hor_move += DIRECTION;

    Character::update();
    trail_data.emplace_front(get_position());
    if (trail_data.size() > MAX_TRAIL_LENGTH)
        trail_data.pop_back();
}

void Player::render() const noexcept {
    using Position_t = decltype(position);
    constexpr static auto MAX_ALPHA = 255.f;
    constexpr static auto ALPHA_STEP = MAX_ALPHA / MAX_TRAIL_LENGTH;
    constexpr static auto ALPHA_FACTOR = ALPHA_STEP / MAX_ALPHA;

    std::set<Position_t, VectorComparator<Position_t>> drawn_positions;
    for (const auto& [idx, pos] : trail_data | std::views::enumerate | std::views::drop(1)) {
        if (const auto& [it, success] = drawn_positions.insert(pos); !success)
            continue;

        const auto lerp_ratio = idx * ALPHA_FACTOR, alpha = 1.0f - lerp_ratio;
        const auto& lerped_color = RColor{lerp_between_colors(color, DARKGRAY, lerp_ratio)};
        draw_shape<DrawMode::Filled>(shape_type, pos, shape_size, lerped_color.Alpha(alpha));
    }
    Character::render();
}

void Player::upgrade() noexcept {}

float Player::get_speed_multiplier() const noexcept {
    if (RKeyboard::IsKeyDown(KEY_LEFT_CONTROL))
        return 0.5f;
    else if (RKeyboard::IsKeyDown(KEY_LEFT_SHIFT))
        return 1.5f;
    else
        return 1.0f;
}