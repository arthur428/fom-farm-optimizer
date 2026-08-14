#include "fom_farm_optimizer/season.hpp"

Season::Season(const uint8_t& year) : year_(year) {}
Season::~Season() {}

Summer::Summer(const uint8_t& year) : Season(year) {
  (void)year;

  // Name, Cost, Growth, Regrowth, Sell Price
  seeds_ = {{{"Cucumber", 25, 4, 0, 40}},    {{"Chili Pepper", 40, 6, 0, 75}},
            {{"Watermelon", 70, 9, 0, 180}}, {{"Tomato", 300, 5, 3, 125}},
            {{"Corn", 300, 5, 3, 125}},      {{"Sunflower", 20, 6, 0, 30}},
            {{"Daisy", 10, 4, 0, 15}},       {{"Catmint", 20, 6, 0, 30}},
            {{"Cosmos", 30, 6, 0, 30}},      {{"Pear", 400, 14, 3, 45 * 3}},
            {{"Peach", 400, 14, 3, 45 * 3}}};
}

std::vector<Seed> Summer::get_seeds() const { return seeds_; }
