#pragma once
#include "raylib.h"
#include "Player.h"
#include "Event.h"
#include <memory>

class ChillGame {
public:
    ChillGame();
    ~ChillGame();

    void Main();
    void Start();
    void Update();
    void Draw();
    void Reset();
    void InitializeEvents();
    void HandleRestartButton();

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Event> event1;
    std::unique_ptr<Event> event2;
    Texture2D backgroundTexture;
    Texture2D restartButton;
    Rectangle restartButtonRect;
    bool showRestartButton;
    float triggerTime;
    int deathCount;
    bool isEvent1Unlocked;
    bool isEvent2Unlocked;

    // Screen dimensions
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    static constexpr float RESTART_SCALE = 0.15f;
    static constexpr float RESTART_PADDING_RATIO = 0.02f;
};
