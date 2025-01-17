#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#define OPTIMIZER_ARGUMENT_TYPE "optimizer_type"
#define OPTIMIZER_ARGUMENT_TYPE_P3 "p3"
#define OPTIMIZER_ARGUMENT_TYPE_LTGA "ltga"
#define OPTIMIZER_ARGUMENT_TYPE_LTGA_ORIGINAL "ltga_original"
#define OPTIMIZER_ARGUMENT_TYPE_LTGA_ORIGINAL_POPULATION_SIZING "ltga_original_population_sizing"
#define OPTIMIZER_ARGUMENT_TYPE_DSMGA2 "dsmga2"
#define OPTIMIZER_ARGUMENT_TYPE_DSMGA2_POPULATION_SIZING "dsmga2_population_sizing"
#define OPTIMIZER_ARGUMENT_TYPE_POPULATION_SIZING "population_sizing"

#define OPTIMIZER_ARGUMENT_CONFIG_FILE_PATH "optimizer_config_file_path"


#include "Error.h"
#include "Generation.h"
#include "Individual.h"
#include "Log.h"
#include "Problem.h"
#include "StopCondition.h"

#include <cstdint>
#include <ctime>
#include <functional>
#include <istream>

#include  "util\timer.h"

using namespace std;
using namespace TimeCounters;


enum EOptimizerType
{
	OPTIMIZER_P3 = 1,
	OPTIMIZER_LTGA = 2,
	OPTIMIZER_LTGA_ORIGINAL = 3,
	OPTIMIZER_LTGA_ORIGINAL_POPULATION_SIZING = 4,
	OPTIMIZER_DSMGA2 = 5,
	OPTIMIZER_DSMGA2_POPULATION_SIZING = 6,
	OPTIMIZER_POPULATION_SIZING = 7
};//enum EOptimizerType


template <class TGenotype, class TFenotype>
class COptimizer
{
public:
	COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	COptimizer(COptimizer<TGenotype, TFenotype> *pcOther);

	virtual ~COptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() = 0;

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime) = 0;

	virtual void vRun();

	void vResetBestIndividual();

	CProblem<TGenotype, TFenotype> *pcGetProblem() { return pc_problem; };

	CStopCondition<TGenotype, TFenotype> *pcGetStopCondition() { return pc_stop_condition; };

	CIndividual<TGenotype, TFenotype> *pcGetBestIndividual() { return pc_best_individual; };
	void vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy = true);

	
	time_t tGetBestTime() { return t_best_time; };
	uint64_t iGetBestFFE() { return i_best_ffe; };

	CString  sGetLogName();

protected:
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual);
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype);

	virtual CIndividual<TGenotype, TFenotype> *pc_create_individual(TGenotype *pcGenotype)
	{
		return new CIndividual<TGenotype, TFenotype>(pcGenotype, pc_problem->pcGetEvaluation(), pc_problem->pcGetTransformation());
	};

	void v_clear_params();

	CProblem<TGenotype, TFenotype> *pc_problem;
	
	CStopCondition<TGenotype, TFenotype> *pc_stop_condition;

	CGeneration<TGenotype> *pc_empty_generation;

	CIndividual<TGenotype, TFenotype> *pc_best_individual;

	time_t t_best_time;
	uint64_t i_best_ffe;

	CLog *pc_log;

	uint32_t i_random_seed;

	bool b_own_params;

private:
	void v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime);
};//class COptimizer

template <class TGenotype, class TFenotype>
class CSteadyStateOptimizer : public COptimizer<TGenotype, TFenotype>
{
public:
	CSteadyStateOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CSteadyStateOptimizer(CSteadyStateOptimizer<TGenotype, TFenotype> *pcOther);

	virtual bool bIsSteadyState() = 0;
};//class CSteadyStateOptimizer : public COptimizer<TGenotype, TFenotype>

#endif//OPTIMIZER_H