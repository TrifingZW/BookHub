#include "text_book.h"

void text_book::ex_display_info() const
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 灰色背景
    ImGui::BeginChild("课程信息", ImVec2(0, 50), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextColored(red_color, "%s %d年级 %d学期", course.c_str(), grade, semester);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
