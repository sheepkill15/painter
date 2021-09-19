//
// Created by Aron on 9/19/2021.
//

#ifndef PAINTER_CANVAS_H
#define PAINTER_CANVAS_H

#include <list>
#include <utility>
#include <SFML/Graphics.hpp>
using Layer = std::tuple<sf::Sprite, sf::Texture, sf::Image>;
class Canvas {
public:
    explicit Canvas(const char* file, const sf::Vector2i& size = sf::Vector2i(0, 0));
    ~Canvas();

    void setPixel(const sf::Vector2i& pos, const sf::Color& col);
    void setPixel(int x, int y, const sf::Color& col);
    void setPixel(int x, int y, const sf::Color& col, bool transformed);
    void apply();
    void draw(sf::RenderWindow& window);

    void resize(sf::Vector2i new_size);
    static sf::Image resize(const sf::Image& original, const sf::Vector2i &new_size);

    [[nodiscard]] inline const sf::Vector2i& getSize() const { return _size; }

    void move(const sf::Vector2i& amount);
    void scale(float amount);
    sf::Vector2i  transform_pos(const sf::Vector2i& pos);
    static sf::Vector2i transform_pos(const sf::Vector2i& original, const sf::Vector2i& offset, float scale);

private:
    std::list<Layer> _layers;
    sf::Vector2i _offset = sf::Vector2i(0, 0);
    sf::Vector2i _size = sf::Vector2i(0, 0);
    float _scale = 1;
    Layer* _selected;
};


#endif //PAINTER_CANVAS_H
