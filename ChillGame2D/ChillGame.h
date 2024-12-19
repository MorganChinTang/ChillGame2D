#pragma once
#include "raylib.h"
#include "Player.h"
#include "Event.h"
#include <memory>

class ChillGame {
public:
    ChillGame() :
        player{},
        event1{ nullptr },
        event2{ nullptr },
        restartSprite{ 0 },
        restartButtonRect{ 0 },
        backgroundTexture{ 0 },
        deathCount{ 0 },
        showRestartButton{ false },
        triggerTime{ 0.0 },
        isEvent1Unlocked{ false },
        isEvent2Unlocked{ false }
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
    Vector2 GetRandomEventPosition() const;

    Player player;
    std::unique_ptr<Event> event1;
    std::unique_ptr<Event> event2;
    Texture2D restartSprite;
    Rectangle restartButtonRect;
    Texture2D backgroundTexture;
    int deathCount;
    bool showRestartButton;
    double triggerTime;
    bool isEvent1Unlocked;
    bool isEvent2Unlocked;

    // Screen dimensions
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    static constexpr float RESTART_SCALE = 0.15f;
    static constexpr float RESTART_PADDING_RATIO = 0.02f;
    static constexpr float EVENT_BORDER_PADDING_RATIO = 0.1f;
};
