// File: user.cpp
// Author: TrifingZW
// Created: 2024-10-26 at 12:10:59
// Description:
// Dependencies:

#include "user.h"

#include "main_user_interface.h"

void user::show_user_info()
{
    // 外层卡片样式
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f); // 圆角边框

    // 背景色和边框颜色
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f)); // 浅灰色背景
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // 边框颜色

    // 用户卡片容器
    ImGui::BeginChild("UserCard", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // 顶部区域：标题与头像
    ImGui::PushFont(large_font); // 更大的字体用于标题
    ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "用户信息");
    ImGui::PopFont();
    ImGui::Separator();

    ImGui::Spacing();

    // 用户头像模拟（占位圆形头像）
    ImGui::Dummy(ImVec2(60, 60)); // 占位
    ImGui::SameLine();
    ImGui::Text("Hello, %s", name.c_str());

    ImGui::Spacing();

    // ID 信息
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.9f, 1.0f), "ID:");
    ImGui::SameLine(100);
    ImGui::TextWrapped("%s", id.c_str());

    // 名称信息
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.9f, 1.0f), "姓名:");
    ImGui::SameLine(100);
    ImGui::Text("%s", name.c_str());

    // 密码信息
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.9f, 1.0f), "密码:");
    ImGui::SameLine(100);
    ImGui::Text("********");

    // 余额信息
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.9f, 1.0f), "余额:");
    ImGui::SameLine(100);
    ImGui::Text("%.2f", balance);

    ImGui::Spacing();
    ImGui::Separator();

    // 底部区域：操作按钮
    if (ImGui::Button("退出"))
    {
        main_user_interface::instance().login = false;
        main_user_interface::instance().current_user = nullptr;
    }
    ImGui::SameLine();
    if (ImGui::Button("订单"))
        ImGui::OpenPopup("OrdersModalPopup");
    show_orders_popup();

    // 恢复样式
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(3);
    ImGui::EndChild();
}

void user::show_orders_popup()
{
    if (ImGui::BeginPopupModal("OrdersModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::BeginChild("OrderList", ImVec2(600, 500), false);
        ImGui::PopStyleColor();

        for (int i1 = 0; i1 < static_cast<int>(orders.size()); i1++)
        {
            ImGui::PushID(i1);
            const auto order1 = orders[i1];
            const auto books = order1->books;
            ImGui::BeginChild("Order", ImVec2(0, 200), true);

            for (int i2 = 0; i2 < static_cast<int>(books.size()); i2++)
            {
                const auto& book = books[i2];
                ImGui::PushID(i2);
                ImGui::TextColored(red_color, "%s", book->name.c_str());
                ImGui::SameLine();
                ImGui::TextColored(red_color, "单品价格: ￥%.2f", book->price);
                ImGui::PopID();
            }

            const auto prices = std::accumulate(
                books.begin(),
                books.end(),
                0.0f,
                [](const float acc, const std::shared_ptr<book>& book) { return acc + book->price; });
            ImGui::TextColored(red_color, "总价格: ￥%.2f", prices);

            ImGui::PushFont(large_font);
            ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 80, 20));
            if (order1->pay)
                ImGui::TextColored(green_color, "已支付");
            else
                ImGui::TextColored(red_color, "未支付");
            ImGui::PopFont();

            if (order1->valid)
            {
                ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 160, 140));
                if (ImGui::Button("取消订单"))
                {
                    std::ranges::copy(books, std::back_inserter(main_user_interface::instance().books));
                    order1->valid = false;
                    main_user_interface::instance().save_books_to_file();
                    main_user_interface::instance().save_users_to_file();
                }
                ImGui::SameLine();
                if (ImGui::Button("支付"))
                {
                    if (balance >= prices)
                    {
                        balance -= prices;
                        order1->pay = true;
                        order1->valid = false;
                        main_user_interface::instance().save_books_to_file();
                        main_user_interface::instance().save_users_to_file();
                    }
                    else std::cout << "余额不足！" << '\n';
                }
            }

            ImGui::EndChild();
            ImGui::PopID();
        }
        ImGui::EndChild();

        if (ImGui::Button("关闭窗口", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            ImGui::CloseCurrentPopup();


        ImGui::EndPopup();
    }
}

nlohmann::json user::to_json() const
{
    auto j_orders = nlohmann::json::array();
    for (const auto& order : orders)
        j_orders.push_back(order->to_json());
    return {
        {"id", id},
        {"name", name},
        {"password", password},
        {"balance", balance},
        {"orders", j_orders}
    };
}

user user::from_json(const nlohmann::json& j)
{
    user user1;
    user1.id = j.at("id").get<std::string>();
    user1.name = j.at("name").get<std::string>();
    user1.password = j.at("password").get<std::string>();
    user1.balance = j.at("balance").get<float>();
    for (const auto& basic_jsons : j.at("orders"))
        user1.orders.push_back(std::make_shared<order>(order::from_json(basic_jsons)));
    return user1;
}
