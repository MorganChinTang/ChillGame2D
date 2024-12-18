#pragma once
#include "raylib.h"

enum class PlayerState {
    IDLE_RIGHT,
    IDLE_LEFT,
    RUN_RIGHT,
    RUN_LEFT
};

class Player {
public:
    void Main();
    void Start();
    void Update();
    void Draw();

    void SetPosition(Vector2 pos) { position = pos; }
    Vector2 GetPosition() const { return position; }
    float GetFrameWidth() const { return frameRec.width; }
    float GetFrameHeight() const { return frameRec.height; }

private:
    void LoadSprites();
    void UpdateAnimation();
    void SetState(PlayerState newState);

    // Sprite and animation properties
    Texture2D spriteIdleRight;
    Texture2D spriteIdleLeft;
    Texture2D spriteRunRight;
    Texture2D spriteRunLeft;
    Rectangle frameRec;    // Current frame rectangle
    Vector2 position;      // Player position
    Vector2 velocity;      // Player velocity

    // Animation properties
    PlayerState currentState;
    int currentFrame;      // Current animation frame
    int framesCounter;     // General frames counter
    int framesSpeed;       // Animation frame speed
    int numFrames;         // Number of frames in the sprite sheet
    bool wasMovingRight;   // Track last movement direction

    // Movement properties
    static constexpr float MOVE_SPEED = 200.0f;  // Pixels per second
};
