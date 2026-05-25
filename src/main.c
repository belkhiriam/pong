#include <raylib.h>
#include <stdbool.h>
#include "ball.h"
#include "paddle.h"
#include "ui.h"


typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER } GameState;

int main(void)
{
    //Initialize window
    const int screenWidth = 800;
    const int screenHeight = 560;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();      // Initialize audio device

    // Load textures
    Texture2D ballTex     = LoadTexture("assets/ball.png");
    Texture2D paddleLeft  = LoadTexture("assets/paddle_left.png");
    Texture2D paddleRight = LoadTexture("assets/paddle_right.png");
    Texture2D netSeg      = LoadTexture("assets/net_segment.png");
    Texture2D digitsCyan  = LoadTexture("assets/digits.png");   // sprite sheet
    Texture2D digitsPink  = LoadTexture("assets/digits_pink.png");   // sprite sheet
    Texture2D panel       = LoadTexture("assets/ui_panel.png");
    Texture2D framePlaying = LoadTexture("assets/frame_playing.png");
    Texture2D btnNormal   = LoadTexture("assets/button_play.png");
    Texture2D btnHover    = LoadTexture("assets/button_play_hover.png");
    Texture2D particle    = LoadTexture("assets/particle.png");
    
    // Load sounds and music
    Music music            = LoadMusicStream("assets/music_loop.wav");
    Sound btnHoverSound    = LoadSound("assets/sfx_menu_blip.wav");
    Sound gameStartSound   = LoadSound("assets/sfx_start.wav");
    Sound scoreSound       = LoadSound("assets/sfx_score.wav");
    Sound wallHitSound      = LoadSound("assets/sfx_wall_hit.wav");
    Sound paddleHitSound    = LoadSound("assets/sfx_paddle_hit.wav");
    Sound gameOverSound     = LoadSound("assets/sfx_game_over.wav");

    
    //color definitions
    Color neonCyan = GetColor(0x00ffdcff);
    Color neonPink = GetColor(0xff00b4ff);

    // Game state management
    
    GameState state = STATE_MENU;
    
    // Button setup
    float btnScale = 0.75f;
    int btnx = screenWidth/2 - (btnNormal.width/2)*btnScale;
    int btny = 200;

    Rectangle btn1bounds = { btnx, btny, btnNormal.width*btnScale, btnNormal.height*btnScale };
    Rectangle btn2bounds = { btnx, btny + btnNormal.height*btnScale, btnNormal.width*btnScale, btnNormal.height*btnScale };
    Vector2 mousePoint ;

    bool hovered1 = false, hovered2 = false;
    bool wasHovered1 = false, wasHovered2 = false;
    bool isAI = false;

    // padlles
    Paddle paddleL, paddleR;
    PaddleInit(&paddleL, 50, screenHeight/2 - 32, neonCyan, false);
    PaddleInit(&paddleR, screenWidth - 50, screenHeight/2 - 32, neonPink, false);
   

    //ball
    Ball ball;
    BallInit(&ball);

    // scores
    int scoreL = 0;
    int scoreR = 0; 





    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    

    // Main game loop
    PlayMusicStream(music);
    
    

    while (!WindowShouldClose()) 
{
    
    mousePoint = GetMousePosition();

    // --- UPDATE ---
    if (state == STATE_MENU) {
        UpdateMusicStream(music);
        hovered1 = CheckCollisionPointRec(mousePoint, btn1bounds);
        hovered2 = CheckCollisionPointRec(mousePoint, btn2bounds);
        if (hovered1 && !wasHovered1) PlaySound(btnHoverSound);
        wasHovered1 = hovered1;

        if (hovered1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(gameStartSound);
            isAI = false;
            state = STATE_PLAYING;  // switch state
        }
        if (hovered2 && !wasHovered2) PlaySound(btnHoverSound);
        wasHovered2 = hovered2;

        if (hovered2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(gameStartSound);
            isAI = true;
            state = STATE_PLAYING;  // switch state
        }
    }
    else if (state == STATE_PLAYING) {
        // ball movement, paddle input, collision etc goes here
        int scored = BallUpdate(&ball, &paddleL, &paddleR, wallHitSound, paddleHitSound);
        if (scored ==  1) {
            PlaySound(scoreSound);
            scoreL++;
        }
        if (scored == -1) {
            PlaySound(scoreSound);
            scoreR++;
        }
        if (scoreL >= 7 || scoreR >= 7) {
            PlaySound(gameOverSound);
            state = STATE_GAMEOVER;
        }

        
        PaddleUpdate(&paddleL, ball.pos.y, KEY_W, KEY_S);
        if  (!isAI) {
           PaddleUpdate(&paddleR, ball.pos.y, KEY_UP, KEY_DOWN);
        }
        else{
            paddleR.isAI = true;
            PaddleUpdate(&paddleR, ball.pos.y, 0, 0);
        }
        
        
    }
    else if (state == STATE_GAMEOVER) {
        // restart button, score display etc goes here
        if (IsKeyPressed(KEY_R)) {
        scoreL = 0; scoreR = 0;
        BallInit(&ball);
        PaddleInit(&paddleL, 30,  screenHeight/2 - 32, neonCyan, false);
        PaddleInit(&paddleR, 758, screenHeight/2 - 32, neonPink, false);
        
        state = STATE_MENU;
      }
    }

    // --- DRAW ---
    BeginDrawing();
        ClearBackground(BLACK);
        
        if (state == STATE_MENU) {
            
           UiDrawMenu(panel, btnNormal, btnHover, hovered1, hovered2, btnx, btny, btnScale);
        }
        else if (state == STATE_PLAYING) {
            // draw paddles, ball, net, score etc
            DrawTextureEx(framePlaying, (Vector2){ 0, 0 }, 0.0f, 1.0f, WHITE);
            PaddleDraw(&paddleL, paddleLeft);
            
            PaddleDraw(&paddleR, paddleRight);
            
            
            UiDrawNet(netSeg);
            UiDrawScore(digitsCyan, digitsPink, scoreL, scoreR);
            BallDraw(&ball, ballTex);
            

        }
        else if (state == STATE_GAMEOVER) {
            // draw winner screen, restart button etc
            UiDrawGameOver(scoreL, scoreR);
        }

    EndDrawing();
    }
   
    UnloadTexture(panel);
    UnloadTexture(btnNormal);
    UnloadTexture(btnHover);
    UnloadTexture(particle);
    UnloadTexture(ballTex);
    UnloadTexture(paddleLeft);
    UnloadTexture(paddleRight);
    UnloadTexture(netSeg);
    UnloadTexture(digitsCyan);
    UnloadTexture(digitsPink);
    UnloadTexture(framePlaying);

    UnloadMusicStream(music);
    UnloadSound(btnHoverSound);
    UnloadSound(gameStartSound);
    UnloadSound(scoreSound);
    UnloadSound(wallHitSound);
    UnloadSound(paddleHitSound);
    UnloadSound(gameOverSound);

    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)
    CloseWindow();        // Close window and OpenGL context

    return 0;
}