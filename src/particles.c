#include "../include/particles.h"
#include <math.h>

void ParticleSpawn(ParticleSystem *ps, Vector2 pos, Color color, int n) {
    for (int i = 0; i < n && ps->count < MAX_PARTICLES; i++) {
        Particle *p = &ps->pool[ps->count++];
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = GetRandomValue(2, 8);
        p->pos      = pos;
        p->vel      = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
        p->lifetime = 0.4f + GetRandomValue(0, 30) * 0.01f;
        p->maxLife  = p->lifetime;
        p->color    = color;
    }
}

void ParticleUpdate(ParticleSystem *ps) {
    float dt = GetFrameTime();
    for (int i = ps->count - 1; i >= 0; i--) {
        Particle *p = &ps->pool[i];
        p->lifetime -= dt;
        if (p->lifetime <= 0) {
            ps->pool[i] = ps->pool[--ps->count]; // swap with last
            continue;
        }
        p->pos.x += p->vel.x;
        p->pos.y += p->vel.y;
        p->vel.x *= 0.93f;  // drag
        p->vel.y *= 0.93f;
    }
}

void ParticleDraw(ParticleSystem *ps, Texture2D tex) {
    for (int i = 0; i < ps->count; i++) {
        Particle *p = &ps->pool[i];
        float t     = p->lifetime / p->maxLife; // 1→0 as it dies
        float alpha = t;
        float scale = t * 0.5f;
        Color c     = Fade(p->color, alpha);
        DrawTextureEx(tex,
            (Vector2){ p->pos.x - tex.width * scale / 2,
                       p->pos.y - tex.height * scale / 2 },
            0.0f, scale, c);
    }
}