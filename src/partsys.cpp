
#include "vectest.h"

#include <iostream>

using namespace std;

void TrackParticle(const std::vector<SParticle> & background, SParticle & particle, int iterations)
{
    for(int j = 0; j < iterations; ++j)
    {
        // Compute background forces at particle location
        fvec4 a = 0.0f;
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

