#include <cstring>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <string>

#include "render.hpp"

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

    DrawMenu();
    //ImGui::SameLine(); /// don't move to the next line
    ImGui::Separator();
    DrawContent();
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100.0F);
    ImGui::Separator();
    DrawActions();
    ImGui::Separator();
    DrawFilter();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    if (ImGui::Button("Go Up "))
    {
        if (current_path_.has_parent_path())
        {
            current_path_ = current_path_.parent_path();
        }
    }
    ImGui::SameLine();

    ImGui::Text("Current Path: %s", current_path_.string().c_str());
}

void WindowClass::DrawContent()
{
    for (const auto &entry : fs::directory_iterator(current_path_))
    {
        const auto is_entry_selected = entry.path() == selected_entry_;
        const auto is_directory = entry.is_directory();
        const auto is_file = entry.is_regular_file();

        const auto &entry_name = entry.path().filename().string();

        auto prefix = std::string{"[?] "};
        if (is_directory)
        {
            prefix = "[D] ";
        }
        else if (is_file)
        {
            prefix = "[F] ";
        }

        if (ImGui::Selectable((prefix + entry_name).c_str(), is_entry_selected))
        {
            selected_entry_ = entry.path();
            if (is_directory)
            {
                current_path_ /= selected_entry_.filename();
            }
        }
    }
}

void WindowClass::DrawActions()
{
    if (fs::exists(selected_entry_))
    {
        ImGui::Text("Selected: %s", selected_entry_.string().c_str());

        if (fs::is_regular_file(selected_entry_) && ImGui::Button("Open"))
        {
            OpenFileWithDefaultEditor();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete"))
        {
            is_delete_dialod_open_ = true;
            ImGui::OpenPopup("Delete");
        }
        ImGui::SameLine();
        if (ImGui::Button("Rename"))
        {
            ImGui::OpenPopup("Rename");
        }
    }
    else
    {
        ImGui::Text("No file selected");
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                            ImGui::GetStyle().Alpha * 0.0F);
        ImGui::Button("Invisible Non-clickable Button");
        ImGui::PopStyleVar();
    }

    RenameEntryPopup();
    DeleteEntryPopup();
}

void WindowClass::DrawFilter()
{
    static char filter_buffer_[16] = {"\0"};
    ImGui::Text("Filter by extension");
    ImGui::SameLine();
    ImGui::InputText("##Filter", filter_buffer_, sizeof(filter_buffer_));

    if (std::strlen(filter_buffer_) == 0)
    {
        return;
    }

    auto filtered_file_count = std::size_t{0};
    for (const auto &entry : fs::directory_iterator(current_path_))
    {
        if (entry.is_regular_file())
        {
            const auto &entry_extension = entry.path().extension().string();
            if (entry_extension.ends_with(filter_buffer_))
            {
                ++filtered_file_count;
            }
        }
    }

    ImGui::Text("Filtered files: %zu", filtered_file_count);
}

void WindowClass::OpenFileWithDefaultEditor()
{
#ifdef _WIN32
    const auto command = "start \"\" \"" + selected_entry_.string() + "\"";
#elif __APPLE__
    const auto command = "open \"" + selected_entry_.string() + "\"";
#else
    const auto command = "xdg-open \"" + selected_entry_.string() + "\"";
#endif
    std::system(command.c_str());
}

void WindowClass::RenameEntryPopup()
{
    if (ImGui::BeginPopupModal("Rename",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        const auto entry_name = selected_entry_.filename().string();
        static char new_name_buffer_[512] = {"\0"};

        ImGui::Text("Rename");
        ImGui::InputText("###New Name",
                         new_name_buffer_,
                         sizeof(new_name_buffer_));
        if (ImGui::Button("Rename"))
        {
            const auto new_name = fs::path{new_name_buffer_};
            const auto new_path = selected_entry_.parent_path() / new_name;
            if (RenameEntry(new_path))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        memset(new_name_buffer_, 0, sizeof(new_name_buffer_));

        ImGui::EndPopup();
    }
}

void WindowClass::DeleteEntryPopup()
{
    if (ImGui::BeginPopupModal("Delete",
                               &is_delete_dialod_open_,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete %s?",
                    selected_entry_.filename().string().c_str());
        if (ImGui::Button("Yes"))
        {
            if (DeleteEntry())
            {
                is_delete_dialod_open_ = false;
                // ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            is_delete_dialod_open_ = false;
            // ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

bool WindowClass::RenameEntry(const fs::path &new_path)
{
    try
    {
        fs::rename(selected_entry_, new_path);
        selected_entry_ = new_path;
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error renaming file: " << e.what() << '\n';
        return false;
    }
}

bool WindowClass::DeleteEntry()
{
    try
    {
        fs::remove_all(selected_entry_);
        selected_entry_.clear();
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error deleting file: " << e.what() << '\n';
        return false;
    }
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
