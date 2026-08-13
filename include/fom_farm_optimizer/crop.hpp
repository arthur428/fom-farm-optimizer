#ifndef FOM_FARM_OPTIMIZER__CROP_HPP_
#define FOM_FARM_OPTIMIZER__CROP_HPP_

#include <cstdint>

class Crop {
 public:
  Crop(const uint16_t& sell_price);

 private:
  uint16_t sell_price_;
};

#endif  // FOM_FARM_OPTIMIZER__CROP_HPP_