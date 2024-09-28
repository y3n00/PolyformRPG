#include "../include/Character.hpp"

#include "../include/Game.hpp"
#include "../include/GameObjects.hpp"
#include "../include/ResourceManager.hpp"
#include "../include/Utils.hpp"
#include "../include/other/random.hpp"

Character::Character(Color color, float speed, float size, uint16_t max_health, int level, ShapeType shape_type) noexcept
    : StaticObject(color, size, shape_type, Vector2Zero()),
      UpgradableObject(color, size, shape_type, Vector2Zero(), level),
      DynamicObject(color, speed, size, shape_type, Vector2Zero(), Vector2Zero()),
      health{max_health, max_health} {
    items.reserve(25);
}

void Character::render() const noexcept {
    const auto& armor_color = RAYWHITE;  // inventory.get_slot(ItemType::ARMOR)->get_color();
    draw_glow(this);
    StaticObject::render();
    draw_shape<DrawMode::Lines>(shape_type, position, shape_size, armor_color);
    draw_healthbar();

    if (is_alive()) [[likely]] {
        draw_move_direction();
    }
}

void Character::update() noexcept {
    if (!is_alive()) [[unlikely]] {
        constexpr static auto DEAD_PLAYER_COLOR = Color(0x25, 0x25, 0x25, 0x25);
        color = lerp_between_colors(color, DEAD_PLAYER_COLOR, 0.02f);
        return;
    }

    DynamicObject::update();
    if (velocity.Length() > 0) {
        constexpr static auto ROTATION_SPEED = 15;
        const auto new_rotation = velocity_to_degrees(velocity);
        in_world.rotation = rotate_towards(in_world.rotation, new_rotation, ROTATION_SPEED);
    }
    // inventory.update();
}

void Character::draw_healthbar() const noexcept {
    enum { SMALL,
           BIG };

    constexpr static RVector2 BAR_SIZE[]{{2, 4}, {3, 15}};
    constexpr static float INTERPOLATION_SPEED = 5.0f;
    constexpr static float TEXT_SIZE[]{2.0f, 10.f};

    static const auto& states = Game::get_states();
    static const auto& font = ResourceManager::get<Font>("main_font");

    if (!health_bar.is_initialized()) [[unlikely]] {
        health_bar.bar_parameters = BAR_SIZE[SMALL];
        health_bar.text_size = TEXT_SIZE[SMALL];
        health_bar.display_health = health.current;
    }

    const auto expand = states.value_at("expand_info");
    const auto& bar_size_to = BAR_SIZE[expand];
    const auto& text_size_to = TEXT_SIZE[expand];
    const auto interp_ratio = GetFrameTime() * INTERPOLATION_SPEED;

    health_bar.bar_parameters = Vector2Lerp(health_bar.bar_parameters, bar_size_to, interp_ratio);
    health_bar.text_size = lerp_wrapper<float>(health_bar.text_size, text_size_to, interp_ratio);
    health_bar.display_health = lerp_wrapper<float>(health_bar.display_health, health.current, interp_ratio);

    const auto healthbar_color = [this, health_ratio = health_bar.display_health / health.max]() -> Color {
        if (health_ratio >= 0.8f) return GREEN;
        if (health_ratio >= 0.5f) return lerp_between_colors(YELLOW, GREEN, (health_ratio - 0.5f) / 0.3f);
        if (health_ratio >= 0.2f) return lerp_between_colors(RED, YELLOW, (health_ratio - 0.2f) / 0.3f);
        return RED;
    }();

    const auto healthbar_size = RVector2(shape_size * health_bar.bar_parameters.x, health_bar.bar_parameters.y);
    const auto current_health_size = Vector2(healthbar_size.x * (health_bar.display_health / health.max), healthbar_size.y);

    const auto healthbar_offset = Vector2(healthbar_size.x / 2, healthbar_size.y * 2 + shape_size / 2);
    const auto healthbar_pos = get_position() - healthbar_offset;

    const RRectangle(healthbar_pos, current_health_size).Draw(healthbar_color);
    const RRectangle(healthbar_pos, healthbar_size).DrawLines(RAYWHITE, 1);

    if (expand) {
        const auto str = health.current > 0 ? std::to_string(health.current) : "DEAD";
        const auto& txt = RText{str, health_bar.text_size, RAYWHITE, font, 1};
        const auto txt_offset = (healthbar_size - txt.MeasureEx()) / 2;
        txt.Draw(healthbar_pos + txt_offset);
    }
}

void Character::draw_move_direction(const Color& draw_color) const noexcept {
    constexpr static auto BASE_SIZE = 10;

    static const auto& state = Game::get_states();

    const auto& rot = in_world.rotation;
    const auto mult_factor = velocity.Length() * 0.3f;
    const auto triangle_sz = state.value_at("expand_info") ? BASE_SIZE : BASE_SIZE * mult_factor;

    const auto& [x, y] = position;
    const auto offset_angle = rot * DEG2RAD;
    const auto offset_distance = get_size() + 40 * mult_factor;

    const Vector2 triangle_pos = {
        std::fma(offset_distance, std::cos(offset_angle), x),
        std::fma(offset_distance, std::sin(offset_angle), y)};

    draw_shape<DrawMode::Lines>(ShapeType::Triangle, triangle_pos, triangle_sz, draw_color, rot + 90.f);
}

auto attack(Character& attacker, Character& attacked) noexcept {
    thread_local static Random_t dmg_rand;
    const auto curr_time = GetTime();
    auto& attack = attacker.attack;

    if (attack.prev_time + attack.delay_sec >= curr_time)
        return;

    const auto is_crit = dmg_rand.from_zero_to(100) <= attacker.get_crit_chance();
    const auto dmg = attack.damage * (is_crit ? attacker.get_crit_multiplier() : 1);
    attacked.take_damage(dmg);
}