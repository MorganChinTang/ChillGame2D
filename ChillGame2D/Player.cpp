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
    const char* event1Path = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_e1.png";
    const char* event2Path = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/chicken_poisoned.png";

    idleRightTexture = LoadTexture(idleRightPath);
    idleLeftTexture = LoadTexture(idleLeftPath);
    runRightTexture = LoadTexture(runRightPath);
    runLeftTexture = LoadTexture(runLeftPath);
    deadChickenTexture = LoadTexture(event1Path);
    poisonedChickenTexture = LoadTexture(event2Path);

    if (idleRightTexture.id == 0 || idleLeftTexture.id == 0 ||
        runRightTexture.id == 0 || runLeftTexture.id == 0 ||
        deadChickenTexture.id == 0 || poisonedChickenTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load player textures");
        return;
    }

    Start();
}

void Player::Start() {
    position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    state = PlayerState::IDLE_RIGHT;
    lastDirection = Direction::RIGHT;
    currentFrame = 0;
    frameCounter = 0;
    isEventAnimationComplete = false;
    isEventActive = false;
    canMove = true;
}

void Player::HandleMovement() {
    if (!canMove || isEventActive) return;

    bool isMoving = false;
    Vector2 movement = { 0, 0 };

    if (IsKeyDown(KEY_A)) {
        movement.x -= MOVEMENT_SPEED;
        lastDirection = Direction::LEFT;
        isMoving = true;
    }
    if (IsKeyDown(KEY_D)) {
        movement.x += MOVEMENT_SPEED;
        lastDirection = Direction::RIGHT;
        isMoving = true;
    }
    if (IsKeyDown(KEY_W)) {
        movement.y -= MOVEMENT_SPEED;
        isMoving = true;
    }
    if (IsKeyDown(KEY_S)) {
        movement.y += MOVEMENT_SPEED;
        isMoving = true;
    }

    if (isMoving) {
        position.x += movement.x;
        position.y += movement.y;
        state = (lastDirection == Direction::RIGHT) ? PlayerState::RUN_RIGHT : PlayerState::RUN_LEFT;
    }
    else {
        state = (lastDirection == Direction::RIGHT) ? PlayerState::IDLE_RIGHT : PlayerState::IDLE_LEFT;
    }
}

void Player::Draw() {
    if (isEventActive) {
        Rectangle sourceRec = { 0, 0,
            (float)(currentEventType == EventType::EVENT_1 ? deadChickenTexture.width : poisonedChickenTexture.width),
            (float)(currentEventType == EventType::EVENT_1 ? deadChickenTexture.height : poisonedChickenTexture.height)
        };
        Rectangle destRec = {
            position.x,
            position.y,
            sourceRec.width * 7.0f,
            sourceRec.height * 7.0f
        };
        DrawTexturePro(
            currentEventType == EventType::EVENT_1 ? deadChickenTexture : poisonedChickenTexture,
            sourceRec,
            destRec,
            { 0, 0 },
            0.0f,
            WHITE
        );
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

void Player::UpdateAnimation() {
    if (isEventActive) return;

    frameCounter++;
    if (frameCounter >= 60 / FRAME_SPEED) {
        frameCounter = 0;
        currentFrame++;

        int maxFrames;
        switch (state) {
        case PlayerState::IDLE_RIGHT:
        case PlayerState::IDLE_LEFT:
            maxFrames = IDLE_FRAME_COUNT;
            break;
        case PlayerState::RUN_RIGHT:
        case PlayerState::RUN_LEFT:
            maxFrames = RUN_FRAME_COUNT;
            break;
        default:
            maxFrames = 1;
        }

        if (currentFrame >= maxFrames) {
            currentFrame = 0;
        }
    }
}

void Player::Update() {
    HandleMovement();
    UpdateAnimation();
}

void Player::SetEventActive(bool active, EventType type) {
    isEventActive = active;
    if (active) {
        currentEventType = type;
        canMove = false;
    }
}

float Player::GetFrameWidth(Texture2D texture, int frameCount) const {
    return (float)texture.width / frameCount;
}

float Player::GetFrameWidth() const {
    if (isEventActive) {
        return (currentEventType == EventType::EVENT_1 ? deadChickenTexture.width : poisonedChickenTexture.width) * 7.0f;
    }

    Texture2D currentTexture;
    int frameCount;

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
    default:
        return 0;
    }

    return GetFrameWidth(currentTexture, frameCount) * SCALE;
}

float Player::GetFrameHeight() const {
    if (isEventActive) {
        return (currentEventType == EventType::EVENT_1 ? deadChickenTexture.height : poisonedChickenTexture.height) * 7.0f;
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
    default:
        return 0;
    }
}