#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <ranges>
#include <raylib-cpp.hpp>
#include <type_traits>

#include "GameObjects.hpp"

template <typename VectorN>
struct VectorComparator {
    [[nodiscard]] inline constexpr auto operator()(const VectorN& lhs, const VectorN& rhs) const {
        using Internal_t = float;
        constexpr static auto size = sizeof(VectorN) / sizeof(Internal_t);
        using arr_ptr = const std::array<Internal_t, size>*;

        const auto &vec_1 = *(arr_ptr)(&lhs), &vec_2 = *(arr_ptr)(&rhs);
        for (int i = 0; i < size; i++) {
            if (vec_1[i] < vec_2[i]) return true;
            if (vec_1[i] > vec_2[i]) return false;
        }
        return false;
    }
};

void draw_glow(const StaticObject* ent, bool pulse = false) noexcept;

template <typename T>
    requires std::is_arithmetic_v<T>
[[nodiscard]] inline constexpr static T lerp_wrapper(T from, T to, float lerp_ratio) noexcept {
    return static_cast<T>(std::lerp(from, to, lerp_ratio));
}

[[nodiscard]] inline constexpr static auto lerp_between_colors(const Color& from_color, const Color& to_color, float lerp_ratio) noexcept {
    constexpr auto color_channel_lerp = lerp_wrapper<uint8_t>;
    return Color{
        color_channel_lerp(from_color.r, to_color.r, lerp_ratio),
        color_channel_lerp(from_color.g, to_color.g, lerp_ratio),
        color_channel_lerp(from_color.b, to_color.b, lerp_ratio),
        color_channel_lerp(from_color.a, to_color.a, lerp_ratio),
    };
}

inline constexpr static auto log_change(std::string_view name, auto& prev, const std::ranges::sized_range auto& objects) noexcept {
    if (const auto sz = objects.size(); sz != prev) {
        const auto fmt_msg = std::format("{} changed from {} to {}", name, prev, sz);
        TraceLog(TraceLogLevel::LOG_INFO, fmt_msg.c_str());
        prev = sz;
    }
}

[[nodiscard]] inline static auto velocity_to_degrees(const Vector2& velocity) noexcept {
    const auto direction_in_radians = atan2f(velocity.y, velocity.x);
    const auto direction_in_degrees = direction_in_radians * RAD2DEG;
    return std::fmod(direction_in_degrees + 360.0f, 360.0f);
}

[[nodiscard]] inline static auto rotate_towards(float current_rotation, float target_rotation, float max_rotation_speed) noexcept {
    auto rotation_difference = target_rotation - current_rotation;
    if (rotation_difference > 180.0f)
        rotation_difference -= 360.0f;
    else if (rotation_difference < -180.0f)
        rotation_difference += 360.0f;

    if (std::abs(rotation_difference) > max_rotation_speed)
        return current_rotation + (rotation_difference > 0 ? max_rotation_speed : -max_rotation_speed);
    else
        return target_rotation;
}