//
// Created by Aron on 10/3/2021.
//

#include "utils.h"

sf::Image Utils::resize(const sf::Image &original, const sf::Vector2u &new_size, const sf::Color& color) {
    sf::Image new_image;
    new_image.create(new_size.x, new_size.y);
    auto old_x = original.getSize().x;
    auto old_y = original.getSize().y;
    for(int y = 0; y < new_size.y; y++) {
        for(int x = 0; x < new_size.x; x++) {
            new_image.setPixel(x, y, x >= old_x || y >= old_y ? color : original.getPixel(x, y));
        }
    }
    return new_image;
}
