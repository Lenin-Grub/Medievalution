#include "SceneDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>

SceneDisplay::SceneDisplay(BattleMap& battle_map)
    :battle_map(battle_map)
{
}

void SceneDisplay::draw()
{
    ImGui::Begin("Scene");
    ImGui::Text("Game View - %dx%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    if (canvasSize.x > 0 && canvasSize.y > 0)
    {
        static sf::RenderTexture renderTexture;
        if (renderTexture.getSize().x != (unsigned int)canvasSize.x ||
            renderTexture.getSize().y != (unsigned int)canvasSize.y) {
            renderTexture.create((unsigned int)canvasSize.x, (unsigned int)canvasSize.y);
        }

        sf::View sceneView;
        sceneView.reset(sf::FloatRect(0, 0, canvasSize.x, canvasSize.y));

        sf::Vector2f cameraOffset = common::view.getCenter() - (sf::Vector2f(canvasSize.x, canvasSize.y) * 0.5f);
        float zoomFactorX = common::view.getSize().x / canvasSize.x;
        float zoomFactorY = common::view.getSize().y / canvasSize.y;

        sceneView.move(cameraOffset);
        sceneView.zoom(zoomFactorX);

        renderTexture.setView(sceneView);

        renderTexture.clear(sf::Color(50, 50, 50, 255));
        battle_map.draw(renderTexture, sf::RenderStates::Default);
        renderTexture.display();

        renderTexture.setView(renderTexture.getDefaultView());

        const sf::Texture& texture = renderTexture.getTexture();
        ImTextureID textureID = (ImTextureID)(uintptr_t)texture.getNativeHandle();

        ImGui::Image(textureID,
            ImVec2(canvasSize.x, canvasSize.y),
            ImVec2(0, 1),
            ImVec2(1, 0));
    }

    ImGui::End();
}

void SceneDisplay::update(const float& delta_time)
{
}