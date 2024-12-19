#include "Player.h"
#include <string>
#include <vector>

void Player::LoadDeadChickenTextures() {
    deadChickenTextures.clear();
    std::string path = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_e1.png";
    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load dead chicken texture: %s", path.c_str());
    }
    deadChickenTextures.push_back(texture);
}

void Player::Main() {
    // Load textures
    const char* idleRightPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_idle_right-Sheet.png";
    const char* idleLeftPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_idle_left-Sheet.png";
    const char* runRightPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_run_right-Sheet.png";
    const char* runLeftPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_run_left-Sheet.png";
    const char* eventPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_event_1-Sheet.png";
    const char* deadChickenPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_e1.png";

    idleRightTexture = LoadTexture(idleRightPath);
    idleLeftTexture = LoadTexture(idleLeftPath);
    runRightTexture = LoadTexture(runRightPath);
    runLeftTexture = LoadTexture(runLeftPath);
    eventTexture = LoadTexture(eventPath);
    deadChickenTexture = LoadTexture(deadChickenPath);

    if (idleRightTexture.id == 0 || idleLeftTexture.id == 0 ||
        runRightTexture.id == 0 || runLeftTexture.id == 0 ||
        eventTexture.id == 0 || deadChickenTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load player textures");
        return;
    }

    Start();
}

void Player::Start() {
    position = {
        (float)GetScreenWidth() / 2 - (GetFrameWidth(idleRightTexture, IDLE_FRAME_COUNT) / 2),
        (float)GetScreenHeight() / 2 - (idleRightTexture.height * SCALE / 2)
    };
    state = PlayerState::IDLE_RIGHT;
    lastDirection = Direction::RIGHT;
    currentFrame = 0;
    frameCounter = 0;
    isEventAnimationComplete = false;
    isEventActive = false;
    canMove = true;
    currentDeadChickenIndex = 0;
}

void Player::HandleMovement() {
    if (isEventActive) return;  // No movement during event

    bool isMoving = false;

    if (IsKeyDown(KEY_A)) {
        position.x -= MOVEMENT_SPEED;
        lastDirection = Direction::LEFT;
        SetState(PlayerState::RUN_LEFT);
        isMoving = true;
    }
    else if (IsKeyDown(KEY_D)) {
        position.x += MOVEMENT_SPEED;
        lastDirection = Direction::RIGHT;
        SetState(PlayerState::RUN_RIGHT);
        isMoving = true;
    }

    if (IsKeyDown(KEY_W)) {
        position.y -= MOVEMENT_SPEED;
        SetState(lastDirection == Direction::LEFT ? PlayerState::RUN_LEFT : PlayerState::RUN_RIGHT);
        isMoving = true;
    }
    else if (IsKeyDown(KEY_S)) {
        position.y += MOVEMENT_SPEED;
        SetState(lastDirection == Direction::LEFT ? PlayerState::RUN_LEFT : PlayerState::RUN_RIGHT);
        isMoving = true;
    }

    if (!isMoving) {
        SetState(lastDirection == Direction::LEFT ? PlayerState::IDLE_LEFT : PlayerState::IDLE_RIGHT);
    }
}

void Player::UpdateAnimation() {
    if (isEventActive) return;  // No animation during event

    frameCounter++;

    if (frameCounter >= (60 / FRAME_SPEED)) {
        frameCounter = 0;
        currentFrame++;

        switch (state) {
        case PlayerState::EVENT_1:
            if (currentFrame >= EVENT_FRAME_COUNT) {
                currentFrame = EVENT_FRAME_COUNT - 1;
                isEventAnimationComplete = true;
            }
            break;

        case PlayerState::IDLE_LEFT:
        case PlayerState::IDLE_RIGHT:
            if (currentFrame >= IDLE_FRAME_COUNT) {
                currentFrame = 0;
            }
            break;

        case PlayerState::RUN_LEFT:
        case PlayerState::RUN_RIGHT:
            if (currentFrame >= RUN_FRAME_COUNT) {
                currentFrame = 0;
            }
            break;
        }
    }
}

void Player::Update() {
    HandleMovement();
    UpdateAnimation();
}

void Player::Draw() {
    if (isEventActive) {
        // Draw dead chicken at the player's last position
        Rectangle sourceRec = {
            0, 0,
            (float)deadChickenTexture.width,
            (float)deadChickenTexture.height
        };
        Rectangle destRec = {
            position.x,
            position.y,
            deadChickenTexture.width * 7.0f,
            deadChickenTexture.height * 7.0f
        };
        DrawTexturePro(deadChickenTexture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
        return;
    }

    Rectangle sourceRec;
    Rectangle destRec;
    Texture2D currentTexture;
    int frameCount = 1;

    switch (state) {
    case PlayerState::IDLE_RIGHT:
        currentTexture = idleRightTexture;
        frameCount = IDLE_FRAME_COUNT;
        break;
    case PlayerState::IDLE_LEFT:
        currentTexture = idleLeftTexture;
        frameCount = IDLE_FRAME_COUNT;
        break;
    case PlayerState::RUN_RIGHT:
        currentTexture = runRightTexture;
        frameCount = RUN_FRAME_COUNT;
        break;
    case PlayerState::RUN_LEFT:
        currentTexture = runLeftTexture;
        frameCount = RUN_FRAME_COUNT;
        break;
    case PlayerState::EVENT_1:
        currentTexture = eventTexture;
        frameCount = EVENT_FRAME_COUNT;
        break;
    default:
        return;
    }

    float frameWidth = GetFrameWidth(currentTexture, frameCount);
    sourceRec = {
        currentFrame * frameWidth,
        0,
        frameWidth,
        (float)currentTexture.height
    };
    destRec = {
        position.x,
        position.y,
        frameWidth * SCALE,
        currentTexture.height * SCALE
    };

    DrawTexturePro(currentTexture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

void Player::SetEventActive(bool active) {
    isEventActive = active;
    if (active) {
        // Choose a random dead chicken texture when event becomes active
        currentDeadChickenIndex = 0;
    }
}

void Player::SetState(PlayerState newState) {
    if (state != newState && !isEventActive) {  // Only allow state changes if not in event
        state = newState;
        currentFrame = 0;
        frameCounter = 0;
    }
}

float Player::GetFrameWidth(Texture2D texture, int frameCount) const {
    return (float)texture.width / frameCount;
}

float Player::GetFrameWidth() const {
    if (isEventActive) {
        return deadChickenTexture.width * 7.0f;
    }

    switch (state) {
    case PlayerState::IDLE_RIGHT:
        return GetFrameWidth(idleRightTexture, IDLE_FRAME_COUNT);
    case PlayerState::IDLE_LEFT:
        return GetFrameWidth(idleLeftTexture, IDLE_FRAME_COUNT);
    case PlayerState::RUN_RIGHT:
        return GetFrameWidth(runRightTexture, RUN_FRAME_COUNT);
    case PlayerState::RUN_LEFT:
        return GetFrameWidth(runLeftTexture, RUN_FRAME_COUNT);
    case PlayerState::EVENT_1:
        return GetFrameWidth(eventTexture, EVENT_FRAME_COUNT);
    default:
        return 0;
    }
}

float Player::GetFrameHeight() const {
    if (isEventActive) {
        return deadChickenTexture.height * 7.0f;
    }

    switch (state) {
    case PlayerState::IDLE_RIGHT:
        return idleRightTexture.height * SCALE;
    case PlayerState::IDLE_LEFT:
        return idleLeftTexture.height * SCALE;
    case PlayerState::RUN_RIGHT:
        return runRightTexture.height * SCALE;
    case PlayerState::RUN_LEFT:
        return runLeftTexture.height * SCALE;
    case PlayerState::EVENT_1:
        return eventTexture.height * SCALE;
    default:
        return 0;
    }
}