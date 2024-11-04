#include "../include/Game.hpp"

#include <algorithm>
#include <format>

#include "../include/Interface.hpp"
#include "../include/ResourceManager.hpp"
#include "../include/Utils.hpp"

constexpr static auto FONT_COLOR = RAYWHITE, BG_COLOR = DARKPURPLE;

[[nodiscard]] static inline auto get_screen_center() noexcept {
    return Vector2{GetScreenWidth() / 2.f, GetScreenHeight() / 2.f};
}

Game::Game(uint32_t world_seed) noexcept
    : world(world_seed) {
    game_states = States({
        {"expand_info", false},
        {"pause", false},
        {"show_player_info", false},
    });
    ResourceManager::add("default_font", GetFontDefault());
    ResourceManager::add("main_font", LoadFont("assets/moscow2024.otf"));
    ResourceManager::add("dbg_font", LoadFont("assets/courbd.ttf"));

    camera = RCamera2D(get_screen_center(), player.get_position(), 0.0f, 2.0f);
}

void Game::update() noexcept {
    handle_input();
    update_game_state();

    if (game_states.value_at("pause") || game_states["inventory"])
        return;

    update_player();
    update_camera();
    update_world();
}

void Game::handle_input() noexcept {
    game_states["expand_info"] = RKeyboard::IsKeyDown(KEY_LEFT_ALT);

    if (RKeyboard::IsKeyPressed(KEY_TAB))
        game_states.toggle("inventory");

    if (RKeyboard::IsKeyPressed(KEY_ESCAPE)) {
        const auto is_inventory_open = game_states.value_at("inventory");
        const auto& to_toggle = is_inventory_open ? "inventory" : "pause";
        game_states.toggle(to_toggle);
    }

    if (RKeyboard::IsKeyPressed(KEY_F1))
        game_states.toggle("show_player_info");
}

void Game::update_game_state() noexcept {
    if (game_states.value_at("show_player_info")) {
        Interface::add_to_drawlist(
            [this]() noexcept { this->show_player_info(); });
    }
    if (game_states.value_at("pause")) {
        Interface::add_to_drawlist(
            [this]() noexcept { this->show_pause_screen(); });
    }
    if (game_states.value_at("inventory")) {
        Interface::add_to_drawlist(
            [this]() noexcept { this->show_player_info(); });
    }
}

void Game::show_player_info() noexcept {
    constexpr static auto font_sz = 14;
    constexpr static auto y_delta = font_sz * 2;
    static const auto& dbg_font = ResourceManager::get<Font>("dbg_font");

    const auto [player_x, player_y] = player.get_position();
    const auto speed = player.get_velocity().Length();
    const auto rotation = player.get_rotation();

    Vector2 draw_pos(10, 10);

    auto&& player_info = {
        std::format("PLAYER_POS: [{:.2f}, {:.2f}]", player_x, player_y),
        std::format("PLAYER_SPEED: {:.2f}", speed),
        std::format("CAMERA_ZOOM: {:.2f}", camera.zoom),
        std::format("ROTATION: {:.2f}", rotation),
        std::format("FPS: {}", GetFPS())};

    for (auto&& str : player_info) {
        RText(dbg_font, str, font_sz, 0, RAYWHITE).Draw(draw_pos);
        draw_pos.y += y_delta;
    }
}

void Game::show_pause_screen() noexcept {
    static const auto& main_font = ResourceManager::get<Font>("main_font");
    static const auto txt = RText(main_font, "PAUSED", 28, 1, FONT_COLOR);
    static const auto text_sz = txt.MeasureEx();

    const auto screen_sz = RVector2(GetScreenWidth(), GetScreenHeight());
    RRectangle(Vector2Zero(), screen_sz).Draw(RColor(BG_COLOR).Alpha(0.7f));
    txt.Draw((screen_sz - text_sz) / 2);
}

void Game::update_player() noexcept {
    static const auto [player_w, player_h] = RVector2(player.get_size().operator Vector2()) / 2;
    static const auto [left_bound, top_bound] = World::world_pos;
    static const auto [right_bound, bottom_bound] = Vector2Add(World::world_pos, World::world_size);

    auto& [player_x, player_y] = player.position;
    player_x = std::clamp(player_x, left_bound + player_w, right_bound - player_w);
    player_y = std::clamp(player_y, top_bound + player_h, bottom_bound - player_h);
}

void Game::update_camera() noexcept {
    constexpr static auto ZOOM_MULT = 0.5f, CAMERA_LERP = 0.1f;
    constexpr static auto MIN_ZOOM = 1.f, MAX_ZOOM = 5.f;

    camera.zoom = std::clamp(camera.zoom + GetMouseWheelMove() * ZOOM_MULT, MIN_ZOOM, MAX_ZOOM);
    camera.target = Vector2Lerp(camera.target, player.get_position(), CAMERA_LERP);
    camera.offset = get_screen_center();
}

void Game::update_world() noexcept {
    player.update();
    world.update(player, camera);
}

void Game::render(RWindow& w) noexcept {
    w.BeginDrawing();
    {
        w.ClearBackground(BG_COLOR);
        camera.BeginMode();
        {
            world.render();
            player.render();
        }
        camera.EndMode();
        Interface::draw_all();
    }
    w.EndDrawing();
}