

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <vector>

#include <sys/time.h>

#include "cppformat/cppformat.h"
#include "vectest.h"

using namespace std;

inline double real_seconds() {
    timeval newTime;
    gettimeofday(&newTime, NULL);
    return newTime.tv_sec + newTime.tv_usec/1e6;
}

double frand() {return (double)random()/0x7FFFFFFF;}
double sfrand() {return (double)random()/0x3FFFFFFF - 1.0;}

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

static inline fstring operator%(const fstring & lhs, float4 rhs) {
    std::string fs = lhs.next_fmt("f");
    fs = (fstring("(%s, %s, %s, %s)")% fs % fs % fs % fs).str();
    return lhs.append((fstring(fs)% rhs.x % rhs.y % rhs.z % rhs.w).str());
}


int main(int argc, const char * argv[])
{
    double startT;
    double extT = 0, intT = 0, structT = 0, arrayT = 0;
    
    size_t len = 100000;
    
    fvec4 * structDst = new fvec4[len];
    float4 * vecDst = new float4[len];
    float * arrayDst = new float[len*4];
    
    fvec4 * structSrcA = new fvec4[len];
    float4 * vecSrcA = new float4[len];
    float * arraySrcA = new float[len*4];
    
    fvec4 * structSrcB = new fvec4[len];
    float4 * vecSrcB = new float4[len];
    float * arraySrcB = new float[len*4];
    
    fvec4 * structSrcC = new fvec4[len];
    float4 * vecSrcC = new float4[len];
    float * arraySrcC = new float[len*4];
    
    fray * rays = new fray[len];
    
    for(int j = 0; j < len; ++j)
    {
        float a = frand();
        float b = frand();
        float c = frand();
        float d = frand();
        structSrcA[j] = fvec4(a, b, c, d);
        vecSrcA[j] = (float4){a, b, c, d};
        arraySrcA[j*4 + 0] = a;
        arraySrcA[j*4 + 1] = b;
        arraySrcA[j*4 + 2] = c;
        arraySrcA[j*4 + 3] = d;
        
        a = frand();
        b = frand();
        c = frand();
        d = frand();
        structSrcB[j] = fvec4(a, b, c, d);
        vecSrcB[j] = (float4){a, b, c, d};
        arraySrcB[j*4 + 0] = a;
        arraySrcB[j*4 + 1] = b;
        arraySrcB[j*4 + 2] = c;
        arraySrcB[j*4 + 3] = d;
        
        a = frand();
        b = frand();
        c = frand();
        d = frand();
        structSrcC[j] = fvec4(a, b, c, d);
        vecSrcC[j] = (float4){a, b, c, d};
        arraySrcC[j*4 + 0] = a;
        arraySrcC[j*4 + 1] = b;
        arraySrcC[j*4 + 2] = c;
        arraySrcC[j*4 + 3] = d;
    }
    
    /*for(int j = 0; j < 1000; ++j)
    {
        // printf(".");
        for(int k = 0; k < len; ++k)
            structDst[k] = fvec4(0, 0, 0, 0);
        
        startT = real_seconds();
        TestStruct(structDst, structSrcA, structSrcB, structSrcC, len);
        structT += real_seconds() - startT;
        
        
        for(int k = 0; k < len; ++k)
            vecDst[k] = (float4)(0);
        
        startT = real_seconds();
        TestExtension(vecDst, vecSrcA, vecSrcB, vecSrcC, len);
        extT += real_seconds() - startT;
        
        
        for(int k = 0; k < len*4; ++k)
            arrayDst[k] = 0;
        
        startT = real_seconds();
        TestArray(arrayDst, arraySrcA, arraySrcB, arraySrcC, len);
        arrayT += real_seconds() - startT;
        
        
        for(int k = 0; k < len; ++k)
            vecDst[k] = (float4)(0);
        
        startT = real_seconds();
        TestIntrinsics(vecDst, vecSrcA, vecSrcB, vecSrcC, len);
        intT += real_seconds() - startT;
    }
    printf("\n\n");
    printf("Structs: %f s\n", structT);
    printf("Array: %f s\n", arrayT);
    printf("Vector extension: %f s\n", extT);
    printf("intrinsics: %f s\n", intT);*/
    
    extT = 0; intT = 0; structT = 0; arrayT = 0;
    // Turn srcA into ray origin, srcB into ray direction
    for(int j = 0; j < len; ++j)
    {
        structSrcA[j] = structSrcA[j]*fvec4(10.0f);
        structSrcB[j] = structSrcB[j] - structSrcA[j];
        structSrcB[j] = structSrcB[j]*(1.0f/vlength3(structSrcB[j]));
        
        vecSrcA[j] = vecSrcA[j]*10.0f;
        vecSrcB[j] = vecSrcB[j] - vecSrcA[j];
        vecSrcB[j] = vecSrcB[j]*(1.0f/vlength3(vecSrcB[j]));
        
        // rays[j].origin = structSrcA[j];
        rays[j].origin = vecSrcA[j];
        // rays[j].dir = structSrcB[j];
        rays[j].dir = vecSrcB[j];
    }
    
    // float4 vorigin = 0;
    // fvec4 sorigin(0);
    
    // for(int j = 0; j < 1000; ++j)
    // {
    //     // printf(".");
    //     
    //     startT = real_seconds();
    //     for(int k = 0; k < len; ++k)
    //         TraceTestS(structSrcA[k], structSrcB[k], fvec4(0.0f), 1.0f);
    //     structT += real_seconds() - startT;
    //     
    //     startT = real_seconds();
    //     for(int k = 0; k < len; ++k)
    //         TraceTestI(vecSrcA[k], vecSrcB[k], (float4)(0.0f), 1.0f);
    //     intT += real_seconds() - startT;
    //     
    //     startT = real_seconds();
    //     for(int k = 0; k < len; ++k)
    //         // TraceTestR(rays[k], fvec4(0.0f), 1.0f);
    //         TraceTestR(rays[k], (float4)(0.0f), 1.0f);
    //     arrayT += real_seconds() - startT;
    //     
    //     startT = real_seconds();
    //     for(int k = 0; k < len; ++k)
    //         TraceTestE(vecSrcA[k], vecSrcB[k], (float4)(0.0f), 1.0f);
    //     extT += real_seconds() - startT;
    // }
    
    cout << "\n\n";
    double t;
    Sphere sph((float4)(0.0f), 1.0f);
    
    structT = Benchmark("Structs", 1000, [&]()->void {
        for(int k = 0; k < len; ++k)
            TraceTestS(structSrcA[k], structSrcB[k], fvec4(0.0f), 1.0f);
    });
    cout << fstring("Structs: %f s\n")% structT;
    
    intT = Benchmark("Intrinsics", 1000, [&]()->void {
        for(int k = 0; k < len; ++k)
            TraceTestI(vecSrcA[k], vecSrcB[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Intrinsics: %f s\n")% intT;
    
    arrayT = Benchmark("Rays", 1000, [&]()->void {
        for(int k = 0; k < len; ++k)
            // TraceTestR(rays[k], fvec4(0.0f), 1.0f);
            TraceTestR(rays[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Rays: %f s\n")% arrayT;
    
    arrayT = Benchmark("Ray+obj", 1000, [&]()->void {
        for(int k = 0; k < len; ++k)
            // TraceTestR(rays[k], fvec4(0.0f), 1.0f);
            sph.TraceTestR(rays[k]);
    });
    cout << fstring("Ray+obj: %f s\n")% arrayT;
    
    extT = Benchmark("Extension", 1000, [&]()->void {
        for(int k = 0; k < len; ++k)
            TraceTestE(vecSrcA[k], vecSrcB[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Extension: %f s\n")% extT;
    
    
    
    std::vector<SParticle> sbackground;
    std::vector<Particle> background;
    for(int j = 0; j < 5; ++j)
    {
        float x = sfrand(), y = sfrand(), z = sfrand();
        sbackground.push_back(SParticle(fvec4{x, y, z, 0}, 0));
        background.push_back(Particle((float4){x, y, z, 0}, 0));
    }
    
    t = Benchmark("Extension", 1000, [&]()->void {
        for(int k = 0; k < 1000; ++k)
        {
            SParticle particle(0, fvec4{(float)sfrand(), (float)sfrand(), (float)sfrand(), 0}*0.0001f);
            TrackParticle(sbackground, particle, 100);
        }
    });
    cout << fstring("SParticles: %f s\n")% t;
    
    t = Benchmark("Extension", 1000, [&]()->void {
        for(int k = 0; k < 1000; ++k)
        {
            Particle particle(0, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand(), 0}*0.0001f);
            TrackParticle(background, particle, 100);
        }
    });
    cout << fstring("Particles: %f s\n")% t;
    
    t = Benchmark("Extension", 1000, [&]()->void {
        for(int k = 0; k < 1000; ++k)
        {
            Particle particle(0, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand(), 0}*0.0001f);
            TrackParticleI(background, particle, 100);
        }
    });
    cout << fstring("IParticles: %f s\n")% t;
    
    
/*    cout << "Format test:" << endl;
    cout << (fstring("Testing...%d, %f, %d\n")% 1 % 2.0 % 42).str();
    cout << fstring("Testing...%d, %f, %d\n")% 1 % 2.0 % 42;
    cout << fstring("%s%d, %f, %d\n")% "Testing..." % 1 % 2.0 % 42;
    cout << fstring("%s%c%c%c%d, %f, %d\n")% "Testing" % '.' % '.' % '.' % 1 % 2.0 % 42;
    cout << fstring("%s%d, %f, %d\n")% string("Testing...") % 1 % 2.0 % 42;
    
    int foo[] = {1, 2, 3, 4, 5};
    fstring f("Testing...%d, %d, %d, %d, %d\n");
    for(auto i : foo)
        f %= i;
    cout << f;
    
    float4 result = 2.0f*(float4){1, 2, 3, 4};
    cout << (fstring("Testing...%0.3f\n")% result).str();*/
    
    
    // vector<int> data {1, 2, 3, 4, 5};
    // cout << to_string("%d", data.begin(), data.end(), ":") << endl;
    
    
    // double t = Benchmark("", 100000, [&]()->void {
    //     string s = (fstring("Testing...%d, %f, %d\n")% 1 % 2.0 % 42).str();
    // });
    // cout << "fstring time: " << t << endl;
    // 
    // t = Benchmark("", 100000, [&]()->void {
    //     char bfr[1024];
    //     snprintf(bfr, sizeof(bfr), "Testing...%d, %f, %d\n", 1, 2.0, 42);
    // });
    // cout << "snprintf time: " << t << endl;
    // 
    // t = Benchmark("", 100000, [&]()->void {
    //     ostringstream ostrm;
    //     ostrm << "Testing..." << 1 << ", " << 2.0 << ", " << 42 << endl;
    // });
    // cout << "sstream time: " << t << endl;
    // cout << fstring("snprintf time: %f\n")% t;
    
    // printf("%f %f %f %f\n", result.x, result.y, result.z, result.w);
    
    return EXIT_SUCCESS;
}
