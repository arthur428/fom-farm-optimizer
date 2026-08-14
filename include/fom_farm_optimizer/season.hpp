#ifndef FOM_FARM_OPTIMIZER__SEASON_HPP_
#define FOM_FARM_OPTIMIZER__SEASON_HPP_

#include <string>
#include <vector>

#include "fom_farm_optimizer/seed.hpp"

class Season {
 public:
  Season(const uint8_t& year);
  virtual ~Season();

  virtual std::vector<Seed> get_seeds() const;

protected:
  std::vector<Seed> seeds_;

 private:
  uint8_t year_;
  
};

// class Spring : public Season {
//  public:
//   Spring(const uint8_t& year);
//   std::vector<Seed> get_seeds() const override;
// };

class Summer : public Season {
 public:
  Summer(const uint8_t& year);
};

// class Fall : public Season {
//  public:
//   Fall(const uint8_t& year);
//   std::vector<Seed> get_seeds() const override;
// };

// class Winter : public Season {
//  public:
//   Winter(const uint8_t& year);
//   std::vector<Seed> get_seeds() const override;
// };

#endif  // FOM_FARM_OPTIMIZER__SEASON_HPP_
