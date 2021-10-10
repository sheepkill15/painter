//
// Created by Aron on 9/27/2021.
//

#ifndef PAINTER_BRUSH_H
#define PAINTER_BRUSH_H

#include <SFML/Graphics/Color.hpp>

class Canvas;

namespace sf {
    class Shader;
}

struct BrushSettings {
    float size = 5;
    int feathering = 0;
    sf::Color color = sf::Color::White;

    void DrawUI();
};

class Brush {
public:
    Brush() = default;
    explicit Brush(Canvas& canvas);
    virtual ~Brush() = default;
    virtual void onMouseDown(const sf::Vector2f& pos) = 0;
    virtual void onMouseUp(const sf::Vector2f& pos) = 0;
    virtual void onMouseMoved(const sf::Vector2f& pos) = 0;
    void setCanvas(Canvas& canvas);
    inline static BrushSettings settings{};

    static void prepareForDraw(const sf::Vector2f& pos1, const sf::Vector2f& pos2, int type = 0);

    static void initShader();

protected:
    Canvas* _canvas{};
    static sf::Shader renderShader;
};


#endif //PAINTER_BRUSH_H
