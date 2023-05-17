#include <imgui.h>
#include "Laser.hpp"
#include "SimulationManager.hpp"

void ImGuiInterface(bool& lowQuality, Parameters& obstaclesParameters, NeighborsParameters& neighborsParameters, Parameters& bordersParameters, LaserParameters& lasersParameters)
{
    ImGui::Begin("Simulation control");
    ImGui::SliderFloat("Alignment distance", &neighborsParameters.alignment.distance, 0.f, 2.f);
    ImGui::SliderFloat("Alignment strength", &neighborsParameters.alignment.strength, 0.f, 1.f);
    ImGui::SliderFloat("Cohesion distance", &neighborsParameters.cohesion.distance, 0.f, 2.f);
    ImGui::SliderFloat("Cohesion strength", &neighborsParameters.cohesion.strength, 0.f, 1.f);
    ImGui::SliderFloat("Separation distance", &neighborsParameters.separation.distance, 0.f, 2.f);
    ImGui::SliderFloat("Separation strength", &neighborsParameters.separation.strength, 0.f, 1.f);
    ImGui::SliderFloat("Obstacles distance", &obstaclesParameters.distance, 0.f, 0.2f);
    ImGui::SliderFloat("Obstacles strength", &obstaclesParameters.strength, 0.f, 1.f);
    ImGui::SliderFloat("Borders distance", &bordersParameters.distance, 0.f, 1.f);
    ImGui::SliderFloat("Borders strength", &bordersParameters.strength, 0.f, 1.f);
    ImGui::SliderFloat("Lasers range", &lasersParameters.range, 0.f, 2.f);
    ImGui::SliderFloat("Lasers speed", &lasersParameters.speed, 0.f, 3.f);
    bool changeQuality = ImGui::Button("HD/LD");
    ImGui::End();

    if (changeQuality)
    {
        lowQuality = !lowQuality;
    }
}
