//
// Created by Aron on 10/3/2021.
//

#include "pencilbrush.h"
#include <canvas/canvas.h>
#include <shapes/lineshape.h>

void PencilBrush::onMouseDown(const sf::Vector2f &pos) {
    auto circle = sf::CircleShape(settings.size);
    circle.setFillColor(settings.color);
    circle.setOrigin(settings.size, settings.size);

    prepareForDraw(_canvas->flip_vertical(pos), sf::Vector2f(), 1);

    _canvas->preview(circle, pos, false, &renderShader);
    _prevPos = pos;
}

void PencilBrush::onMouseUp(const sf::Vector2f& pos) {
    _canvas->apply_preview();
}

void PencilBrush::onMouseMoved(const sf::Vector2f &pos) {
    auto circle = sf::CircleShape(settings.size);
    circle.setOrigin(settings.size, settings.size);
    circle.setFillColor(settings.color);
    const auto pos1 = (sf::Vector2f)_canvas->transform_pos(_prevPos);
    const auto pos2 = (sf::Vector2f)_canvas->transform_pos(pos);
    //    _canvas->draw(circle, pos);
    prepareForDraw(_canvas->flip_vertical(pos1), sf::Vector2f(), 1);
    _canvas->preview(circle, pos, false, &renderShader);
    auto line = sf::LineShape(pos1, pos2);
    line.setThickness(settings.size * 2);
    line.setFillColor(settings.color);
    prepareForDraw(_canvas->flip_vertical(pos1), _canvas->flip_vertical(pos2), 0);
    _canvas->preview(line, _prevPos, false, &renderShader);
    _prevPos = pos;
}

PencilBrush::PencilBrush(Canvas& canvas) : Brush(canvas) {

}