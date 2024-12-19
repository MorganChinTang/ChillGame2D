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
    if (currentEvent) {
        currentEvent.reset();  // Ensure event is destroyed before textures
    }
    UnloadTexture(backgroundTexture);
    UnloadTexture(restartSprite);
    CloseWindow();
}

void ChillGame::Start() {
    showRestartButton = false;
    triggerTime = 0;
    player.Start();
}

void ChillGame::InitializeEvents() {
    currentEvent = std::make_unique<Event>();
    Vector2 eventPos = {
        GetScreenWidth() * 0.75f - 32.0f,
        GetScreenHeight() * 0.5f - 32.0f
    };
    currentEvent->Main(EventType::EVENT_1, eventPos);
    if (isEvent1Unlocked) {
        currentEvent->SetCollected(true);  // Keep star colored if event was previously completed
    }
}

void ChillGame::Update() {
    player.Update();

    if (currentEvent) {
        if (currentEvent->CheckCollision(player)) {
            showRestartButton = false;
            triggerTime = GetTime();
            isEvent1Unlocked = true;
        }

        // After 3 seconds of event trigger
        if (!showRestartButton && currentEvent->IsTriggered()) {
            float currentTime = static_cast<float>(GetTime());
            float elapsedTime = currentTime - static_cast<float>(triggerTime);

            if (elapsedTime >= 3.0f) {
                player.SetEventActive(true);  // Activate event state after delay
                showRestartButton = true;
                currentEvent->SetCollected(true);
                deathCount++;
            }
        }
    }

    HandleRestartButton();
}

void ChillGame::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (backgroundTexture.id != 0) {
        // Calculate scaling to maintain aspect ratio
        float scale = fmax(
            (float)screenWidth / backgroundTexture.width,
            (float)screenHeight / backgroundTexture.height
        );

        // Calculate centered position and dimensions
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

    if (currentEvent) {
        currentEvent->Draw();
    }

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
    player.SetEventActive(false);  // Reset event state
    InitializeEvents();  // This will maintain the unlocked state
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
