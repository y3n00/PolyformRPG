#pragma once

#include <deque>

#include "Character.hpp"

class Player final : public Character {
    friend class Game;

   public:
    Player() noexcept;
    ~Player() noexcept override = default;

    void update() noexcept override;
    void render() const noexcept override;
    void upgrade() noexcept override;

   private:
    uint64_t exp = 0;
    std::deque<Vector2> trail_data;
    inline constexpr static auto MAX_TRAIL_LENGTH = 12;
    float get_speed_multiplier() const noexcept override;
};