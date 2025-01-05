#include "DSMGA2PopulationSizing.h"

#include "DSMGA2.h"

CDSMGA2PopulationSizing::CDSMGA2PopulationSizing(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CPopulationSizingOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{

}//CDSMGA2PopulationSizing::CDSMGA2PopulationSizing(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CDSMGA2PopulationSizing::CDSMGA2PopulationSizing(CDSMGA2PopulationSizing *pcOther)
	: CPopulationSizingOptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{

}//CDSMGA2PopulationSizing::CDSMGA2PopulationSizing(CDSMGA2PopulationSizing *pcOther)

CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding> * CDSMGA2PopulationSizing::pc_get_params_optimizer(istream *psSettings, CError *pcError)
{
	CDSMGA2 *pc_dsmga = new CDSMGA2(pc_problem, pc_log, i_random_seed);
	*pcError = pc_dsmga->eConfigure(psSettings);

	return pc_dsmga;
}//CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding> * CDSMGA2PopulationSizing::pc_get_params_optimizer(istream *psSettings, CError *pcError)

bool CDSMGA2PopulationSizing::b_run_proper_optimizer_iteration(CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding> *pcProperOptimizer, uint32_t iIterationNumber, time_t tStartTime)
{
	CDSMGA2 *pc_proper_dsmga2 = (CDSMGA2*)pcProperOptimizer;
	return pc_proper_dsmga2->bRunIteration(iIterationNumber, tStartTime, pc_best_individual);
}//bool CDSMGA2PopulationSizing::b_run_proper_optimizer_iteration(CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding> *pcProperOptimizer, uint32_t iIterationNumber, time_t tStartTime)