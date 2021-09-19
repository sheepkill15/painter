//
// Created by Aron on 9/19/2021.
//

#include <iostream>
#include "canvas.h"

Canvas::Canvas(const char* file, const sf::Vector2i &size) {
    _layers.emplace_back(std::make_tuple(sf::Sprite(), sf::Texture(), sf::Image()));
    _selected = &_layers.front();
    auto& tex = std::get<1>(*_selected);
    auto& img = std::get<2>(*_selected);
    if(file != nullptr) {
        img.loadFromFile(file);
        if(size.x != 0 && size.y != 0) {
            img = resize(img, size);
        }
    } else {
        img.create(0, 0);
        img = resize(img, size);
    }
    tex.create(img.getSize().x, img.getSize().y);
    tex.loadFromImage(img);
    auto& sprite = std::get<0>(*_selected);
    sprite.setTexture(tex);
    _size = (sf::Vector2i)tex.getSize();
}

Canvas::~Canvas() {

}

void Canvas::setPixel(const sf::Vector2i &pos, const sf::Color &col) {
    setPixel(pos.x, pos.y, col);
}

void Canvas::apply() {
    auto& tex = std::get<1>(*_selected);
    auto& img = std::get<2>(*_selected);
    tex.loadFromImage(img);
}

void Canvas::draw(sf::RenderWindow &window) {
    window.draw(std::get<0>(*_selected));
}

void Canvas::setPixel(int x, int y, const sf::Color &col) {
    auto& img = std::get<2>(*_selected);
    auto new_pos = transform_pos({x, y}, _offset, _scale);
    new_pos.x = std::min(new_pos.x, _size.x - 1);
    new_pos.x = std::max(0, new_pos.x);
    new_pos.y = std::min(new_pos.y, _size.y - 1);
    new_pos.y = std::max(0, new_pos.y);
    img.setPixel(new_pos.x, new_pos.y, col);
}

sf::Image Canvas::resize(const sf::Image &original, const sf::Vector2i &new_size) {
    sf::Image new_image;
    new_image.create(new_size.x, new_size.y);
    int old_x = original.getSize().x;
    int old_y = original.getSize().y;
    for(int y = 0; y < new_size.y; y++) {
        for(int x = 0; x < new_size.x; x++) {
            new_image.setPixel(x, y, x >= old_x || y >= old_y ? sf::Color::White : original.getPixel(x, y));
        }
    }
    return new_image;
}

void Canvas::resize(sf::Vector2i new_size) {
    auto& img = std::get<2>(*_selected);
    img = resize(img, new_size);
    auto& tex = std::get<1>(*_selected);
    tex.create(new_size.x, new_size.y);
    tex.loadFromImage(img);
    std::get<0>(*_selected).setTexture(tex, true);
    _size = new_size;
}

void Canvas::move(const sf::Vector2i &amount) {
    for(auto& layer : _layers) {
        auto& sprite = std::get<0>(layer);
        sprite.move((sf::Vector2f) amount);
    }
    _offset += amount;
}

void Canvas::scale(float amount) {
    for(auto& layer : _layers) {
        auto& sprite = std::get<0>(layer);
        sprite.scale(1 + amount / 10.f, 1 + amount / 10.f);
    }
    _scale *= 1 + amount / 10.f;
}

sf::Vector2i Canvas::transform_pos(const sf::Vector2i &original, const sf::Vector2i &offset, float scale) {
    return {static_cast<int>((float)(original.x - offset.x) / scale), static_cast<int>((float)(original.y - offset.y) / scale)};
}

sf::Vector2i Canvas::transform_pos(const sf::Vector2i &pos) {
    return transform_pos(pos, _offset, _scale);
}

void Canvas::setPixel(int x, int y, const sf::Color &col, bool transformed) {
    if(!transformed) {
        setPixel(x, y, col);
        return;
    }
    x = std::min(x, _size.x -1);
    x = std::max(0, x);
    y = std::min(y, _size.y -1);
    y = std::max(0, y);
    auto& img = std::get<2>(*_selected);
    img.setPixel(x, y, col);
}
