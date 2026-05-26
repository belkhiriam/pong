#ifndef PARTICLES_H
#define PARTICLES_H
#define MAX_PARTICLES 64

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float lifetime;   // seconds remaining
    float maxLife;    // for fading/scaling
    Color color;
} Particle;

typedef struct {
    Particle pool[MAX_PARTICLES];
    int count;
} ParticleSystem;

void ParticleSpawn(ParticleSystem *ps, Vector2 pos, Color color, int n);
void ParticleUpdate(ParticleSystem *ps);
void ParticleDraw(ParticleSystem *ps, Texture2D tex);

#endif