#include <cmath>
#include <fitness_graph.h>

#include <imgui.h>
#include <implot.h>
#include <iostream>
#include <ranges>
#include <sstream>

void CFitnessGraph::render()
{
    constexpr size_t max_count = 1000;
    const auto count = std::min(static_cast<int>(max_count), static_cast<int>(_data.size()));
    static float xs[max_count];
    static float ys[max_count];

    for (int i = 0; i < count; i++)
    {
        // Obtém fator de 10 que reduz o número para um valor entre 1 e 10
        const auto factor = static_cast<float>(std::pow(10, std::floor(std::log10(_data[i].second))));
        // Divide o valor pelo fator
        ys[i] = _data[i].second / factor;
        xs[i] = static_cast<float>(_data[i].first);
        // ys[i] = _data[i].second;
    }
    std::stringstream ss;
    ss << "Fitness (x 10^";
    if (!_data.empty() > 0)
    {
        ss << std::floor(std::log10(_data[0].second)) << ")";
    }
    else
    {
        ss << "0)";
    }

    ImGui::Begin("Fitness Graph");

    if (ImPlot::BeginPlot("Fitness"))
    {
        constexpr auto xflags = ImPlotAxisFlags_AutoFit;
        constexpr auto yflags = ImPlotAxisFlags_AutoFit;
        ImPlot::SetupAxes("Generation",ss.str().c_str(), xflags, yflags);
        ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross);
        ImPlot::PlotLine("Fitness", xs, ys, count);
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void CFitnessGraph::set(const data_set_t &data)
{
    _data = data;
}
