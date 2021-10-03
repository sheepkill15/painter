//
// Created by Aron on 10/3/2021.
//

#include "pencilbrush.h"
#include <canvas/canvas.h>
#include <shapes/lineshape.h>

void PencilBrush::onMouseDown(const sf::Vector2i &pos) {
    auto circle = sf::CircleShape(settings.size);
    circle.setFillColor(settings.color);
    circle.setOrigin(settings.size, settings.size);
    _canvas->draw(circle, pos);
    _prevPos = pos;
}

void PencilBrush::onMouseUp(const sf::Vector2i& pos) {
    // pass
}

void PencilBrush::onMouseMoved(const sf::Vector2i &pos) {

    auto circle = sf::CircleShape(settings.size);
    circle.setOrigin(settings.size, settings.size);
    circle.setFillColor(settings.color);
    _canvas->draw(circle, pos);
    _canvas->draw(circle, _prevPos);
    auto line = sf::LineShape((sf::Vector2f)_canvas->transform_pos(_prevPos), (sf::Vector2f)_canvas->transform_pos(pos));
    line.setThickness(settings.size * 2);
    line.setFillColor(settings.color);
    _canvas->draw(line, _prevPos);
    _prevPos = pos;
}

PencilBrush::PencilBrush(Canvas& canvas) : Brush(canvas) {

}
