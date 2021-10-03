#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <brush/pencilbrush.h>
#include "canvas/canvas.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Painter");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable some options

    ImGui::SFML::Init(window);

    Canvas canvas("res/random.jpg");

    sf::Vector2i prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;

    sf::Clock deltaClock;
    sf::Vector2i size = canvas.getSize();

    PencilBrush brush(canvas);

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
                    brush.onMouseDown(tex_pos);
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased) {
                if(event.mouseButton.button == sf::Mouse::Middle) {
                    pressed = false;
                } else if(event.mouseButton.button == sf::Mouse::Left) {
                    left_pressed = false;
                }
            }
            else if(event.type == sf::Event::MouseMoved) {
                if(pressed) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    canvas.move({(tex_pos.x - prev_pos.x), (tex_pos.y - prev_pos.y)});
                    prev_pos = tex_pos;
                } else if(left_pressed) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window);
                    brush.onMouseMoved(tex_pos);
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
        ImGui::Begin("Lol");
        ImGui::Text("Hello");
        ImGui::InputInt2("Size of canvas", &size.x);
        if(ImGui::Button("Commit size (resize)")) {
            canvas.resize(size);
        }
        ImGui::SliderInt("Radius of line", &brush.settings.size, 0, 100);

        ImGui::SliderScalarN("Brush color", ImGuiDataType_U8, &brush.settings.color.r, 4, &sf::Color::Black.r, &sf::Color::White.r);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}
