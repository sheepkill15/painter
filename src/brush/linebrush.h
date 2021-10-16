//
// Created by Aron on 10/9/2021.
//

#ifndef PAINTER_LINEBRUSH_H
#define PAINTER_LINEBRUSH_H


#include <SFML/System/Vector2.hpp>
#include "brush.h"

class LineBrush : public Brush {
public:

    void onMouseDown(const sf::Vector2f &pos) override;

    void onMouseUp(const sf::Vector2f &pos) override;

    void onMouseMoved(const sf::Vector2f &pos) override;
private:
    sf::Vector2f m_FirstPos;
    sf::Vector2f m_SecondPos;
};


#endif //PAINTER_LINEBRUSH_H
