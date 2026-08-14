#include "fom_farm_optimizer/seed.hpp"

Seed::Seed(const Data& data)
    : name_(data.name),
      cost_(data.cost),
      growth_days_(data.growth_days),
      regrowth_days_(data.regrowth_days),
      sell_price_(data.sell_price) {}
