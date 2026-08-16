#include <algorithm>
#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

#include "fom_farm_optimizer/season.hpp"
#include "highs/Highs.h"

enum TupleIdx { NAME = 0, LOWER_BOUND, UPPER_BOUND };
enum ConstraintIdx { BUDGET = 0, STAMINA, PLANT_LIMIT };
enum Seasons { SPRING = 0, SUMMER, FALL, WINTER };

// Possible User Inputs
const int64_t kMoney = 100000;
const uint8_t kCurrentDay = 1;
const int64_t kStaminaPerDay = 120;
const uint8_t kYear = 1;
const uint8_t kPlantLimit = 100;
const uint8_t kDefMinSeeds = 0;

// General Constants
const uint8_t kTotalDays = 28;
const std::vector<std::string> kTrees = {"Apple", "Orange", "Pear", "Peach"};

// Control lower and upper bounds for specific seeds (0 to infinity)
std::vector<std::tuple<std::string, double, double>> bounds_input = {
    // {"Cucumber", 0.0, 100.0}
};

int main(int argc, char** argv) {
  // Setup "environment" for optimization
  std::unique_ptr<Season> p_season = nullptr;
  switch (kCurrentSeason) {
    case (Seasons::SPRING):
      p_season = std::make_unique<Spring>(kYear);
      break;
    case (Seasons::SUMMER):
      p_season = std::make_unique<Summer>(kYear);
      break;
    case (Seasons::FALL):
      p_season = std::make_unique<Fall>(kYear);
      break;
    case (Seasons::WINTER):
      p_season = std::make_unique<Winter>(kYear);
      break;
    default:
      return 1;
  }
  std::vector<Seed> seeds = p_season->get_seeds();
  uint8_t planning_days = kTotalDays - kCurrentDay;

  // Calculate profit per seed
  for (size_t idx = 0; idx < seeds.size(); idx++) {
    Seed::Data data = seeds[idx].get_data();

    uint8_t harvests = 0;
    bool b_can_harvest = (planning_days >= data.growth_days);

    if (b_can_harvest) {
    if (data.regrowth_days > 0) {
        harvests = 1 + (planning_days - data.growth_days) / data.regrowth_days;
    } else {
        harvests = planning_days / data.growth_days;
      }
      seeds[idx].set_profit(static_cast<int32_t>((data.sell_price * harvests)) -
                            data.cost);
    }
  }

  const size_t kNumConstraints = 3;  // Money, Stamina, and Planting Limit

  // Setup equations
  HighsModel model;
  model.lp_.num_col_ = seeds.size();
  model.lp_.num_row_ = kNumConstraints;
  model.lp_.sense_ = ObjSense::kMaximize;

  // Reset Vectors
  model.lp_.col_names_ = {};
  model.lp_.col_cost_ = {};
  model.lp_.a_matrix_.start_ = {};
  model.lp_.a_matrix_.index_ = {};
  model.lp_.a_matrix_.value_ = {};
  model.lp_.col_lower_ = {};
  model.lp_.col_upper_ = {};
  model.lp_.row_lower_ = {};
  model.lp_.row_upper_ = {};

  // Debug Logs
  for (const auto& seed : seeds) {
    Seed::Data data = seed.get_data();
    std::cout << "Seed: " << data.name << ", Profit: " << seed.get_profit()
              << std::endl;
  }

  // Setup seed variables
  for (size_t idx = 0; idx < seeds.size(); idx++) {
    Seed::Data data = seeds[idx].get_data();
    model.lp_.col_names_.push_back(data.name);

    // Maximize Profit
    model.lp_.col_cost_.push_back(seeds[idx].get_profit());

    // Seed effects as a flat vector
    model.lp_.a_matrix_.start_.push_back(model.lp_.a_matrix_.index_.size());

    model.lp_.a_matrix_.index_.push_back(0);          // Cost
    model.lp_.a_matrix_.value_.push_back(data.cost);  // Cost per seed

    model.lp_.a_matrix_.index_.push_back(1);  // Stamina
    if (std::find(kTrees.begin(), kTrees.end(), data.name) == kTrees.end()) {
      model.lp_.a_matrix_.value_.push_back(2);  // Watering only
    } else {
      model.lp_.a_matrix_.value_.push_back(0);  // No stamina required for trees
    }

    model.lp_.a_matrix_.index_.push_back(2);  // Planting Limit
    model.lp_.a_matrix_.value_.push_back(1);  // One plant/space per seed

    // Set lower and upper bounds for each seed based on user input.
    if (bounds_input.empty()) {
      model.lp_.col_lower_.push_back(kDefMinSeeds);
      model.lp_.col_upper_.push_back(kPlantLimit);
    } else {
      bool b_found = false;

      for (const auto& bound : bounds_input) {
        if (std::get<TupleIdx::NAME>(bound) == data.name) {
          model.lp_.col_lower_.push_back(
              std::get<TupleIdx::LOWER_BOUND>(bound));
          model.lp_.col_upper_.push_back(
              std::get<TupleIdx::UPPER_BOUND>(bound));
          b_found = true;
          break;
        }
      }

      if (!b_found) {
        model.lp_.col_lower_.push_back(kDefMinSeeds);
        model.lp_.col_upper_.push_back(kPlantLimit);
      }
    }
  }
  // Highs expect num_col_+1 entries for matrix_.start_
  // This shows number of nonzeros in the last column to be defined
  model.lp_.a_matrix_.start_.push_back(model.lp_.a_matrix_.index_.size());

  // Setup constraints
  model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;

  // Constraints Lower Bounds
  model.lp_.row_lower_.push_back(0);  // Budget
  model.lp_.row_lower_.push_back(0);  // Stamina per Day
  model.lp_.row_lower_.push_back(0);  // Planting Limit or Space

  // Constraints Upper Bounds
  model.lp_.row_upper_.push_back(kMoney);          // Budget
  model.lp_.row_upper_.push_back(kStaminaPerDay);  // Stamina per Day
  model.lp_.row_upper_.push_back(kPlantLimit);     // Planting Limit or Space

  // Integrality
  for (int col = 0; col < model.lp_.num_col_; col++) {
    model.lp_.integrality_.push_back(HighsVarType::kInteger);
  }

  // Checks prior to passing the model
  assert(model.lp_.a_matrix_.start_.size() ==
         static_cast<size_t>(model.lp_.num_col_) + 1);
  assert(model.lp_.a_matrix_.start_.front() == 0);
  assert(model.lp_.a_matrix_.start_.back() ==
         static_cast<HighsInt>(model.lp_.a_matrix_.index_.size()));

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
  std::cout << "----------\n";
  const std::vector<double>& solution = highs.getSolution().col_value;
  for (size_t idx = 0; idx < solution.size(); idx++) {
    if (solution[idx] > 0) {
      std::cout << "Plant " << solution[idx] << " of " << lp.col_names_[idx]
                << std::endl;
    }
  }
  std::cout << "----------\n";

  HighsSparseMatrix rowwise = lp.a_matrix_;
  rowwise.ensureRowwise();
  for (int row = 0; row < lp.num_row_; row++) {
    double activity = 0.0;
    for (int k = rowwise.start_[row]; k < rowwise.start_[row + 1]; k++) {
      activity += rowwise.value_[k] * solution[rowwise.index_[k]];
    }

    std::string activity_name;
    switch (row) {
      case (ConstraintIdx::BUDGET):
        activity_name = "Budget";
        break;
      case (ConstraintIdx::STAMINA):
        activity_name = "Stamina";
        break;
      case (ConstraintIdx::PLANT_LIMIT):
        activity_name = "Num Plants";
        break;
      default:
        break;
    }
    std::cout << activity_name << " = " << activity << ", bounds = ["
              << lp.row_lower_[row] << ", " << lp.row_upper_[row] << "]\n";
  }

  return 0;
}
