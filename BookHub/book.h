// File: book.h
// Author: TrifingZW
// Created: 2024-10-26 at 01:10:01
// Description:
// Dependencies:

#pragma once
#include <iostream>
#include <string>

#include "imgui.h"
#include "json.hpp"

constexpr auto red_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
constexpr auto green_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
constexpr auto blue_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
constexpr auto yellow_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

extern ImFont* large_font;

class book
{
public:
    std::string name;
    std::string description;
    std::string isbn;
    std::string author;
    float price;

    virtual ~book() = default;
    book(const book& other) = default;
    book& operator=(const book& other) = default;
    book(book&& other) noexcept = default;
    book& operator=(book&& other) noexcept = default;

    book()
        : name("未知名称"),
          description("未知描述"),
          isbn("未知isbn"),
          author("未知作者"),
          price(0.0f)
    {
    }

    book(
        std::string n,
        std::string d,
        std::string i,
        std::string a,
        float p)
        : name(std::move(n)),
          description(std::move(d)),
          isbn(std::move(i)),
          author(std::move(a)),
          price(p)
    {
    }

    [[nodiscard]] virtual nlohmann::json to_json() const
    {
        return nlohmann::json{
            {"name", name},
            {"description", description},
            {"isbn", isbn},
            {"author", author},
            {"price", price}
        };
    }

    // 从 JSON 对象反序列化
    static book from_json(const nlohmann::json& j)
    {
        return book{
            j.at("name").get<std::string>(),
            j.at("description").get<std::string>(),
            j.at("isbn").get<std::string>(),
            j.at("author").get<std::string>(),
            j.at("price").get<float>()
        };
    }

    static nlohmann::json books_to_json(const std::vector<book>& books)
    {
        nlohmann::json j_array = nlohmann::json::array();
        for (const auto& book : books)
            j_array.push_back(book.to_json());
        return j_array;
    }

    // 用于临时存储输入的缓冲区
    struct book_input_buffer
    {
        char name[128] = "";
        char description[256] = "";
        char isbn[32] = "";
        char author[64] = "";
        char course[128] = "";
        int grade;
        int semester;
        char publishing_house[128] = "";
        char publication_date[64] = "";
        float price = 0.0f;
    };

    void display_info() const
    {
        // 卡片内容
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

    virtual void ex_display_info() const
    {
    }
};
