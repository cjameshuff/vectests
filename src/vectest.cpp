
#include "vectest.h"

#include <iostream>

using namespace std;

void TestExtension(std::vector<float4> & dst, const std::vector<float4> & srcA,
                   const std::vector<float4> & srcB, const std::vector<float4> & srcC, size_t len)
{
    float4 result = (float4)(0);
    for(int j = 0; j < len; ++j)
        // dst[j] = srcA[j] + srcB[j]*srcC[j];
        // dst[j] = srcA[j] + srcB[j]*srcC[0];
        result += srcA[j] + srcB[j]*srcC[0];
    dst[0] = result;
}

void TestIntrinsics(std::vector<float4> & dst, const std::vector<float4> & srcA,
                    const std::vector<float4> & srcB, const std::vector<float4> & srcC, size_t len)
{
    float4 result = (float4)(0);
    // for(int j = 0; j < len; ++j)
    //     dst[j] = _mm_add_ps(srcA[j], _mm_mul_ps(srcB[j], srcC[j]));
    auto a = srcA.begin();
    auto b = srcB.begin();
    for(int j = 0; j < len; ++j)
        // *dst++ = _mm_add_ps(*srcA++, _mm_mul_ps(*srcB++, *srcC++));
        // *dst++ = _mm_add_ps(*srcA++, _mm_mul_ps(*srcB++, srcC[0]));
        result = _mm_add_ps(result, _mm_add_ps(*a++, _mm_mul_ps(*b++, srcC[0])));
    dst[0] = result;
}

void TestStruct(std::vector<fvec4> & dst, const std::vector<fvec4> & srcA,
                const std::vector<fvec4> & srcB, const std::vector<fvec4> & srcC, size_t len)
{
    fvec4 result(0);
    for(int j = 0; j < len; ++j)
        // dst[j] = srcA[j] + srcB[j]*srcC[j];
        // dst[j] = srcA[j] + srcB[j]*srcC[0];
        result = result + srcA[j] + srcB[j]*srcC[0];
    dst[0] = result;
}

void TestArray(std::vector<float> & dst, const std::vector<float> & srcA,
               const std::vector<float> & srcB, const std::vector<float> & srcC, size_t len)
{
#ifdef USE_4D_STRUCTS_ARRAYS
    float result[4] = {0, 0, 0, 0};
    auto a = srcA.begin();
    auto b = srcB.begin();
    for(int j = 0; j < len; ++j)
    {
        for(int k = 0; k < 4; ++k)
            // *dst++ = *srcA++ + (*srcB++)*srcC[k];
            // *dst++ = *srcA++ + (*srcB++)*srcC[0];
            result[k] += *a++ + (*b++)*srcC[0];
    }
    for(int k = 0; k < 4; ++k)
        dst[k] = result[k];
#else
    float result[3] = {0, 0, 0};
    auto a = srcA.begin();
    auto b = srcB.begin();
    for(int j = 0; j < len; ++j)
    {
        for(int k = 0; k < 3; ++k)
            // *dst++ = *srcA++ + (*srcB++)*srcC[k];
            // *dst++ = *srcA++ + (*srcB++)*srcC[0];
            result[k] += *a++ + (*b++)*srcC[0];
    }
    for(int k = 0; k < 3; ++k)
        dst[k] = result[k];
#endif
}

