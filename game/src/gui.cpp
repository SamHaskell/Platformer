#include "gui.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

namespace GUI
{
    void DrawLevelSelector(i32& currentLevelIndex, std::vector<std::string>& levelPaths, std::function<void(const std::string&)> onSelect)
    {
        ImGui::Separator();

        ImGui::PushID(&currentLevelIndex);
        if (ImGui::Combo(
                "", &currentLevelIndex,
                [](void *data, int idx, const char **out_text) {
                    auto &levelPaths =
                        *static_cast<std::vector<std::string> *>(data);
                    if (idx >= levelPaths.size() || idx < 0)
                        return false;
                    *out_text = levelPaths[idx].c_str();
                    return true;
                },
                static_cast<void*>(&levelPaths), levelPaths.size())) {
        }
        ImGui::PopID();

        ImGui::SameLine();

        if (ImGui::Button("Load")) {
            onSelect(levelPaths[currentLevelIndex]);
        }
    }
}