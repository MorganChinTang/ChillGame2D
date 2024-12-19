#include "Event.h"

void Event::Main(EventType type, Vector2 pos) {
    eventType = type;
    Initialize(pos);
}

void Event::Initialize(Vector2 pos) {
    position = pos;
    isCollected = false;
    Start();

    // Load event sprite
    const char* spritePath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/susGrass_e1.png";
    eventSprite = LoadTexture(spritePath);

    // Load star sprites
    const char* starGrayPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/starGray.png";
    const char* starPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/star.png";
    starGraySprite = LoadTexture(starGrayPath);
    starSprite = LoadTexture(starPath);

    if (eventSprite.id == 0 || starGraySprite.id == 0 || starSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load Event sprites");
        return;
    }

    // Calculate scaled dimensions
    float scaledWidth = eventSprite.width * EVENT_SCALE;
    float scaledHeight = eventSprite.height * EVENT_SCALE;

    // Initialize event rectangle for collision
    eventRect = {
        position.x,
        position.y,
        scaledWidth,
        scaledHeight
    };

    // Set star position at bottom left
    starPosition = {
        10.0f,  // Padding from left
        (float)GetScreenHeight() - starGraySprite.height * STAR_SCALE - 10.0f  // Padding from bottom
    };
}

void Event::Start() {
    isTriggered = false;
}

void Event::Update() {
    // No update needed for now
}

void Event::HandleEvent() {
    isTriggered = true;
}

bool Event::CheckCollision(Player& player) {
    if (isTriggered) return false;

    Rectangle playerRect = {
        player.GetPosition().x,
        player.GetPosition().y,
        player.GetFrameWidth(),
        player.GetFrameHeight()
    };

    bool collided = CheckCollisionRecs(eventRect, playerRect);
    if (collided) {
        HandleEvent();
        player.SetState(PlayerState::EVENT_1);
    }
    return collided;
}

void Event::Draw() {
    if (!isTriggered) {
        // Draw the event sprite
        Rectangle sourceRec = { 0.0f, 0.0f, (float)eventSprite.width, (float)eventSprite.height };
        Rectangle destRec = { position.x, position.y, eventRect.width, eventRect.height };
        DrawTexturePro(eventSprite, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
    }

    // Draw star (either gray or colored)
    Rectangle starSourceRec = { 0.0f, 0.0f,
        (float)(isCollected ? starSprite.width : starGraySprite.width),
        (float)(isCollected ? starSprite.height : starGraySprite.height) };
    Rectangle starDestRec = {
        starPosition.x,
        starPosition.y,
        (float)(isCollected ? starSprite.width : starGraySprite.width) * STAR_SCALE,
        (float)(isCollected ? starSprite.height : starGraySprite.height) * STAR_SCALE
    };
    DrawTexturePro(
        isCollected ? starSprite : starGraySprite,
        starSourceRec,
        starDestRec,
        { 0, 0 },
        0.0f,
        WHITE
    );
}
