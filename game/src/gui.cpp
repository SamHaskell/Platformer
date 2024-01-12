#include "gui.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

#include "core/resources.hpp"

namespace GUI
{
    void DrawLevelSelector(i32& currentLevelIndex, std::vector<std::string>& levelPaths, std::function<void(const std::string&)> onSelect)
    {
        ImGui::Separator();

        ImGui::PushID(&currentLevelIndex);
        if (ImGui::Combo(
                "", &currentLevelIndex,
                [](void *data, int idx, const char **out_text) 
                {
                    auto &levelPaths =
                        *static_cast<std::vector<std::string> *>(data);
                    if (idx >= levelPaths.size() || idx < 0)
                        return false;
                    *out_text = levelPaths[idx].c_str();
                    return true;
                },
                static_cast<void*>(&levelPaths), levelPaths.size())
            ) 
        {
            
        }

        ImGui::PopID();

        ImGui::SameLine();

        if (ImGui::Button("Load")) 
        {
            onSelect(levelPaths[currentLevelIndex]);
        }
    }

    void DrawTilePainter(bool& paintingEnabled, bool& eraseEnabled, bool& paintColliders, std::map<std::string, TileData>& tileData, TileData& currentTile, sf::Sprite& currentTileSprite)
    {
        ImGui::Separator();

        if (ImGui::Button("Paint Tiles"))
        {
            paintColliders = true;
            paintingEnabled = true;
            eraseEnabled = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Paint Decorations"))
        {
            paintColliders = false;
            paintingEnabled = true;
            eraseEnabled = false;
        }

        ImGui::Separator();

        if (ImGui::Button("Erase Tiles"))
        {
            eraseEnabled = true;
            paintingEnabled = false;
        }

        ImGui::Separator();

        for (auto &[key, value] : tileData) {
            sf::Sprite sprite(ResourceManager::GetInstance().GetTexture(
                                    value.TextureSource),
                                sf::IntRect(value.OffsetX, value.OffsetY,
                                            value.Width, value.Height));

            if (ImGui::ImageButton(key.c_str(), sprite,
                                    sf::Vector2f(32, 32))) 
            {
                currentTile = value;
                currentTileSprite = sprite;
            }

            ImGui::SameLine();

            ImGui::Text("%s", key.c_str());
        }
    }
}