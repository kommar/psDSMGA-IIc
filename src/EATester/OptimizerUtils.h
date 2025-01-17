#ifndef OPTIMIZER_UTILS_H
#define OPTIMIZER_UTILS_H

#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "PopulationOptimizer.h"
#include "PopulationSizingOptimizer.h"
#include "Problem.h"

#include <cstdint>
#include <istream>

using namespace std;

namespace OptimizerUtils
{
	template <class TGenotype, class TFenotype>
	COptimizer<TGenotype, TFenotype> *pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory = true);

	template <class TGenotype, class TFenotype>
	CSteadyStateOptimizer<TGenotype, TFenotype> *pcGetSteadyStateOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory = true);

	template <class TGenotype, class TFenotype>
	CPopulationSizingSingleOptimizer<TGenotype, TFenotype> *pcGetPopulationSizingSingleOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory = true);

	template <class TGenotype, class TFenotype>
	CPopulationOptimizer<TGenotype, TFenotype> *pcGetPopulationOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace OptimizerUtils

#endif//OPTIMIZER_UTILS_H
