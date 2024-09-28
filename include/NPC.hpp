#pragma once

#include <raylib.h>

#include <memory>
#include <type_traits>

#include "Character.hpp"

class EnemyEntity final : public Character {
   public:
    ~EnemyEntity() noexcept override = default;

   private:
};

class FriendEntity final : public Character {
   public:
    ~FriendEntity() noexcept override = default;

   private:
};