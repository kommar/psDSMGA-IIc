// Brian Goldman

// Trival implementation of repeated local search

#include "RandomRestartHC.h"

// Start from a random location and keep going until a local optima
// is found
bool RandomRestartHC::iterate() {
  auto solution = rand_vector(rand, length);
  float fitness = evaluator.evaluate(solution);
  hill_climber(rand, solution, fitness, evaluator, b_multi_obj_domination_fitness_compare);

  // Always possible
  return true;
}
