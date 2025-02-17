#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

namespace fs = std::filesystem;

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    draw_menu();
    draw_content();
    draw_info();

    ImGui::End();
}

void WindowClass::draw_menu()
{
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto s_pressed = ImGui::IsKeyPressed(ImGuiKey_S);
    const auto l_pressed = ImGui::IsKeyPressed(ImGuiKey_L);

    if (ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(text_buffer_, 0, buffer_size);
    }

    draw_save_popup();
    draw_load_popup();
}

void WindowClass::draw_save_popup()
{
    static char saveFilenameBuffer[256] = "text.txt";
    const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(popup_pos);
    if (ImGui::BeginPopupModal("Save File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", popup_button_size))
        {
            save_to_file(saveFilenameBuffer);
            current_filename_ = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popup_button_size) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void WindowClass::draw_load_popup()
{
    static char loadFilenameBuffer[256] = "text.txt";
    const auto esc_pressed = ImGui::IsKeyPressed(ImGuiKey_Escape);

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(popup_pos);
    if (ImGui::BeginPopupModal("Load File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename",
                         loadFilenameBuffer,
                         sizeof(loadFilenameBuffer));

        if (ImGui::Button("Load", popup_button_size))
        {
            load_from_file(loadFilenameBuffer);
            current_filename_ = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popup_button_size) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void WindowClass::draw_content()
{
    static constexpr auto inputTextSize = ImVec2(1200.0F, 625.0F);
    static constexpr auto lineNumberSize = ImVec2(30.0F, inputTextSize.y);
    static constexpr auto inputTextFlags =
        ImGuiInputTextFlags_AllowTabInput |
        ImGuiInputTextFlags_NoHorizontalScroll;

    ImGui::BeginChild("LineNumbers", lineNumberSize);

    const auto line_count =
        std::count(text_buffer_, text_buffer_ + buffer_size, '\n') + 1;

    for (auto i = 1; i <= line_count; ++i)
        ImGui::Text("%d", i);

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::InputTextMultiline("###inputField",
                              text_buffer_,
                              buffer_size,
                              inputTextSize,
                              inputTextFlags);
}

void WindowClass::draw_info()
{
    if (current_filename_.size() == 0)
    {
        ImGui::Text("No File Opened!");
        return;
    }

    const auto file_extension = get_file_extension(current_filename_);
    ImGui::Text("Opened file %s | File extension %s",
                current_filename_.data(),
                file_extension.data());
}

void WindowClass::save_to_file(std::string_view filename)
{
    auto out = std::ofstream{filename.data()};

    if (out.is_open())
    {
        out << text_buffer_;
        out.close();
    }
}

void WindowClass::load_from_file(std::string_view filename)
{
    auto in = std::ifstream{filename.data()};

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        std::memcpy(text_buffer_, buffer.str().data(), buffer_size);
        in.close();
    }
}

std::string WindowClass::get_file_extension(std::string_view filename)
{
    const auto file_path = fs::path{filename};

    return file_path.extension().string();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Text Editor");
}
