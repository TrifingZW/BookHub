// File: text_book.h
// Author: TrifingZW
// Created: 2024-10-26 at 13:10:09
// Description:
// Dependencies:

#pragma once
#include "book.h"

class text_book : public book
{
public:
    std::string course;
    int grade;
    int semester;

    text_book()
        : course("未知课程"),
          grade(0),
          semester(0)
    {
    }

    text_book(std::string n,
              std::string d,
              std::string i,
              std::string a,
              const float p,
              std::string c,
              const int g,
              const int pub)
        : book(
              std::move(n),
              std::move(d),
              std::move(i),
              std::move(a),
              p),
          course(std::move(c)),
          grade(g),
          semester(pub)
    {
    }

    [[nodiscard]] nlohmann::json to_json() const override
    {
        return {
            {"course", course},
            {"grade", grade},
            {"semester", semester}
        };
    }

    static text_book from_json(const nlohmann::json& j)
    {
        return text_book{
            j.at("name").get<std::string>(),
            j.at("description").get<std::string>(),
            j.at("isbn").get<std::string>(),
            j.at("author").get<std::string>(),
            j.at("price").get<float>(),
            j.at("course").get<std::string>(),
            j.at("grade").get<int>(),
            j.at("semester").get<int>()
        };
    }

    void ex_display_info() const override;
};
