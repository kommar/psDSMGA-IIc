#include "LTGAOriginalPopulationSizing.h"

#include "UIntCommandParam.h"

CLTGAOriginalPopulationSizing::CLTGAOriginalPopulationSizing(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: COptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed), c_params_ltga(pcProblem, pcLog, iRandomSeed)
{

}//CLTGAOriginalPopulationSizing::CLTGAOriginalPopulationSizing(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CLTGAOriginalPopulationSizing::CLTGAOriginalPopulationSizing(CLTGAOriginalPopulationSizing *pcOther)
	: COptimizer<CBinaryCoding, CBinaryCoding>(pcOther), c_params_ltga(pcOther->c_params_ltga), c_population_sizing_counter(pcOther->c_population_sizing_counter)
{

}//CLTGAOriginalPopulationSizing::CLTGAOriginalPopulationSizing(CLTGAOriginalPopulationSizing *pcOther)

CLTGAOriginalPopulationSizing::~CLTGAOriginalPopulationSizing()
{
	v_clear_ltgas();
}//CLTGAOriginalPopulationSizing::~CLTGAOriginalPopulationSizing()

CError CLTGAOriginalPopulationSizing::eConfigure(istream *psSettings)
{
	CError c_error = COptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		c_error = c_params_ltga.eConfigure(psSettings);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_population_creation_frequency(LTGA_ORIGINAL_POPULATION_SIZING_ARGUMENT_POPULATION_CREATION_FREQUENCY);
		i_population_creation_frequency = p_population_creation_frequency.iGetValue(psSettings, &c_error);

		if (!c_error)
		{
			c_population_sizing_counter.vReset();
			
			if (!c_population_sizing_counter.bSetBase(i_population_creation_frequency))
			{
				c_error.vSetError("cannot set the base of population sizing counter");
			}//if (!c_population_sizing_counter.bSetBase(i_population_creation_frequency))
		}//if (!c_error)
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_population_size_multiplier(LTGA_ORIGINAL_POPULATION_SIZING_ARGUMENT_POPULATION_SIZE_MULTIPLIER);
		i_population_size_multiplier = p_population_size_multiplier.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CLTGAOriginalPopulationSizing::eConfigure(istream *psSettings)

void CLTGAOriginalPopulationSizing::vInitialize(time_t tStartTime)
{
	COptimizer<CBinaryCoding, CBinaryCoding>::vInitialize(tStartTime);

	v_clear_ltgas();

	i_next_population_size = c_params_ltga.iGetPopulationSize();
	v_add_new_ltga(0, tStartTime);

	c_population_sizing_counter.vReset();

	b_update_best_individual(0, tStartTime);
}//void CLTGAOriginalPopulationSizing::vInitialize(time_t tStartTime)

bool CLTGAOriginalPopulationSizing::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	if (c_population_sizing_counter.iIncrement() == (uint32_t)v_ltgas.size())
	{
		v_add_new_ltga(iIterationNumber, tStartTime);
	}//if (c_population_sizing_counter.iIncrement() == (uint32_t)v_ltgas.size())


	bool b_updated = false;

	if (b_run_proper_ltga_iteration(iIterationNumber, tStartTime))
	{
		b_updated = b_update_best_individual(iIterationNumber, tStartTime);
	}//if (b_run_proper_ltga_iteration(iIterationNumber, tStartTime))


	CString s_averages_fitnesses;

	s_averages_fitnesses.Append("[");

	for (size_t i = 0; i < v_ltgas_average_fitnesses.size(); i++)
	{
		s_averages_fitnesses.AppendFormat("%f", v_ltgas_average_fitnesses.at(i));

		if (i + 1 < v_ltgas_average_fitnesses.size())
		{
			s_averages_fitnesses.Append(", ");
		}//if (i + 1 < v_ltgas_average_fitnesses.size())
	}//for (size_t i = 0; i < v_ltgas_average_fitnesses.size(); i++)

	s_averages_fitnesses.Append("]");

	CString s_log;

	s_log.Format("iteration: %d; time: %u; number of ltgas: %d %s; best: %f", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		v_ltgas.size(), s_averages_fitnesses, pc_best_individual->dGetFitnessValue());

	pc_log->vPrintLine(s_log, true);
	pc_log->vPrintEmptyLine(true);


	v_delete_ltgas();


	return b_updated;
}//bool CLTGAOriginalPopulationSizing::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

bool CLTGAOriginalPopulationSizing::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)
{
	CLTGAOriginal *pc_ltga = v_ltgas.at((size_t)c_population_sizing_counter.iGetLastMostSignificantChangeIndex());
	CIndividual<CBinaryCoding, CBinaryCoding> *pc_ltga_best_individual = pc_ltga->pcGetBestIndividual();

	bool b_updated = false;

	if (pc_ltga_best_individual)
	{
		if (b_update_best_individual(iIterationNumber, tStartTime, pc_ltga_best_individual))
		{
			b_updated = true;
		}//if (b_update_best_individual(iIterationNumber, tStartTime, pc_ltga_best_individual))
	}//if (pc_ltga_best_individual)

	return b_updated;
}//bool CLTGAOriginalPopulationSizing::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)

void CLTGAOriginalPopulationSizing::v_clear_ltgas()
{
	for (size_t i = 0; i < v_ltgas.size(); i++)
	{
		delete v_ltgas.at(i);
	}//for (size_t i = 0; i < v_ltgas.size(); i++)

	v_ltgas.clear();
	v_ltgas_average_fitnesses.clear();
}//void CLTGAOriginalPopulationSizing::v_clear_ltgas()

bool CLTGAOriginalPopulationSizing::b_run_proper_ltga_iteration(uint32_t iIterationNumber, time_t tStartTime)
{
	uint32_t i_proper_ltga_index = c_population_sizing_counter.iGetLastMostSignificantChangeIndex();
	CLTGAOriginal *pc_proper_ltga = v_ltgas.at((size_t)i_proper_ltga_index);
	
	bool b_updated = pc_proper_ltga->bRunIteration(iIterationNumber, tStartTime);
	v_ltgas_average_fitnesses.at((size_t)i_proper_ltga_index) = pc_proper_ltga->dComputeAverageFitnessValue();

	return b_updated;
}//bool CLTGAOriginalPopulationSizing::b_run_proper_ltga_iteration(uint32_t iIterationNumber, time_t tStartTime)

void CLTGAOriginalPopulationSizing::v_delete_ltgas()
{
	unordered_set<size_t> s_ltgas_to_delete_indexes;
	s_ltgas_to_delete_indexes.reserve(v_ltgas.size());

	v_get_ltgas_to_delete(&s_ltgas_to_delete_indexes);

	if (!s_ltgas_to_delete_indexes.empty())
	{
		for (size_t i = v_ltgas.size(); i > 0; i--)
		{
			if (s_ltgas_to_delete_indexes.count(i - 1) > 0)
			{
				delete v_ltgas.at(i - 1);
				v_ltgas.erase(v_ltgas.begin() + i - 1);
				v_ltgas_average_fitnesses.erase(v_ltgas_average_fitnesses.begin() + i - 1);
			}//if (s_ltgas_to_delete_indexes.count(i - 1) > 0)
		}//for (size_t i = v_ltgas.size(); i > 0; i--)

		c_population_sizing_counter.vReset();
	}//if (!s_ltgas_to_delete_indexes.empty())
}//void CLTGAOriginalPopulationSizing::v_delete_ltgas()

void CLTGAOriginalPopulationSizing::v_get_ltgas_to_delete(unordered_set<size_t> *psIndexes)
{
	size_t i_last_iteration_ltga_index = (size_t)c_population_sizing_counter.iGetLastMostSignificantChangeIndex();

	if (v_ltgas.at(i_last_iteration_ltga_index)->bAreAllIndividualsTheSame())
	{
		psIndexes->insert(i_last_iteration_ltga_index);
	}//if (v_ltgas.at(i_last_iteration_ltga_index)->bAreAllIndividualsTheSame())
	else
	{
		bool b_to_delete = false;

		double d_last_iteration_ltga_average_fitness = v_ltgas_average_fitnesses.at(i_last_iteration_ltga_index);

		for (size_t i = i_last_iteration_ltga_index + 1; i < v_ltgas.size() && !b_to_delete; i++)
		{
			b_to_delete = d_last_iteration_ltga_average_fitness < v_ltgas_average_fitnesses.at(i);
		}//for (size_t i = i_last_iteration_ltga_index + 1; i < v_ltgas.size() && !b_to_delete; i++)

		if (b_to_delete)
		{
			for (size_t i = 0; i <= i_last_iteration_ltga_index; i++)
			{
				psIndexes->insert(i);
			}//for (size_t i = 0; i <= i_last_iteration_ltga_index; i++)
		}//if (b_to_delete)

		for (size_t i = i_last_iteration_ltga_index && !b_to_delete; i > 0; i--)
		{
			b_to_delete = v_ltgas_average_fitnesses.at(i - 1) < d_last_iteration_ltga_average_fitness;

			if (b_to_delete)
			{
				for (size_t j = 0; j <= i - 1; j++)
				{
					psIndexes->insert(j);
				}//for (size_t j = 0; j <= i - 1; j++)
			}//if (b_to_delete)
		}//for (size_t i = i_last_iteration_ltga_index; i > 0; i--)
	}//else if (v_ltgas.at(i_last_iteration_ltga_index)->bAreAllIndividualsTheSame())
}//void CLTGAOriginalPopulationSizing::v_get_ltgas_to_delete(unordered_set<size_t> *psIndexes)

void CLTGAOriginalPopulationSizing::v_add_new_ltga(uint32_t iIterationNumber, time_t tStartTime)
{
	CLTGAOriginal *pc_ltga = new CLTGAOriginal(&c_params_ltga);

	pc_ltga->vSetPopulationSize(i_next_population_size);
	pc_ltga->vInitialize(tStartTime);
	
	v_ltgas.push_back(pc_ltga);
	v_ltgas_average_fitnesses.push_back(pc_ltga->dComputeAverageFitnessValue());

	i_next_population_size *= i_population_size_multiplier;
}//void CLTGAOriginalPopulationSizing::v_add_new_ltga(uint32_t iIterationNumber, time_t tStartTime)