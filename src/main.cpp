#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "canvas/canvas.h"

int radius = 5;
ImColor brush_color;

int calculate_y_from_x(int x, int a, int b, int c) {
    return (-x * a - c) / b;
}

int calculate_x_from_y(int y, int a, int b, int c) {
    return (y * b + c) / -a;
}

void rectangle_line(Canvas& canvas, sf::Vector2i pos1, sf::Vector2i pos2) {
    pos1 = canvas.transform_pos(pos1);
    pos2 = canvas.transform_pos(pos2);
    const int a = pos1.y - pos2.y;
    const int b = pos2.x - pos1.x;
    const int c = pos1.x * pos2.y - pos2.x * pos1.y;
    for(int y = pos1.y - radius / 2; y <= pos2.y + radius / 2; y++) {
        int curr_x = a ? calculate_x_from_y(y, a, b, c) : pos1.x - 2 * radius;
        for(int x = pos1.x - radius / 2; x <= pos2.x + radius / 2; x++) {
            int curr_y = b ? calculate_y_from_x(x, a, b, c) : pos1.y - 2 * radius;
            if(std::abs(curr_y - y) <= radius || std::abs(curr_x - x) <= radius) {
                canvas.setPixel(x, y, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), true);
            }
        }
    }
}

void circle(Canvas& canvas, const sf::Vector2i& pos, bool transformed = false) {
    int width = (int)canvas.getSize().x;
    int height = (int)canvas.getSize().y;
    sf::Vector2i cursor = pos;

    for(int i = std::max(0, cursor.y - radius); i <= std::min(height -1, cursor.y + radius); i++) {
        int i_ = i - cursor.y;
        for(int j = std::max(0, cursor.x - radius); j <= std::min(width -1, cursor.x + radius); j++) {
            int j_ = j - cursor.x;
            if(i_*i_ + j_*j_ < radius*radius) {
                canvas.setPixel(j, i, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), transformed);
            }
        }
    }
}


int sign(int x) {
    return (x>>31)|(!!x);
}

void draw_line(Canvas& canvas, const sf::Vector2i& pos1, const sf::Vector2i& pos2, const int a, const int b, const int c) {
    int prev_y = pos1.y;
    const int sgn_x = sign(pos2.x - pos1.x);
    const int sgn_y = sign(pos2.y - pos1.y);

    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
    {
        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
            canvas.setPixel(pos1.x + i, j, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), true);
        }
        canvas.setPixel(pos1.x + i, curr_y, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), true);
        prev_y = curr_y;
    }
    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {
        canvas.setPixel(pos2.x, j, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), true);
    }
    canvas.setPixel(pos2.x, pos2.y, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255), true);
}

void line(Canvas& canvas, const sf::Vector2i& pos1, const sf::Vector2i& pos2) {
    // calculate equation of the line
    const int a = pos1.y - pos2.y;
    const int b = pos2.x - pos1.x;
    const int c = pos1.x * pos2.y - pos2.x * pos1.y;

    const int dx = pos1.x - pos2.x;
    const int dy = pos1.y - pos2.y;
    const double dist = std::sqrt(dx * dx + dy * dy);
    if(dist == 0) return;
    const float normX = dx / dist;
    const float normY = dy / dist;


    sf::Vector2i p_pos1;
    sf::Vector2i p_pos2;

    int prev_y = pos1.y;
    const int sgn_x = sign(pos2.x - pos1.x);
    const int sgn_y = sign(pos2.y - pos1.y);

    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
    {
        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
            for(int z = 0; z <= radius; z++) {

                const int xPerp = z * normX;
                const int yPerp = z * normY;
                p_pos1 = sf::Vector2i(pos1.x + i + yPerp, j - xPerp);
                p_pos2 = sf::Vector2i(pos1.x + i - yPerp, j + xPerp);
                canvas.setPixel(p_pos1.x, p_pos1.y, sf::Color::White, true);
                canvas.setPixel(p_pos2.x, p_pos2.y, sf::Color::White, true);
            }
        }

        for(int z = 0; z <= radius; z++) {

            const int xPerp = z * normX;
            const int yPerp = z * normY;
            p_pos1 = sf::Vector2i(pos1.x + i + yPerp, curr_y - xPerp);
            p_pos2 = sf::Vector2i(pos1.x + i - yPerp, curr_y + xPerp);
            canvas.setPixel(p_pos1.x, p_pos1.y, sf::Color::White, true);
            canvas.setPixel(p_pos2.x, p_pos2.y, sf::Color::White, true);
        }
        prev_y = curr_y;
    }
    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {

        for(int z = 0; z <= radius; z++) {

            const int xPerp = z * normX;
            const int yPerp = z * normY;
            p_pos1 = sf::Vector2i(pos2.x + yPerp, j - xPerp);
            p_pos2 = sf::Vector2i(pos2.x - yPerp, j + xPerp);
            canvas.setPixel(p_pos1.x, p_pos1.y, sf::Color::White, true);
            canvas.setPixel(p_pos2.x, p_pos2.y, sf::Color::White, true);
        }
    }

    for(int z = 0; z <= radius; z++) {

        const int xPerp = z * normX;
        const int yPerp = z * normY;
        p_pos1 = sf::Vector2i(pos2.x + yPerp, pos2.y - xPerp);
        p_pos2 = sf::Vector2i(pos2.x - yPerp, pos2.y + xPerp);
        canvas.setPixel(p_pos1.x, p_pos1.y, sf::Color::White, true);
        canvas.setPixel(p_pos2.x, p_pos2.y, sf::Color::White, true);
    }

    circle(canvas, pos1, true);
    circle(canvas, pos2, true);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Painter");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable some options

    ImGui::SFML::Init(window);

    Canvas canvas("res/random.jpg");
//
//    rectangle_line(canvas, {50, 200}, {100, 200});
//    canvas.apply();

    brush_color = ImColor(1, 1, 1);
    sf::Vector2i prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;

    sf::Vector2i prev_move_pos(0, 0);
    sf::Clock deltaClock;
    sf::Vector2i size = canvas.getSize();
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
                    prev_move_pos = tex_pos;
                    left_pressed = true;
                    circle(canvas, tex_pos);
                    canvas.apply();
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
//                    circle(canvas, sf::Mouse::getPosition(window));
                    line(canvas, canvas.transform_pos(prev_move_pos), canvas.transform_pos(tex_pos));
//                    rectangle_line(canvas, prev_move_pos, tex_pos);
canvas.apply();
                    prev_move_pos = tex_pos;
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
        ImGui::SliderInt("Radius of line", &radius, 0, 100);
        ImGui::ColorPicker3("Brush color", &brush_color.Value.x);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}
