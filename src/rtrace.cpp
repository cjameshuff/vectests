
#include "vectest.h"

#include <iostream>

using namespace std;


float TraceTestE(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr)
{
    float4 sphDir = center - rayOrigin;
    
    float sphDistSqr = vlength_sqr3(sphDir);
    float closest = vdot3(rayDir, sphDir);
    float halfChordLenSqr = radiusSqr - sphDistSqr + closest*closest;
    if(halfChordLenSqr > 1.0e-5f)
    {
        float halfChordLen = sqrt(halfChordLenSqr);
        if(sphDistSqr <= radiusSqr) {//inside sphere, take second intersection
            return closest + halfChordLen;
        }
        else {//outside sphere, take first intersection
            return closest - halfChordLen;
        }
    }
    
    return 0.0f;
}

float TraceTestI(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr)
{
    float4 sphDir = center - rayOrigin;
    
    float4 sphDistSqr = _mm_dp_ps(sphDir, sphDir, 0x71);
    float4 closest = _mm_dp_ps(rayDir, sphDir, 0x71);
    float halfChordLenSqr = _mm_cvtss_f32(_mm_sub_ss(_mm_add_ss(_mm_mul_ss(closest, closest), (float4)radiusSqr), sphDistSqr));
    if(halfChordLenSqr > 1.0e-5f)
    {
        float halfChordLen = sqrt(halfChordLenSqr);
        if(_mm_cvtss_f32(sphDistSqr) <= radiusSqr) {//inside sphere, take second intersection
            return _mm_cvtss_f32(closest) + halfChordLen;
        }
        else {//outside sphere, take first intersection
            return _mm_cvtss_f32(closest) - halfChordLen;
        }
    }
    
    // float sphDistSqr = vlength_sqr3_i(sphDir);
    // float closest = vdot3_i(rayDir, sphDir);
    // float halfChordLenSqr = radiusSqr - sphDistSqr + closest*closest;
    // if(halfChordLenSqr > 1.0e-5f)
    // {
    //     float halfChordLen = sqrt(halfChordLenSqr);
    //     if(sphDistSqr <= radiusSqr) {//inside sphere, take second intersection
    //         return closest + halfChordLen;
    //     }
    //     else {//outside sphere, take first intersection
    //         return closest - halfChordLen;
    //     }
    // }
    
    return 0.0f;
}

float TraceTestS(const fvec4 & rayOrigin, const fvec4 & rayDir, const fvec4 & center, float radiusSqr)
{
    fvec4 sphDir = center - rayOrigin;
    
    float sphDistSqr = vlength_sqr3(sphDir);
    float closest = vdot3(rayDir, sphDir);
    float halfChordLenSqr = radiusSqr - sphDistSqr + closest*closest;
    if(halfChordLenSqr > 1.0e-5f)
    {
        float halfChordLen = sqrt(halfChordLenSqr);
        if(sphDistSqr <= radiusSqr) {//inside sphere, take second intersection
            return closest + halfChordLen;
        }
        else {//outside sphere, take first intersection
            return closest - halfChordLen;
        }
    }
    
    return 0.0f;
}

// float TraceTestR(const fray & ray, const fvec4 & center, float radiusSqr)
float TraceTestR(const fray<float> & ray, const float4 & center, float radiusSqr)
{
    float4 sphDir = center - ray.origin;
    
    float sphDistSqr = vlength_sqr3(sphDir);
    float closest = vdot3(ray.dir, sphDir);
    float halfChordLenSqr = radiusSqr - sphDistSqr + closest*closest;
    if(halfChordLenSqr > 1.0e-5f)
    {
        float halfChordLen = sqrt(halfChordLenSqr);
        if(sphDistSqr <= radiusSqr) {//inside sphere, take second intersection
            return closest + halfChordLen;
        }
        else {//outside sphere, take first intersection
            return closest - halfChordLen;
        }
    }
    
    return 0.0f;
}


float Sphere::TraceTestR(const fray<float> & ray)
{
    vec4<float> sphDir = center - ray.origin;
    
    float sphDistSqr = vlength_sqr3(sphDir);
    float closest = vdot3(ray.dir, sphDir);
    float halfChordLenSqr = radiusSqr - sphDistSqr + closest*closest;
    if(halfChordLenSqr > 1.0e-5f)
    {
        float halfChordLen = sqrt(halfChordLenSqr);
        if(sphDistSqr <= radiusSqr) {//inside sphere, take second intersection
            return closest + halfChordLen;
        }
        else {//outside sphere, take first intersection
            return closest - halfChordLen;
        }
    }
    
    return 0.0f;
}
