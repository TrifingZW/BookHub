// File: order.h
// Author: TrifingZW
// Created: 2024-10-27 at 00:10:46
// Description:
// Dependencies:

#include "order.h"

#include "novel.h"
#include "text_book.h"

nlohmann::json order::to_json() const
{
    nlohmann::json j_object;
    j_object["books"] = nlohmann::json::array();
    j_object["text_books"] = nlohmann::json::array();
    j_object["novels"] = nlohmann::json::array();
    for (const auto& book1 : books)
        if (const auto ptr = std::dynamic_pointer_cast<text_book>(book1))
            j_object["text_books"].push_back(ptr->to_json());
        else if (const auto ptr1 = std::dynamic_pointer_cast<novel>(book1))
            j_object["novels"].push_back(ptr1->to_json());
        else if (const auto ptr2 = std::dynamic_pointer_cast<book>(book1))
            j_object["books"].push_back(ptr2->to_json());
    j_object["valid"] = valid;
    j_object["pay"] = pay;
    j_object["date"] = date;

    return j_object;
}

order order::from_json(const nlohmann::json& j_object)
{
    order order1;

    // 加载 books 数组
    if (j_object.contains("books") && j_object["books"].is_array())
        for (const auto& item : j_object["books"])
            order1.books.push_back(std::make_shared<book>(book::from_json(item)));

    // 加载 text_books 数组
    if (j_object.contains("text_books") && j_object["text_books"].is_array())
        for (const auto& item : j_object["text_books"])
            order1.books.push_back(std::make_shared<text_book>(text_book::from_json(item)));

    // 加载 novels 数组
    if (j_object.contains("novels") && j_object["novels"].is_array())
        for (const auto& item : j_object["novels"])
            order1.books.push_back(std::make_shared<novel>(novel::from_json(item)));

    order1.valid = j_object.at("valid").get<bool>();
    order1.pay = j_object.at("pay").get<bool>();
    order1.date = j_object.at("date").get<std::string>();
    return order1;
}
