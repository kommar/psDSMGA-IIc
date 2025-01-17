#ifndef BINARY_GENERATION_H
#define BINARY_GENERATION_H

#include "BinaryCoding.h"
#include "Error.h"
#include "Generation.h"

#include <cstdint>
#include <istream>

using namespace std;

class CBinaryGeneration : public CGeneration<CBinaryCoding>
{
public:
	CBinaryGeneration(uint16_t iNumberOfBits);
	CBinaryGeneration(CBinaryCoding *pcSample);

	virtual CBinaryCoding *pcGenerate() { return pcGenerateEmpty(); };
	virtual CBinaryCoding *pcGenerateEmpty() { return new CBinaryCoding(i_number_of_bits); };

protected:
	uint16_t i_number_of_bits;
};//class CBinaryGeneration

class CBinaryRandomGeneration : public CBinaryGeneration
{
public:
	CBinaryRandomGeneration(uint16_t iNumberOfBits);
	CBinaryRandomGeneration(CBinaryCoding *pcSample);

	virtual CBinaryCoding *pcGenerate();

	virtual void vUpdateBestGenotypes(uint8_t iCount, CBinaryCoding **ppcBestGenotypes) { };
};//class CBinaryRandomGeneration

#endif//BINARY_GENERATION_H