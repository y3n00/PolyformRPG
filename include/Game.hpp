#pragma once

#include <raylib-cpp.hpp>

#include "Items.hpp"
#include "Player.hpp"
#include "States.hpp"
#include "World.hpp"
#include "other/random.hpp"

class Game {
    static inline Random_t game_rand;
    static inline States game_states;

   public:
    Game(uint32_t world_seed = 0) noexcept;
    ~Game() = default;

    void update() noexcept;
    void render(RWindow& w) noexcept;

    static inline auto& get_states() { return game_states; }

   private:
    RCamera2D camera;
    World world;
    Player player;

   private:
    void handle_input() noexcept;
    void update_game_state() noexcept;
    void update_player() noexcept;
    void update_camera() noexcept;
    void update_world() noexcept;
    void show_player_info() noexcept;
    void show_pause_screen() noexcept;
};