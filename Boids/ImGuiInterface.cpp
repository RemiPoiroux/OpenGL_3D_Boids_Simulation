#include <imgui.h>
#include "Boid.hpp"
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "RandomManager.hpp"
#include "RandomVariables.hpp"
#include "Render.hpp"
#include "SimulationManager.hpp"

std::string BoidBehaviorToString(BoidBehavior behavior)
{
    switch (behavior)
    {
    case BoidBehavior::Attacks:
        return "Attacks";
    case BoidBehavior::Neutral:
        return "Neutral";
    case BoidBehavior::Flees:
        return "Flees";
    default:
        return "Unknown";
    }
}

void gameImGuiInterface(LodsParameters& lodsParameters, Parameters& obstaclesParameters, NeighborsParameters& neighborsParameters, Parameters& bordersParameters, LaserParameters& lasersParameters, const int lives, Parameters& characterForce, const int boidsNb, RandomVariablesParameters& randomParameters, RandomVariables& randomVariables)
{
    ImGui::Begin("Simulation forces control");
    ImGui::Text("Ennemies' alignment force");
    ImGui::SliderFloat("Alignement distance", &neighborsParameters.alignment.distance, 0.f, 2.f);
    ImGui::SliderFloat("Alignement strength", &neighborsParameters.alignment.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Ennemies' cohesion force");
    ImGui::SliderFloat("Cohesion distance", &neighborsParameters.cohesion.distance, 0.f, 2.f);
    ImGui::SliderFloat("Cohesion strength", &neighborsParameters.cohesion.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Ennemies' separation force");
    ImGui::SliderFloat("Separation distance", &neighborsParameters.separation.distance, 0.f, 2.f);
    ImGui::SliderFloat("Separation strength", &neighborsParameters.separation.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Character's avoidance force");
    ImGui::SliderFloat("Character distance", &characterForce.distance, 0.f, 2.f);
    ImGui::SliderFloat("Character strength", &characterForce.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Asteroids' avoidance force");
    ImGui::SliderFloat("Asteroid distance", &obstaclesParameters.distance, 0.f, 0.2f);
    ImGui::SliderFloat("Asteroid strength", &obstaclesParameters.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Obstacles' avoidance force");
    ImGui::SliderFloat("Obstacle Distance", &bordersParameters.distance, 0.f, 1.f);
    ImGui::SliderFloat("Obstacle strength", &bordersParameters.strength, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Lasers");
    ImGui::SliderFloat("Range", &lasersParameters.range, 0.f, 0.069f);
    ImGui::SliderFloat("Speed", &lasersParameters.speed, 0.f, 3.f);
    ImGui::Separator();
    ImGui::Text("Objects' quality");
    ImGui::SliderFloat("HD distance", &lodsParameters.HDDistance, 0.f, 2.f);
    bool changeQuality = ImGui::Button("HD/LD");
    ImGui::End();

    ImGui::Begin("Game UI");
    ImGui::Text("Lives: %d", lives);
    ImGui::Text("%d enemies left", boidsNb);
    ImGui::End();

    ImGui::Begin("Random variables control");

    ImGui::Text("Ennemies' behavior : discrete variable");
    float sumProbs = 0.0f;
    bool  edited   = false;
    for (size_t i = 0; i < randomParameters.boidsAttitude.stateNames.size(); i++)
    {
        std::string label = BoidBehaviorToString(randomParameters.boidsAttitude.stateNames[i]) + " prob";
        ImGui::SliderFloat(label.c_str(), &randomParameters.boidsAttitude.stateProbs[i], 0.f, 1.f);
        sumProbs += randomParameters.boidsAttitude.stateProbs[i];
        if (ImGui::IsItemEdited())
        {
            edited = true;
        };
    }
    if (sumProbs != 1.0f)
    {
        for (float& stateProb : randomParameters.boidsAttitude.stateProbs)
        {
            stateProb /= sumProbs;
        }
    }
    if (edited)
    {
        randomVariables.boidsAttitudeVar.modify(randomParameters.boidsAttitude.stateProbs);
    }
    ImGui::Separator();

    ImGui::Text("Ennemies' collision with asteroid : Bernoulli variable");
    ImGui::SliderFloat("Collision prob", &randomParameters.collisionWithObstacles.collisionProb, 0.f, 1.f);
    if (ImGui::IsItemEdited())
    {
        randomVariables.collisionWithObstaclesVar.modify(randomParameters.collisionWithObstacles.collisionProb);
    }
    ImGui::Separator();

    ImGui::Text("Ennemies' fire : Binomial variable");
    bool editedBoidsFiring = false;
    ImGui::SliderFloat("Shoot prob/frame", &randomParameters.boidsFiring.shootProbPerFrame, 0.f, 1.f);
    if (ImGui::IsItemEdited())
    {
        editedBoidsFiring = true;
    }
    ImGui::SliderFloat("Seconds to shoot", &randomParameters.boidsFiring.secondsToShoot, 0.f, 2.f);
    if (ImGui::IsItemEdited())
    {
        editedBoidsFiring = true;
    }
    if (editedBoidsFiring)
    {
        randomVariables.boidsFiringVar.modify(randomParameters.boidsFiring.shootProbPerFrame, static_cast<uint>(randomParameters.boidsFiring.secondsToShoot * 60));
    }

    ImGui::Separator();
    ImGui::Text("Ennemies' inaccuracy : Normal variable");
    ImGui::SliderFloat("Inaccuracy variance", &randomParameters.boidsAccuracy.inaccuracyExpectation, 0.f, 1.f);
    if (ImGui::IsItemEdited())
    {
        randomVariables.boidsPrecisionVar.modify(randomParameters.boidsAccuracy.inaccuracyExpectation);
    }

    ImGui::Separator();
    ImGui::Text("Character's fire : Geometric variable");
    ImGui::SliderFloat("My shoot prob/frame", &randomParameters.characterFiring.shootProbPerFrame, 0.f, 1.f);
    if (ImGui::IsItemEdited())
    {
        randomVariables.characterFiringVar.modify(randomParameters.characterFiring.shootProbPerFrame);
    }

    if (changeQuality)
    {
        lodsParameters.lowQuality = !lodsParameters.lowQuality;
    }

    ImGui::End();
}

void endImGuiInterface(bool victory, RandomVariables randomVariables)
{
    if (victory)
    {
        ImGui::Begin("Victory!");
    }
    else
    {
        ImGui::Begin("Game over!");
    }
    ImGui::End();
}

// #include <imgui.h>
// #include <cmath>
// #include "implot.h"

// void drawGraph(const UnchangedStats& stats)
// {
//     double x_min = 0.0;
//     double x_max = stats.generations.size();
//     double y_min = 0.0;
//     double y_max = stats.generations.size() / 100.0;

//     ImPlot::SetupAxisLimits(ImPlotAxis_X, x_min, x_max);
//     ImPlot::SetupAxisLimits(ImPlotAxis_Y, y_min, y_max);
//     if (ImPlot::BeginPlot("Graphique", "Génération", "Nombre de valeurs", ImVec2(800, 600)))
//     {
//         ImPlot::PlotStems("Nombre de valeurs", stats.generations.data(), stats.generations.size(), 0.5f);

//         ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
//         ImPlot::PlotLine("Espérance", &stats.expectation, 1);
//         ImPlot::PopStyleColor();

//         float varianceMin = stats.expectation - std::sqrt(stats.variance);
//         float varianceMax = stats.expectation + std::sqrt(stats.variance);

//         ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
//         ImPlot::PlotLine("Espérance", &varianceMin, 1);
//         ImPlot::PlotLine("Espérance", &varianceMax, 1);
//         ImPlot::PopStyleColor();

//         ImPlot::EndPlot();
//     }
// }