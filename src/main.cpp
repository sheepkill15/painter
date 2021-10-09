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

    sf::Vector2i prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;

    sf::Clock deltaClock;
    std::unique_ptr<Brush> brush = std::make_unique<PencilBrush>(canvas);
    Brush::initShader();
    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if(event.type == sf::Event::Closed)
                window.close();
            if(io.WantCaptureMouse) continue;
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Middle) {
                    prev_pos = sf::Mouse::getPosition(window);
                    pressed = true;
                } else if(event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    left_pressed = true;
                    brush->onMouseDown(tex_pos);
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased) {
                if(event.mouseButton.button == sf::Mouse::Middle) {
                    pressed = false;
                } else if(event.mouseButton.button == sf::Mouse::Left) {
                    left_pressed = false;
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    brush->onMouseUp(tex_pos);
                }
            }
            else if(event.type == sf::Event::MouseMoved) {
                if(pressed) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    canvas.move({(tex_pos.x - prev_pos.x), (tex_pos.y - prev_pos.y)});
                    prev_pos = tex_pos;
                } else if(left_pressed) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    brush->onMouseMoved(tex_pos);
                }
            }
            else if(event.type == sf::Event::MouseWheelScrolled) {
                canvas.scale(event.mouseWheelScroll.delta);
            }
            else if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
                window.setView(sf::View(visibleArea));
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
