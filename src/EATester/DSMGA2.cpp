#include "DSMGA2.h"

#include "CommandParam.h"
#include "PopulationOptimizer.h"
#include "UIntCommandParam.h"

#include "../DSMGA2/chromosome.h"
#include "../DSMGA2/dsmga2.h"

#include <atlstr.h>
#include <list>

CDSMGA2::CDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{
	pc_dsmga2 = nullptr;
	Chromosome::injectedEvaluation = pcProblem->pcGetEvaluation();
	myRand.seed((unsigned long)iRandomSeed);
}//CDSMGA2::CDSMGA2(CDSMGA2Params *pcParams, CLog *pcLog, uint32_t iRandomSeed)

CDSMGA2::CDSMGA2(CDSMGA2 *pcOther)
	: CPopulationSizingSingleOptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{
	pc_dsmga2 = nullptr;
	i_population_size = pcOther->i_population_size;
	b_two_edge = pcOther->b_two_edge;
	b_comparative_mixing = pcOther->b_comparative_mixing;
}//CDSMGA2::CDSMGA2(CDSMGA2 *pcOther)

CDSMGA2::~CDSMGA2()
{
	delete pc_dsmga2;
}//CDSMGA2::~CDSMGA2()

CError CDSMGA2::eConfigure(istream *psSettings)
{
	CError c_error = COptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_population_size(POPULATION_OPTIMIZER_ARGUMENT_POPULATION_SIZE);
		i_population_size = p_population_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_two_edge(DSMGA2_ARGUMENT_TWO_EDGE);
		b_two_edge = p_two_edge.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_comparative_mixing(DSMGA2_ARGUMENT_COMPARATIVE_MIXING);
		b_comparative_mixing = p_comparative_mixing.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CDSMGA2::eConfigure(istream *psSettings)

void CDSMGA2::vInitialize(time_t tStartTime)
{
	COptimizer<CBinaryCoding, CBinaryCoding>::vInitialize(tStartTime);

	delete pc_dsmga2;
	pc_dsmga2 = new DSMGA2(pc_problem->pcGetEvaluation()->iGetNumberOfElements(), i_population_size, b_two_edge, b_comparative_mixing, -1, -1, 7, -1, pc_log);
}//void CDSMGA2::vInitialize(time_t tStartTime)

bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	return bRunIteration(iIterationNumber, tStartTime, nullptr);
}//bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime, CIndividual<CBinaryCoding, CBinaryCoding> *pcBestIndividual)
{
	if (pcBestIndividual)
	{
		Chromosome c_dsmga2_individual((int)pc_problem->pcGetEvaluation()->iGetNumberOfElements());
		
		for (uint16_t i = 0; i < pcBestIndividual->pcGetGenotype()->iGetNumberOfBits(); i++)
		{
			c_dsmga2_individual.setValF((int)i, (int)(*(pcBestIndividual->pcGetGenotype()->piGetBits() + i)));
		}//for (uint16_t i = 0; i < pcBestIndividual->pcGetGenotype()->iGetNumberOfBits(); i++)

		c_dsmga2_individual.setFitness(pcBestIndividual->dGetFitnessValue());
		c_dsmga2_individual.setEvaluated(true);

		pc_dsmga2->oneRun((int)iIterationNumber, false, &c_dsmga2_individual);
	}//if (pcBestIndividual)
	else
	{
		pc_dsmga2->oneRun((int)iIterationNumber, false);
	}//else if (pcBestIndividual)

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log_message;
	s_log_message.Format("best fitness: %f; best unitation: %f; ffe: %u; time: %u; population size: %u", pc_best_individual->dGetFitnessValue(),
		pc_best_individual->pcGetGenotype()->dGetUnitation(), pc_problem->pcGetEvaluation()->iGetFFE(), (uint32_t)(time(nullptr) - tStartTime),
		i_population_size);

	pc_log->vPrintEmptyLine();
	pc_log->vPrintLine(s_log_message, true);
	pc_log->vPrintEmptyLine();

	return b_updated;
}//bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime, CIndividual<CBinaryCoding, CBinaryCoding> *pcBestIndividual)

bool CDSMGA2::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime, pc_dsmga2->bestFitness, [&](CBinaryCoding *pcBestGenotype)
	{
		for (uint16_t i = 0; i < pc_problem->pcGetEvaluation()->iGetNumberOfElements(); i++)
		{
			*(pcBestGenotype->piGetBits() + i) = (int32_t)*(pc_dsmga2->bestSolution + i);
		}//for (uint16_t i = 0; i < pc_problem->pcGetEvaluation()->iGetNumberOfElements(); i++)
	});

	return b_updated;
}//bool CDSMGA2::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)