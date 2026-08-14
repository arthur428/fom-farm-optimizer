#ifndef FOM_FARM_OPTIMIZER__OPTIMIZER_HPP_
#define FOM_FARM_OPTIMIZER__OPTIMIZER_HPP_

#include "fom_farm_optimizer/player.hpp"
#include "fom_farm_optimizer/season.hpp"

class Optimizer {
 public:
  Optimizer(const Player& player, const Season& season);

 private:
  Player player_;
  Season season_;
};

#endif  // FOM_FARM_OPTIMIZER__OPTIMIZER_HPP_
