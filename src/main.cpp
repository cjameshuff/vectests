

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <vector>

#include "vectest.h"

using namespace std;

std::uniform_real_distribution<double> ur_distribution(0, 1.0);
std::uniform_real_distribution<double> sur_distribution(-1.0, 1.0);
std::mt19937 rng_engine;


int main(int argc, const char * argv[])
{
    MathTests(100000, 3000);
    RaytraceTests(1000, 130000);
    ParticleTests(100, 1700);
    
    return EXIT_SUCCESS;
}


void MathTests(int bufsize, int ntrials)
{
    std::vector<fvec4> structDst(bufsize);
    std::vector<fvec4> structSrcA(bufsize);
    std::vector<fvec4> structSrcB(bufsize);
    std::vector<fvec4> structSrcC(bufsize);
    
    std::vector<float4> vecDst(bufsize);
    std::vector<float4> vecSrcA(bufsize);
    std::vector<float4> vecSrcB(bufsize);
    std::vector<float4> vecSrcC(bufsize);
    
#ifdef USE_4D_STRUCTS_ARRAYS
    std::vector<float> arrayDst(bufsize*4);
    std::vector<float> arraySrcA(bufsize*4);
    std::vector<float> arraySrcB(bufsize*4);
    std::vector<float> arraySrcC(bufsize*4);
#else
    std::vector<float> arrayDst(bufsize*3);
    std::vector<float> arraySrcA(bufsize*3);
    std::vector<float> arraySrcB(bufsize*3);
    std::vector<float> arraySrcC(bufsize*3);
#endif
    
    for(int j = 0; j < bufsize; ++j)
    {
        float a = frand();
        float b = frand();
        float c = frand();
        float d = frand();
        structSrcA[j] = fvec4(a, b, c);
        vecSrcA[j] = (float4){a, b, c, d};
        // vecSrcA[j] = (float4){a, b, c};
#ifdef USE_4D_STRUCTS_ARRAYS
        arraySrcA[j*4 + 0] = a;
        arraySrcA[j*4 + 1] = b;
        arraySrcA[j*4 + 2] = c;
        arraySrcA[j*4 + 3] = d;
#else
        arraySrcA[j*3 + 0] = a;
        arraySrcA[j*3 + 1] = b;
        arraySrcA[j*3 + 2] = c;
#endif
        
        a = frand();
        b = frand();
        c = frand();
        d = frand();
        structSrcB[j] = fvec4(a, b, c);
        vecSrcB[j] = (float4){a, b, c, d};
        // vecSrcB[j] = (float4){a, b, c};
#ifdef USE_4D_STRUCTS_ARRAYS
        arraySrcA[j*4 + 0] = a;
        arraySrcA[j*4 + 1] = b;
        arraySrcA[j*4 + 2] = c;
        arraySrcA[j*4 + 3] = d;
#else
        arraySrcA[j*3 + 0] = a;
        arraySrcA[j*3 + 1] = b;
        arraySrcA[j*3 + 2] = c;
#endif
        
        a = frand();
        b = frand();
        c = frand();
        d = frand();
        structSrcC[j] = fvec4(a, b, c);
        vecSrcC[j] = (float4){a, b, c, d};
        // vecSrcC[j] = (float4){a, b, c};
#ifdef USE_4D_STRUCTS_ARRAYS
        arraySrcA[j*4 + 0] = a;
        arraySrcA[j*4 + 1] = b;
        arraySrcA[j*4 + 2] = c;
        arraySrcA[j*4 + 3] = d;
#else
        arraySrcA[j*3 + 0] = a;
        arraySrcA[j*3 + 1] = b;
        arraySrcA[j*3 + 2] = c;
#endif
    }
    
    
    cout << "\n\nMath tests\n" << endl;
    double t;
    
    t = Benchmark("Struct", ntrials, [&]()->void {
        TestStruct(structDst, structSrcA, structSrcB, structSrcC, bufsize);
    });
    cout << fstring("Struct: %f s\n")% t;
    
    t = Benchmark("Array", ntrials, [&]()->void {
        TestArray(arrayDst, arraySrcA, arraySrcB, arraySrcC, bufsize);
    });
    cout << fstring("Array: %f s\n")% t;
    
    t = Benchmark("Extension", ntrials, [&]()->void {
        TestExtension(vecDst, vecSrcA, vecSrcB, vecSrcC, bufsize);
    });
    cout << fstring("Extension: %f s\n")% t;
    
    t = Benchmark("Intrinsics", ntrials, [&]()->void {
        TestIntrinsics(vecDst, vecSrcA, vecSrcB, vecSrcC, bufsize);
    });
    cout << fstring("Intrinsics: %f s\n")% t;
}



void ParticleTests(int nparts, int ntrials)
{
    cout << "\n\nParticle system tests\n" << endl;
    
    double t;
    std::vector<SParticle> sbackground;
    std::vector<Particle> background;
    for(int j = 0; j < 5; ++j)
    {
        float x = sfrand(), y = sfrand(), z = sfrand();
        sbackground.push_back(SParticle(fvec4{x, y, z}, 0.0f));
        background.push_back(Particle((float4){x, y, z, 0}, 0.0f));
        // background.push_back(Particle((float4){x, y, z}, 0.0f));
    }
    
    t = Benchmark("Structs", ntrials, [&]()->void {
        for(int k = 0; k < nparts; ++k)
        {
            SParticle particle(0.0f, fvec4{(float)sfrand(), (float)sfrand(), (float)sfrand()}*0.0001f);
            TrackParticle(sbackground, particle, 100);
        }
    });
    cout << fstring("Structs: %f s\n")% t;
    
    t = Benchmark("Extension", ntrials, [&]()->void {
        for(int k = 0; k < nparts; ++k)
        {
            Particle particle(0.0f, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand(), 0}*0.0001f);
            // Particle particle(0.0f, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand()}*0.0001f);
            TrackParticle(background, particle, 100);
        }
    });
    cout << fstring("extension: %f s\n")% t;
    
    t = Benchmark("Extension", ntrials, [&]()->void {
        for(int k = 0; k < nparts; ++k)
        {
            Particle particle(0.0f, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand(), 0}*0.0001f);
            // Particle particle(0.0f, (float4){(float)sfrand(), (float)sfrand(), (float)sfrand()}*0.0001f);
            TrackParticleI(background, particle, 100);
        }
    });
    cout << fstring("intrinsics: %f s\n")% t;
}


void RaytraceTests(int nrays, int ntrials)
{
    std::vector<fvec4> structDir(nrays);
    std::vector<fvec4> structOrig(nrays);
    std::vector<float4> vecDir(nrays);
    std::vector<float4> vecOrig(nrays);
    std::vector<fray<float> > rays(nrays);
    
    for(int j = 0; j < nrays; ++j)
    {
        float a, b, c;
        
        a = frand(); b = frand(); c = frand();
        structOrig[j] = fvec4(a, b, c)*10.0f;
        
        a = sfrand(); b = sfrand(); c = sfrand();
        structDir[j] = fvec4(a, b, c)*2.0f - structOrig[j];
        structDir[j] = structDir[j]*(1.0f/vlength3(structDir[j]));
        
        vecDir[j] = (float4){structDir[j].x, structDir[j].y, structDir[j].z, 0.0f};
        vecOrig[j] = (float4){structOrig[j].x, structOrig[j].y, structOrig[j].z, 0.0f};
        // vecDir[j] = (float4){structDir[j].x, structDir[j].y, structDir[j].z};
        // vecOrig[j] = (float4){structOrig[j].x, structOrig[j].y, structOrig[j].z};
        
        rays[j].origin = vecOrig[j];
        rays[j].dir = vecDir[j];
    }
    
    
    cout << "\n\nRay tracing tests\n" << endl;
    double t;
    Sphere sph((float4)(0.0f), 1.0f);
    
    t = Benchmark("Structs", ntrials, [&]()->void {
        for(int k = 0; k < nrays; ++k)
            TraceTestS(structOrig[k], structDir[k], fvec4(0.0f), 1.0f);
    });
    cout << fstring("Structs: %f s\n")% t;
    
    t = Benchmark("Extension", ntrials, [&]()->void {
        for(int k = 0; k < nrays; ++k)
            TraceTestE(vecOrig[k], vecDir[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Extension: %f s\n")% t;
    
    t = Benchmark("Intrinsics", ntrials, [&]()->void {
        for(int k = 0; k < nrays; ++k)
            TraceTestI(vecOrig[k], vecDir[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Intrinsics: %f s\n")% t;
    
    t = Benchmark("Rays", ntrials, [&]()->void {
        for(int k = 0; k < nrays; ++k)
            // TraceTestR(rays[k], fvec4(0.0f), 1.0f);
            TraceTestR(rays[k], (float4)(0.0f), 1.0f);
    });
    cout << fstring("Rays: %f s\n")% t;
    
    t = Benchmark("Ray+obj", ntrials, [&]()->void {
        for(int k = 0; k < nrays; ++k)
            // TraceTestR(rays[k], fvec4(0.0f), 1.0f);
            sph.TraceTestR(rays[k]);
    });
    cout << fstring("Ray+obj: %f s\n")% t;
}



