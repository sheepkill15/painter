#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int radius = 5;
ImColor brush_color;

void circle(sf::Image& image, const sf::Vector2i& pos) {
    int width = (int)image.getSize().x;
    int height = (int)image.getSize().y;
    sf::Vector2i cursor = pos;

    for(int i = std::max(0, cursor.y - radius); i <= std::min(height -1, cursor.y + radius); i++) {
        int i_ = i - cursor.y;
        for(int j = std::max(0, cursor.x - radius); j <= std::min(width -1, cursor.x + radius); j++) {
            int j_ = j - cursor.x;
            if(i_*i_ + j_*j_ < radius*radius) {
                image.setPixel(j, i, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255));
            }
        }
    }
}

int calculate_y_from_x(int x, int a, int b, int c) {
    return (-x * a - c) / b;
}

int sign(int x) {
    return (x>>31)|(!!x);
}

void draw_line(sf::Image& image, const sf::Vector2i& pos1, const sf::Vector2i& pos2, const int a, const int b, const int c) {
    int prev_y = pos1.y;
    const int sgn_x = sign(pos2.x - pos1.x);
    const int sgn_y = sign(pos2.y - pos1.y);

    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
    {
        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
            image.setPixel(pos1.x + i, j, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255));
        }
        image.setPixel(pos1.x + i, curr_y, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255));
        prev_y = curr_y;
    }
    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {
        image.setPixel(pos2.x, j, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255));
    }
    image.setPixel(pos2.x, pos2.y, sf::Color(brush_color.Value.x * 255, brush_color.Value.y * 255, brush_color.Value.z * 255));
}

void line(sf::Image& image, const sf::Vector2i& pos1, const sf::Vector2i& pos2) {
    // calculate equation of the line
    const int a = pos1.y - pos2.y;
    const int b = pos2.x - pos1.x;
    const int c = pos1.x * pos2.y - pos2.x * pos1.y;

    const int dx = pos1.x - pos2.x;
    const int dy = pos1.y - pos2.y;
    const double dist = std::sqrt(dx * dx + dy * dy);

    const float normX = dx / dist;
    const float normY = dy / dist;

    const int xPerp = radius * normX;
    const int yPerp = radius * normY;

    sf::Vector2i p_pos1;
    sf::Vector2i p_pos2;

    int prev_y = pos1.y;
    const int sgn_x = sign(pos2.x - pos1.x);
    const int sgn_y = sign(pos2.y - pos1.y);

    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
    {
        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {

            p_pos1 = sf::Vector2i(pos1.x + i + yPerp, j - xPerp);
            p_pos2 = sf::Vector2i(pos1.x + i - yPerp, j + xPerp);
            const int par_a = p_pos1.y - p_pos2.y;
            const int par_b = p_pos2.x - p_pos1.x;
            const int par_c = p_pos1.x * p_pos2.y - p_pos2.x * p_pos1.y;
            draw_line(image, p_pos1, p_pos2, par_a, par_b, par_c);
        }

        p_pos1 = sf::Vector2i(pos1.x + i + yPerp, curr_y - xPerp);
        p_pos2 = sf::Vector2i(pos1.x + i - yPerp, curr_y + xPerp);
        const int par_a = p_pos1.y - p_pos2.y;
        const int par_b = p_pos2.x - p_pos1.x;
        const int par_c = p_pos1.x * p_pos2.y - p_pos2.x * p_pos1.y;
        draw_line(image, p_pos1, p_pos2, par_a, par_b, par_c);
        prev_y = curr_y;
    }
    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {

        p_pos1 = sf::Vector2i(pos2.x + yPerp, j - xPerp);
        p_pos2 = sf::Vector2i(pos2.x - yPerp, j + xPerp);
        const int par_a = p_pos1.y - p_pos2.y;
        const int par_b = p_pos2.x - p_pos1.x;
        const int par_c = p_pos1.x * p_pos2.y - p_pos2.x * p_pos1.y;
        draw_line(image, p_pos1, p_pos2, par_a, par_b, par_c);
    }

    p_pos1 = sf::Vector2i(pos2.x + yPerp, pos2.y - xPerp);
    p_pos2 = sf::Vector2i(pos2.x - yPerp, pos2.y + xPerp);
    const int par_a = p_pos1.y - p_pos2.y;
    const int par_b = p_pos2.x - p_pos1.x;
    const int par_c = p_pos1.x * p_pos2.y - p_pos2.x * p_pos1.y;
    draw_line(image, p_pos1, p_pos2, par_a, par_b, par_c);

    circle(image, pos1);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Painter");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable some options

    ImGui::SFML::Init(window);

    sf::Texture texture;
    if(!texture.loadFromFile("res/random.jpg")) {
        std::cout << "Something went wrong" << std::endl;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    brush_color = ImColor(1, 1, 1);
    sf::Vector2i prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;
    auto image = texture.copyToImage();
    texture.loadFromImage(image);
    sf::Vector2i prev_move_pos(0, 0);
    sf::Clock deltaClock;

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
//                    circle(texture, sprite, sf::Mouse::getPosition(window) - (sf::Vector2i)sprite.getPosition());
sf::Vector2i tex_pos = sf::Mouse::getPosition(window) - (sf::Vector2i)sprite.getPosition();
tex_pos.x = (int)((float)tex_pos.x / sprite.getScale().x);
tex_pos.y = (int)((float)tex_pos.y / sprite.getScale().y);
                    prev_move_pos = tex_pos;
                    left_pressed = true;
                    circle(image, tex_pos);
                    texture.loadFromImage(image);
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
                    sprite.move((float)(tex_pos.x - prev_pos.x), (float)(tex_pos.y - prev_pos.y));
                    prev_pos = tex_pos;
                } else if(left_pressed) {
                    sf::Vector2i tex_pos = sf::Mouse::getPosition(window) - (sf::Vector2i)sprite.getPosition();
                    tex_pos.x = (int)((float)tex_pos.x / sprite.getScale().x);
                    tex_pos.y = (int)((float)tex_pos.y / sprite.getScale().y);
//                    circle(texture, sprite, sf::Mouse::getPosition(window) - (sf::Vector2i)sprite.getPosition());
                    line(image, prev_move_pos, tex_pos);
                    texture.loadFromImage(image);
                    prev_move_pos = tex_pos;
                }
            }
            else if(event.type == sf::Event::MouseWheelScrolled) {
                sprite.scale(event.mouseWheelScroll.delta / 10.f + 1, event.mouseWheelScroll.delta / 10.f + 1);
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
        window.draw(sprite);
        ImGui::Begin("Lol");
        ImGui::Text("Hello");
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
