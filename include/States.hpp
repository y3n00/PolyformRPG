#pragma once

#include <string>
#include <unordered_map>

class States {
    using Title_t = std::string;
    using Value_t = bool;
    using Pair_t = std::pair<const Title_t, Value_t>;

   public:
    States() noexcept = default;
    States(std::initializer_list<Pair_t> init) : states(init) {}

    [[nodiscard]] inline auto& operator[](const Title_t& title) {
        return states[title];
    }

    inline const auto& toggle(const Title_t& title) {
        auto& v = states[title];
        return (v = !v);
    }

    [[nodiscard]] inline auto value_at(const Title_t& title) const {
        auto it = states.find(title);
        return it != states.end() ? it->second : false;
    }

   private:
    std::unordered_map<Title_t, Value_t> states;
};