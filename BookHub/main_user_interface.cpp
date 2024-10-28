// File: main_user_interface.cpp
// Author: TrifingZW
// Created: 2024-10-26 at 01:10:13
// Description: Implements the main user interface class, which manages rendering
//              and updating the Dear ImGui user interface. This file includes
//              initialization, event handling, and drawing functions to create
//              the interactive UI components.
// Dependencies: GLFW (for window management), ImGui (UI framework), OpenGL (graphics API)

#include "main_user_interface.h"

#include <fstream>
#include <iostream>
#include <random>

#include "imgui.h"
#include "novel.h"
#include "text_book.h"

std::string get_current_time_string()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time;
    std::cout << localtime_s(&local_time, &current_time) << '\n';
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

template <typename T>
void initialize_book(T& book, const book::book_input_buffer& buffer)
{
    book.name = buffer.name;
    book.description = buffer.description;
    book.isbn = buffer.isbn;
    book.author = buffer.author;
    book.price = buffer.price;
}

bool contains_char(const std::string& str1, const std::string& str2, const std::string& str3, char ch[])
{
    return str1.find(ch) != std::string::npos ||
        str2.find(ch) != std::string::npos ||
        str3.find(ch) != std::string::npos;
}

std::string generate_key(const size_t length)
{
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string key;
    key.reserve(length);
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);
    for (size_t i = 0; i < length; ++i)
        key += characters[distribution(generator)];
    return key;
}

void main_user_interface::render_ui()
{
    static ImGuiDockNodeFlags docks_pace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // 全屏
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    if (docks_pace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    dock_space_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), docks_pace_flags);

    if (ImGui::BeginMenuBar())
    {
        //增加主题切换
        if (ImGui::BeginMenu("主题（Other）"))
        {
            if (ImGui::MenuItem("暗黑（Dark）")) { ImGui::StyleColorsDark(); }
            if (ImGui::MenuItem("明亮（Light）")) { ImGui::StyleColorsLight(); }
            if (ImGui::MenuItem("经典（Classic）")) { ImGui::StyleColorsClassic(); }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

    show_book_window();
    show_tool_window();
    show_user_window();
    if (!login)
        show_logo_popup();
}

void main_user_interface::show_book_window()
{
    ImGui::SetNextWindowDockID(dock_space_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("书本列表");

    // 固定搜索栏
    ImGui::BeginChild("SearchBar", ImVec2(0, 40), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    static char search[256] = "";
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 50);
    ImGui::InputText("搜索", search, sizeof(search));
    ImGui::EndChild();

    // 滚动区域用于显示书本列表
    ImGui::BeginChild("BookList", ImVec2(0, 0), true);

    int index1 = -1;
    int index2 = -1;
    static int selected1 = -1;
    static int selected2 = -1;
    for (int i = 0; i < static_cast<int>(books.size()); ++i)
    {
        const auto book = books[i];
        if (search[0] != '\0' && !contains_char(book->name, book->isbn, book->author, search))
            continue;

        ImGui::PushID(i); // 推入ID，避免控件ID冲突
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); // 设置内边距
        ImGui::BeginGroup();

        book->display_info();

        // 购买按钮，放置在右侧
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 150);
        if (ImGui::Button("购物车"))
            index2 = i;
        ImGui::SameLine();
        if (ImGui::Button("购买"))
            index1 = i;
        ImGui::PopID();
        ImGui::PopStyleVar();
        ImGui::EndGroup();
    }
    if (index1 != -1)
    {
        ImGui::OpenPopup("OrderModalPopup");
        selected1 = index1;
    }
    show_order_popup(selected1);

    if (index2 != -1)
    {
        selected2 = index2;
        shopping_cart.push_back(std::move(books[selected2]));
        books.erase(books.begin() + selected2);
    }
    ImGui::EndChild();
    ImGui::End();
}

void main_user_interface::show_tool_window()
{
    ImGui::SetNextWindowDockID(dock_space_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("工具列表");
    const float window_width = ImGui::GetContentRegionAvail().x;

    if (ImGui::Button("售卖书籍", ImVec2(window_width, 0)))
        ImGui::OpenPopup("SaleModalPopup");
    show_sale_popup();

    if (ImGui::Button("余额充值", ImVec2(window_width, 0)))
        ImGui::OpenPopup("RechargeModalPopup");
    show_recharge_popup();

    if (ImGui::Button("购物车", ImVec2(window_width, 0)))
        ImGui::OpenPopup("ShoppingCartModalPopup");
    show_shopping_cart_popup();

    ImGui::End();
}

void main_user_interface::show_user_window() const
{
    ImGui::SetNextWindowDockID(dock_space_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("账户");
    current_user ? current_user->show_user_info() : ImGui::Text("未登录");
    ImGui::End();
}

void main_user_interface::show_sale_popup()
{
    if (ImGui::BeginPopupModal("SaleModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGuiStyle& style = ImGui::GetStyle();
        const float old_frame_border_size = style.FrameBorderSize;
        style.FrameBorderSize = 1.0f;

        static book::book_input_buffer buffer; // 使用静态缓冲区以保持状态

        const char* items[] = {"普通书籍", "教材", "小说"}; // 选项列表
        static int item_current = 0; // 记录当前选中的选项索引
        if (ImGui::Combo("书本类型", &item_current, items, IM_ARRAYSIZE(items)))
        {
            // 当用户选择了一个新的选项时，可以在这里执行相应的代码
        }

        ImGui::InputText("书名", buffer.name, sizeof(buffer.name));
        ImGui::InputText("描述", buffer.description, sizeof(buffer.description));
        ImGui::InputText("ISBN", buffer.isbn, sizeof(buffer.isbn));
        ImGui::InputText("作者", buffer.author, sizeof(buffer.author));
        ImGui::InputFloat("价格", &buffer.price, 0.01f, 1.0f, "%.2f");

        switch (item_current)
        {
        case 1:
            ImGui::InputText("课程", buffer.course, sizeof(buffer.course));
            ImGui::InputInt("年级", &buffer.grade);
            ImGui::InputInt("学期", &buffer.semester);
            break;
        case 2:
            ImGui::InputText("出版社", buffer.publishing_house, sizeof(buffer.publishing_house));
            ImGui::InputText("出版时间", buffer.publication_date, sizeof(buffer.publication_date));
            break;
        default:
            break;
        }

        style.FrameBorderSize = old_frame_border_size;

        ImGui::Dummy(ImVec2(0, 20));
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 180);

        if (ImGui::Button("取消", ImVec2(80, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::SameLine();
        if (ImGui::Button("确定", ImVec2(80, 0)))
        {
            book book1;
            text_book text_book1;
            novel novel1;
            switch (item_current)
            {
            case 0:
                initialize_book(book1, buffer);
                books.push_back(std::make_shared<book>(book1));
                break;
            case 1:
                initialize_book(text_book1, buffer);
                text_book1.course = buffer.course;
                text_book1.grade = buffer.grade;
                text_book1.semester = buffer.semester;
                books.push_back(std::make_shared<text_book>(text_book1));
                break;
            case 2:
                initialize_book(novel1, buffer);
                novel1.publishing_house = buffer.publishing_house;
                novel1.publication_date = buffer.publication_date;
                books.push_back(std::make_shared<novel>(novel1));
                break;
            default:
                break;
            }
            save_books_to_file();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void main_user_interface::show_recharge_popup() const
{
    if (ImGui::BeginPopupModal("RechargeModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static float amount = 1.0f;
        ImGui::InputFloat("充值金额", &amount, 1.0f, 1.0f, "%.2f");

        // 创建一些空白区域，让按钮对齐到右下角
        ImGui::Dummy(ImVec2(0, 20));

        // 设置按钮到右下角
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 180);

        if (ImGui::Button("取消", ImVec2(80, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::SameLine();
        if (ImGui::Button("确定", ImVec2(80, 0)))
        {
            current_user->balance += amount;
            save_users_to_file();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void main_user_interface::show_logo_popup()
{
    ImGui::OpenPopup("LogoModalPopup");
    if (ImGui::BeginPopupModal("LogoModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char name[256] = "";
        static char password[256] = "";

        ImGuiStyle& style = ImGui::GetStyle();
        const float old_frame_border_size = style.FrameBorderSize;
        style.FrameBorderSize = 1.0f;

        ImGui::InputText("用户名", name, sizeof(name));
        ImGui::InputText("密码", password, sizeof(password));

        style.FrameBorderSize = old_frame_border_size;

        ImGui::Dummy(ImVec2(0, 20));
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 180);

        if (ImGui::Button("注册", ImVec2(80, 0)))
        {
            if (!std::ranges::any_of(users, [](const std::shared_ptr<user>& u) { return u->name == name; }))
            {
                const auto new_user = std::make_shared<user>(user{generate_key(64), name, password, 0.0f, {}});
                current_user = new_user;
                users.push_back(new_user);
                save_users_to_file();
                login = true;
                ImGui::CloseCurrentPopup();
            }
            else std::cout << "该用户已注册！" << '\n';
        }
        ImGui::SameLine();
        if (ImGui::Button("登录", ImVec2(80, 0)))
        {
            for (const auto& user : users)
                if (user->name == name && user->password == password)
                {
                    current_user = user;
                    login = true;
                    ImGui::CloseCurrentPopup();
                    break;
                }
            if (!current_user)std::cout << "账号或密码不正确！" << '\n';
        }

        ImGui::EndPopup();
    }
}

void main_user_interface::show_order_popup(const int index)
{
    if (index == -1) return;
    if (ImGui::BeginPopupModal("OrderModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        const auto book = books[index];
        book->display_info();

        ImGui::Dummy(ImVec2(500, 20));
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 180);

        if (ImGui::Button("取消", ImVec2(80, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::SameLine();
        if (ImGui::Button("下单", ImVec2(80, 0)))
        {
            order order1;
            order1.books.push_back(std::move(books[index]));
            books.erase(books.begin() + index);
            order1.date = get_current_time_string();
            current_user->orders.push_back(std::make_shared<order>(order1));
            save_books_to_file();
            save_users_to_file();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void main_user_interface::show_shopping_cart_popup()
{
    if (ImGui::BeginPopupModal("ShoppingCartModalPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::BeginChild("OrderList", ImVec2(600, 500), false);

        int index = -1;
        for (int i = 0; i < static_cast<int>(shopping_cart.size()); i++)
        {
            const auto& book = shopping_cart[i];
            ImGui::PushID(i);
            ImGui::BeginChild("Order", ImVec2(0, 150), true);
            ImGui::Text("书名: %s", book->name.c_str());
            ImGui::Text("作者: %s", book->author.c_str());
            ImGui::TextColored(red_color, "价格: ￥%.2f", book->price);
            ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 50, 100));
            if (ImGui::Button("删除"))
                index = i;
            ImGui::EndChild();
            ImGui::PopID();
        }

        if (index != -1)
        {
            books.push_back(std::move(shopping_cart[index]));
            shopping_cart.erase(shopping_cart.begin() + index);
        }

        ImGui::EndChild();

        const auto region = ImGui::GetContentRegionAvail();

        if (ImGui::Button("关闭", ImVec2(region.x, 0)))
            ImGui::CloseCurrentPopup();

        if (ImGui::Button("下单", ImVec2(region.x, 0)))
        {
            order order1;
            order1.books = shopping_cart;
            order1.date = get_current_time_string();
            current_user->orders.push_back(std::make_shared<order>(order1));
            shopping_cart.clear();
            save_books_to_file();
            save_users_to_file();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void main_user_interface::load_books_from_file()
{
    if (std::ifstream file(books_filename); file.is_open())
    {
        nlohmann::json j_object;
        file >> j_object;

        // 加载 books 数组
        if (j_object.contains("books") && j_object["books"].is_array())
            for (const auto& item : j_object["books"])
            {
                book book1;
                book1.from_json(item);
                books.push_back(std::make_shared<book>(book1));
            }

        // 加载 text_books 数组
        if (j_object.contains("text_books") && j_object["text_books"].is_array())
            for (const auto& item : j_object["text_books"])
            {
                text_book text_book1;
                text_book1.from_json(item);
                books.push_back(std::make_shared<text_book>(text_book1));
            }

        // 加载 novels 数组
        if (j_object.contains("novels") && j_object["novels"].is_array())
            for (const auto& item : j_object["novels"])
            {
                novel novel1;
                novel1.from_json(item);
                books.push_back(std::make_shared<novel>(novel1));
            }

        file.close();
    }
    else
        std::cerr << "无法打开文件以进行读取: " << books_filename << '\n';
}

void main_user_interface::save_books_to_file() const
{
    nlohmann::json j_object;
    j_object["books"] = nlohmann::json::array();
    j_object["text_books"] = nlohmann::json::array();
    j_object["novels"] = nlohmann::json::array();

    for (const auto& book1 : books)
    {
        if (const auto ptr = std::dynamic_pointer_cast<text_book>(book1))
            j_object["text_books"].push_back(ptr->to_json());
        else if (const auto ptr1 = std::dynamic_pointer_cast<novel>(book1))
            j_object["novels"].push_back(ptr1->to_json());
        else if (const auto ptr2 = std::dynamic_pointer_cast<book>(book1))
            j_object["books"].push_back(ptr2->to_json());
    }

    if (std::ofstream file(books_filename); file.is_open())
    {
        file << j_object.dump(4);
        file.close();
    }
    else
        std::cerr << "无法打开文件以进行写入: " << books_filename << '\n';
}

void main_user_interface::load_users_from_file()
{
    if (std::ifstream file(users_filename); file.is_open())
    {
        nlohmann::json j_object;
        file >> j_object;
        for (const auto& item : j_object)
            users.push_back(std::make_shared<user>(user::from_json(item)));
    }
    else
        std::cerr << "无法打开文件以进行读取: " << users_filename << '\n';
}

void main_user_interface::save_users_to_file() const
{
    nlohmann::json j_object;
    for (const auto& user1 : users)
        j_object.push_back(user1->to_json());
    if (std::ofstream file(users_filename); file.is_open())
    {
        file << j_object.dump(4);
        file.close();
    }
    else
        std::cerr << "无法打开文件以进行写入: " << users_filename << '\n';
}
