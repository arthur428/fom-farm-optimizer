#ifndef FOM_FARM_OPTIMIZER__PLAYER_HPP_
#define FOM_FARM_OPTIMIZER__PLAYER_HPP_

#include <cstdint>

#include "fom_farm_optimizer/seed.hpp"

class Player {
 public:
  Player(const uint64_t& stamina, const int64_t& money);

  void buy_seed(const Seed& seed);
  uint64_t get_stamina() const;
  int64_t get_money() const;

 private:
  uint64_t stamina_;
  int64_t money_;
};

#endif  // FOM_FARM_OPTIMIZER__PLAYER_HPP_
