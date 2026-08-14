#include "fom_farm_optimizer/player.hpp"

Player::Player(const int64_t& stamina, const int64_t& money)
    : stamina_(stamina), money_(money) {}

int64_t Player::get_stamina() const { return stamina_; }
int64_t Player::get_money() const { return money_; }
