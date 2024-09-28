#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "GameObjects.hpp"
#include "Player.hpp"

using Seed_t = uint32_t;
using ObjectsStorage = std::vector<std::shared_ptr<StaticObject>>;

class World {
    inline constexpr static auto _SINGLE_SIZE = 2e6;
    inline constexpr static auto _SINGLE_POS = -(_SINGLE_SIZE / 2);

   public:
    World(Seed_t seed) noexcept;

    void update(const Player& player, const RCamera2D& camera) noexcept;
    void render() const noexcept;

    inline constexpr static auto world_size = Vector2{_SINGLE_SIZE, _SINGLE_SIZE};
    inline constexpr static auto world_pos = Vector2{_SINGLE_POS, _SINGLE_POS};

   private:
    size_t world_hash;
    ObjectsStorage world_objects, viewport;
    ObjectsStorage get_viewport(const RCamera2D& camera) noexcept;
};
