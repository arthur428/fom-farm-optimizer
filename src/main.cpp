#include <tuple>
#include <vector>

#include "fom_farm_optimizer/season.hpp"
#include "highs/Highs.h"

enum TupleIdx { NAME = 0, LOWER_BOUND, UPPER_BOUND };

// Possible User Inputs
const int64_t kMoney = 100000;
const uint8_t kCurrentDay = 1;
const int64_t kStaminaPerDay = 120;
const uint8_t kYear = 1;
const uint8_t kPlantLimit = 100;
const uint8_t kDefMinSeeds = 0;

// General Constant
const uint8_t kTotalDays = 28;

// Control lower and upper bounds for specific seeds (0 to infinity)
std::vector<std::tuple<std::string, double, double>> bounds_input = {
    // {"Cucumber", 0.0, 100.0}
};

int main(int argc, char** argv) {
  // Setup "environment" for optimization
  std::vector<Seed> seeds = Summer(kYear).get_seeds();
  uint8_t planning_days = kTotalDays - kCurrentDay + 1;

  // Calculate profit per seed
  for (size_t idx = 0; idx < seeds.size(); idx++) {
    Seed::Data data = seeds[idx].get_data();

    uint8_t harvests = 0;
    bool b_can_harvest = (planning_days >= data.growth_days);

    if (data.regrowth_days > 0) {
      if (b_can_harvest) {
        harvests = 1 + (planning_days - data.growth_days) / data.regrowth_days;
      }
    } else {
      if (b_can_harvest) {
        harvests = planning_days / data.growth_days;
      }
    }
    seeds[idx].set_profit(data.sell_price * harvests - data.cost);
  }

  const size_t kNumConstraints = 3;  // Money, Stamina, and Planting Limit

  // Setup equations
  HighsModel model;
  model.lp_.sense_ = ObjSense::kMaximize;

  // Setup constraints
  model.lp_.num_row_ = kNumConstraints;

  // Constraints Lower Bounds
  model.lp_.row_lower_.push_back(0);  // Money
  model.lp_.row_lower_.push_back(0);  // Stamina per Day
  model.lp_.row_lower_.push_back(0);  // Planting Limit or Space

  // Constraints Upper Bounds
  model.lp_.row_upper_.push_back(kMoney);          // Money
  model.lp_.row_upper_.push_back(kStaminaPerDay);  // Stamina per Day
  model.lp_.row_upper_.push_back(kPlantLimit);     // Planting Limit or Space

  // Setup seed variables
  model.lp_.num_col_ = seeds.size();

  model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;

  for (const auto& seed : seeds) {
    Seed::Data data = seed.get_data();
    std::cout << "Seed: " << data.name << ", Profit: " << seed.get_profit()
              << std::endl;
  }

  for (size_t idx = 0; idx < seeds.size(); idx++) {
    Seed::Data data = seeds[idx].get_data();
    model.lp_.col_names_.push_back(data.name);

    // Maximize Profit
    model.lp_.col_cost_.push_back(seeds[idx].get_profit());

    model.lp_.a_matrix_.start_.push_back(model.lp_.a_matrix_.index_.size());

    model.lp_.a_matrix_.index_.push_back(0);          // Money Constraint
    model.lp_.a_matrix_.value_.push_back(data.cost);  // Cost per seed

    model.lp_.a_matrix_.index_.push_back(1);  // Stamina Constraint
    model.lp_.a_matrix_.value_.push_back(2);  // Stamina cost per seed

    model.lp_.a_matrix_.index_.push_back(2);  // Planting Limit
    model.lp_.a_matrix_.value_.push_back(1);  // One plant/space per seed

    // Set lower and upper bounds for each seed based on user input.
    // Default 0 to infinity.
    if (bounds_input.empty()) {
      model.lp_.col_lower_.push_back(kDefMinSeeds);
      model.lp_.col_upper_.push_back(kHighsInf);
    } else {
      for (const auto& bound : bounds_input) {
        if (std::get<TupleIdx::NAME>(bound) == data.name) {
          model.lp_.col_lower_.push_back(
              std::get<TupleIdx::LOWER_BOUND>(bound));
          model.lp_.col_upper_.push_back(
              std::get<TupleIdx::UPPER_BOUND>(bound));
          break;
        }
      }
    }
  }

  // Solve the optimization problem
  Highs highs;
  HighsStatus status = highs.passModel(model);
  assert(HighsStatus::kOk == status);

  const HighsLp& lp = highs.getLp();
  status = highs.run();
  assert(HighsStatus::kOk == status);

  const HighsModelStatus& model_status = highs.getModelStatus();
  assert(HighsModelStatus::kOptimal == model_status);

  const HighsInfo& info = highs.getInfo();
  std::cout << "Simplex iteration count: " << info.simplex_iteration_count
            << std::endl;
  std::cout << "Objective function value: " << info.objective_function_value
            << std::endl;
  std::cout << "Primal  solution status: "
            << highs.solutionStatusToString(info.primal_solution_status)
            << std::endl;
  std::cout << "Dual    solution status: "
            << highs.solutionStatusToString(info.dual_solution_status)
            << std::endl;
  std::cout << "Basis: " << highs.basisValidityToString(info.basis_validity)
            << std::endl;

  // Print Solution
  const std::vector<double>& solution = highs.getSolution().col_value;
  for (size_t idx = 0; idx < solution.size(); idx++) {
    if (solution[idx] > 0) {
      std::cout << "Plant " << solution[idx] << " of " << lp.col_names_[idx]
                << std::endl;
    }
  }

  return 0;
}
