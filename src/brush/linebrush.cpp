//
// Created by Aron on 10/9/2021.
//

#include <shapes/lineshape.h>
#include <canvas/canvas.h>
#include "linebrush.h"

void LineBrush::onMouseDown(const sf::Vector2f &pos) {
    m_FirstPos = pos;
}

void LineBrush::onMouseUp(const sf::Vector2f &pos) {
    _canvas->apply_preview();
}

void LineBrush::onMouseMoved(const sf::Vector2f &pos) {
    auto line = sf::LineShape(_canvas->transform_pos(m_FirstPos), _canvas->transform_pos(pos));
    line.setThickness(settings.size * 2);
    line.setFillColor(settings.color);
    _canvas->preview(line, m_FirstPos, true, &renderShader);
    m_SecondPos = pos;
}

LineBrush::LineBrush(Canvas &canvas) : Brush(canvas) {}
