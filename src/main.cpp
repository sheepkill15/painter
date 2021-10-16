#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <glad/glad.h>
#include "brush/pencilbrush.h"
#include "canvas/canvas.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Painter");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable some options
    window.setActive();
    gladLoadGL();
    ImGui::SFML::Init(window);

    Canvas canvas("res/random.jpg");

    sf::Vector2f prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;

    sf::Clock deltaClock;
    std::unique_ptr<Brush> brush = std::make_unique<PencilBrush>(canvas);
    Brush::initShader();
    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if(io.WantCaptureMouse) continue;

            switch(event.type) {
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Middle) {
                        prev_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        pressed = true;
                    } else if(event.mouseButton.button == sf::Mouse::Left) {
                        const sf::Vector2f tex_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        left_pressed = true;
                        brush->onMouseDown(tex_pos);
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.mouseButton.button == sf::Mouse::Middle) {
                        pressed = false;
                    } else if(event.mouseButton.button == sf::Mouse::Left) {
                        left_pressed = false;
                        const sf::Vector2f tex_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        brush->onMouseUp(tex_pos);
                    }
                    break;
                case sf::Event::MouseMoved:
                    if(pressed) {
                        const sf::Vector2f tex_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        canvas.move({(tex_pos.x - prev_pos.x), (tex_pos.y - prev_pos.y)});
                        prev_pos = tex_pos;
                    } else if(left_pressed) {
                        const sf::Vector2f tex_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        brush->onMouseMoved(tex_pos);
                    }
                    break;
                case sf::Event::MouseWheelScrolled:
                    canvas.scale(event.mouseWheelScroll.delta);
                    break;
                case sf::Event::Resized:
                    {
                        // update the view to the new size of the window
                        const sf::FloatRect visibleArea(0, 0, (float) event.size.width, (float) event.size.height);
                        window.setView(sf::View(visibleArea));
                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color::Black);

        canvas.draw(window);
        brush->settings.DrawUI();
        canvas.DrawUI();

        ImGui::Begin("Brushes");
        if(ImGui::Button("Pencil")) {
            brush = std::make_unique<PencilBrush>(canvas);
        }

        if(ImGui::Button("Line")) {

        }
        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}
