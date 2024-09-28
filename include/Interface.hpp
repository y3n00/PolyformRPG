#pragma once

#include <functional>
#include <utility>
#include <vector>

class Interface {
   public:
    inline static void add_to_drawlist(std::function<void()> f) {
        funcs.emplace_back(std::move(f));
    }

    inline static void draw_all() {
        for (auto&& f : funcs) {
            f();
        }
        funcs.clear();
    }

   private:
    inline static std::vector<std::function<void()>> funcs;
};
