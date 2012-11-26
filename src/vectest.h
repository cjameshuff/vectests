#ifndef VECTEST_H
#define VECTEST_H

#include <cstdlib>
#include <cmath>
#include <vector>

#include "smmintrin.h"

#include <sys/time.h>

#include "cppformat/cppformat.h"

#include <random>
#include <functional>

// #define USE_4D_STRUCTS_ARRAYS

extern std::uniform_real_distribution<double> ur_distribution;
extern std::uniform_real_distribution<double> sur_distribution;
extern std::mt19937 rng_engine;
static auto frand = std::bind(ur_distribution, rng_engine);
static auto sfrand = std::bind(sur_distribution, rng_engine);

// *****************************************************************************

inline double real_seconds() {
    timeval newTime;
    gettimeofday(&newTime, NULL);
    return newTime.tv_sec + newTime.tv_usec/1e6;
}

template<typename Func>
double Benchmark(const std::string & name, int runs, Func func) {
    double startT, totalT = 0;
    func();// warmup
    for(int j = 0; j < runs; ++j) {
        startT = real_seconds();
        func();
        totalT += real_seconds() - startT;
    }
    return totalT;
}

// *****************************************************************************

#ifdef USE_4D_STRUCTS_ARRAYS
template<typename T>
struct svec4 {
    T x, y, z, w;
    svec4() {}
    constexpr svec4(T rhs): x(rhs), y(rhs), z(rhs), w(rhs) {}
    constexpr svec4(const svec4<T> & rhs): x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
    constexpr svec4(T xx, T yy, T zz, T ww = 0): x(xx), y(yy), z(zz), w(ww) {}
    
    svec4<T> & operator=(const svec4<T> & rhs) {x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this;}
    
    constexpr svec4<T> operator+(const svec4<T> & rhs) const {return svec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);}
    constexpr svec4<T> operator-(const svec4<T> & rhs) const {return svec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);}
    constexpr svec4<T> operator*(const svec4<T> & rhs) const {return svec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);}
    constexpr svec4<T> operator/(const svec4<T> & rhs) const {return svec4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);}
};
#else
template<typename T>
struct svec4 {
    T x, y, z;
    svec4() {}
    constexpr svec4(T rhs): x(rhs), y(rhs), z(rhs) {}
    constexpr svec4(const svec4<T> & rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
    constexpr svec4(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}
    
    svec4<T> & operator=(const svec4<T> & rhs) {x = rhs.x; y = rhs.y; z = rhs.z; return *this;}
    
    constexpr svec4<T> operator+(const svec4<T> & rhs) const {return svec4<T>(x + rhs.x, y + rhs.y, z + rhs.z);}
    constexpr svec4<T> operator-(const svec4<T> & rhs) const {return svec4<T>(x - rhs.x, y - rhs.y, z - rhs.z);}
    constexpr svec4<T> operator*(const svec4<T> & rhs) const {return svec4<T>(x * rhs.x, y * rhs.y, z * rhs.z);}
    constexpr svec4<T> operator/(const svec4<T> & rhs) const {return svec4<T>(x / rhs.x, y / rhs.y, z / rhs.z);}
};
#endif

template<typename T>
T vdot3(const svec4<T> & a, const svec4<T> & b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

template<typename T>
T vlength_sqr3(const svec4<T> & v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

template<typename T>
T vlength3(const svec4<T> & v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

typedef svec4<float> fvec4;

// This only gives a moderate speedup over a struct-of-scalars. Presumably because
// the vector can no longer be passed via SSE registers.
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

// *****************************************************************************

template<typename T> using vec4 = T __attribute__((ext_vector_type(4)));

// typedef float float4 __attribute__((ext_vector_type(4)));
typedef vec4<float> float4;

static inline fstring operator%(const fstring & lhs, float4 rhs) {
    std::string fs = lhs.next_fmt("f");
    fs = (fstring("(%s, %s, %s, %s)")% fs % fs % fs % fs).str();
    return lhs.append((fstring(fs)% rhs.x % rhs.y % rhs.z % rhs.w).str());
    // std::string fs = lhs.next_fmt("f");
    // fs = (fstring("(%s, %s, %s)")% fs % fs % fs).str();
    // return lhs.append((fstring(fs)% rhs.x % rhs.y % rhs.z).str());
}

// *****************************************************************************

template<typename T>
static inline float vdot3(vec4<T> a, vec4<T> b)
{
    vec4<T> tmp = a*b;
    return tmp.x + tmp.y + tmp.z;
}

template<typename T>
static inline float vlength_sqr3(vec4<T> v)
{
    vec4<T> tmp = v*v;
    return tmp.x + tmp.y + tmp.z;
}

template<typename T>
static inline float vlength3(vec4<T> v)
{
    vec4<T> tmp = v*v;
    return std::sqrt(tmp.x + tmp.y + tmp.z);
}

// *****************************************************************************

static inline float vdot3_i(vec4<float> a, vec4<float> b)
{
    // vec4<float> tmp = a*b;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return tmp.x;
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x71));
}

static inline float vlength_sqr3_i(vec4<float> v)
{
    // vec4<float> tmp = v*v;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return tmp.x;
    return _mm_cvtss_f32(_mm_dp_ps(v, v, 0x71));
}

static inline float vlength3_i(vec4<float> v)
{
    // vec4<float> tmp = v*v;
    // tmp = _mm_hadd_ps(tmp, tmp);
    // tmp = _mm_hadd_ps(tmp, tmp);
    // return _mm_cvtss_f32(_mm_sqrt_ss(tmp));
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));
}

// *****************************************************************************

void MathTests(int bufsize, int ntrials);
void RaytraceTests(int nrays, int ntrials);
void ParticleTests(int nparts, int ntrials);



void TestExtension(std::vector<float4> & dst, const std::vector<float4> & srcA,
                   const std::vector<float4> & srcB, const std::vector<float4> & srcC, size_t len);
void TestIntrinsics(std::vector<float4> & dst, const std::vector<float4> & srcA,
                    const std::vector<float4> & srcB, const std::vector<float4> & srcC, size_t len);
void TestStruct(std::vector<fvec4> & dst, const std::vector<fvec4> & srcA,
                const std::vector<fvec4> & srcB, const std::vector<fvec4> & srcC, size_t len);
void TestArray(std::vector<float> & dst, const std::vector<float> & srcA,
               const std::vector<float> & srcB, const std::vector<float> & srcC, size_t len);


template<typename T>
struct fray {
    vec4<T> origin;
    vec4<T> dir;
    fray() {}
    constexpr fray(const vec4<T> & o, const vec4<T> & d): origin(o), dir(d) {}
    constexpr fray(const fray<T> & rhs): origin(rhs.origin), dir(rhs.dir) {}
};

class Sphere {
    vec4<float> center;
    float radiusSqr;
  public:
    Sphere(float4 c, float r): center(c), radiusSqr(r*r) {}
    float TraceTestR(const fray<float> & ray);
};

float TraceTestE(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr);
float TraceTestI(float4 rayOrigin, float4 rayDir, float4 center, float radiusSqr);
float TraceTestS(const fvec4 & rayOrigin, const fvec4 & rayDir, const fvec4 & center, float radiusSqr);
// float TraceTestR(const fray & ray, const fvec4 & center, float radiusSqr);
float TraceTestR(const fray<float> & ray, const float4 & center, float radiusSqr);


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



#endif // VECTEST_H
