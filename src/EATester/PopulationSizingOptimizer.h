#ifndef POPULATION_SIZING_OPTIMIZER_H
#define POPULATION_SIZING_OPTIMIZER_H

#define POPULATION_SIZING_OPTIMIZER_ARGUMENT_POPULATION_CREATION_FREQUENCY "population_creation_frequency"
#define POPULATION_SIZING_OPTIMIZER_ARGUMENT_POPULATION_SIZE_MULTIPLIER "population_size_multiplier"

#define POPULATION_SIZING_COUNTER_DEFAULT_BASE 4

#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"

#include <ctime>
#include <cstdint>
#include <istream>
#include <unordered_set>
#include <vector>

using namespace std;


class CPopulationSizingCounter
{
public:
	CPopulationSizingCounter();
	CPopulationSizingCounter(uint32_t iBase);

	uint32_t iIncrement();
	void vReset();

	bool bIsReset() { return v_numbers.empty(); };

	uint32_t iGetLastMostSignificantChangeIndex() { return i_last_most_significant_change_index; };

	bool bSetBase(int iBase);

private:
	uint32_t i_base;
	uint32_t i_last_most_significant_change_index;
	vector<uint32_t> v_numbers;
};//class CPopulationSizingCounter


template <class TGenotype, class TFenotype>
class CPopulationSizingSingleOptimizer : public CSteadyStateOptimizer<TGenotype, TFenotype>
{
public:
	CPopulationSizingSingleOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CPopulationSizingSingleOptimizer(CPopulationSizingSingleOptimizer<TGenotype, TFenotype> *pcOther);

	virtual double dComputeAverageFitnessValue() = 0;

	virtual uint32_t iGetPopulationSize() = 0;
	virtual void vSetPopulationSize(uint32_t iPopulationSize) = 0;
};//class CPopulationSizingSingleOptimizer : public CSteadyStateOptimizer<TGenotype, TFenotype>


template <class TGenotype, class TFenotype>
class CPopulationSizingOptimizer : public COptimizer<TGenotype, TFenotype>
{
public:
	CPopulationSizingOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CPopulationSizingOptimizer(CPopulationSizingOptimizer<TGenotype, TFenotype> *pcOther);

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CPopulationSizingOptimizer(this); };

	virtual ~CPopulationSizingOptimizer();

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CPopulationSizingSingleOptimizer<TGenotype, TFenotype>* pc_get_params_optimizer(istream *psSettings, CError *pcError);

	using COptimizer<TGenotype, TFenotype>::b_update_best_individual;
	virtual bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime);

	void v_clear_optimizers();
	void v_clear_params();

	bool b_run_proper_optimizer_iteration(uint32_t iIterationNumber, time_t tStartTime);
	virtual bool b_run_proper_optimizer_iteration(CPopulationSizingSingleOptimizer<TGenotype, TFenotype>* pcProperOptimizer, uint32_t iIterationNumber, time_t tStartTime);

	void v_delete_optimizers();
	void v_get_optimizers_to_delete(unordered_set<size_t> *psIndexes);
	void v_add_new_optimizer(uint32_t iIterationNumber, time_t tStartTime);

	vector<CPopulationSizingSingleOptimizer<TGenotype, TFenotype>*> v_optimizers;
	vector<double> v_optimizers_average_fitnesses;

private:
	uint32_t i_population_creation_frequency;
	uint32_t i_population_size_multiplier;

	uint32_t i_next_population_size;
	CPopulationSizingCounter c_population_sizing_counter;

	CPopulationSizingSingleOptimizer<TGenotype, TFenotype> *pc_params_optimizer;
};//class CPopulationSizingOptimizer : public COptimizer<TGenotype, TFenotype>

#endif//POPULATION_SIZING_OPTIMIZER_H