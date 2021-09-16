#include <iostream>
//#include <cmath>
#include <SFML/Graphics.hpp>

const int radius = 5;

void circle(sf::Image& image, const sf::Vector2i& pos) {
    int width = (int)image.getSize().x;
    int height = (int)image.getSize().y;
    sf::Vector2i cursor = pos;

    for(int i = std::max(0, cursor.y - radius); i <= std::min(height -1, cursor.y + radius); i++) {
        int i_ = i - cursor.y;
        for(int j = std::max(0, cursor.x - radius); j <= std::min(width -1, cursor.x + radius); j++) {
            int j_ = j - cursor.x;
            if(i_*i_ + j_*j_ < radius*radius) {
                image.setPixel(j, i, sf::Color::White);
            }
        }
    }
}

int calculate_y_from_x(int x, int a, int b, int c) {
    return (x * b + c) / a;
}

int sign(int x) {
    return (x>>31)|(!!x);
}

void line(sf::Image& image, const sf::Vector2i& pos1, const sf::Vector2i& pos2) {
    // calculate equation of the line
    const int a = pos2.x - pos1.x;
    const int b = pos2.y - pos1.y;
    const int c = -pos1.x * pos2.y + pos2.x * pos1.y;

    int prev_y = pos1.y;
    const int sgn_x = sign(a);
    const int sgn_y = sign(b);

//    const float angle = std::atan2((float)b, (float)a);
//    std::cout << angle << std::endl;
//    const bool dir = (std::abs(angle) <= M_PI_4) || (std::abs(angle) >= M_PI - M_PI_4);
//    bool dir = true;
    for(int i = 0; std::abs(a - i) > 0; i+=sgn_x)
    {
        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
//            if(dir) { // y
//                for(int y = j - radius; y <= j + radius; y++) {
//                    image.setPixel(pos1.x + i, y, sf::Color::White);
//                }
//            } else {
//                for(int x = i - radius; x <= i + radius; x++) {
//                    image.setPixel(pos1.x + x, j, sf::Color::White);
//                }
//            }
                circle(image, {pos1.x + i, j});
//            image.setPixel(pos1.x + i, j, sf::Color::White);
        }
//        if(dir) { // y
//            for(int y = curr_y - radius; y <= curr_y + radius; y++) {
//                image.setPixel(pos1.x + i, y, sf::Color::White);
//            }
//        } else {
//            for(int x = i - radius; x <= i + radius; x++) {
//                image.setPixel(pos1.x + x, curr_y, sf::Color::White);
//            }
//        }
        circle(image, {pos1.x + i, curr_y});
//        image.setPixel(pos1.x + i, curr_y, sf::Color::White);
        prev_y = curr_y;
    }
    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {
//        if(dir) { // y
//            for(int y = j - radius; y <= j + radius; y++) {
//                image.setPixel(pos2.x, y, sf::Color::White);
//            }
//        } else {
//            for(int x = pos2.x - radius; x <= pos2.x + radius; x++) {
//                image.setPixel(x, j, sf::Color::White);
//            }
//        }
        circle(image, {pos2.x, j});
//        image.setPixel(pos2.x, j, sf::Color::White);
    }
//    if(dir) { // y
//        for(int y = pos2.y - radius; y <= pos2.y + radius; y++) {
//            image.setPixel(pos2.x, y, sf::Color::White);
//        }
//    } else {
//        for(int x = pos2.x - radius; x <= pos2.x + radius; x++) {
//            image.setPixel(x, pos2.y, sf::Color::White);
//        }
//    }
        circle(image, pos2);
//    image.setPixel(pos2.x, pos2.y, sf::Color::White);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Painter");
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    sf::Texture texture;
    if(!texture.loadFromFile("res/random.jpg")) {
        std::cout << "Something went wrong" << std::endl;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2i prev_pos(0, 0);
    bool left_pressed = false;
    bool pressed = false;
    auto image = texture.copyToImage();
    line(image, sf::Vector2i(10, 100), sf::Vector2i(50, 100));
    texture.loadFromImage(image);
    sf::Vector2i prev_move_pos(0, 0);
    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed) {
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
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
