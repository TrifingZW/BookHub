// File: novel.h
// Author: TrifingZW
// Created: 2024-10-26 at 18:10:43
// Description:
// Dependencies:

#pragma once
#include "book.h"

class novel : public book
{
public:
    std::string publishing_house;
    std::string publication_date;

    novel()
        : publishing_house("未知出版社"),
          publication_date("未知出版时间")
    {
    }

    novel(std::string n,
          std::string d,
          std::string i,
          std::string a,
          const float p,
          std::string ph,
          std::string pd)
        : book(
              std::move(n),
              std::move(d),
              std::move(i),
              std::move(a),
              p),
          publishing_house(std::move(ph)),
          publication_date(std::move(pd))
    {
    }

    [[nodiscard]] nlohmann::json to_json() const override
    {
        nlohmann::json json = book::to_json();
        json.push_back({"publishing_house", publishing_house});
        json.push_back({"publication_date", publication_date});
        return json;
    }

    static novel from_json(const nlohmann::json& j)
    {
        return novel{
            j.at("name").get<std::string>(),
            j.at("description").get<std::string>(),
            j.at("isbn").get<std::string>(),
            j.at("author").get<std::string>(),
            j.at("price").get<float>(),
            j.at("publishing_house").get<std::string>(),
            j.at("publication_date").get<std::string>()
        };
    }

    void ex_display_info() const override;
};
