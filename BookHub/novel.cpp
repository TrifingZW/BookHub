// File: novel.h
// Author: TrifingZW
// Created: 2024-10-26 at 18:10:43
// Description:
// Dependencies:

#include "novel.h"

nlohmann::json novel::to_json() const
{
    nlohmann::json json = book::to_json();
    json.push_back({"publishing_house", publishing_house});
    json.push_back({"publication_date", publication_date});
    return json;
}

void novel::from_json(const nlohmann::json& j)
{
    book::from_json(j);
    publishing_house = j.at("publishing_house").get<std::string>();
    publication_date = j.at("publication_date").get<std::string>();
}

void novel::ex_display_info() const
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 0.9f, 1.0f)); // 灰色背景
    ImGui::BeginChild("出版信息", ImVec2(0, 50), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextColored(blue_color, "%s 于 %s 出版", publishing_house.c_str(), publication_date.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
