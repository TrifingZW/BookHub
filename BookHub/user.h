// File: user.h
// Author: TrifingZW
// Created: 2024-10-26 at 12:10:59
// Description:
// Dependencies:

#pragma once
#include <string>

#include "json.hpp"
#include "order.h"

class user
{
public:
    std::string id;
    std::string name;
    std::string password;
    float balance;
    std::vector<std::shared_ptr<order>> orders;

    // 显示用户信息
    void show_user_info();

    // 显示订单弹窗
    void show_orders_popup();

    [[nodiscard]] nlohmann::json to_json() const;
    static user from_json(const nlohmann::json& j);
};
