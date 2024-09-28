#include "../include/Utils.hpp"

[[nodiscard]] inline static auto calculate_pulsing_glow_radius(float base_glow_amount) noexcept {
    constexpr static auto PULSE_SPEED = 3.f, PULSE_AMPLITUDE = 5.0f;
    const auto pulse_offset = GetTime() * PULSE_SPEED;
    return base_glow_amount + std::sin(pulse_offset) * PULSE_AMPLITUDE;
}

void draw_glow(const StaticObject* obj, bool pulse) noexcept {
    const auto [center_x, center_y] = obj->get_position();
    const auto color = obj->get_color();
    const auto base_glow_amount = obj->get_size() * 2;
    const auto glow_radius = pulse ? calculate_pulsing_glow_radius(base_glow_amount) : base_glow_amount;

    DrawCircleGradient(center_x, center_y, glow_radius, color, BLANK);
}