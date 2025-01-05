#include "BinaryCoding.h"

using namespace std;

CBinaryCoding::CBinaryCoding(uint16_t iNumberOfBits)
{
	i_number_of_bits = iNumberOfBits;
	pi_bits = new int32_t[iNumberOfBits];
	b_own_bits = true;
}//CBinaryCoding::CBinaryCoding(uint16_t iNumberOfBits, int32_t *piBits)

CBinaryCoding::CBinaryCoding(uint16_t iNumberOfBits, int32_t *piBits, bool bOwnBits)
{
	i_number_of_bits = iNumberOfBits;
	pi_bits = piBits;
	b_own_bits = bOwnBits;
}//CBinaryCoding::CBinaryCoding(uint16_t iNumberOfBits, int32_t *piBits, bool bOwnBits)

CBinaryCoding::CBinaryCoding(CBinaryCoding *pcOther)
{
	i_number_of_bits = pcOther->i_number_of_bits;

	pi_bits = new int32_t[pcOther->i_number_of_bits];
	
	for (uint16_t i = 0; i < pcOther->i_number_of_bits; i++)
	{
		*(pi_bits + i) = *(pcOther->pi_bits + i);
	}//for (uint16_t i = 0; i < pcOther->i_number_of_bits; i++)

	b_own_bits = true;
}//CBinaryCoding::CBinaryCoding(CBinaryCoding *pcOther)

CBinaryCoding::~CBinaryCoding()
{
	if (b_own_bits)
	{
		delete pi_bits;
	}//if (b_own_bits)
}//CBinaryCoding::~CBinaryCoding()

void CBinaryCoding::vMerge(CBinaryCoding *pcOther)
{
	//TODO
}//void CBinaryCoding::vMerge(CBinaryCoding *pcOther)



bool  CBinaryCoding::bGenotypesDiffer(CBinaryCoding *pcOther)
{
	if (iGetNumberOfBits() != pcOther->iGetNumberOfBits())  return(true);

	for (int ii = 0; ii < iGetNumberOfBits(); ii++)
	{
		if (pi_bits[ii] != pcOther->pi_bits[ii])  return(true);
	}//for (int ii = 0; ii < iGetNumberOfBits(); ii++)

	return(false);
}//bool  CBinaryCoding::bGenotypesDiffer(CBinaryCoding *pcOther)



int  CBinaryCoding::iGetNumberOfDifferentBits(CBinaryCoding *pcOther)
{
	int  i_result;

	if (iGetNumberOfBits() != pcOther->iGetNumberOfBits())  return(-1);

	i_result = 0;
	for (int ii = 0; ii < iGetNumberOfBits(); ii++)
	{
		if (pi_bits[ii] != pcOther->pi_bits[ii])  i_result++;
	}//for (int ii = 0; ii < iGetNumberOfBits(); ii++)

	return(i_result);
}//int  CBinaryCoding::iGetNumberOfDifferentBits(CBinaryCoding *pcOther)



double CBinaryCoding::dComputeDistance(CBinaryCoding *pcOther, CGenePattern *pcGenePattern)
{
	double d_distance = 0;

	uint16_t i_gene_index;

	for (uint16_t i = 0; i < pcGenePattern->iGetSize(); i++)
	{
		i_gene_index = *(pcGenePattern->piGetPattern() + i);

		if (*(pi_bits + i_gene_index) != *(pcOther->pi_bits + i_gene_index))
		{
			d_distance++;
		}//if (*(pi_bits + i_gene_index) != *(pcOther->pi_bits + i_gene_index))
	}//for (uint16_t i = 0; i < pcGenePattern->iGetSize(); i++)

	return d_distance;
}//double CBinaryCoding::dComputeDistance(CBinaryCoding *pcOther, CGenePattern *pcGenePattern)

bool CBinaryCoding::bExceededMaxDistance(CBinaryCoding *pcOther, CGenePattern *pcGenePattern, double dMaxDistance, double *pdDistance, bool bOnlyPattern)
{
	double d_distance = 0;

	uint16_t i_gene_index;

	if (bOnlyPattern)
	{
		for (uint16_t i = 0; i < pcGenePattern->iGetSize() && d_distance < dMaxDistance; i++)
		{
			i_gene_index = *(pcGenePattern->piGetPattern() + i);

			if (*(pi_bits + i_gene_index) != *(pcOther->pi_bits + i_gene_index))
			{
				d_distance++;
			}//if (*(pi_bits + i_gene_index) != *(pcOther->pi_bits + i_gene_index))
		}//for (uint16_t i = 0; i < pcGenePattern->iGetSize() && d_distance < dMaxDistance; i++)
	}//if (bOnlyPattern)
	else
	{
		uint16_t i_gene_pattern_gene_index = 0;

		for (uint16_t i = 0; i < i_number_of_bits && d_distance <= dMaxDistance; i++)
		{
			if (i < *(pcGenePattern->piGetPattern() + i_gene_pattern_gene_index))
			{
				if (*(pi_bits + i) != *(pcOther->pi_bits + i))
				{
					d_distance++;
				}//if (*(pi_bits + i) != *(pcOther->pi_bits + i))
			}//if (i < *(pcGenePattern->piGetPattern() + i_gene_pattern_gene_index))
			else
			{
				i_gene_pattern_gene_index++;
			}//else if (i < *(pcGenePattern->piGetPattern() + i_gene_pattern_gene_index))
		}//for (uint16_t i = 0; i < i_number_of_bits && d_distance <= dMaxDistance; i++)
	}//else if (bOnlyPattern)

	bool b_exceeded = d_distance > dMaxDistance;

	if (!b_exceeded)
	{
		*pdDistance = d_distance;
	}//if (!b_exceeded)

	return b_exceeded;
}//bool CBinaryCoding::bExceededMaxDistance(CBinaryCoding *pcOther, CGenePattern *pcGenePattern, double dMaxDistance, double *pdDistance, bool bOnlyPattern)

#include <iostream>


int  CBinaryCoding::iGetUnitation()
{
	uint16_t i_number_of_ones = 0;

	for (uint16_t i = 0; i < i_number_of_bits; i++)
	{
		if (*(pi_bits + i) == 1)
		{
			i_number_of_ones++;
		}//if (*(pi_bits + i) == 1)
	}//for (uint16_t i = 0; i < i_number_of_bits; i++)

	return(i_number_of_ones);
}//int  CBinaryCoding::iGetUnitation()



double CBinaryCoding::dGetUnitation()
{
	uint16_t i_number_of_ones = 0;
	i_number_of_ones = iGetUnitation();
	
	return (double)i_number_of_ones / (double)i_number_of_bits;
}//double CBinaryCoding::dGetUnitation()

void CBinaryCoding::vSetBits(int32_t *piBits, bool bOwnBits)
{
	if (b_own_bits)
	{
		delete pi_bits;
	}//if (b_own_bits)

	pi_bits = piBits;
	b_own_bits = bOwnBits;
}//void CBinaryCoding::vSetBits(int32_t *piBits, bool bOwnBits)

CString CBinaryCoding::sToString(uint16_t iSpaceFrequency)
{
	CString s_result;

	for (uint16_t i = 0; i < i_number_of_bits; i++)
	{
		if (i > 0 && i % iSpaceFrequency == 0)
		{
			s_result.Append(" ");
		}//if (i > 0 && i % iSpaceFrequency == 0)

		s_result.AppendFormat("%d", *(pi_bits + i));
	}//for (uint16_t i = 0; i < i_number_of_bits; i++)

	return s_result;
}//CString CBinaryCoding::sToString(uint16_t iSpaceFrequency)

bool CBinaryCoding::operator==(CBinaryCoding &cOther)
{
	bool b_equal = true;

	for (uint16_t i = 0; i < i_number_of_bits && b_equal; i++)
	{
		b_equal &= *(pi_bits + i) == *(cOther.pi_bits + i);
	}//for (uint16_t i = 0; i < i_number_of_bits && b_equal; i++)

	return b_equal;
}//bool CBinaryCoding::operator==(CBinaryCoding &cOther)

bool CBinaryCoding::operator!=(CBinaryCoding &cOther)
{
	return !(*this == cOther);
}//bool CBinaryCoding::operator!=(CBinaryCoding &cOther)

ostream& operator<<(ostream &sOutput, CBinaryCoding *pcBinaryCoding)
{
	sOutput << pcBinaryCoding->sToString();

	return sOutput;
}//ostream& operator<<(ostream &sOutput, CBinaryCoding &cBinaryCoding)