#ifndef FOM_FARM_OPTIMIZER__SEASON_HPP_
#define FOM_FARM_OPTIMIZER__SEASON_HPP_

#include <string>
#include <vector>

#include "fom_farm_optimizer/seed.hpp"

class Season {
 public:
  Season();
  virtual ~Season();

  virtual std::vector<Seed> get_seeds() const;

protected:
  std::vector<Seed> seeds_;

 private:
  uint8_t year_;
};

class Spring : public Season {
 public:
  explicit Spring(const uint8_t& year);
};

class Summer : public Season {
 public:
  explicit Summer(const uint8_t& year);
};

class Fall : public Season {
 public:
  explicit Fall(const uint8_t& year);
};

class Winter : public Season {
 public:
  explicit Winter(const uint8_t& year);
};

#endif  // FOM_FARM_OPTIMIZER__SEASON_HPP_
