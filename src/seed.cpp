#include "fom_farm_optimizer/seed.hpp"

Seed::Seed(const Data& data)
    : name_(data.name),
      cost_(data.cost),
      growth_days_(data.growth_days),
      regrowth_days_(data.regrowth_days),
      sell_price_(data.sell_price) {}

Seed::Data Seed::get_data() const {
  return Data{name_, cost_, growth_days_, regrowth_days_, sell_price_};
}

uint32_t Seed::get_profit() const { return profit_; }

void Seed::set_profit(const uint32_t& profit) { profit_ = profit; }
