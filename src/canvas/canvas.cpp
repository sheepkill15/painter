//
// Created by Aron on 9/19/2021.
//

#include <iostream>
#include <utils/utils.h>
#include "canvas.h"

Canvas::Canvas(const char* file, const sf::Vector2i &size) {
    _layers.emplace_back();
    _selected = &_layers.front();
    auto& renderTexture = _selected->second;
    if(file != nullptr) {
        sf::Image newImage;
        newImage.loadFromFile(file);
        sf::Texture texture;
        texture.loadFromImage(newImage);
        sf::Sprite sprite(texture);
        renderTexture.create(std::max((unsigned int)size.x, texture.getSize().x), std::max((unsigned int)size.y, texture.getSize().y));
        renderTexture.clear(sf::Color::White);
        renderTexture.draw(sprite);
    } else {
        renderTexture.create(size.x, size.y);
        renderTexture.clear(sf::Color::White);
    }
    _size = (sf::Vector2i)renderTexture.getSize();

    auto& sprite = _selected->first;
    sprite.setTexture(renderTexture.getTexture());
}

Canvas::~Canvas() {

}

void Canvas::draw(sf::RenderWindow &window) {
    _selected->second.display();
    window.draw(std::get<0>(*_selected));
}

void Canvas::resize(sf::Vector2i new_size) {

    auto img = _selected->second.getTexture().copyToImage();
    img = Utils::resize(img, (sf::Vector2u)new_size);
    _selected->second.create(new_size.x, new_size.y);
    auto newTexture = sf::Texture();
    newTexture.loadFromImage(img);
    auto newSprite = sf::Sprite(newTexture);
    _selected->second.draw(newSprite);
    _selected->first.setTexture(_selected->second.getTexture(), true);
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

void Canvas::draw(sf::Shape &drawable, const sf::Vector2i &pos) {

    auto& renderTexture = _selected->second;
    drawable.setPosition((sf::Vector2f)transform_pos(pos));
    renderTexture.draw(drawable);
}
