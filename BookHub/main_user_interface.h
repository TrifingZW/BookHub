// File: main_user_interface.h
// Author: TrifingZW
// Created: 2024-10-26 at 01:10:13
// Description:  Declares the main user interface class for rendering and managing
//               the Dear ImGui-based user interface. This class provides 
//               functions for initializing and updating the UI components.
//               It serves as the interface between the application logic 
// Dependencies: GLFW (for window management), ImGui (UI framework), OpenGL (graphics API)

#pragma once
#include <vector>

#include "book.h"
#include "imgui.h"
#include "user.h"

#include <memory>

class main_user_interface
{
public:
    // 示例的书籍列表
    std::vector<std::shared_ptr<book>> books;
    std::vector<std::shared_ptr<user>> users;
    std::vector<std::shared_ptr<book>> shopping_cart;
    std::shared_ptr<user> current_user;
    bool login;

    // 静态方法，获取单例实例
    static main_user_interface& instance()
    {
        static main_user_interface user_interface;
        return user_interface;
    }

    // UI相关函数
    void render_ui();
    void show_book_window();
    void show_tool_window();
    void show_user_window() const;
    void show_sale_popup();
    void show_recharge_popup() const;
    void show_logo_popup();
    void show_order_popup(int index);
    void show_shopping_cart_popup();
    ImGuiID dock_space_id;

    // 读取json
    std::string books_filename = "books.json";
    void load_books_from_file();
    void save_books_to_file() const;

    // 保存json
    std::string users_filename = "users.json";
    void load_users_from_file();
    void save_users_to_file() const;
};
