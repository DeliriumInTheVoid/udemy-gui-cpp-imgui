#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

class WindowClass
{
public:
    WindowClass()
        : current_path_{fs::current_path()}, selected_entry_{fs::path{}} {};
    /** main method for draw UI */
    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawContent();
    void DrawActions();
    void DrawFilter();

    void OpenFileWithDefaultEditor();

    void RenameEntryPopup();
    void DeleteEntryPopup();

    bool RenameEntry(const fs::path &new_path);
    bool DeleteEntry();

private:
    fs::path current_path_;
    fs::path selected_entry_;

    bool is_delete_dialod_open_{};
};

void render(WindowClass &window_obj);
