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
    Texture2D ballTex        = LoadTexture("assets/ball.png");
    Texture2D paddleLeft  = LoadTexture("assets/paddle_left.png");
    Texture2D paddleRight = LoadTexture("assets/paddle_right.png");
    Texture2D netSeg      = LoadTexture("assets/net_segment.png");
    Texture2D digits      = LoadTexture("assets/digits.png");   // sprite sheet
    Texture2D panel       = LoadTexture("assets/ui_panel.png");
    Texture2D btnNormal   = LoadTexture("assets/button_play.png");
    Texture2D btnHover    = LoadTexture("assets/button_play_hover.png");
    Texture2D particle    = LoadTexture("assets/particle.png");
    
    // Load sounds and music
    Music music            = LoadMusicStream("assets/music_loop.wav");
    Sound btnHoverSound        = LoadSound("assets/sfx_menu_blip.wav");
    Sound gameStartSound = LoadSound("assets/sfx_start.wav");
    
    //color definitions
    Color neonCyan = GetColor(0x00ffdcff);
    Color neonPink = GetColor(0xff00b4ff);

    // Game state management
    
    GameState state = STATE_MENU;
    
    // Button setup
    float btnScale = 0.75f;
    int btnx = screenWidth/2 - (btnNormal.width/2)*btnScale;
    int btny = 200;

    Rectangle btnbounds = { btnx, btny, btnNormal.width*btnScale, btnNormal.height*btnScale };
    Vector2 mousePoint ;

    bool hovered = false;
    bool wasHovered = false;

    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    

    // Main game loop
    PlayMusicStream(music);

    while (!WindowShouldClose()) 
{
    UpdateMusicStream(music);
    mousePoint = GetMousePosition();

    // --- UPDATE ---
    if (state == STATE_MENU) {
        hovered = CheckCollisionPointRec(mousePoint, btnbounds);
        if (hovered && !wasHovered) PlaySound(btnHoverSound);
        wasHovered = hovered;

        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(gameStartSound);
            state = STATE_PLAYING;  // switch state
        }
    }
    else if (state == STATE_PLAYING) {
        // ball movement, paddle input, collision etc goes here
    }

    // --- DRAW ---
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(panel, (Vector2){ 0, 0 }, 0.0f, 2.0f, WHITE);
        if (state == STATE_MENU) {
            
            if (hovered) {
                DrawTextureEx(btnHover, (Vector2){ btnx, btny }, 0.0f, btnScale, WHITE);
                DrawTextEx(GetFontDefault(), "Play", (Vector2){ screenWidth/2 - 20, 210 }, 20, 1, neonPink);
            } else {
                DrawTextureEx(btnNormal, (Vector2){ btnx, btny }, 0.0f, btnScale, WHITE);
                DrawTextEx(GetFontDefault(), "Play", (Vector2){ screenWidth/2 - 20, 210 }, 20, 1, neonCyan);
            }
        }
        else if (state == STATE_PLAYING) {
            // draw paddles, ball, net, score etc
        }
        else if (state == STATE_GAMEOVER) {
            // draw winner screen, restart button etc
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
    UnloadTexture(digits);

    UnloadMusicStream(music);
    UnloadSound(btnHoverSound);
    UnloadSound(gameStartSound);

    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)
    CloseWindow();        // Close window and OpenGL context

    return 0;
}