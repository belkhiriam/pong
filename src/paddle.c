#include "../include/paddle.h"

void PaddleInit(Paddle *p, float x, float y, Color color, bool isAI) {
    p->bounds = (Rectangle){ x, y, 12, 64 };
    p->speed  = 5.5f;
    p->color  = color;
    p->isAI   = isAI;
}

void PaddleUpdate(Paddle *p, float ballY, int upKey, int downKey) {
    if (p->isAI) {
        // AI tracks the ball with a slight delay
        float center = p->bounds.y + p->bounds.height / 2.0f;
        float diff   = ballY - center;
        if (diff > 6.0f)  p->bounds.y += p->speed * 0.88f;
        if (diff < -6.0f) p->bounds.y -= p->speed * 0.88f;
    } else {
        if (IsKeyDown(upKey))   p->bounds.y -= p->speed;
        if (IsKeyDown(downKey)) p->bounds.y += p->speed;
    }

    // clamp to screen
    if (p->bounds.y < 0)                          p->bounds.y = 0;
    if (p->bounds.y + p->bounds.height > 560)     p->bounds.y = 560 - p->bounds.height;
}

void PaddleDraw(Paddle *p, Texture2D tex) {
    // glow layers
    for (int i = 4; i >= 1; i--) {
        DrawRectangleRounded(
            (Rectangle){ p->bounds.x - i*3, p->bounds.y - i*3,
                         p->bounds.width + i*6, p->bounds.height + i*6 },
            0.3f, 8, Fade(p->color, 0.04f * i)
        );
    }
    // draw texture scaled to bounds size
    DrawTextureEx(tex, (Vector2){ p->bounds.x, p->bounds.y }, 0.0f,
                  p->bounds.width / (float)tex.width, WHITE);
}