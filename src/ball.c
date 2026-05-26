#include "../include/ball.h"
#include <math.h>

void BallInit(Ball *b) {
    b->pos    = (Vector2){ 400, 280 };
    b->radius = 8.0f;
    // random starting direction
    float angle = ((GetRandomValue(-30, 30)) * DEG2RAD);
    float dir   = GetRandomValue(0, 1) ? 1.0f : -1.0f;
    b->vel = (Vector2){ cosf(angle) * 300.0f * dir, sinf(angle) * 300.0f };
}

static void SpeedUp(Ball *b) {
    float maxSpeed = 840.0f;
    float speed = sqrtf(b->vel.x * b->vel.x + b->vel.y * b->vel.y);
    float newSpeed = fminf(speed * 1.04f, maxSpeed);
    b->vel.x = (b->vel.x / speed) * newSpeed;
    b->vel.y = (b->vel.y / speed) * newSpeed;
}

int BallUpdate(Ball *b, Paddle *left, Paddle *right,Sound wallHitSound, Sound paddleHitSound,Vector2 *hitPos, bool *didHit) 
{
    float dt = GetFrameTime();
    *didHit = false;

    b->pos.x += b->vel.x * dt;
    b->pos.y += b->vel.y * dt;

    // top / bottom wall bounce
    if (b->pos.y - b->radius <= 0) {
        b->pos.y = b->radius;
        b->vel.y = fabsf(b->vel.y);
        PlaySound(wallHitSound);
    }
    if (b->pos.y + b->radius >= 560) {
        b->pos.y = 560 - b->radius;
        b->vel.y = -fabsf(b->vel.y);
        PlaySound(wallHitSound);
    }

    // left paddle collision
    if (b->vel.x < 0 &&
        b->pos.x - b->radius <= left->bounds.x + left->bounds.width &&
        b->pos.x + b->radius >= left->bounds.x &&
        b->pos.y >= left->bounds.y &&
        b->pos.y <= left->bounds.y + left->bounds.height)

    {
        b->pos.x = left->bounds.x + left->bounds.width + b->radius;
        float rel   = (b->pos.y - (left->bounds.y + left->bounds.height / 2))
                      / (left->bounds.height / 2);
        float angle = rel * (PI / 4);
        float speed = sqrtf(b->vel.x*b->vel.x + b->vel.y*b->vel.y);
        b->vel.x =  fabsf(cosf(angle) * speed);
        b->vel.y =  sinf(angle) * speed;
        SpeedUp(b);
        PlaySound(paddleHitSound);

        *hitPos = b->pos;
        *didHit = true;
    }

    // right paddle collision
    if (b->vel.x > 0 &&
        b->pos.x + b->radius >= right->bounds.x &&
        b->pos.x - b->radius <= right->bounds.x + right->bounds.width &&
        b->pos.y >= right->bounds.y &&
        b->pos.y <= right->bounds.y + right->bounds.height)
    {
        b->pos.x = right->bounds.x - b->radius;
        float rel   = (b->pos.y - (right->bounds.y + right->bounds.height / 2))
                      / (right->bounds.height / 2);
        float angle = rel * (PI / 4);
        float speed = sqrtf(b->vel.x*b->vel.x + b->vel.y*b->vel.y);
        b->vel.x = -fabsf(cosf(angle) * speed);
        b->vel.y =  sinf(angle) * speed;
        SpeedUp(b);
        PlaySound(paddleHitSound);

        *hitPos = b->pos;
        *didHit = true;
    }

    // scoring
    if (b->pos.x < 0)    { BallInit(b); return -1; }  // right scores
    if (b->pos.x > 800)  { BallInit(b); return  1; }  // left scores

    return 0;
}

void BallDraw(Ball *b, Texture2D tex) {
    // glow
    DrawCircle(b->pos.x, b->pos.y, b->radius + 8, Fade(WHITE, 0.08f));
    DrawCircle(b->pos.x, b->pos.y, b->radius + 4, Fade(WHITE, 0.15f));
    // texture
    float scale = (b->radius * 2) / (float)tex.width;
    DrawTextureEx(tex,
        (Vector2){ b->pos.x - b->radius, b->pos.y - b->radius },
        0.0f, scale, WHITE);
}