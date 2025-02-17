#pragma once

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

#include <imgui.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
    static constexpr auto buffer_size = std::size_t{1024};
    static constexpr auto popup_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popup_size = ImVec2(300.0F, 100.0F);
    static constexpr auto popup_button_size = ImVec2(120.0F, 0.0F);
    static constexpr auto popup_pos =
        ImVec2(1280.0F / 2.0F - popup_size.x / 2.0F,
               720.0F / 2.0F - popup_size.y / 2.0F);

public:
    WindowClass() : current_filename_({})
    {
        std::memset(text_buffer_, 0, buffer_size);
    }

    void Draw(std::string_view label);

private:
    void draw_menu();
    void draw_save_popup();
    void draw_load_popup();
    void draw_content();
    void draw_info();

    void save_to_file(std::string_view filename);
    void load_from_file(std::string_view filename);
    std::string get_file_extension(std::string_view filename);

private:
    char text_buffer_[buffer_size];
    std::string current_filename_;
};

void render(WindowClass &window_obj);
