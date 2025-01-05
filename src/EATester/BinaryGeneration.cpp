#include "BinaryGeneration.h"

#include "CommandParam.h"
#include "FloatCommandParam.h"
#include "GenePattern.h"
#include "RandUtils.h"

#include <vector>

using namespace std;

CBinaryGeneration::CBinaryGeneration(uint16_t iNumberOfBits)
{
	i_number_of_bits = iNumberOfBits;
}//CBinaryGeneration::CBinaryGeneration(uint16_t iNumberOfBits)

CBinaryGeneration::CBinaryGeneration(CBinaryCoding *pcSample)
	: CBinaryGeneration(pcSample->iGetNumberOfBits())
{

}//CBinaryGeneration::CBinaryGeneration(CBinaryCoding *pcSample)


CBinaryRandomGeneration::CBinaryRandomGeneration(uint16_t iNumberOfBits)
	: CBinaryGeneration(iNumberOfBits)
{

}//CBinaryRandomGeneration::CBinaryRandomGeneration(uint16_t iNumberOfBits)

CBinaryRandomGeneration::CBinaryRandomGeneration(CBinaryCoding *pcSample)
	: CBinaryGeneration(pcSample)
{

}//CBinaryRandomGeneration::CBinaryRandomGeneration(CBinaryCoding *pcSample)

CBinaryCoding * CBinaryRandomGeneration::pcGenerate()
{
	int32_t *pi_bits = new int32_t[i_number_of_bits];

	for (uint16_t i = 0; i < i_number_of_bits; i++)
	{
		*(pi_bits + i) = RandUtils::iRandNumber(0, 1);
	}//for (uint16_t i = 0; i < i_number_of_bits; i++)

	return new CBinaryCoding(i_number_of_bits, pi_bits);
}//CBinaryCoding * CBinaryRandomGeneration::pcGenerate()