// File: order.h
// Author: TrifingZW
// Created: 2024-10-27 at 00:10:46
// Description:
// Dependencies:

#include "order.h"

nlohmann::json order::to_json() const
{
    return {
        {"book1", book1->to_json()},
        {"valid", valid},
        {"pay", pay},
        {"date", date}
    };
}

order order::from_json(const nlohmann::json& j)
{
    order order1;
    order1.book1 = std::make_shared<book>(book::from_json(j.at("book1")));
    order1.valid = j.at("valid").get<bool>();
    order1.pay = j.at("pay").get<bool>();
    order1.date = j.at("date").get<std::string>();
    return order1;
}
