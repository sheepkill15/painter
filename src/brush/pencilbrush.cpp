//
// Created by Aron on 10/3/2021.
//

#include "pencilbrush.h"
#include <canvas/canvas.h>
#include <shapes/lineshape.h>

void PencilBrush::onMouseDown(const sf::Vector2i &pos) {
//    prepareForDraw();
    auto circle = sf::CircleShape(settings.size);
    circle.setFillColor(settings.color);
    circle.setOrigin(settings.size, settings.size);

    prepareForDraw(sf::Vector2f(pos.x, _canvas->getSize().y - pos.y), sf::Vector2f(), 1);

    _canvas->preview(circle, pos, renderShader, false);
    _prevPos = pos;
}

void PencilBrush::onMouseUp(const sf::Vector2i& pos) {
    _canvas->apply_preview();
}

void PencilBrush::onMouseMoved(const sf::Vector2i &pos) {
    auto circle = sf::CircleShape(settings.size);
    circle.setOrigin(settings.size, settings.size);
    circle.setFillColor(settings.color);
    auto pos1 = (sf::Vector2f)_canvas->transform_pos(_prevPos);
    auto pos2 = (sf::Vector2f)_canvas->transform_pos(pos);
    //    _canvas->draw(circle, pos);
    prepareForDraw(sf::Vector2f(pos1.x, _canvas->getSize().y - pos1.y), sf::Vector2f(), 1);
    _canvas->preview(circle, _prevPos, renderShader, false);
    auto line = sf::LineShape(pos1, pos2);
    line.setThickness(settings.size * 2);
    line.setFillColor(settings.color);
    prepareForDraw(sf::Vector2f(pos1.x, _canvas->getSize().y - pos1.y), sf::Vector2f(pos2.x, _canvas->getSize().y - pos2.y), 0);
    _canvas->preview(line, _prevPos, renderShader, false);
    _prevPos = pos;
}

PencilBrush::PencilBrush(Canvas& canvas) : Brush(canvas) {

}

PencilBrush::~PencilBrush() = default;
