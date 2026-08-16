#ifndef FOM_FARM_OPTIMIZER__SEED_HPP_
#define FOM_FARM_OPTIMIZER__SEED_HPP_

#include <cstdint>
#include <string>

class Seed {
 public:
  struct Data {
    std::string name;
    uint16_t cost;
    uint8_t growth_days;
    uint8_t regrowth_days;
    uint16_t sell_price;
  };


  Seed(const Data& data);

  Data get_data() const;
  int32_t get_profit() const;
  void set_profit(const int32_t& profit);

 private:
  std::string name_;
  uint16_t cost_;
  uint8_t growth_days_;
  uint8_t regrowth_days_;
  uint16_t sell_price_;
  int32_t profit_;
};

#endif  // FOM_FARM_OPTIMIZER__SEED_HPP_
