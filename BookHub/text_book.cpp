#include "text_book.h"

nlohmann::json text_book::to_json() const
{
    nlohmann::json json = book::to_json();
    json.push_back({"course", course});
    json.push_back({"grade", grade});
    json.push_back({"semester", semester});
    return json;
}

void text_book::from_json(const nlohmann::json& j)
{
    book::from_json(j);
    course = j.at("course").get<std::string>();
    grade = j.at("grade").get<int>();
    semester = j.at("semester").get<int>();
}

void text_book::ex_display_info() const
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 灰色背景
    ImGui::BeginChild("课程信息", ImVec2(0, 50), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextColored(red_color, "%s %d年级 %d学期", course.c_str(), grade, semester);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
