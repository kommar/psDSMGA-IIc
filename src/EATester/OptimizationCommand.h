#ifndef OPTIMIZATION_COMMAND_H
#define OPTIMIZATION_COMMAND_H

#include "BinaryCoding.h"
#include "CommandExecution.h"
#include "CommandParamParser.h"
#include "Error.h"

#include <atlstr.h>
#include <iostream>

using namespace std;


class COptimizationBasedCommand : public CCommandExecution
{
public:
	COptimizationBasedCommand(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);

protected:
	virtual CError e_optimize_binary_binary(uint32_t iRandomSeed) = 0;
};//class COptimizationBasedCommand : public CCommandExecution


class COptimizationCommand : public COptimizationBasedCommand
{
public:
	COptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary);

protected:
	virtual CError e_optimize_binary_binary(uint32_t iRandomSeed) { return e_optimize<CBinaryCoding, CBinaryCoding>(iRandomSeed); };

private:
	template <class TGenotype, class TFenotype>
	CError e_optimize(uint32_t iRandomSeed);
};//class COptimizationCommand : public COptimizationBasedCommand


#endif//OPTIMIZATION_COMMAND_H