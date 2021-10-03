//
// Created by Aron on 10/3/2021.
//

#ifndef PAINTER_UTILS_H
#define PAINTER_UTILS_H


#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>

class Utils {
public:
    Utils() = delete;
    static sf::Image resize(const sf::Image& original, const sf::Vector2u &new_size, const sf::Color& color = sf::Color::White);
};


#endif //PAINTER_UTILS_H
