#ifndef FOM_FARM_OPTIMIZER__SEED_HPP_
#define FOM_FARM_OPTIMIZER__SEED_HPP_

#include <cstdint>

#include "fom_farm_optimizer/crop.hpp"

class Seed {
 public:
  struct Data {
    uint16_t cost;
    uint8_t growth_days;
    uint8_t regrowth_days;
  };

  Seed(const Data& data, const Crop& crop);

 private:
  uint16_t cost_;
  uint8_t growth_days_;
  uint8_t regrowth_days_;
  Crop crop_;
};

#endif  // FOM_FARM_OPTIMIZER__SEED_HPP_
