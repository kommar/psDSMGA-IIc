#include "LTGA.h"

#include <atlstr.h>
#include <vector>

CLTGA::CLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CP3BasedOptimizer("LTGA", pcProblem, pcLog, iRandomSeed)
{

}//CLTGA::CLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CLTGA::CLTGA(CLTGA *pcOther)
	: CP3BasedOptimizer(pcOther)
{

}//CLTGA::CLTGA(CLTGA *pcOther)

void CLTGA::vInitialize(time_t tStartTime)
{
	CP3BasedOptimizer::vInitialize(tStartTime);
	pc_ltga = static_pointer_cast<LTGA>(pc_optimizer);
}//void CLTGA::vInitialize(time_t tStartTime)

bool CLTGA::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CP3BasedOptimizer::bRunIteration(iIterationNumber, tStartTime);

	return b_updated;
}//bool CLTGA::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)