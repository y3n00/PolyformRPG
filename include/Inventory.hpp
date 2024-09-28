#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Items.hpp"

enum class InventorySlot : uint8_t {

};

using Item_t = std::unique_ptr<Item>;
using InventoryStorage = std::vector<Item_t>;
using SlotedStorage = std::unordered_map<InventorySlot, Item_t>;

class Inventory {
   public:
   private:
    SlotedStorage slots;
    InventoryStorage storage;
};
