#include "scene_test.hpp"

#include "core/game.hpp"
#include "core/resources.hpp"
#include "maths/maths.hpp"
#include "physics/collisions.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <map>
#include <fstream>

SceneTest::SceneTest(Game *game, const char* levelPath) : Scene(game)
{
    LoadLevel(levelPath);
}

void SceneTest::OnSceneEnter()
{
    // Register actions.

    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");

    RegisterAction(sf::Keyboard::Space, "Jump");

    RegisterAction(sf::Keyboard::F1, "ToggleDebugOverlay");

    NT_INFO("Entering test scene.");

    SpawnPlayer();
}

void SceneTest::OnSceneExit()
{
    NT_INFO("Exiting test scene.");
}

void SceneTest::OnAction(Action action)
{
    auto &playerActions = m_Player->GetComponent<CPlayerActions>();

    if (action.Name == "Up")
    {
        playerActions.Up = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Left")
    {
        playerActions.Left = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Right")
    {
        playerActions.Right = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Down")
    {
        playerActions.Down = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Jump")
    {
        playerActions.Jump = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "ToggleDebugOverlay" && action.Type == ActionType::Begin)
    {
        m_ShowDebugOverlay = !m_ShowDebugOverlay;
    }
    else if (action.Name == "LeftClick")
    {
        if (action.Type == ActionType::Begin)
        {
        }
        else if (action.Type == ActionType::End)
        {
        }
    }
}

void SceneTest::Update(f64 dt)
{
    m_World.Update();

    if (m_SystemToggles.UpdateGravity)
    {
        UpdateGravity(dt);
    }

    if (m_SystemToggles.UpdatePlayerMovement)
    {
        UpdatePlayerMovement(dt);
    }

    if (m_SystemToggles.PhysicsCheckCollisions)
    {
        PhysicsCheckCollisions(dt);
    }

    if (m_SystemToggles.UpdatePositions)
    {
        UpdatePositions(dt);
    }

    if (m_SystemToggles.UpdatePlayerAnimationState)
    {
        UpdatePlayerAnimationState(dt);
    }

    if (m_SystemToggles.UpdateAnimations)
    {
        UpdateAnimations(dt);
    }

    if (m_SystemToggles.UpdateCamera)
    {
        UpdateCamera(dt);
    }
}

void SceneTest::Render(sf::RenderWindow *window)
{
    if (m_SystemToggles.RenderBackground)
    {
        RenderBackground(window);
    }

    if (m_SystemToggles.RenderSprites)
    {
        RenderSprites(window);
    }

    if (m_SystemToggles.DebugRenderColliders)
    {
        DebugRenderColliders(window);
    }

    if (m_SystemToggles.DebugRenderWorldGrid)
    {
        DebugRenderWorldGrid(window);
    }

    if (m_SystemToggles.DebugRenderCamera)
    {
        DebugRenderCamera(window);
    }
}

void SceneTest::DrawGUI()
{
    if (!m_ShowDebugOverlay)
    {
        return;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 mousePos = io.MousePos;
    f32 deltaTime = io.DeltaTime;

    static bool useOverlay = false;

    if (useOverlay)
    {
        ImGui::Begin("Overlay Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        ImGui::SetWindowPos(ImVec2(10, 10));
        ImGui::SetWindowSize(ImVec2(250, 50));

        ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);
        ImGui::Text("Frame Time: %.3f (ms)", deltaTime * 1000.0f);
        ImGui::PopStyleVar();
        ImGui::End();
    }

    if (!ImGui::Begin("Debug"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("##TabBar"))
    {
        if (ImGui::BeginTabItem("Entities"))
        {
            ImGui::Separator();

            ImGui::Text("Entity Count: %u", m_World.GetEntityCount());

            ImGui::Separator();

            for (auto e : m_World.GetEntities())
            {
                ImGui::Text("Entity: %s", e->GetTag().c_str());
            }

            ImGui::Separator();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::SeparatorText("OnUpdate");

            ImGui::Checkbox("Update Gravity", &m_SystemToggles.UpdateGravity);
            ImGui::Checkbox("Update Positions", &m_SystemToggles.UpdatePositions);
            ImGui::Checkbox("Update Player Movement", &m_SystemToggles.UpdatePlayerMovement);
            ImGui::Checkbox("Update Player Animation State", &m_SystemToggles.UpdatePlayerAnimationState);
            ImGui::Checkbox("Update Animations", &m_SystemToggles.UpdateAnimations);

            ImGui::SeparatorText("OnPhysicsUpdate");

            ImGui::Checkbox("Physics Check Collisions", &m_SystemToggles.PhysicsCheckCollisions);

            ImGui::SeparatorText("OnRender");

            ImGui::Checkbox("Render Background", &m_SystemToggles.RenderBackground);
            ImGui::Checkbox("Render Sprites", &m_SystemToggles.RenderSprites);

            ImGui::SeparatorText("Debug");

            ImGui::Checkbox("Debug Render World Grid", &m_SystemToggles.DebugRenderWorldGrid);
            ImGui::Checkbox("Debug Render Colliders", &m_SystemToggles.DebugRenderColliders);
            ImGui::Checkbox("Debug Render Camera", &m_SystemToggles.DebugRenderCamera);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Tiles"))
        {
            ImGui::Separator();

            for (auto &[key, value] : m_TileData)
            {
                // Render a selectable image button of the tile

                ImGui::Text("%s", key.c_str());
                ImGui::Text("Texture: %s", value.TextureSource.c_str());

                sf::Sprite sprite(ResourceManager::GetInstance().GetTexture(value.TextureSource), sf::IntRect(value.OffsetX, value.OffsetY, value.Width, value.Height));

                if (ImGui::ImageButton(
                    key.c_str(),
                    sprite,
                    sf::Vector2f(64, 64)))
                {
                    // TODO: Select tile
                }

                ImGui::Separator();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings"))
        {
            ImGui::Checkbox("Debug Overlay", &useOverlay);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void SceneTest::SpawnPlayer()
{
    if (m_Player != nullptr)
    {
        m_Player->Destroy();
    }

    m_Player = m_World.AddEntity("player");

    m_Player->AddComponent<CTransform>(
        Vec2{16, 62},
        Vec2{2, 2},
        0.0f);

    m_Player->AddComponent<CVelocity>(
        Vec2{0, 0},
        0.0f);

    m_Player->AddComponent<CSprite>(
        ResourceManager::GetInstance().GetTexture("tex_player"),
        0, 0,
        32, 32,
        2.0f);

    m_Player->AddComponent<CPlayerActions>();

    m_Player->AddComponent<CSpriteAnimator>(
        ResourceManager::GetInstance().GetAnimation("anim_player_idle"));

    m_Player->AddComponent<CGravity>(800.0f);

    m_Player->AddComponent<CBoxCollider>(Vec2{24, 22});

    m_Player->AddComponent<CPlayerController>(400.0f, 400.0f);
}

Vec2 SceneTest::ScreenToWorld(Vec2 screenPos)
{
    Vec2 worldPos;
    Vec2 windowSize = m_Game->GetWindowSize();
    sf::Vector2f cameraPos = m_Camera.getCenter();
    sf::Vector2f cameraSize = m_Camera.getSize();

    worldPos.x = ((screenPos.x / windowSize.x) - 0.5f) * cameraSize.x + cameraPos.x;
    worldPos.y = (1.5f - (screenPos.y / windowSize.y)) * cameraSize.y - cameraPos.y;
    return worldPos;
}

Vec2 SceneTest::WorldToScreen(Vec2 worldPos)
{
    Vec2 screenPos;
    Vec2 windowSize = m_Game->GetWindowSize();
    sf::Vector2f cameraPos = m_Camera.getCenter();
    sf::Vector2f cameraSize = m_Camera.getSize();

    screenPos.x = ((worldPos.x - cameraPos.x) / cameraSize.x + 0.5f) * windowSize.x;
    screenPos.y = -((worldPos.y + cameraPos.y) / cameraSize.y - 1.5f) * windowSize.y;
    return screenPos;
}

void SceneTest::UpdatePositions(f64 dt)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CVelocity>() && e->HasComponent<CTransform>())
        {
            auto &tf = e->GetComponent<CTransform>();
            auto &vel = e->GetComponent<CVelocity>();
            tf.Position += vel.Velocity * dt;
            tf.Rotation += vel.AngularVelocity * dt;
        }
    }
}

void SceneTest::UpdateGravity(f64 dt)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CGravity>() && e->HasComponent<CVelocity>())
        {
            auto &vel = e->GetComponent<CVelocity>();
            vel.Velocity.y -= e->GetComponent<CGravity>().Gravity * dt;
        }
    }
}

void SceneTest::UpdatePlayerMovement(f64 dt)
{
    if (!m_Player->HasComponent<CPlayerActions>())
    {
        return;
    }

    if (!m_Player->HasComponent<CVelocity>())
    {
        return;
    }

    if (!m_Player->HasComponent<CPlayerController>())
    {
        return;
    }

    auto &actions = m_Player->GetComponent<CPlayerActions>();
    auto &vel = m_Player->GetComponent<CVelocity>();
    auto &controller = m_Player->GetComponent<CPlayerController>();

    vel.Velocity.x = 0.0f;

    if (actions.Jump && controller.IsGrounded)
    {
        vel.Velocity.y = controller.JumpSpeed;
    }

    f32 moveSpeed = controller.MoveSpeed;
    if (!controller.IsGrounded)
    {
        moveSpeed *= 0.8f;
    }

    if (actions.Left)
    {
        vel.Velocity.x -= moveSpeed;
    }

    if (actions.Right)
    {
        vel.Velocity.x += moveSpeed;
    }
}

void SceneTest::UpdatePlayerAnimationState(f64 dt)
{
    if (!m_Player->HasComponent<CPlayerActions>())
    {
        return;
    }

    auto &actions = m_Player->GetComponent<CPlayerActions>();

    if (m_Player->HasComponent<CTransform>() && m_Player->HasComponent<CSpriteAnimator>())
    {
        auto &anim = m_Player->GetComponent<CSpriteAnimator>();
        auto &tf = m_Player->GetComponent<CTransform>();

        if (m_Player->HasComponent<CVelocity>() && fabsf(m_Player->GetComponent<CVelocity>().Velocity.y) > 10.0f)
        {
            auto &vel = m_Player->GetComponent<CVelocity>();
            if (vel.Velocity.y > 1.0f)
            {
                anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_rise"));
            }
            else if (vel.Velocity.y < 1.0f)
            {
                anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_fall"));
            }
        }
        else if (actions.Left || actions.Right)
        {
            anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_run"));
        }
        else
        {
            anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_idle"));
        }

        if (actions.Left)
        {
            tf.Scale.x = -1.0f * fabsf(tf.Scale.x);
        }
        else if (actions.Right)
        {
            tf.Scale.x = 1.0f * fabsf(tf.Scale.x);
        }
    }
}

void SceneTest::UpdateAnimations(f64 dt)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CSpriteAnimator>())
        {
            auto &anim = e->GetComponent<CSpriteAnimator>();
            anim.TimeAccumulator += dt;

            while (anim.TimeAccumulator > anim.AnimationSource.FrameTime)
            {
                anim.TimeAccumulator -= anim.AnimationSource.FrameTime;
                anim.CurrentFrame++;
            }

            if (anim.CurrentFrame == anim.AnimationSource.FrameCount)
            {
                anim.IsFinished = true;
                if (anim.IsLooping)
                {
                    anim.CurrentFrame = 0;
                }
                else
                {
                    anim.CurrentFrame = anim.AnimationSource.FrameCount - 1;
                }
            }
            else
            {
                anim.IsFinished = false;
            }

            if (e->HasComponent<CSprite>())
            {
                auto &sprite = e->GetComponent<CSprite>();

                sprite.Sprite.setTexture(
                    ResourceManager::GetInstance().GetTexture(anim.AnimationSource.TextureSourceName));

                sprite.Sprite.setTextureRect(sf::IntRect(
                    anim.AnimationSource.OffsetX + (anim.AnimationSource.FrameWidth * anim.CurrentFrame),
                    anim.AnimationSource.OffsetY,
                    anim.AnimationSource.FrameWidth,
                    anim.AnimationSource.FrameHeight));
            }
        }
    }
}

void SceneTest::UpdateCamera(f64 dt)
{
    if (m_Player->HasComponent<CTransform>())
    {
        Vec2 playerPos = m_Player->GetComponent<CTransform>().Position;
        m_CameraParams.CurrentTarget = playerPos;

        m_CameraParams.CurrentPosition = Vec2{
            Maths::Lerp(m_CameraParams.CurrentPosition.x, m_CameraParams.CurrentTarget.x, (1.0f / m_CameraParams.DampTimeX) * dt),
            Maths::Lerp(m_CameraParams.CurrentPosition.y, m_CameraParams.CurrentTarget.y, (1.0f / m_CameraParams.DampTimeY) * dt)
        };

        m_Camera.setSize(m_CameraParams.FrameWidth, m_CameraParams.FrameHeight);

        m_Camera.setCenter(
            (i32) m_CameraParams.CurrentPosition.x, 
            (i32)(m_CameraParams.FrameHeight - m_CameraParams.CurrentPosition.y)
        );
    }
}

void SceneTest::PhysicsCheckCollisions(f64 dt)
{
    /*
        Swept AABB (interpolated) collision checks for player controller.
        Intensive, but only performed for the player so probably fine.
        If wanted to extend to some generic rigidbody then would certainly need to optimise, e.g. broadphase checks first.

        Currently we check for collisions with all tiles, order hits based on distance and then
        iterate through a second time, running another collision check. This is to ensure
        the player does not get stuck at boundaries between tiles.
    */

    // Check collisions between player and tiles

    if (m_Player->HasComponent<CBoxCollider>() && m_Player->HasComponent<CTransform>() && m_Player->HasComponent<CVelocity>())
    {
        auto &playerCollider = m_Player->GetComponent<CBoxCollider>();
        auto &playerTransform = m_Player->GetComponent<CTransform>();
        auto &playerVelocity = m_Player->GetComponent<CVelocity>();

        m_Player->GetComponent<CPlayerController>().IsGrounded = false;

        AABB playerBox = {
            playerTransform.Position.x - (playerCollider.Size.x * fabsf(playerTransform.Scale.x)) / 2.0f,
            playerTransform.Position.y,
            playerCollider.Size.x * fabsf(playerTransform.Scale.x),
            playerCollider.Size.y * fabsf(playerTransform.Scale.y)};

        struct BoxHit
        {
            AABB box;
            RaycastHit hit;
        };

        std::vector<BoxHit> hits;

        for (auto e : m_World.GetEntitiesWithTag("tile"))
        {
            if (e->HasComponent<CBoxCollider>() && e->HasComponent<CTransform>())
            {
                auto &collider = e->GetComponent<CBoxCollider>();
                auto &tf = e->GetComponent<CTransform>();

                AABB tileBox = {
                    tf.Position.x - (collider.Size.x * fabsf(tf.Scale.x)) / 2.0f,
                    tf.Position.y,
                    collider.Size.x * fabsf(tf.Scale.x),
                    collider.Size.y * fabsf(tf.Scale.x)};

                RaycastHit hit{};
                if (Physics2D::AABBcast(
                        playerBox,
                        tileBox,
                        Vec2::Normalised(playerVelocity.Velocity),
                        hit,
                        Vec2::Magnitude(playerVelocity.Velocity) * dt))
                {
                    BoxHit boxhit = {tileBox, hit};
                    hits.push_back(boxhit);
                }
            }
        }

        std::sort(hits.begin(), hits.end(), [](BoxHit a, BoxHit b)
                  { return a.hit.Distance < b.hit.Distance; });

        for (auto hit : hits)
        {
            RaycastHit second_hit{};
            if (Physics2D::AABBcast(
                    playerBox,
                    hit.box,
                    Vec2::Normalised(playerVelocity.Velocity),
                    second_hit,
                    Vec2::Magnitude(playerVelocity.Velocity) * dt))
            {
                f32 dist = Vec2::Magnitude(playerVelocity.Velocity) * dt;

                if (second_hit.Distance < 0.0f)
                {
                    playerTransform.Position = second_hit.Point + second_hit.Normal * 0.1f;
                    playerTransform.Position.y -= (playerCollider.Size.y * fabsf(playerTransform.Scale.y)) / 2.0f;
                } 

                Vec2 flippedNormal = Vec2{second_hit.Normal.y, second_hit.Normal.x};
                playerVelocity.Velocity = flippedNormal * Vec2::Dot(flippedNormal, playerVelocity.Velocity);

                if (second_hit.Normal.y > 0.0f)
                {
                    m_Player->GetComponent<CPlayerController>().IsGrounded = true;
                }
            }
        }
    }
}

void SceneTest::RenderBackground(sf::RenderWindow* window)
{
    window->setView(window->getDefaultView());

    // Draw background texture to whole screen

    sf::Texture& tex = ResourceManager::GetInstance().GetTexture("tex_background");

    auto texSize = tex.getSize();
    auto windowSize = window->getSize();

    sf::Sprite sprite(tex);
    sprite.setScale(
        (f32)windowSize.x / (f32)texSize.x,
        (f32)windowSize.y / (f32)texSize.y
    );

    window->draw(sprite);
}

void SceneTest::RenderSprites(sf::RenderWindow *window)
{
    window->setView(m_Camera);

    std::vector<CSprite *> sprites;

    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CSprite>() && e->HasComponent<CTransform>())
        {
            auto &sprite = e->GetComponent<CSprite>();
            auto &tf = e->GetComponent<CTransform>();
            Vec2 pos = tf.Position;
            f32 rot = tf.Rotation;
            Vec2 scale = tf.Scale;

            sprite.Sprite.setPosition((i32)pos.x, (i32)m_CameraParams.FrameHeight - (i32)pos.y);
            sprite.Sprite.setRotation(rot);
            sprite.Sprite.setScale(scale.x, scale.y);

            sprites.push_back(&sprite);
        }
    }

    std::sort(sprites.begin(), sprites.end(), [](CSprite *a, CSprite *b)
              { return a->Depth < b->Depth; });

    for (auto sprite : sprites)
    {
        window->draw(sprite->Sprite);
    }
}

void SceneTest::DebugRenderWorldGrid(sf::RenderWindow *window)
{
    window->setView(window->getDefaultView());

    sf::Vector2i cameraSize = (sf::Vector2i)m_Camera.getSize();
    sf::Vector2i cameraPos = (sf::Vector2i)m_Camera.getCenter();

    i32 gridLeft = ((cameraPos.x - cameraSize.x / 2) / 32) * 32 - 32;
    i32 gridRight = ((cameraPos.x + cameraSize.x / 2) / 32) * 32 + 32;

    i32 gridTop = ((cameraSize.y / 2 - cameraPos.y) / 32) * 32 - 32;
    i32 gridBottom = ((cameraSize.y + cameraSize.y / 2 - cameraPos.y) / 32) * 32 + 32;

    sf::Color col = sf::Color::Black;
    col.a = 80;

    for (i32 i = gridLeft; i <= gridRight; i += 32)
    {
        Vec2 screenTop = WorldToScreen({(f32)i, (f32)(gridTop)});
        Vec2 screenBottom = WorldToScreen({(f32)i, (f32)(gridBottom)});

        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(screenTop.x, screenTop.y), col),
                sf::Vertex(sf::Vector2f(screenBottom.x, screenBottom.y), col)};
        window->draw(line, 2, sf::Lines);
    }

    for (i32 j = gridTop; j <= gridBottom; j += 32)
    {
        Vec2 screenLeft = WorldToScreen({(f32)gridLeft, (f32)j});
        Vec2 screenRight = WorldToScreen({(f32)gridRight, (f32)j});

        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(screenLeft.x, screenLeft.y), col),
                sf::Vertex(sf::Vector2f(screenRight.x, screenRight.y), col)};
        window->draw(line, 2, sf::Lines);
    }

    // Draw coords in corners of grid cells

    col.a = 255;

    sf::Text text;
    text.setFont(ResourceManager::GetInstance().GetFont("font_noto"));
    text.setCharacterSize(8);
    text.setFillColor(col);

    for (i32 i = gridLeft; i <= gridRight; i += 32)
    {
        for (i32 j = gridTop; j <= gridBottom; j += 32)
        {
            Vec2 screenPos = WorldToScreen({(f32)i + 2, (f32)j + 62});
            text.setString(std::to_string(i / 32) + ", " + std::to_string(j / 32));
            text.setPosition(screenPos.x, screenPos.y);
            window->draw(text);
        }
    }
}

void SceneTest::DebugRenderColliders(sf::RenderWindow *window)
{
    window->setView(m_Camera);

    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CBoxCollider>() && e->HasComponent<CTransform>())
        {
            auto &tf = e->GetComponent<CTransform>();
            auto &collider = e->GetComponent<CBoxCollider>();

            Vec2 pos = tf.Position;
            Vec2 scale = tf.Scale;
            Vec2 size = collider.Size;

            sf::Color col = sf::Color::Red;
            col.a = 255;

            sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(col);
            rect.setOutlineThickness(0.2f);
            rect.setOrigin(size.x / 2.0f, size.y);
            rect.setPosition((i32)pos.x, m_CameraParams.FrameHeight - (i32)pos.y);
            rect.setScale(scale.x, scale.y);
            window->draw(rect, sf::BlendAlpha);
        }
    }
}

void SceneTest::DebugRenderCamera(sf::RenderWindow *window)
{
    window->setView(m_Camera);

    sf::Color col = sf::Color::Green;
    col.a = 255;

    sf::RectangleShape rect(sf::Vector2f(m_CameraParams.BoxWidth, m_CameraParams.BoxHeight));
    
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(col);
    rect.setOutlineThickness(0.2f);
    rect.setOrigin(m_CameraParams.BoxWidth / 2.0f, m_CameraParams.BoxHeight / 2.0f);

    rect.setPosition(
        (i32)m_CameraParams.CurrentPosition.x, 
        m_CameraParams.FrameHeight - (i32)m_CameraParams.CurrentPosition.y
    );
    
    window->draw(rect, sf::BlendAlpha);
}

void SceneTest::LoadLevel(const std::string &path)
{
    m_World = World();
    NT_INFO("Loading level from file: %s", path.c_str());

    std::ifstream file(path);
    if (!file.is_open())
    {
        NT_ERROR("Failed to open level data from path: %s", path.c_str());
        return;
    }

    json data = json::parse(file);

    if (data.contains("tileset"))
    {
        // Process the tile types

        json tileset = data["tileset"];
        for (auto &[key, value] : tileset.items())
        {
            std::string name = value["name"];
            std::string textureSource = value["texture-source"];
            u32 width = value["width"];
            u32 height = value["height"];
            i32 offsetX = value["offset-x"];
            i32 offsetY = value["offset-y"];

            m_TileData[name] = {
                textureSource,
                width,
                height,
                offsetX,
                offsetY};
        }
    }

    if (data.contains("tiles"))
    {
        // Instantiate tile entities

        json tiles = data["tiles"];
        for (auto &[key, value] : tiles.items())
        {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("tile");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX,
                tile.OffsetY,
                tile.Width,
                tile.Height,
                1.0f);

            e->AddComponent<CBoxCollider>(Vec2{(f32)tile.Width, (f32)tile.Height});
        }
    }

    if (data.contains("decorations"))
    {
        // Instantiate decoration entities

        json decs = data["decorations"];
        for (auto &[key, value] : decs.items())
        {
            std::string type = value["type"];
            TileData tile = m_TileData[type];

            Vec2 position;
            position.x = (8.0f * 2.0f) + (f32)value["x"] * 2.0 * tile.Width;
            position.y = (f32)value["y"] * 2.0 * tile.Height;

            auto e = m_World.AddEntity("decoration");
            e->AddComponent<CTransform>(position, Vec2{2.0f, 2.0f}, 0.0f);
            e->AddComponent<CSprite>(
                ResourceManager::GetInstance().GetTexture(tile.TextureSource),
                tile.OffsetX,
                tile.OffsetY,
                tile.Width,
                tile.Height,
                0.0f);
        }
    }
}