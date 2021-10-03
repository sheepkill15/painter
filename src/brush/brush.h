//
// Created by Aron on 9/27/2021.
//

#ifndef PAINTER_BRUSH_H
#define PAINTER_BRUSH_H

#include <SFML/Graphics/Color.hpp>

class Canvas;

struct BrushSettings {
    int size = 5;
    int feathering = 100;
    sf::Color color = sf::Color::White;
};

class Brush {
public:
    Brush() = default;
    explicit Brush(Canvas& canvas);
    ~Brush() = default;
    virtual void onMouseDown(const sf::Vector2i& pos) = 0;
    virtual void onMouseUp(const sf::Vector2i& pos) = 0;
    virtual void onMouseMoved(const sf::Vector2i& pos) = 0;
    void setCanvas(Canvas& canvas);
    BrushSettings settings;
protected:
//    void circle(const sf::Vector2i& pos);
//    void draw_line(const sf::Vector2i& pos1, const sf::Vector2i& pos2, int a, int b, int c);
//    void line(const sf::Vector2i& pos1, const sf::Vector2i& pos2);
//
//    void setPixel(const sf::Vector2i& pos, const sf::Color& color);

    Canvas* _canvas{};
};


#endif //PAINTER_BRUSH_H
