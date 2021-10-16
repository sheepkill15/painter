//
// Created by Aron on 9/19/2021.
//

#include <iostream>
#include <utils/utils.h>
#include <imgui.h>
#include <sstream>
#include <glad/glad.h>
#include "canvas.h"

Canvas::Canvas(const char* file, const sf::Vector2u &size) : m_PreviewLayer() {
    _selected = &_layers.emplace_back();
    auto& renderTexture = _selected->second;

    if(file != nullptr) {
        sf::Image newImage;
        newImage.loadFromFile(file);
        sf::Texture texture;
        texture.loadFromImage(newImage);
        sf::Sprite sprite(texture);
        renderTexture.create(std::max(size.x, texture.getSize().x), std::max(size.y, texture.getSize().y));
        renderTexture.clear(sf::Color::White);
        renderTexture.draw(sprite);
    } else {
        renderTexture.create(size.x, size.y);
        renderTexture.clear(sf::Color::White);
    }
    _size = renderTexture.getSize();

    auto& sprite = _selected->first;
    sprite.setTexture(renderTexture.getTexture());

    m_PreviewLayer.second.create(_size.x, _size.y);
    m_PreviewLayer.second.clear(sf::Color::Transparent);
    m_PreviewLayer.first.setTexture(m_PreviewLayer.second.getTexture(), true);
}

void Canvas::draw(sf::RenderWindow &window) {
    for(auto & [sprite, texture] : _layers) {
        texture.display();
        window.draw(sprite);
        if(&sprite == &_selected->first) {
            m_PreviewLayer.second.display();
            window.draw(m_PreviewLayer.first);
        }
    }
}

void Canvas::resize(const sf::Vector2u& new_size) {
    for(auto& [sprite, texture] : _layers) {
        auto img = texture.getTexture().copyToImage();
        texture.create(new_size.x, new_size.y);
        texture.clear(sf::Color::Transparent);
        auto newTexture = sf::Texture();
        newTexture.loadFromImage(img);
        auto newSprite = sf::Sprite(newTexture);
        texture.draw(newSprite);
        sprite.setTexture(texture.getTexture(), true);
    }
    m_PreviewLayer.second.create(new_size.x, new_size.y);
    m_PreviewLayer.second.clear(sf::Color::Transparent);
    m_PreviewLayer.first.setTexture(m_PreviewLayer.second.getTexture(), true);
    _size = new_size;
}

void Canvas::move(const sf::Vector2f &amount) {
    for(auto& [sprite, _] : _layers) {
        sprite.move(amount);
    }
    m_PreviewLayer.first.move(amount);
    _offset += amount;
}

void Canvas::scale(float amount) {
    for(auto& [sprite, _] : _layers) {
        sprite.scale(1 + amount / 10.f, 1 + amount / 10.f);
    }
    m_PreviewLayer.first.scale(1 + amount / 10.f, 1 + amount / 10.f);
    _scale *= 1 + amount / 10.f;
}

sf::Vector2f Canvas::transform_pos(const sf::Vector2f &original, const sf::Vector2f &offset, float scale) {
    return {(original.x - offset.x) / scale, (original.y - offset.y) / scale};
}

sf::Vector2f Canvas::transform_pos(const sf::Vector2f &pos) {
    return transform_pos(pos, _offset, _scale);
}

void Canvas::DrawUI() {
    ImGui::Begin("Canvas Settings");
    ImGui::InputInt2("Size of canvas", reinterpret_cast<int *>(&_size.x));
    if(ImGui::Button("Commit size (resize)")) {
        resize(_size);
    }
    int i = 0;
    const std::string label = "Layer";
    for(auto& layer : _layers) {
        std::stringstream ss;
        ss << label << i++;
        if(ImGui::Button(ss.str().c_str())) {
            _selected = &layer;
        }
    }
    if(ImGui::Button("Add Layer")) {
        add_layer();
    }
    ImGui::End();
}

void Canvas::add_layer() {
    _selected = &_layers.emplace_back();
    auto& renderTexture = _selected->second;

    renderTexture.create(_size.x, _size.y);
    renderTexture.clear(sf::Color::Transparent);

    auto& sprite = _selected->first;
    sprite.setTexture(renderTexture.getTexture(), true);
    sprite.setPosition(_offset);
    sprite.setScale(sf::Vector2f(_scale, _scale));
}

void Canvas::draw(sf::Shape &drawable, const sf::Vector2f &pos, sf::Shader* shader, Layer* layer) {
    if(layer == nullptr) {
        layer = _selected;
    }
    auto& renderTexture = layer->second;
    drawable.setPosition(transform_pos(pos));
    if(shader != nullptr) {
        sf::Shader::bind(shader);
    }
    renderTexture.draw(drawable);
    sf::Shader::bind(nullptr);
}

void Canvas::preview(sf::Shape &drawable, const sf::Vector2f &pos, bool clear, sf::Shader *shader) {
    if(clear) {
        m_PreviewLayer.second.clear(sf::Color::Transparent);
    }
    m_PreviewLayer.second.pushGLStates();
    m_PreviewLayer.second.setActive(true);
    glBlendEquationSeparate(GL_MAX, GL_MAX);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    draw(drawable, pos, shader, &m_PreviewLayer);
    m_PreviewLayer.second.setActive(false);
    m_PreviewLayer.second.popGLStates();
}

void Canvas::apply_preview() {
    auto& renderTexture = _selected->second;
    auto oldPos = m_PreviewLayer.first.getPosition();
    auto oldScale = m_PreviewLayer.first.getScale();
    m_PreviewLayer.first.setPosition(transform_pos(oldPos));
    m_PreviewLayer.first.setScale(1, 1);
    renderTexture.draw(m_PreviewLayer.first);
    m_PreviewLayer.second.clear(sf::Color::Transparent);
    m_PreviewLayer.first.setPosition(oldPos);
    m_PreviewLayer.first.setScale(oldScale);
}

sf::Vector2f Canvas::flip_vertical(const sf::Vector2f &pos) const {
    return {pos.x, static_cast<float>(_size.y) - pos.y};
}
