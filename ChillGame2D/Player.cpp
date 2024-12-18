#include "Player.h"

void Player::Main() {
    Start();
    LoadSprites();

    // Set initial position to center of screen
    position = {
        (float)GetScreenWidth() / 2.0f - frameRec.width / 2.0f,
        (float)GetScreenHeight() / 2.0f - frameRec.height / 2.0f
    };
}

void Player::LoadSprites() {
    const float SCALE = 3.0f;  // Make sprites 3 times bigger

    // Load sprite sheets
    const char* idleRightPath = "E:/LaSalle/vgp230_2dGames/2D_ChillGame/Assets/chicken_idle_right-Sheet.png";
    const char* idleLeftPath = "E:/LaSalle/vgp230_2dGames/2D_ChillGame/Assets/chicken_idle_left-Sheet.png";
    const char* runRightPath = "E:/LaSalle/vgp230_2dGames/2D_ChillGame/Assets/chicken_run_right-Sheet.png";
    const char* runLeftPath = "E:/LaSalle/vgp230_2dGames/2D_ChillGame/Assets/chicken_run_left-Sheet.png";

    spriteIdleRight = LoadTexture(idleRightPath);
    spriteIdleLeft = LoadTexture(idleLeftPath);
    spriteRunRight = LoadTexture(runRightPath);
    spriteRunLeft = LoadTexture(runLeftPath);

    // Set up animation frame rectangle
    frameRec = {
        0,
        0,
        (float)spriteIdleRight.width / 4.0f * SCALE,  // Divide by 4 for 4 frames, multiply by scale
        (float)spriteIdleRight.height * SCALE
    };
}

void Player::Start() {
    // Initialize variables
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 8;
    numFrames = 4;  // Assuming 4 frames per animation
    velocity = { 0, 0 };
    wasMovingRight = true;
    currentState = PlayerState::IDLE_RIGHT;
}

void Player::Update() {
    // Reset velocity
    velocity = { 0, 0 };

    // Handle horizontal movement
    if (IsKeyDown(KEY_D)) {
        velocity.x = MOVE_SPEED;
        SetState(PlayerState::RUN_RIGHT);
        wasMovingRight = true;
    }
    else if (IsKeyDown(KEY_A)) {
        velocity.x = -MOVE_SPEED;
        SetState(PlayerState::RUN_LEFT);
        wasMovingRight = false;
    }
    else {
        // Set idle state based on last movement direction
        SetState(wasMovingRight ? PlayerState::IDLE_RIGHT : PlayerState::IDLE_LEFT);
    }

    // Handle vertical movement
    if (IsKeyDown(KEY_W)) {
        velocity.y = -MOVE_SPEED;
        // Keep the current left/right state while moving up
        if (currentState == PlayerState::IDLE_RIGHT || currentState == PlayerState::IDLE_LEFT) {
            SetState(wasMovingRight ? PlayerState::RUN_RIGHT : PlayerState::RUN_LEFT);
        }
    }
    if (IsKeyDown(KEY_S)) {
        velocity.y = MOVE_SPEED;
        // Keep the current left/right state while moving down
        if (currentState == PlayerState::IDLE_RIGHT || currentState == PlayerState::IDLE_LEFT) {
            SetState(wasMovingRight ? PlayerState::RUN_RIGHT : PlayerState::RUN_LEFT);
        }
    }

    // Update position
    float deltaTime = GetFrameTime();
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    UpdateAnimation();
}

void Player::UpdateAnimation() {
    framesCounter++;
    if (framesCounter >= (60 / framesSpeed)) {
        framesCounter = 0;
        currentFrame++;
        if (currentFrame >= numFrames) currentFrame = 0;
        frameRec.x = (float)currentFrame * frameRec.width;
    }
}

void Player::SetState(PlayerState newState) {
    if (currentState != newState) {
        currentState = newState;
        currentFrame = 0;
        framesCounter = 0;
    }
}

void Player::Draw() {
    Texture2D* currentSprite = &spriteIdleRight;

    // Select the appropriate sprite sheet based on state
    switch (currentState) {
    case PlayerState::IDLE_RIGHT:
        currentSprite = &spriteIdleRight;
        break;
    case PlayerState::IDLE_LEFT:
        currentSprite = &spriteIdleLeft;
        break;
    case PlayerState::RUN_RIGHT:
        currentSprite = &spriteRunRight;
        break;
    case PlayerState::RUN_LEFT:
        currentSprite = &spriteRunLeft;
        break;
    }

    // Draw the current frame with proper scaling
    if (currentSprite->id != 0) {
        Rectangle sourceRec = {
            frameRec.x,
            frameRec.y,
            (float)currentSprite->width / 4.0f,  // Divide by 4 for the 4 animation frames
            (float)currentSprite->height
        };

        Rectangle destRec = {
            position.x,
            position.y,
            sourceRec.width * 3.0f,  // Scale up by 3
            sourceRec.height * 3.0f   // Scale up by 3
        };

        DrawTexturePro(*currentSprite, sourceRec, destRec, { 0, 0 }, 0, WHITE);
    }
    else {
        // Fallback if sprite failed to load
        DrawRectangle((int)position.x, (int)position.y, 50, 50, RED);
    }
}
