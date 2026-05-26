#include <raylib.h>
#include <stdbool.h>
#include "ball.h"
#include "paddle.h"
#include "ui.h"
#include "particles.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

// --- all your globals that were local to main() ---
typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER } GameState;

static GameState state;
static Texture2D ballTex, paddleLeft, paddleRight, netSeg;
static Texture2D digitsCyan, digitsPink, panel, framePlaying;
static Texture2D btn1v1Normal, btn1v1Hover, btnAINormal, btnAIHover, particle;
static Music music;
static Sound btnHoverSound, gameStartSound, scoreSound;
static Sound wallHitSound, paddleHitSound, gameOverSound;
static Paddle paddleL, paddleR;
static Ball ball;
static ParticleSystem ps;
static int scoreL, scoreR;
static bool isAI;
static bool hovered1, hovered2, wasHovered1, wasHovered2;
static Rectangle btn1v1bounds, btnAINbounds;
static float btnScale;
static int btnx, btny;
static Color neonCyan, neonPink;

// everything inside your while loop goes here
static void GameLoop(void) {
    Vector2 mousePoint = GetMousePosition();
    Vector2 hitPos = {0};
    bool didHit = false;

    // --- UPDATE ---
    if (state == STATE_MENU) {
        UpdateMusicStream(music);
        hovered1 = CheckCollisionPointRec(mousePoint, btn1v1bounds);
        hovered2 = CheckCollisionPointRec(mousePoint, btnAINbounds);
        if (hovered1 && !wasHovered1) PlaySound(btnHoverSound);
        wasHovered1 = hovered1;
        if (hovered1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(gameStartSound);
            isAI = false;
            state = STATE_PLAYING;
        }
        if (hovered2 && !wasHovered2) PlaySound(btnHoverSound);
        wasHovered2 = hovered2;
        if (hovered2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(gameStartSound);
            isAI = true;
            state = STATE_PLAYING;
        }
    }
    else if (state == STATE_PLAYING) {
        int scored = BallUpdate(&ball, &paddleL, &paddleR,
                                wallHitSound, paddleHitSound,
                                &hitPos, &didHit);
        if (didHit) {
            Color burstColor = (ball.vel.x > 0) ? neonCyan : neonPink;
            ParticleSpawn(&ps, hitPos, burstColor, 20);
        }
        if (scored == 1)  { PlaySound(scoreSound); scoreL++;
            ParticleSpawn(&ps, (Vector2){0,   ball.pos.y}, neonCyan, 35); }
        if (scored == -1) { PlaySound(scoreSound); scoreR++;
            ParticleSpawn(&ps, (Vector2){800, ball.pos.y}, neonPink, 35); }
        if (scoreL >= 7 || scoreR >= 7) {
            PlaySound(gameOverSound);
            state = STATE_GAMEOVER;
        }
        PaddleUpdate(&paddleL, ball.pos.y, KEY_W, KEY_S);
        if (!isAI) PaddleUpdate(&paddleR, ball.pos.y, KEY_UP, KEY_DOWN);
        else { paddleR.isAI = true; PaddleUpdate(&paddleR, ball.pos.y, 0, 0); }
        ParticleUpdate(&ps);
    }
    else if (state == STATE_GAMEOVER) {
        if (IsKeyPressed(KEY_R)) {
            scoreL = 0; scoreR = 0;
            BallInit(&ball);
            PaddleInit(&paddleL, 30,  280, neonCyan, false);
            PaddleInit(&paddleR, 758, 280, neonPink, false);
            state = STATE_MENU;
        }
    }

    // --- DRAW ---
    BeginDrawing();
        ClearBackground(BLACK);
        if (state == STATE_MENU) {
            UiDrawMenu(panel, btn1v1Normal, btn1v1Hover, btnAINormal, btnAIHover,
                       hovered1, hovered2, btnx, btny, btnScale);
        }
        else if (state == STATE_PLAYING) {
            DrawTextureEx(framePlaying, (Vector2){0,0}, 0.0f, 1.0f, WHITE);
            PaddleDraw(&paddleL, paddleLeft);
            PaddleDraw(&paddleR, paddleRight);
            UiDrawNet(netSeg);
            UiDrawScore(digitsCyan, digitsPink, scoreL, scoreR);
            BallDraw(&ball, ballTex);
            ParticleDraw(&ps, particle);
        }
        else if (state == STATE_GAMEOVER) {
            UiDrawGameOver(scoreL, scoreR);
        }
    EndDrawing();
}

int main(void) {
    neonCyan = GetColor(0x00ffdcff);
    neonPink = GetColor(0xff00b4ff);

    InitWindow(800, 560, "Pong");
    InitAudioDevice();

    // load all textures and sounds exactly as before
    ballTex      = LoadTexture("assets/ball.png");
    paddleLeft   = LoadTexture("assets/paddle_left.png");
    paddleRight  = LoadTexture("assets/paddle_right.png");
    netSeg       = LoadTexture("assets/net_segment.png");
    digitsCyan   = LoadTexture("assets/digits.png");
    digitsPink   = LoadTexture("assets/digits_pink.png");
    panel        = LoadTexture("assets/ui_panel.png");
    framePlaying = LoadTexture("assets/frame_playing_v3.png");
    btn1v1Normal = LoadTexture("assets/btn_1v1_normal.png");
    btn1v1Hover  = LoadTexture("assets/btn_1v1_hover.png");
    btnAINormal  = LoadTexture("assets/btn_vs_ai_normal.png");
    btnAIHover   = LoadTexture("assets/btn_vs_ai_hover.png");
    particle     = LoadTexture("assets/particle.png");

    music         = LoadMusicStream("assets/music_loop.wav");
    btnHoverSound = LoadSound("assets/sfx_menu_blip.wav");
    gameStartSound= LoadSound("assets/sfx_start.wav");
    scoreSound    = LoadSound("assets/sfx_score.wav");
    wallHitSound  = LoadSound("assets/sfx_wall_hit.wav");
    paddleHitSound= LoadSound("assets/sfx_paddle_hit.wav");
    gameOverSound = LoadSound("assets/sfx_game_over.wav");

    btnScale = 0.75f;
    btnx = 800/2 - (btn1v1Normal.width/2)*btnScale;
    btny = 200;
    btn1v1bounds = (Rectangle){ btnx, btny,
        btn1v1Normal.width*btnScale, btn1v1Normal.height*btnScale };
    btnAINbounds = (Rectangle){ btnx, btny + btnAINormal.height*btnScale,
        btnAINormal.width*btnScale, btnAINormal.height*btnScale };

    PaddleInit(&paddleL, 50,  280, neonCyan, false);
    PaddleInit(&paddleR, 750, 280, neonPink, false);
    BallInit(&ball);
    ps = (ParticleSystem){0};
    state = STATE_MENU;

    PlayMusicStream(music);

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) GameLoop();
    // unload everything
    CloseAudioDevice();
    CloseWindow();
#endif

    return 0;
}