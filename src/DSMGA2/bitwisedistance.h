
#ifndef _BITWISEDISTANCE_H_
#define _BITWISEDISTANCE_H_

#include <intrin.h>

class BitwiseDistance {

public:

    int countOne(unsigned long bitString) {
#if _WIN32
		return __popcnt(bitString);
#else
		return __popcnt64(bitString);
#endif//_WIN32
    }

    int getHammingDistance(unsigned long a, unsigned long b) {
        return countOne(a^b);
    }

};


#endif
