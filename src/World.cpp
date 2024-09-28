
#include <functional>
#include <print>
#include <ranges>
#include <vector>

// #define STB_PERLIN_IMPLEMENTATION
// #include <stb_perlin.h>

#include <type_traits>

#include "../include/Character.hpp"
#include "../include/Items.hpp"
#include "../include/NPC.hpp"
#include "../include/Player.hpp"
#include "../include/Utils.hpp"
#include "../include/World.hpp"
// #include "../include/other/random.hpp"

inline constexpr static auto default_terrain_color = BLACK;
inline constexpr static auto hash_obj = std::hash<Seed_t>{};

World::World(Seed_t seed) noexcept
    : world_hash(hash_obj(seed)) {
    world_objects.emplace_back(std::make_shared<Character>(ORANGE, 5, 15, 50, 2, ShapeType::Circle));
}

void World::update(const Player& player, const RCamera2D& camera) noexcept {
    const auto& player_pos = player.get_position();
    const auto& player_sz = player.get_size();

    if (!player.is_alive())
        return;

    get_viewport(camera).swap(viewport);

    for (auto& obj : viewport) {
        obj->update();
        const auto& obj_pos = obj->get_position();
        const auto& obj_size = obj->get_size();
        const auto dist_to_player = obj_pos.Distance(player_pos);
    }
}

void World::render() const noexcept {
    static const RRectangle terrain_shape{world_pos, world_size};
    terrain_shape.Draw(default_terrain_color);
    terrain_shape.DrawLines(MAGENTA, 5);
    for (auto&& obj : viewport) {
        obj->render();
    }
}

ObjectsStorage World::get_viewport(const RCamera2D& camera) noexcept {
    const auto& target_center = camera.GetTarget();
    const auto [view_w, view_h] = RVector2(GetScreenWidth(), GetScreenHeight()) / camera.zoom;
    const auto view_obj = StaticObject(BLANK, ShapeSize{view_w, view_h}, ShapeType::Rectangle, target_center);

    const auto filter_f = [&target_center](auto&& ptr) {
        constexpr static auto DISTANCE_THRESHOLD = 1000;
        return ptr->get_position().Distance(target_center) <= DISTANCE_THRESHOLD;
    };

    auto&& prepared = world_objects | std::views::filter(filter_f);
    const auto prepared_sz = std::ranges::distance(prepared);

    ObjectsStorage visible_objects;
    visible_objects.reserve(prepared_sz);

    for (auto&& prep : prepared) {
        if (prep->check_collision(view_obj))
            visible_objects.push_back(prep);
    }
#ifdef _DEBUG
    static size_t global_prev = 0, visible_prev = 0;
    log_change("[World]: world_objects", global_prev, world_objects);
    log_change("[World]: visible_objects", visible_prev, visible_objects);
#endif  // _DEBUG
    return visible_objects;
}
