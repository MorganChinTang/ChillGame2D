#include "ChillGame.h"
#include "Event.h"

void ChillGame::Main() {
    Start();

    InitWindow(screenWidth, screenHeight, "Chill Game");
    SetTargetFPS(60);

    // Load background texture
    const char* backgroundPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/grassBG.png";
    backgroundTexture = LoadTexture(backgroundPath);
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load background texture: %s", backgroundPath);
    }

    // Initialize game objects
    player.Main();
    InitializeEvents();
    LoadRestartButton();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    // Clean up resources
    if (event1) event1.reset();
    if (event2) event2.reset();
    UnloadTexture(backgroundTexture);
    UnloadTexture(restartSprite);
    CloseWindow();
}

Vector2 ChillGame::GetRandomEventPosition() const {
    const float borderPadding = screenWidth * EVENT_BORDER_PADDING_RATIO;
    float randomX = borderPadding + (float)GetRandomValue(0, (int)(screenWidth - 2 * borderPadding));
    float randomY = borderPadding + (float)GetRandomValue(0, (int)(screenHeight - 2 * borderPadding));
    return { randomX, randomY };
}

void ChillGame::InitializeEvents() {
    // Initialize Event 1
    event1 = std::make_unique<Event>();
    Vector2 event1Pos = GetRandomEventPosition();
    event1->Main(EventType::EVENT_1, event1Pos);
    event1->SetStarPosition({ 30.0f, (float)GetScreenHeight() - 150.0f });
    if (isEvent1Unlocked) {
        event1->SetCollected(true);
    }

    // Initialize Event 2
    event2 = std::make_unique<Event>();
    Vector2 event2Pos = GetRandomEventPosition();
    event2->Main(EventType::EVENT_2, event2Pos);
    event2->SetStarPosition({ 240.0f, (float)GetScreenHeight() - 150.0f });
    if (isEvent2Unlocked) {
        event2->SetCollected(true);
    }
}

void ChillGame::Start() {
    showRestartButton = false;
    triggerTime = 0;
    player.Start();
}

void ChillGame::Update() {
    player.Update();

    bool eventTriggered = false;

    // Check Event 1
    if (event1 && event1->CheckCollision(player)) {
        showRestartButton = false;
        triggerTime = GetTime();
        isEvent1Unlocked = true;
        eventTriggered = true;
    }

    // Check Event 2
    if (event2 && event2->CheckCollision(player)) {
        showRestartButton = false;
        triggerTime = GetTime();
        isEvent2Unlocked = true;
        eventTriggered = true;
    }

    // Handle event completion after delay
    if (!showRestartButton && (event1->IsTriggered() || event2->IsTriggered())) {
        float currentTime = static_cast<float>(GetTime());
        float elapsedTime = currentTime - static_cast<float>(triggerTime);

        if (elapsedTime >= 3.0f) {
            player.SetEventActive(true);
            showRestartButton = true;
            if (event1->IsTriggered()) event1->SetCollected(true);
            if (event2->IsTriggered()) event2->SetCollected(true);
            deathCount++;
        }
    }

    HandleRestartButton();
}

void ChillGame::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (backgroundTexture.id != 0) {
        float scale = fmax(
            (float)screenWidth / backgroundTexture.width,
            (float)screenHeight / backgroundTexture.height
        );

        float destWidth = backgroundTexture.width * scale;
        float destHeight = backgroundTexture.height * scale;
        float destX = (screenWidth - destWidth) / 2;
        float destY = (screenHeight - destHeight) / 2;

        DrawTexturePro(
            backgroundTexture,
            { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height },
            { destX, destY, destWidth, destHeight },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    // Draw events
    if (event1) event1->Draw();
    if (event2) event2->Draw();

    player.Draw();

    // Draw death counter
    const char* deathText = TextFormat("Deaths: %d", deathCount);
    DrawText(deathText, 20, 20, 42, BLACK);

    if (showRestartButton && restartSprite.id != 0) {
        DrawTexturePro(
            restartSprite,
            { 0, 0, (float)restartSprite.width, (float)restartSprite.height },
            restartButtonRect,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    EndDrawing();
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
    Start();
    player.SetEventActive(false);
    InitializeEvents();
}

void ChillGame::LoadRestartButton() {
    const char* restartPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/tempRestart.png";
    restartSprite = LoadTexture(restartPath);

    if (restartSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load restart sprite: %s", restartPath);
        return;
    }

    float restartScaledWidth = restartSprite.width * RESTART_SCALE;
    float restartScaledHeight = restartSprite.height * RESTART_SCALE;
    float padding = screenWidth * RESTART_PADDING_RATIO;

    restartButtonRect = {
        (float)screenWidth - restartScaledWidth - padding,
        (float)screenHeight - restartScaledHeight - padding,
        restartScaledWidth,
        restartScaledHeight
    };
}
