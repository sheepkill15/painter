//
// Created by Aron on 9/27/2021.
//

#ifndef PAINTER_BRUSH_H
#define PAINTER_BRUSH_H

namespace sf {
    class Event;
    class Image;
}

struct BrushSettings {
    int size = 5;
    int feathering = 100;
};

class Brush {
public:
    Brush() = default;
    ~Brush() = default;
    virtual void processEvents(const sf::Event& event) = 0;
    virtual void draw(const BrushSettings& settings, sf::Image& image) = 0;
private:
};


#endif //PAINTER_BRUSH_H
