#include "fom_farm_optimizer/season.hpp"

Season::Season() : year_() {}
Season::~Season() {}
std::vector<Seed> Season::get_seeds() const { return seeds_; }

Summer::Summer(const uint8_t& year) : Season() {
  (void)year;

  // Name, Cost, Growth, Regrowth, Sell Price
  seeds_ = {
    {{"Cucumber", 25, 4, 0, 40}},
    {{"Chili Pepper", 40, 6, 0, 75}},
    {{"Watermelon", 70, 9, 0, 180}},
    {{"Tomato", 300, 5, 3, 125}},
    {{"Corn", 300, 5, 3, 125}},
    {{"Sunflower", 20, 6, 0, 30}},
    {{"Sugar Cane", 40, 6, 0, 80}},
    {{"Tea", 300, 5, 3, 135}},
    {{"Daisy", 10, 4, 0, 15}},
    {{"Catmint", 20, 6, 0, 30}},
    {{"Cosmos", 20, 6, 0, 30}}};
    // {{"Pear", 400, 14, 3, 45 * 3}},
    // {{"Peach", 400, 14, 3, 45 * 3}}};
  };

Fall::Fall(const uint8_t & year) : Season()
{
  (void)year;

  // Name, Cost, Growth, Regrowth, Sell Price
  seeds_ = {
    {{"Sweet Potato", 25, 4, 0, 40}},
    {{"Broccoli", 40, 6, 0, 75}},
    {{"Cranberry", 300, 5, 3, 125}},
    {{"Pumpkin", 70, 9, 0, 180}},
    {{"Wheat", 300, 9, 3, 150}},
    {{"Rice", 300, 9, 3, 150}},
    {{"Onion", 300, 5, 3, 135}},
    {{"Celosia", 15, 4, 0, 25}},
    {{"Chrysanthemum", 20, 6, 0, 30}},
    // {{"Apple", 400, 14, 3, 45 * 3}},
    // {{"Orange", 400, 14, 3, 45 * 3}}
  };
}

Winter::Winter(const uint8_t& year) : Season() {
  (void)year;

  // Name, Cost, Growth, Regrowth, Sell Price
  seeds_ = {{{"Beet", 25, 4, 0, 40}},
            {{"Cauliflower", 40, 6, 0, 75}},
            {{"Snow Peas", 300, 5, 3, 125}},
            {{"Daikon Radish", 70, 9, 0, 180}},
            {{"Frost Lily", 15, 4, 0, 25}},
            {{"Poinsettia", 20, 6, 0, 30}},
            {{"Pomegranate", 400, 14, 3, 45 * 3}}};
}
