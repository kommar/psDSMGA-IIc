#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#ifndef DEF_TWO_PI
#define DEF_TWO_PI 2.0 * 3.14159265
#endif//DEF_TWO_PI

#include <cfloat>
#include <cstdint>
#include <functional>

using namespace std;

namespace MathUtils
{
	double dComputeAngle(double *pdValues0, double *pdValues1, uint32_t iLength);
	double dComputeDotProduct(double *pdValues0, double *pdValues1, uint32_t iLength);
	double dComputeSecondNorm(double *pdValues, uint32_t iLength);
	double dComputeDistance(double *pdValues0, double *pdValues1, uint32_t iLength, double dMaxValue = DBL_MAX);
	double dComputeSquareDistance(double *pdValues0, double *pdValues1, uint32_t iLength, double dMaxValue = DBL_MAX);

	double dComputeEntropy(uint32_t *piCounts, uint32_t iLength);
	double dComputeEntropy(uint32_t *piCounts, uint32_t iLength, uint32_t iTotal);

	double dMaxValue(double *pdValues, uint32_t iLength);
	double dSum(double *pdValues, uint32_t iLength);
	double dMean(double *pdValues, uint32_t iLength);

	double dComputeMonteCarloIntegral(function<double(double*)> fFunction, double dMaxFunctionValue, double *pdMinValues, 
		double *pdMaxValues, uint8_t iNumberOfDimensions, uint32_t iNumberOfSamples);

	void vNormalize(double *pcValues, uint32_t iLength);
	double dNormalize(double dValue, double dMinValue, double dMaxValue);

	double dComputeGamma(double dMuM, double dN);
}//namespace MathUtils

#endif//MATH_UTILS_H