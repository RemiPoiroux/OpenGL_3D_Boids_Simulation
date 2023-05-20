#include <imgui.h>
#include <string>
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "RandomManager.hpp"
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

void ImGuiInterface(bool& lowQuality, Parameters& obstaclesParameters, NeighborsParameters& neighborsParameters, Parameters& bordersParameters, LaserParameters& lasersParameters, const int lives, Parameters& characterForce, const int boidsNb, RandomVariablesParameters& randomParameters)
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
    bool changeQuality = ImGui::Button("HD/LD");
    ImGui::End();

    ImGui::Begin("Game UI");
    ImGui::Text("Lives: %d", lives);
    ImGui::Text("%d enemies left", boidsNb);
    ImGui::End();

    ImGui::Begin("Random variables control");
    ImGui::Text("Ennemies' behavior : discrete variable");
    float sumProbs = 0.0f;
    for (size_t i = 0; i < randomParameters.boidsAttitude.stateNames.size(); i++)
    {
        std::string label = BoidBehaviorToString(randomParameters.boidsAttitude.stateNames[i]) + " prob";
        ImGui::SliderFloat(label.c_str(), &randomParameters.boidsAttitude.stateProbs[i], 0.f, 1.f);
        sumProbs += randomParameters.boidsAttitude.stateProbs[i];
    }
    if (sumProbs > 1.0f)
    {
        for (float& stateProb : randomParameters.boidsAttitude.stateProbs)
        {
            stateProb /= sumProbs;
        }
    }
    ImGui::Separator();
    ImGui::Text("Ennemies' collision with asteroid : Bernoulli variable");
    ImGui::SliderFloat("Collision prob", &randomParameters.collisionWithObstacles.collisionProb, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Ennemies' fire : Binomial variable");
    ImGui::SliderFloat("Shoot prob/frame", &randomParameters.boidsFiring.shootProbPerFrame, 0.f, 1.f);
    ImGui::SliderFloat("Seconds to shoot", &randomParameters.boidsFiring.secondsToShoot, 0.f, 2.f);
    ImGui::Separator();
    ImGui::Text("Ennemies' inaccuracy : Normal variable");
    ImGui::SliderFloat("Inaccuracy variance", &randomParameters.boidsAccuracy.inaccuracyExpectation, 0.f, 1.f);
    ImGui::Separator();
    ImGui::Text("Character's fire : Geometric variable");
    ImGui::SliderFloat("My shoot prob/frame", &randomParameters.characterFiring.shootProbPerFrame, 0.f, 1.f);
    ImGui::End();

    if (changeQuality)
    {
        lowQuality = !lowQuality;
    }
}
