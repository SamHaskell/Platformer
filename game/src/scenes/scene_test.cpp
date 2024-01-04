#include "scene_test.hpp"

#include "maths/maths.hpp"
#include "core/resources.hpp"

#include "imgui.h"

SceneTest::SceneTest(const Game *game) : Scene(game)
{
}

void SceneTest::OnSceneEnter()
{
    // Register actions.

    RegisterAction(sf::Keyboard::Up, "Up");
    RegisterAction(sf::Keyboard::Left, "Left");
    RegisterAction(sf::Keyboard::Right, "Right");
    RegisterAction(sf::Keyboard::Down, "Down");

    RegisterAction(sf::Keyboard::Space, "Jump");

    // Load a level.

    NT_INFO("Entering test scene.");

    SpawnPlayer();

    // Spawn some test tiles

    for (i32 i = 0; i < 20; i++) {
        auto e = m_World.AddEntity("tile");
        e->AddComponent<CTransform>(Vec2{8.0f + 16.0f * i, 0.0f}, Vec2{1.0, 1.0}, 0.0f);
        e->AddComponent<CSprite>(
            ResourceManager::GetInstance().GetTexture("tex_tilesheet"),
            48, 16, 16, 16
        );
        e->AddComponent<CBoxCollider>(Vec2{16, 16});
    }
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
}

void SceneTest::Update(f64 dt)
{
    m_World.Update();

    // UpdateGravity(dt);
    UpdatePlayerMovement(dt);
    UpdatePositions(dt);
    UpdatePlayerAnimationState(dt);
    UpdateAnimations(dt);
}

void SceneTest::Render(sf::RenderWindow *window)
{
    sf::View view(sf::FloatRect(0, 0, 320, 180));
    window->setView(view);

    RenderSprites(window);
    RenderDebugColliders(window);
}

void SceneTest::DrawGUI()
{
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
            ImGui::Text("Entity Count: %u", m_World.GetEntityCount());
            for (auto e : m_World.GetEntities())
            {
                ImGui::Text("Entity: %s", e->GetTag().c_str());
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Systems"))
        {
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

    m_Player->AddComponent<CTransform>(Vec2{16, 16}, Vec2{1, 1}, 0.0f);

    m_Player->AddComponent<CVelocity>(Vec2{0, 0}, 0.0f);

    m_Player->AddComponent<CSprite>(
        ResourceManager::GetInstance().GetTexture("tex_player"),
        0, 0, 32, 32);

    m_Player->AddComponent<CPlayerActions>();

    m_Player->AddComponent<CSpriteAnimator>(
        ResourceManager::GetInstance().GetAnimation("anim_player_idle"));

    m_Player->AddComponent<CGravity>(300.0f);
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
    if (!m_Player->HasComponent<CPlayerActions>()) { return; }
    if (!m_Player->HasComponent<CVelocity>()) { return; }

    auto &actions = m_Player->GetComponent<CPlayerActions>();
    auto& vel = m_Player->GetComponent<CVelocity>();

    vel.Velocity.x = 0.0f;

    if (actions.Jump)
    {
        vel.Velocity.y = 100.0f;
    }

    if (actions.Left)
    {
        vel.Velocity.x -= 100.0f;
    }
    if (actions.Right)
    {
        vel.Velocity.x += 100.0f;
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
        auto& anim = m_Player->GetComponent<CSpriteAnimator>();
        auto& tf = m_Player->GetComponent<CTransform>();

        if (m_Player->HasComponent<CVelocity>() && m_Player->GetComponent<CVelocity>().Velocity.y != 0.0f)
        {
            auto& vel = m_Player->GetComponent<CVelocity>();
            if (vel.Velocity.y > 0.0f)
            {
                anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_rise"));
            }
            else if (vel.Velocity.y < 0.0f)
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
            tf.Scale.x = -1.0f;
        }
        else if (actions.Right) 
        {
            tf.Scale.x = 1.0f;
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
                anim.CurrentFrame ++;
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
                auto& sprite = e->GetComponent<CSprite>();
                
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

void SceneTest::RenderSprites(sf::RenderWindow *window)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CSprite>() && e->HasComponent<CTransform>())
        {
            auto& tf = e->GetComponent<CTransform>();
            auto& sprite = e->GetComponent<CSprite>();

            Vec2 pos = tf.Position;
            f32 rot = tf.Rotation;
            Vec2 scale = tf.Scale;

            sprite.Sprite.setPosition(pos.x, 180 - pos.y);
            sprite.Sprite.setRotation(rot);
            sprite.Sprite.setScale(scale.x, scale.y);
            window->draw(sprite.Sprite);
        }
    }
}

void SceneTest::RenderDebugColliders(sf::RenderWindow *window)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->HasComponent<CBoxCollider>() && e->HasComponent<CTransform>())
        {
            auto& tf = e->GetComponent<CTransform>();
            auto& collider = e->GetComponent<CBoxCollider>();

            Vec2 pos = tf.Position;
            Vec2 scale = tf.Scale;
            Vec2 size = collider.Size;

            sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Red);
            rect.setOutlineThickness(0.2f);
            rect.setOrigin(size.x / 2.0f, size.y);
            rect.setPosition(pos.x, 180 - pos.y);
            rect.setScale(scale.x, scale.y);
            window->draw(rect);
        }
    }
}

void SceneTest::LoadLevel(const std::string &path)
{
    m_World = World();
    NT_INFO("Loading level from file: %s", path.c_str());

    // Load entities according to level file spec


}