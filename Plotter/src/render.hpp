#pragma once

#include <array>
#include <cstdint>
#include <set>
#include <string_view>

class WindowClass
{
public:
    constexpr static auto function_names =
        std::array<std::string_view, 3>{"unknown", "sin(x)", "cos(x)"};

    enum class Function
    {
        unknown,
        sin,
        cos
    };

public:
    WindowClass() : selected_functions{{}} {};
    void Draw(std::string_view label);

public:
    std::set<Function> selected_functions;

private:
    void draw_selection();
    void draw_plot();

    Function function_name_mapping(std::string_view function_name);
    double evaluate_function(const Function function, const double x);
};

void render(WindowClass &window_obj);
