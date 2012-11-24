#ifndef VECTEST_H
#define VECTEST_H

#include <cstdlib>
#include <cmath>
#include <vector>

#include "smmintrin.h"

typedef float float4 __attribute__((ext_vector_type(4)));

template<typename T>
typedef T vec4 __attribute__((ext_vector_type(4)));


struct fvec4 {
    float x, y, z, w;
    fvec4() {}
    constexpr fvec4(float rhs): x(rhs), y(rhs), z(rhs), w(rhs) {}
    constexpr fvec4(const fvec4 & rhs): x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
    constexpr fvec4(float xx, float yy, float zz, float ww): x(xx), y(yy), z(zz), w(ww) {}
    
    fvec4 & operator=(const fvec4 & rhs) {x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this;}
    
    constexpr fvec4 operator+(const fvec4 & rhs) const {return fvec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);}
    constexpr fvec4 operator-(const fvec4 & rhs) const {return fvec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);}
    constexpr fvec4 operator*(const fvec4 & rhs) const {return fvec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);}
    constexpr fvec4 operator/(const fvec4 & rhs) const {return fvec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);}
};
static inline float vdot3(const fvec4 & a, const fvec4 & b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
static inline float vlength_sqr3(const fvec4 & v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}
static inline float vlength3(const fvec4 & v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
// struct fvec4 {
//     float4 v;
//     fvec4() {}
//     constexpr fvec4(float rhs): v((float4)(rhs)) {}
//     constexpr fvec4(float4 rhs): v(rhs) {}
//     constexpr fvec4(const fvec4 & rhs): v(rhs.v) {}
//     constexpr fvec4(float xx, float yy, float zz, float ww): v((float4){xx, yy, zz, ww}) {}
//     
//     inline fvec4 & operator=(const fvec4 & rhs) {v = rhs.v; return *this;}
//     
//     inline fvec4 operator+(const fvec4 & rhs) const {return fvec4(v + rhs.v);}
//     inline fvec4 operator-(const fvec4 & rhs) const {return fvec4(v - rhs.v);}
//     inline fvec4 operator*(const fvec4 & rhs) const {return fvec4(v * rhs.v);}
//     inline fvec4 operator/(const fvec4 & rhs) const {return fvec4(v / rhs.v);}
// };
// static inline float vdot3(const fvec4 & a, const fvec4 & b) {
//     float4 tmp = a.v*b.v;
//     return tmp.x + tmp.y + tmp.z;
// }
// static inline float vlength_sqr3(const fvec4 & v) {
//     float4 tmp = v.v*v.v;
//     return tmp.x + tmp.y + tmp.z;
// }
// static inline float vlength3(const fvec4 & v) {
//     float4 tmp = v.v*v.v;
//     return std::sqrt(tmp.x + tmp.y + tmp.z);
// }

struct fray {
    // fvec4 origin, dir;
    float4 origin;
    float4 dir;
    fray() {}
    // fray(const fvec4 & o, const fvec4 & d): origin(o), dir(d) {}
    constexpr fray(const float4 & o, const float4 & d): origin(o), dir(d) {}
    constexpr fray(const fray & rhs): origin(rhs.origin), dir(rhs.dir) {}
};

void TestExtension(float4 * dst, const float4 * srcA, const float4 * srcB, const float4 * srcC, size_t len);
void TestIntrinsics(float4 * dst, const float4 * srcA, const float4 * srcB, const float4 * srcC, size_t len);
void TestStruct(fvec4 * dst, const fvec4 * srcA, const fvec4 * srcB, const fvec4 * srcC, size_t len);
void TestArray(float * dst, const float * srcA, const float * srcB, const float * srcC, size_t len);


float TraceTestE(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr);
float TraceTestI(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr);
float TraceTestS(const fvec4 & rayOrigin, const fvec4 & rayDir, const fvec4 & center, float radiusSqr);
// float TraceTestR(const fray & ray, const fvec4 & center, float radiusSqr);
float TraceTestR(const fray & ray, const float4 & center, float radiusSqr);

class Sphere {
    float4 center;
    float radiusSqr;
  public:
    Sphere(float4 c, float r): center(c), radiusSqr(r*r) {}
    float TraceTestR(const fray & ray);
};


struct SParticle {
    fvec4 pos, vel;
    SParticle(const fvec4 & p, const fvec4 & v): pos(p), vel(v) {}
    SParticle(const SParticle & rhs): pos(rhs.pos), vel(rhs.vel) {}
};

void TrackParticle(const std::vector<SParticle> & background, SParticle & particle, int iterations);


struct Particle {
    float4 pos, vel;
    Particle(float4 p, float4 v): pos(p), vel(v) {}
    Particle(const Particle & rhs): pos(rhs.pos), vel(rhs.vel) {}
};

void TrackParticle(const std::vector<Particle> & background, Particle & particle, int iterations);
void TrackParticleI(const std::vector<Particle> & background, Particle & particle, int iterations);



static inline float vdot3(float4 a, float4 b)
{
    float4 tmp = a*b;
    return tmp.x + tmp.y + tmp.z;
}
static inline float vlength_sqr3(float4 v)
{
    float4 tmp = v*v;
    return tmp.x + tmp.y + tmp.z;
}
static inline float vlength3(float4 v)
{
    float4 tmp = v*v;
    return std::sqrt(tmp.x + tmp.y + tmp.z);
}


static inline float vdot3_i(float4 a, float4 b)
{
    // float4 tmp = a*b;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return tmp.x;
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x71));
}
static inline float vlength_sqr3_i(float4 v)
{
    // float4 tmp = v*v;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return tmp.x;
    return _mm_cvtss_f32(_mm_dp_ps(v, v, 0x71));
}
static inline float vlength3_i(float4 v)
{
    // float4 tmp = v*v;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return _mm_cvtss_f32(_mm_sqrt_ss(tmp));
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));
}


#endif // VECTEST_H
