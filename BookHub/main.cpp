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
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // 设置 Platform/Renderer 后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // 加载字体
    // - 如果没有加载任何字体，Dear ImGui 将使用默认字体。你也可以加载多个字体并使用 ImGui::PushFont()/PopFont() 来选择它们。
    // - AddFontFromFileTTF() 将返回一个 ImFont*，你可以存储它以便在多个字体之间进行选择。
    // - 如果文件无法加载，该函数将返回 nullptr。请在你的应用程序中处理这些错误（例如使用断言，或者显示错误信息并退出）。
    // - 字体将在给定大小下进行栅格化（带有超采样），并在调用 ImFontAtlas::Build()/GetTexDataAsXXXX() 时存储到纹理中，这些函数会在 ImGui_ImplXXXX_NewFrame 调用时使用。
    // - 在你的 imconfig 文件中使用 '#define IMGUI_ENABLE_FREETYPE' 以启用 Freetype，从而实现更高质量的字体渲染。
    // - 阅读 'docs/FONTS.md' 获取更多说明和细节。
    // - 请记住，在 C/C++ 中如果要在字符串字面量中包含反斜杠 \，需要写成双反斜杠 \\。
    // - 我们的 Emscripten 构建过程允许嵌入字体，使其在运行时可以从 "fonts/" 文件夹访问。有关详细信息请参阅 Makefile.emscripten。
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

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
        // 轮询和处理事件（输入、窗口大小调整等）
        // 你可以读取 io.WantCaptureMouse 和 io.WantCaptureKeyboard 标志，以判断 Dear ImGui 是否需要使用你的输入。
        // - 当 io.WantCaptureMouse 为 true 时，不要将鼠标输入数据传递给你的主应用程序，或者清除/覆盖你对鼠标数据的拷贝。
        // - 当 io.WantCaptureKeyboard 为 true 时，不要将键盘输入数据传递给你的主应用程序，或者清除/覆盖你对键盘数据的拷贝。
        // 通常，你可以将所有输入传递给 Dear ImGui，并根据这两个标志将它们隐藏在你的应用程序中。
        glfwPollEvents();
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

        /*// 1. 显示大的演示窗口（大部分示例代码都在 ImGui::ShowDemoWindow() 中！你可以浏览其代码以了解更多关于 Dear ImGui 的信息）。
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. 显示一个我们自己创建的简单窗口。我们使用 Begin/End 成对创建一个命名窗口。
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("你好"); // 创建一个名为 "Hello, world!" 的窗口，并在其中追加内容。

            ImGui::Text("This is some useful text."); // 显示一些文本（你也可以使用格式化字符串）
            ImGui::Checkbox("Demo Window", &show_demo_window); // 编辑存储窗口打开/关闭状态的布尔值
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // 使用滑块编辑一个从 0.0f 到 1.0f 的浮点数

            if (ImGui::Button("Button")) // 按钮在被点击时返回 true（大多数控件在被编辑/激活时返回 true）
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. 显示另一个简单的窗口。
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            // 传递一个指向布尔变量的指针（窗口将有一个关闭按钮，点击时会清除布尔值）
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }*/

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
