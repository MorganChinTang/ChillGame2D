#pragma once
#include "raylib.h"
#include <vector>

enum class PlayerState {
    IDLE_RIGHT,
    IDLE_LEFT,
    RUN_RIGHT,
    RUN_LEFT,
    EVENT_1,
    EVENT_1_FROZEN
};

enum class Direction {
    LEFT,
    RIGHT
};

class Player {
public:
    Player() :
        position{ 0, 0 },
        state(PlayerState::IDLE_RIGHT),
        lastDirection(Direction::RIGHT),
        currentFrame(0),
        frameCounter(0),
        idleRightTexture{ 0 },
        idleLeftTexture{ 0 },
        runRightTexture{ 0 },
        runLeftTexture{ 0 },
        eventTexture{ 0 },
        deadChickenTexture{ 0 },
        isEventAnimationComplete(false),
        isEventActive(false),
        canMove(true),
        currentDeadChickenIndex(0)
    {
    }

    ~Player() {
        UnloadTexture(idleRightTexture);
        UnloadTexture(idleLeftTexture);
        UnloadTexture(runRightTexture);
        UnloadTexture(runLeftTexture);
        UnloadTexture(eventTexture);
        UnloadTexture(deadChickenTexture);
        for (auto& texture : deadChickenTextures) {
            UnloadTexture(texture);
        }
    }

    void Main();
    void Start();
    void Update();
    void Draw();
    void SetState(PlayerState newState);
    void HandleMovement();
    void UpdateAnimation();
    void LoadDeadChickenTextures();
    void SetEventActive(bool active);
    Vector2 GetPosition() const { return position; }
    float GetFrameWidth() const;
    float GetFrameHeight() const;
    float GetFrameWidth(Texture2D texture, int frameCount) const;
    bool IsEventAnimationComplete() const { return isEventAnimationComplete; }
    bool IsEventActive() const { return isEventActive; }

private:
    static constexpr float SCALE = 8.0f;
    static constexpr int FRAME_SPEED = 8;
    static constexpr int MOVEMENT_SPEED = 5;
    static constexpr int EVENT_FRAME_COUNT = 6;
    static constexpr int IDLE_FRAME_COUNT = 4;
    static constexpr int RUN_FRAME_COUNT = 4;
    static constexpr int DEAD_CHICKEN_COUNT = 4;  // Number of dead chicken variations

    Vector2 position;
    PlayerState state;
    Direction lastDirection;
    int currentFrame;
    int frameCounter;
    Texture2D idleRightTexture;
    Texture2D idleLeftTexture;
    Texture2D runRightTexture;
    Texture2D runLeftTexture;
    Texture2D eventTexture;
    Texture2D deadChickenTexture;
    std::vector<Texture2D> deadChickenTextures;
    bool isEventAnimationComplete;
    bool isEventActive;
    bool canMove;
    int currentDeadChickenIndex;
};
