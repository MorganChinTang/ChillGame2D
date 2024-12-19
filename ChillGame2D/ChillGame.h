#pragma once
#include "raylib.h"
#include "Player.h"
#include "Event.h"
#include <memory>

class ChillGame {
public:
    ChillGame() :
        player{},
        currentEvent{ nullptr },
        restartSprite{ 0 },
        restartButtonRect{ 0 },
        backgroundTexture{ 0 },
        deathCount{ 0 },
        showRestartButton{ false },
        triggerTime{ 0.0 },
        isEvent1Unlocked{ false }  // Track if event 1 has been completed
    {
    }
    void Main();
    void Start();
    void Update();
    void Draw();

private:
    void LoadRestartButton();
    void InitializeEvents();
    void HandleRestartButton();
    void Reset();

    Player player;
    std::unique_ptr<Event> currentEvent;
    Texture2D restartSprite;
    Rectangle restartButtonRect;
    Texture2D backgroundTexture;
    int deathCount;
    bool showRestartButton;
    double triggerTime;
    bool isEvent1Unlocked;  // Maintain unlocked state across restarts

    // Screen dimensions
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    static constexpr float RESTART_SCALE = 0.15f;
    static constexpr float RESTART_PADDING_RATIO = 0.02f;
};
