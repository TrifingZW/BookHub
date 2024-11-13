// File: main.cpp
// Author: TrifingZW
// Created: 2024-10-26 at 01:10:13
// Description: This file contains the main entry point for the application.
//              It initializes the GLFW window, sets up ImGui, and manages
//              the main render loop.
// Dependencies:

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <windows.h>
#include "main_user_interface.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

using namespace std;

ImFont* large_font = nullptr; // 定义全局字体指针

// 处理错误信息
void glfw_error_callback(const int error, const char* description)
{
    const int result = fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    cout << result << '\n';
}

// 获取系统 DPI 缩放
float get_dpi_scale_for_window(const HWND hwnd)
{
    const UINT dpi = GetDpiForWindow(hwnd);
    constexpr UINT default_dpi = 96; // 标准 DPI 值为 96
    return static_cast<float>(dpi) / default_dpi;
}

// 设置所有控件的缩放
void apply_dpi_scale(const float dpi_scale)
{
    ImGui::GetStyle().ScaleAllSizes(dpi_scale);
}

// 主代码
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const auto glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

    // 使用图形上下文创建窗口
    GLFWwindow* window = glfwCreateWindow(1280, 720, "BookHub", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 启用 vsync

    // 设置 Dear ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控件
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // 启用游戏手柄控件
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 启动停靠功能

    // 设置 Dear ImGui 样式
    ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    constexpr auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    main_user_interface::instance().load_books_from_file();
    main_user_interface::instance().load_users_from_file();

    // 设置dpi为系统dpi
    const HWND hwnd = glfwGetWin32Window(window);
    const float dpi_scale = get_dpi_scale_for_window(hwnd);
    apply_dpi_scale(dpi_scale);

    io.Fonts->AddFontFromFileTTF(
        "misc/fonts/HarmonyOS_Sans_SC_Bold.ttf",
        16.0f * dpi_scale,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull());
    large_font = io.Fonts->AddFontFromFileTTF(
        "misc/fonts/HarmonyOS_Sans_SC_Bold.ttf",
        24.0f * dpi_scale,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull());

    // 主循环
    while (!glfwWindowShouldClose(window))
    {

        // 处理所有待处理的窗口事件
        glfwPollEvents();

        // 如果窗口最小化,暂停程序一段时间，避免过多的无用计算
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // 启动 Dear ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        main_user_interface::instance().render_ui();

        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
