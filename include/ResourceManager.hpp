#pragma once

#include <any>
#include <string>
#include <unordered_map>

class ResourceManager {
   public:
    template <typename T>
    inline static void add(const std::string& name, T asset) {
        storage.emplace(name, std::make_any<T>(asset));
    }

    template <typename T>
    [[nodiscard]] inline static decltype(auto) get(const std::string& name) {
        return std::any_cast<T&>(storage.find(name)->second);
    }

    template <typename T>
    [[nodiscard]] inline static decltype(auto) has(const std::string& name) {
        return storage.contains(name);
    }

   private:
    inline static std::unordered_map<std::string, std::any> storage;
};