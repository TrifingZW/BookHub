// File: novel.h
// Author: TrifingZW
// Created: 2024-10-26 at 18:10:43
// Description:
// Dependencies:

#include "novel.h"

void novel::ex_display_info() const
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 0.9f, 1.0f)); // 灰色背景
    ImGui::BeginChild("出版信息", ImVec2(0, 50), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextColored(blue_color, "%s 于 %s 出版", publishing_house.c_str(), publication_date.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
