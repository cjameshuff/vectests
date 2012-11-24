
#include "vectest.h"

void TestExtension(float4 * dst, const float4 * srcA, const float4 * srcB, const float4 * srcC, size_t len)
{
    float4 result = (float4)(0);
    for(int j = 0; j < len; ++j)
        // dst[j] = srcA[j] + srcB[j]*srcC[j];
        // dst[j] = srcA[j] + srcB[j]*srcC[0];
        result += srcA[j] + srcB[j]*srcC[0];
    *dst = result;
}

void TestIntrinsics(float4 * dst, const float4 * srcA, const float4 * srcB, const float4 * srcC, size_t len)
{
    float4 result = (float4)(0);
    // for(int j = 0; j < len; ++j)
    //     dst[j] = _mm_add_ps(srcA[j], _mm_mul_ps(srcB[j], srcC[j]));
    for(int j = 0; j < len; ++j)
        // *dst++ = _mm_add_ps(*srcA++, _mm_mul_ps(*srcB++, *srcC++));
        // *dst++ = _mm_add_ps(*srcA++, _mm_mul_ps(*srcB++, srcC[0]));
        result = _mm_add_ps(result, _mm_add_ps(*srcA++, _mm_mul_ps(*srcB++, srcC[0])));
    *dst = result;
}

void TestStruct(fvec4 * dst, const fvec4 * srcA, const fvec4 * srcB, const fvec4 * srcC, size_t len)
{
    fvec4 result(0, 0, 0, 0);
    for(int j = 0; j < len; ++j)
        // dst[j] = srcA[j] + srcB[j]*srcC[j];
        // dst[j] = srcA[j] + srcB[j]*srcC[0];
        result = result + srcA[j] + srcB[j]*srcC[0];
    *dst = result;
}

void TestArray(float * dst, const float * srcA, const float * srcB, const float * srcC, size_t len)
{
    float result[4] = {0, 0, 0, 0};
    for(int j = 0; j < len; ++j)
    {
        for(int k = 0; k < 4; ++k)
            // *dst++ = *srcA++ + (*srcB++)*srcC[k];
            // *dst++ = *srcA++ + (*srcB++)*srcC[0];
            result[k] += *srcA++ + (*srcB++)*srcC[0];
    }
    for(int k = 0; k < 4; ++k)
        dst[k] = result[k];
}



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
float TraceTestR(const fray & ray, const float4 & center, float radiusSqr)
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


float Sphere::TraceTestR(const fray & ray)
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


void TrackParticle(const std::vector<SParticle> & background, SParticle & particle, int iterations)
{
    for(int j = 0; j < iterations; ++j)
    {
        // Compute background forces at particle location
        fvec4 a = 0;
        for(auto & b : background) {
            fvec4 dir = (b.pos - particle.pos);
            float distSqr = vlength_sqr3(dir);
            float dist = sqrt(distSqr);
            a = a + dir/(dist*distSqr*1000.0f);
        }
        particle.vel = particle.vel + a;
        particle.pos = particle.pos + particle.vel;
    }
}

void TrackParticle(const std::vector<Particle> & background, Particle & particle, int iterations)
{
    for(int j = 0; j < iterations; ++j)
    {
        // Compute background forces at particle location
        float4 a = 0;
        for(auto & b : background) {
            float4 dir = (b.pos - particle.pos);
            float distSqr = vlength_sqr3(dir);
            float dist = sqrt(distSqr);
            a = a + dir/(dist*distSqr*1000.0f);
        }
        particle.vel = particle.vel + a;
        particle.pos = particle.pos + particle.vel;
    }
}


void TrackParticleI(const std::vector<Particle> & background, Particle & particle, int iterations)
{
    for(int j = 0; j < iterations; ++j)
    {
        // Compute background forces at particle location
        float4 a = 0;
        for(auto & b : background) {
            float4 dir = (b.pos - particle.pos);
            float distSqr = vlength_sqr3_i(dir);
            float dist = sqrt(distSqr);
            a = a + dir/(dist*distSqr*1000.0f);
        }
        particle.vel = particle.vel + a;
        particle.pos = particle.pos + particle.vel;
    }
}

