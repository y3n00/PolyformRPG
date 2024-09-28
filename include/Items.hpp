#pragma once

#include <map>

#include "GameObjects.hpp"

enum class ObjectRarity : uint16_t {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

enum class DamageType : uint16_t {
    PHYSICAL,
    MAGICAL,
    FIRE,
    ICE,
    LIGHTNING,
    POISON
};

enum class ItemType : uint16_t {
    WEAPON,
    ARMOR,
    CONSUMABLE,
    MISCELLANEOUS
};

enum class Stat : uint16_t {
    DAMAGE,
    ARMOR,
    HEALTH,
    STRENGTH,
    INTELLIGENCE,
    AGILITY,
    LUCK,
    CRITICAL_HIT_CHANCE,
    CRITICAL_HIT_DAMAGE,
    ATTACK_SPEED,
    MOVEMENT_SPEED,
};

class Item : public UpgradableObject {
   public:
    Item(Vector2 pos, ItemType type, ObjectRarity rarity, DamageType damage_type);

    [[nodiscard]] inline auto get_type() const noexcept { return type; }
    [[nodiscard]] inline auto get_damage_type() const noexcept { return damage_type; }
    [[nodiscard]] inline auto get_name() const noexcept { return name; }
    [[nodiscard]] inline auto get_stat(Stat stat) const noexcept { return stats.at(stat); }
    [[nodiscard]] inline auto get_rarity() const noexcept { return rarity; }

    inline void set_stat(Stat stat, int16_t value) noexcept { stats[stat] = value; }
    inline void set_name(std::string_view str) noexcept { name = str; }

    void update() noexcept override;
    void upgrade() noexcept override;
    void render() const noexcept override;

    //~Item() noexcept override = default;

   protected:
    ObjectRarity rarity;
    ItemType type;
    DamageType damage_type;
    std::string name;
    std::map<Stat, int16_t> stats;
};