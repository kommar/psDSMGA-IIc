#ifndef P3_H
#define P3_H

#define P3_ARGUMENT_FEEDBACK "with_feedback"
#define P3_ARGUMENT_FEEDBACK_PROBABILITY "feedback_probability"


#include "BinaryCoding.h"
#include "Error.h"
#include "Log.h"
#include "P3BasedOptimizer.h"
#include "Optimizer.h"
#include "Problem.h"

#include "BinaryOptimizer.h"

#include "../P3/Pyramid.h"

#include <atlstr.h>
#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

class CP3 : public CP3BasedOptimizer
{
public:
	CP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CP3(CP3 *pcOther);

	virtual CError eConfigure(istream *psSettings);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CP3(this); };

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

	virtual void vRun();

	bool bClimb(CBinaryCoding *pcGenotype, uint32_t iIterationNumber, time_t tStartTime);

private:
	void v_reverse_ghc(CIndividual<CBinaryCoding, CBinaryCoding> *pcIndividual);

	CString s_create_linkage_report();
	void v_log_linkage(bool bEcho = false);

	bool b_with_feedback;
	float f_feedback_probability;

	uint32_t i_feedback_counter;
	uint32_t i_effective_feedback_counter;

	shared_ptr<Pyramid> pc_p3;
};//class CP3 : public COptimizer<CBinaryCoding, CBinaryCoding>

#endif//P3_H