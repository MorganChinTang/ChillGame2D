#include "ChillGame.h"
#include "Event.h"

ChillGame::ChillGame() :
    player(std::make_unique<Player>()),
    currentEvent{ nullptr },
    event2{ nullptr },
    backgroundTexture{ 0 },
    restartSprite{ 0 },
    restartButtonRect{ 0 },
    showRestartButton{ false },
    triggerTime{ 0.0f },
    deathCount{ 0 },
    isEvent1Unlocked{ false },
    isEvent2Unlocked{ false }
{
}

ChillGame::~ChillGame() {
    if (currentEvent) {
        currentEvent.reset();
    }
    if (event2) {
        event2.reset();
    }
    UnloadTexture(backgroundTexture);
    UnloadTexture(restartSprite);
    CloseWindow();
}

void ChillGame::Main() {
    InitWindow(1200, 800, "Chill Game");
    SetTargetFPS(60);
    Start();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void ChillGame::Start() {
    showRestartButton = false;
    triggerTime = 0;
    player->Start();
    InitializeEvents();

    // Load background
    const char* backgroundPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/grass.png";
    backgroundTexture = LoadTexture(backgroundPath);

    // Load restart button
    const char* restartPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/restart.png";
    restartSprite = LoadTexture(restartPath);

    float buttonScale = 0.5f;
    restartButtonRect = {
        GetScreenWidth() / 2.0f - (restartSprite.width * buttonScale) / 2,
        GetScreenHeight() / 2.0f - (restartSprite.height * buttonScale) / 2,
        restartSprite.width * buttonScale,
        restartSprite.height * buttonScale
    };
}

void ChillGame::InitializeEvents() {
    currentEvent = std::make_unique<Event>();
    Vector2 event1Pos = {
        GetScreenWidth() * 0.75f - 32.0f,
        GetScreenHeight() * 0.5f - 32.0f
    };
    currentEvent->Main(EventType::EVENT_1, event1Pos);

    event2 = std::make_unique<Event>();
    Vector2 event2Pos = {
        GetScreenWidth() * 0.25f - 32.0f,
        GetScreenHeight() * 0.5f - 32.0f
    };
    event2->Main(EventType::EVENT_2, event2Pos);

    if (isEvent1Unlocked) {
        currentEvent->SetCollected(true);
    }
    if (isEvent2Unlocked) {
        event2->SetCollected(true);
    }
}

void ChillGame::Update() {
    player->Update();

    // Check collisions with both events
    if (currentEvent && currentEvent->CheckCollision(*player)) {
        showRestartButton = false;
        triggerTime = GetTime();
        isEvent1Unlocked = true;
        currentEvent->SetTriggered(true);
    }

    if (event2 && event2->CheckCollision(*player)) {
        showRestartButton = false;
        triggerTime = GetTime();
        isEvent2Unlocked = true;
        event2->SetTriggered(true);
    }

    // Handle event completion after delay
    if (!showRestartButton && (currentEvent->IsTriggered() || event2->IsTriggered())) {
        float currentTime = static_cast<float>(GetTime());
        float elapsedTime = currentTime - triggerTime;

        if (elapsedTime >= 3.0f) {
            player->SetEventActive(true);
            showRestartButton = true;
            if (currentEvent->IsTriggered()) {
                currentEvent->SetCollected(true);
            }
            if (event2->IsTriggered()) {
                event2->SetCollected(true);
            }
            deathCount++;
        }
    }

    HandleRestartButton();
}

void ChillGame::HandleRestartButton() {
    if (showRestartButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, restartButtonRect)) {
            Reset();
        }
    }
}

void ChillGame::Reset() {
    player->Start();
    showRestartButton = false;
    player->SetEventActive(false);
    InitializeEvents();
}

void ChillGame::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw background
    for (int y = 0; y < GetScreenHeight(); y += backgroundTexture.height) {
        for (int x = 0; x < GetScreenWidth(); x += backgroundTexture.width) {
            DrawTexture(backgroundTexture, x, y, WHITE);
        }
    }

    // Draw events
    if (currentEvent) currentEvent->Draw();
    if (event2) event2->Draw();

    // Draw player
    player->Draw();

    // Draw death counter
    DrawText(TextFormat("Deaths: %d", deathCount), 10, 10, 30, BLACK);

    // Draw restart button if needed
    if (showRestartButton) {
        DrawTexturePro(restartSprite,
            { 0, 0, (float)restartSprite.width, (float)restartSprite.height },
            restartButtonRect,
            { 0, 0 }, 0.0f, WHITE);
    }

    EndDrawing();
}
