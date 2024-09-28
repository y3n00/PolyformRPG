#include "../include/Items.hpp"

#include <array>

#include "../include/Game.hpp"
#include "../include/ResourceManager.hpp"
#include "../include/Utils.hpp"

constexpr static auto names = std::to_array({
    "Damage",               // DAMAGE
    "Armor",                // ARMOR
    "Health",               // HEALTH
    "Strength",             // STRENGTH
    "Intelligence",         // INTELLIGENCE
    "Agility",              // AGILITY
    "Luck",                 // LUCK
    "Critical Hit Chance",  // CRITICAL_HIT_CHANCE
    "Critical Hit Damage",  // CRITICAL_HIT_DAMAGE
    "Attack Speed",         // ATTACK_SPEED
    "Movement Speed",       // MOVEMENT_SPEED
});

[[nodiscard]] constexpr static auto get_stat_str(Stat stat) noexcept {
    return std::string_view{names.at(std::to_underlying(stat))};
}

[[nodiscard]] inline static constexpr auto rarity_to_color(ObjectRarity rarity) {
    constexpr auto colors = std::to_array({
        WHITE,   // COMMON
        GREEN,   // UNCOMMON
        BLUE,    // RARE
        PURPLE,  // EPIC
        ORANGE,  // LEGENDARY
    });
    return colors[std::to_underlying(rarity)];
}

constexpr static auto ITEM_SIZE = 10;
constexpr static auto ITEM_SHAPE = ShapeType::Circle;

Item::Item(Vector2 pos, ItemType type, ObjectRarity rarity, DamageType damage_type)
    : StaticObject(RAYWHITE, ITEM_SIZE, ITEM_SHAPE, pos),
      UpgradableObject(RAYWHITE, ITEM_SIZE, ITEM_SHAPE, pos),
      type(type),
      damage_type(damage_type),
      rarity(rarity) {
    this->color = rarity_to_color(rarity);
}

void Item::update() noexcept {}

void Item::upgrade() noexcept {}

void Item::render() const noexcept {
    constexpr static auto GLOW_SHOULD_PULSE = true;
    static const auto& states = Game::get_states();

    draw_glow(this, GLOW_SHOULD_PULSE);
    UpgradableObject::render();

    if (!states.value_at("expand_info"))
        return;
    static const auto& name_font = ResourceManager::get<Font>("main_font");
    const auto& [center_x, center_y] = get_position();
    const auto caption = RText{name, 9, get_color(), name_font, 2.5f};
    const auto caption_w = caption.Measure();
    caption.Draw(Vector2(center_x - caption_w / 2, center_y + get_size() * 1.5f));
}