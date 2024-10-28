// File: order.h
// Author: TrifingZW
// Created: 2024-10-27 at 00:10:46
// Description:
// Dependencies:

#pragma once
#include "book.h"

class order
{
public:
    std::vector<std::shared_ptr<book>> books;
    bool valid = true;
    bool pay;
    std::string date;

    [[nodiscard]] nlohmann::json to_json() const;
    static order from_json(const nlohmann::json& j_object);
};
