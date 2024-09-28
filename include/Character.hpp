#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "GameObjects.hpp"
#include "Items.hpp"
#include "ShapeType.hpp"

class Character : public UpgradableObject, public DynamicObject {
   public:
    Character(Color color, float speed, float size, uint16_t max_health, int level, ShapeType shape_type) noexcept;

    void render() const noexcept override;
    void update() noexcept override;
    void upgrade() noexcept override {}

    void draw_healthbar() const noexcept;
    void draw_move_direction(const Color& draw_color = RAYWHITE) const noexcept;

    [[nodiscard]] inline auto is_alive() const noexcept { return health.current > 0; }
    [[nodiscard]] inline auto get_view_radius() const noexcept { return in_world.view_radius; }

    inline void take_damage(int32_t damage) noexcept {
        constexpr static auto clamp = std::clamp<decltype(health.max)>;
        health.current = clamp(health.current - damage, 0, health.max);
    }
    inline void set_max_health(int new_max_value) noexcept { health.max = new_max_value; }

    [[nodiscard]] inline virtual uint16_t get_crit_chance() const noexcept { return attack.crit_chance; }
    [[nodiscard]] inline virtual float get_crit_multiplier() const noexcept { return attack.crit_multiplier / 10.f; }

    virtual ~Character() noexcept override = default;

    friend static auto attack(Character& attacker, Character& attacked) noexcept;

   protected:
    struct A {
        uint16_t damage = 1, delay_sec = 2;
        uint16_t crit_multiplier = 20, crit_chance = 10;
        double prev_time = 0;
    } attack;
    struct H {
        uint16_t max, current;
    } health;
    struct IW {
        uint16_t view_radius = 50, rotation = 0;
    } in_world;
    std::vector<Item> items;

    mutable struct HB {
        float display_health = -1, text_size = 0;
        Vector2 bar_parameters{};
        bool is_initialized() const noexcept { return display_health >= 0; }
    } health_bar;
};