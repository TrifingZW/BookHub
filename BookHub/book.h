// File: book.h
// Author: TrifingZW
// Created: 2024-10-26 at 01:10:01
// Description:
// Dependencies:

#pragma once
#include <string>

#include "imgui.h"
#include "json.hpp"

// 颜色常量
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

    // 转化为json对象
    [[nodiscard]] virtual nlohmann::json to_json() const;

    // 从json对象读取
    virtual void from_json(const nlohmann::json& j);

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

    // 显示信息
    void display_info() const;

    // 显示额外信息
    virtual void ex_display_info() const
    {
    }
};
