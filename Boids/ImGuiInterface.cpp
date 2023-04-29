#include <imgui.h>
#include "BoidsManager.hpp"

void ImGuiInterface(float& BOIDS_SIZE, NeighborsParameters& NEIGHBORS_PARAMETERS, Parameters& BORDERS_PARAMETERS)
{
    ImGui::Begin("Control");
    ImGui::SliderFloat("Boids size", &BOIDS_SIZE, 0.01f, 1.f);
    ImGui::SliderFloat("Alignment distance", &NEIGHBORS_PARAMETERS.alignment.distance, 0.f, 2.f);
    ImGui::SliderFloat("Alignment strength", &NEIGHBORS_PARAMETERS.alignment.strength, 0.f, 1.f);
    ImGui::SliderFloat("Cohesion distance", &NEIGHBORS_PARAMETERS.cohesion.distance, 0.f, 2.f);
    ImGui::SliderFloat("Cohesion strength", &NEIGHBORS_PARAMETERS.cohesion.strength, 0.f, 1.f);
    ImGui::SliderFloat("Separation distance", &NEIGHBORS_PARAMETERS.separation.distance, 0.f, 2.f);
    ImGui::SliderFloat("Separation strength", &NEIGHBORS_PARAMETERS.separation.strength, 0.f, 1.f);
    ImGui::SliderFloat("Borders distance", &BORDERS_PARAMETERS.distance, 0.f, 1.f);
    ImGui::SliderFloat("Borders strength", &BORDERS_PARAMETERS.strength, 0.f, 1.f);
    ImGui::End();
}
