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

    m_Player = m_World.AddEntity("player");

    m_Player->Transform = std::make_shared<CTransform>(
        Vec2{0, 0}, Vec2{1, 1}, 0.0f);

    m_Player->Velocity = std::make_shared<CVelocity>(
        Vec2{0, 0}, 0.0f);

    m_Player->Sprite = std::make_shared<CSprite>(
        ResourceManager::GetInstance().GetTexture("tex_player"),
        0, 0, 32, 32);

    m_Player->PlayerActions = std::make_shared<CPlayerActions>();

    m_Player->Animator = std::make_shared<CSpriteAnimator>(
        ResourceManager::GetInstance().GetAnimation("anim_player_idle"));
}

void SceneTest::OnSceneExit()
{
    NT_INFO("Exiting test scene.");
}

void SceneTest::OnAction(Action action)
{
    if (action.Name == "Up")
    {
        m_Player->PlayerActions->Up = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Left")
    {
        m_Player->PlayerActions->Left = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Right")
    {
        m_Player->PlayerActions->Right = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Down")
    {
        m_Player->PlayerActions->Down = (action.Type == ActionType::Begin);
    }
    else if (action.Name == "Jump")
    {
        m_Player->PlayerActions->Jump = (action.Type == ActionType::Begin);
    }
}

void SceneTest::Update(f64 dt)
{
    m_World.Update();

    UpdatePositions(dt);
    UpdatePlayerAnimationState(dt);
    UpdateAnimations(dt);
}

void SceneTest::Render(sf::RenderWindow *window)
{
    RenderSprites(window);
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

void SceneTest::UpdatePositions(f64 dt)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->Velocity && e->Transform)
        {
            e->Transform->Position += e->Velocity->Velocity * dt;
            e->Transform->Rotation += e->Velocity->AngularVelocity * dt;
        }
    }
}

void SceneTest::UpdatePlayerAnimationState(f64 dt)
{
    if (m_Player->PlayerActions->Left || m_Player->PlayerActions->Right)
    {
        m_Player->Animator->AnimationSource = ResourceManager::GetInstance().GetAnimation("anim_player_run");
        m_Player->Transform->Scale.x = (m_Player->PlayerActions->Left) ? -1.0f : 1.0f;
    } else {
        m_Player->Animator->AnimationSource = ResourceManager::GetInstance().GetAnimation("anim_player_idle");
    }
}

void SceneTest::UpdateAnimations(f64 dt)
{
    for (auto e : m_World.GetEntities())
    {
        if (e->Animator)
        {
            auto &anim = e->Animator->AnimationSource;
            e->Animator->TimeAccumulator += dt;
            while (e->Animator->TimeAccumulator > anim.FrameTime)
            {
                e->Animator->TimeAccumulator -= anim.FrameTime;
                e->Animator->CurrentFrame = (e->Animator->CurrentFrame + 1) % anim.FrameCount;
            }

            if (e->Sprite)
            {
                e->Sprite->Sprite.setTexture(
                    ResourceManager::GetInstance().GetTexture(anim.TextureSourceName));

                e->Sprite->Sprite.setTextureRect(sf::IntRect(
                    anim.OffsetX + (anim.FrameWidth * e->Animator->CurrentFrame),
                    anim.OffsetY,
                    anim.FrameWidth,
                    anim.FrameHeight));
            }
        }
    }
}

void SceneTest::RenderSprites(sf::RenderWindow *window)
{
    sf::View view(sf::FloatRect(0, 0, 320, 180));
    window->setView(view);

    for (auto e : m_World.GetEntities())
    {
        if (e->Sprite && e->Transform)
        {
            Vec2 pos = e->Transform->Position;
            f32 rot = e->Transform->Rotation;
            Vec2 scale = e->Transform->Scale;

            e->Sprite->Sprite.setPosition(pos.x, 180 - pos.y);
            e->Sprite->Sprite.setRotation(rot);
            e->Sprite->Sprite.setScale(scale.x, scale.y);
            window->draw(e->Sprite->Sprite);
        }
    }
}

void SceneTest::LoadLevel(const std::string &path)
{
    m_World = World();

    NT_INFO("Loading level from file: %s", path.c_str());
}