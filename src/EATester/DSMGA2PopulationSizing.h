#ifndef DSMGA2_POPULATION_SIZING_H
#define DSMGA2_POPULATION_SIZING_H

#include "BinaryCoding.h"
#include "Error.h"
#include "Optimizer.h"
#include "PopulationSizingOptimizer.h"
#include "Problem.h"

#include <ctime>
#include <cstdint>
#include <istream>

using namespace std;

class CDSMGA2PopulationSizing : public CPopulationSizingOptimizer<CBinaryCoding, CBinaryCoding>
{
public:
	CDSMGA2PopulationSizing(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CDSMGA2PopulationSizing(CDSMGA2PopulationSizing *pcOther);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CDSMGA2PopulationSizing(this); };

protected:
	virtual CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding>* pc_get_params_optimizer(istream *psSettings, CError *pcError);

	virtual bool b_run_proper_optimizer_iteration(CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding>* pcProperOptimizer, uint32_t iIterationNumber, time_t tStartTime);
};//class CDSMGA2PopulationSizing : public CPopulationSizingOptimizer<CBinaryCoding, CBinaryCoding>

#endif//DSMGA2_POPULATION_SIZING_H