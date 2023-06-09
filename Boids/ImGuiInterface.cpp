#include <imgui.h>
#include <cmath>
#include "RandomManager.hpp"
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

float calculateMean(const std::vector<float>& generations)
{
    float sum = 0.0f;
    for (float value : generations)
    {
        sum += value;
    }
    return sum / static_cast<float>(generations.size());
}
float calculateVariance(const std::vector<float>& generations)
{
    float mean     = calculateMean(generations);
    float variance = 0.0f;
    for (float value : generations)
    {
        variance += static_cast<float>(pow(value - mean, 2));
    }
    return variance / static_cast<float>(generations.size());
}
std::vector<uint> countValuesInIntervals(const std::vector<float>& generations)
{
    float minGeneration = *std::min_element(generations.begin(), generations.end());
    float maxGeneration = *std::max_element(generations.begin(), generations.end());

    std::vector<uint> intervalCounts(20, 0);
    float             interval = (maxGeneration - minGeneration) / 20.0f;
    for (float generation : generations)
    {
        uint intervalIndex = static_cast<uint>((generation - minGeneration) / interval);
        if (intervalIndex < 20)
        {
            intervalCounts[intervalIndex]++;
        }
    }
    return intervalCounts;
}
void drawUnchangedVarStatsTable(const UnchangedVarStats& stats)
{
    std::vector<uint> intervalCounts = countValuesInIntervals(stats.generations);
    float             mean           = calculateMean(stats.generations);
    float             variance       = calculateVariance(stats.generations);
    float             relativeDiff   = std::abs(variance - stats.variance) / stats.variance;
    size_t            population     = stats.generations.size();

    ImGui::BeginTable("UnchangedVarStatsTable", 2);

    ImGui::TableSetupColumn("Range");
    ImGui::TableSetupColumn("Number of values");
    ImGui::TableHeadersRow();

    float minGeneration = *std::min_element(stats.generations.begin(), stats.generations.end());
    float maxGeneration = *std::max_element(stats.generations.begin(), stats.generations.end());
    float interval      = (maxGeneration - minGeneration) / 20.0f;

    for (size_t i = 0; i < intervalCounts.size(); i++)
    {
        float intervalStart = minGeneration + static_cast<float>(i) * interval;
        float intervalEnd   = minGeneration + static_cast<float>(i + 1) * interval;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("[%f, %f)", intervalStart, intervalEnd);

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", intervalCounts[i]);
    }

    ImGui::EndTable();

    ImGui::Text("Expectation: %f", stats.expectation);
    ImGui::Text("Espected variance: %f", stats.variance);
    ImGui::Text("Mean: %f", mean);
    ImGui::Text("Statistical variance: %f", variance);

    if (relativeDiff <= 0.1f)
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Variance Relative Diff: %.2f", relativeDiff);
    }
    else
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Variance Relative Diff: %.2f", relativeDiff);
    }
    ImGui::Text("On a population of %zu", population);
}

void drawModifiableVarStatsTable(const ModifiableVarStats& stats)
{
    ImGui::BeginTable("ModifiableVarStatsTable", 6);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Expectation");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("Expected Variance");
    ImGui::TableSetColumnIndex(2);
    ImGui::Text("Population");
    ImGui::TableSetColumnIndex(3);
    ImGui::Text("Generations Variance");
    ImGui::TableSetColumnIndex(4);
    ImGui::Text("Variance Relative Diff");

    float  totalRelativeDiff = 0.0f;
    size_t validCount        = 0;

    for (size_t i = 0; i < stats.expectations.size(); i++)
    {
        if (stats.counts[i] != 0)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.2f", stats.expectations[i]);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f", stats.variances[i]);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d", stats.counts[i]);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2f", stats.generationsVariances[i]);

            float relativeDiff = std::abs(stats.variances[i] - stats.generationsVariances[i]) / stats.variances[i];
            if (!std::isnan(relativeDiff) && std::isfinite(relativeDiff))
            {
                totalRelativeDiff += relativeDiff * static_cast<float>(stats.counts[i]);
                validCount += stats.counts[i];
            }

            ImGui::TableSetColumnIndex(4);
            if (relativeDiff <= 0.1f)
            {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.2f", relativeDiff);
            }
            else
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "%.2f", relativeDiff);
            }
        }
    }

    ImGui::EndTable();

    if (validCount != 0)
    {
        float averageRelativeDiff = totalRelativeDiff / static_cast<float>(validCount);
        if (averageRelativeDiff <= 0.1f)
        {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Variance average Relative Diff (Weighted): %.2f", averageRelativeDiff);
        }
        else
        {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Variance average Relative Diff (Weighted): %.2f", averageRelativeDiff);
        }
        ImGui::Text("On a population of %zu", validCount);
    }
}

struct WeightedRelativeDiffAndPopulation
{
    float weightedRelativeDiff;
    uint  population;
};
WeightedRelativeDiffAndPopulation drawDiscreteStatsTable(const DiscreteStats& stats, const std::vector<BoidBehavior>& names)
{
    uint  totalCount                = 0;
    float totalWeightedRelativeDiff = 0.0f;

    for (unsigned int count : stats.counts)
    {
        totalCount += count;
    }

    if (totalCount > 0)
    {
        ImGui::BeginTable("DiscreteStatsTable", 5);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Behavior");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Expectation");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Count");
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("Normalized Count");
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("Relative Diff");

        for (size_t i = 0; i < stats.expectations.size(); i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", BoidBehaviorToString(names[i]).c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f", stats.expectations[i]);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%u", stats.counts[i]);

            ImGui::TableSetColumnIndex(3);
            float normalizedCount = static_cast<float>(stats.counts[i]) / static_cast<float>(totalCount);
            ImGui::Text("%.2f", normalizedCount);

            ImGui::TableSetColumnIndex(4);
            float relativeDiff = std::abs(stats.expectations[i] - normalizedCount) / stats.expectations[i];

            relativeDiff = std::abs(stats.expectations[i] - normalizedCount) / stats.expectations[i];
            ImGui::Text("%.2f", relativeDiff);
            if (!std::isnan(relativeDiff) && std::isfinite(relativeDiff) && !std::isnan(normalizedCount) && std::isfinite(normalizedCount))
            {
                float weightedRelativeDiff = relativeDiff * normalizedCount;
                totalWeightedRelativeDiff += weightedRelativeDiff;
            }
        }

        ImGui::EndTable();

        float averageWeightedRelativeDiff = totalWeightedRelativeDiff / static_cast<float>(stats.expectations.size());
        ImGui::Text("Average Weighted Relative Diff: %.2f", averageWeightedRelativeDiff);
        ImGui::Text("On a population of %u", totalCount);
        return {averageWeightedRelativeDiff, totalCount};
    }
    return {0, 0};
}
void drawModifiableDiscreteVarStatsTables(const ModifiableDiscreteVarStats<BoidBehavior>& stats)
{
    float totalWeightedRelativeDiff = 0;
    uint  population                = 0;

    for (size_t i = 0; i < stats.stats.size(); i++)
    {
        uint count = 0;
        for (uint it : stats.stats[i].counts)
        {
            count += it;
        }
        if (count != 0)
        {
            ImDrawList* drawList      = ImGui::GetWindowDrawList();
            ImVec2      startPos      = ImGui::GetCursorScreenPos();
            ImVec2      endPos        = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
            ImColor     lineColor     = {0, 0, 1};
            float       lineThickness = 1.0f;
            drawList->AddLine(startPos, endPos, lineColor, lineThickness);

            WeightedRelativeDiffAndPopulation relativeDiffAndPopulation =
                drawDiscreteStatsTable(stats.stats[i], stats.names);
            totalWeightedRelativeDiff +=
                relativeDiffAndPopulation.weightedRelativeDiff * static_cast<float>(relativeDiffAndPopulation.population);
            population += relativeDiffAndPopulation.population;
        }
    }
    ImDrawList* drawList      = ImGui::GetWindowDrawList();
    ImVec2      startPos      = ImGui::GetCursorScreenPos();
    ImVec2      endPos        = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    ImColor     lineColor     = {0, 0, 1};
    float       lineThickness = 2.0f;
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);

    float averageWeightedRelativeDiff = 0;
    if (population != 0)
    {
        averageWeightedRelativeDiff = totalWeightedRelativeDiff / static_cast<float>(population);
    }
    if (averageWeightedRelativeDiff <= 0.1f)
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Total Average Weighted Relative Diff: %.2f", averageWeightedRelativeDiff);
    }
    else
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Total Average Weighted Relative Diff: %.2f", averageWeightedRelativeDiff);
    }
    ImGui::Text("On a population of %u", population);
}

void endImGuiInterface(bool victory, const RandomVariables& randomVariables)
{
    if (victory)
    {
        ImGui::Begin("Victory!");
    }
    else
    {
        ImGui::Begin("Game over!");
    }

    ImGui::Text("Random variables' stats");
    ImGui::Separator();
    ImGui::Separator();

    ImGui::Text("World creation random variables :");
    ImGui::Separator();

    ImGui::Text("Asteroids' Sizes: Exponential Var");
    ImDrawList* drawList      = ImGui::GetWindowDrawList();
    ImVec2      startPos      = ImGui::GetCursorScreenPos();
    ImVec2      endPos        = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    ImColor     lineColor     = {0, 0, 1};
    float       lineThickness = 1.0f;
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawUnchangedVarStatsTable(randomVariables.obstacleSizesVar.getStats());
    ImGui::Separator();

    ImGui::Text("Boids' Lives: Poisson Var");
    drawList = ImGui::GetWindowDrawList();
    startPos = ImGui::GetCursorScreenPos();
    endPos   = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawUnchangedVarStatsTable(randomVariables.boidsLivesVar.getStats());
    ImGui::Separator();
    ImGui::Separator();

    ImGui::Text("Evolutive random variables :");
    ImGui::Separator();

    ImGui::Text("Boids' Collisions with Asteroids: Bernouilli Var");
    drawList = ImGui::GetWindowDrawList();
    startPos = ImGui::GetCursorScreenPos();
    endPos   = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawModifiableVarStatsTable(randomVariables.collisionWithObstaclesVar.getStats());
    ImGui::Separator();

    ImGui::Text("Boids' firing : Binomial Var");
    drawList = ImGui::GetWindowDrawList();
    startPos = ImGui::GetCursorScreenPos();
    endPos   = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawModifiableVarStatsTable(randomVariables.boidsFiringVar.getStats());
    ImGui::Separator();

    ImGui::Text("Boids' Precision : Normal Var");
    drawList = ImGui::GetWindowDrawList();
    startPos = ImGui::GetCursorScreenPos();
    endPos   = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawModifiableVarStatsTable(randomVariables.boidsPrecisionVar.getStats());
    ImGui::Separator();

    ImGui::Text("Character's firing delays : Geometric Var");
    drawList = ImGui::GetWindowDrawList();
    startPos = ImGui::GetCursorScreenPos();
    endPos   = ImVec2(startPos.x + ImGui::GetContentRegionAvail().x, startPos.y);
    drawList->AddLine(startPos, endPos, lineColor, lineThickness);
    drawModifiableVarStatsTable(randomVariables.characterFiringVar.getStats());
    ImGui::Separator();

    ImGui::Text("Boids' Behavior : Discrete Var");
    drawModifiableDiscreteVarStatsTables(randomVariables.boidsAttitudeVar.getStats());
    ImGui::Separator();

    ImGui::End();
}