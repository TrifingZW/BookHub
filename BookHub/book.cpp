// File: book.cpp
// Author: TrifingZW
// Created: 2024-11-13 at 00:11:32
// Description:
// Dependencies:

#include "book.h"

nlohmann::json book::to_json() const
{
    return nlohmann::json{
        {"name", name},
        {"description", description},
        {"isbn", isbn},
        {"author", author},
        {"price", price}
    };
}

void book::from_json(const nlohmann::json& j)
{
    name = j.at("name").get<std::string>();
    description = j.at("description").get<std::string>();
    isbn = j.at("isbn").get<std::string>();
    author = j.at("author").get<std::string>();
    price = j.at("price").get<float>();
}

void book::display_info() const
{
    ImGui::PushFont(large_font);
    ImGui::SeparatorText(name.c_str());
    ImGui::PopFont();
    ex_display_info();
    ImGui::Text("作者: %s", author.c_str());
    ImGui::Text("ISBN: %s", isbn.c_str());
    ImGui::TextColored(red_color, "价格: ￥%.2f", price);
    ImGui::Spacing();
    ImGui::TextWrapped("Description: %s", description.c_str());
    ImGui::Spacing();
}
