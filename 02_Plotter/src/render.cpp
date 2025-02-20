#include <array>
#include <cmath>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

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

    draw_selection();
    draw_plot();
    ImGui::End();
}


WindowClass::Function WindowClass::function_name_mapping(
    std::string_view function_name)
{
    if (function_name == "sin(x)")
    {
        return Function::sin;
    }
    else if (function_name == "cos(x)")
    {
        return Function::cos;
    }
    else
    {
        return Function::unknown;
    }
}

double WindowClass::evaluate_function(const Function function, const double x)
{
    switch (function)
    {
    case WindowClass::Function::cos:
    {
        return std::sin(x);
    }
    case WindowClass::Function::sin:
    {
        return std::cos(x);
    }
    case WindowClass::Function::unknown:
    default:
    {
        return 0;
    }
    }
}


void WindowClass::draw_selection()
{
    for (const auto &function_name : function_names)
    {
        const auto cur_function = function_name_mapping(function_name);
        auto selected = selected_functions.contains(cur_function);

        if (ImGui::Checkbox(function_name.data(), &selected))
        {
            if (selected)
            {
                selected_functions.insert(cur_function);
            }
            else
            {
                selected_functions.erase(cur_function);
            }
        }
    }
}

void WindowClass::draw_plot()
{
    static constexpr auto num_point = 10'000;
    static constexpr auto x_min = -100.0;
    static constexpr auto x_max = 100;

    static const auto x_step = (std::abs(x_min) + std::abs(x_max)) / num_point;

    static auto xs = std::array<double, num_point>{};
    static auto ys = std::array<double, num_point>{};


    if (selected_functions.empty() ||
        (selected_functions.size() == 1 &&
         *selected_functions.begin() == Function::unknown))
    {
        ImPlot::BeginPlot("###pot", ImVec2(-1, -1), ImPlotFlags_NoTitle);

        ImPlot::EndPlot();
        return;
    }

    ImPlot::BeginPlot("###pot", ImVec2(-1, -1), ImPlotFlags_NoTitle);

    for (const auto &function : selected_functions)
    {
        auto x = x_min;
        for (std::size_t i = 0; i < num_point; ++i)
        {
            xs[i] = x;
            ys[i] = evaluate_function(function, x);
            x += x_step;
        }

        const auto plot_label =
            fmt::format("##function{}",
                        function_names[static_cast<std::size_t>(function)]);

        ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_point);
    }

    ImPlot::EndPlot();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
