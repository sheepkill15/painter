//
// Created by Aron on 9/19/2021.
//

#ifndef PAINTER_CANVAS_H
#define PAINTER_CANVAS_H

#include <list>
#include <utility>
#include <SFML/Graphics.hpp>
using Layer = std::pair<sf::Sprite, sf::RenderTexture>;
class Canvas {
public:
    explicit Canvas(const char* file, const sf::Vector2u& size = sf::Vector2u(0, 0));
    ~Canvas();

    void draw(sf::RenderWindow& window);

    void resize(const sf::Vector2u& new_size);

    [[nodiscard("Use the size if requested")]] inline const sf::Vector2u& getSize() const { return _size; }

    void move(const sf::Vector2f& amount);
    void scale(float amount);
    sf::Vector2f  transform_pos(const sf::Vector2f& pos);
    static sf::Vector2f transform_pos(const sf::Vector2f& original, const sf::Vector2f& offset, float scale);

    void draw(sf::Shape& drawable, const sf::Vector2f& pos);
    void draw(sf::Shape& drawable, const sf::Vector2f& pos, sf::Shader& shader);

    void preview(sf::Shape& drawable, const sf::Vector2f& pos, bool clear = true);
    void preview(sf::Shape& drawable, const sf::Vector2f& pos, sf::Shader& shader, bool clear = true);

    void apply_preview();

    void add_layer();

    void DrawUI();

    sf::Vector2f flip_vertical(const sf::Vector2f& pos) const;

private:
    std::list<Layer> _layers;
    Layer m_PreviewLayer;
    sf::Vector2f _offset{0, 0};
    sf::Vector2u _size{0, 0};
    float _scale{1};
    Layer* _selected;
};


#endif //PAINTER_CANVAS_H
