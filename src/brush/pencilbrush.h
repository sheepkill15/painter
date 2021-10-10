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

    ~PencilBrush() override;

    explicit PencilBrush(Canvas& canvas);
    void onMouseDown(const sf::Vector2f &pos) override;

    void onMouseUp(const sf::Vector2f& pos) override;

    void onMouseMoved(const sf::Vector2f &pos) override;

private:
    sf::Vector2f _prevPos{0, 0};
};


#endif //PAINTER_PENCILBRUSH_H
