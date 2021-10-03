//
// Created by Aron on 10/3/2021.
//

#ifndef PAINTER_PENCILBRUSH_H
#define PAINTER_PENCILBRUSH_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include "brush.h"

class PencilBrush : public Brush {
public:
    PencilBrush() = default;
    explicit PencilBrush(Canvas& canvas);
    void onMouseDown(const sf::Vector2i &pos) override;

    void onMouseUp(const sf::Vector2i& pos) override;

    void onMouseMoved(const sf::Vector2i &pos) override;

private:
//
//    void updateOffset(const sf::Vector2i& pos);
//
//    sf::Vector2i _offset{0, 0};
    sf::Vector2i _prevPos{0, 0};
};


#endif //PAINTER_PENCILBRUSH_H
