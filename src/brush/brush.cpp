//
// Created by Aron on 10/3/2021.
//


#include <cmath>
#include <canvas/canvas.h>
#include "brush.h"

Brush::Brush(Canvas &canvas) {
    setCanvas(canvas);
}
//
//int calculate_y_from_x(int x, int a, int b, int c) {
//    return (-x * a - c) / b;
//}
//
//void Brush::circle(const sf::Vector2i& pos) {
//    auto radius = settings.size;
//    for(int i = pos.y - radius; i <= pos.y + radius; i++) {
//        int i_ = i - pos.y;
//        for(int j = pos.x - radius; j <= pos.x + radius; j++) {
//            int j_ = j - pos.x;
//            if(i_*i_ + j_*j_ < radius*radius) {
//                setPixel(sf::Vector2i(j, i), settings.color);
//            }
//        }
//    }
//}
//
//
//int sign(int x) {
//    return (x>>31)|(!!x);
//}
//
//void Brush::draw_line(const sf::Vector2i& pos1, const sf::Vector2i& pos2, const int a, const int b, const int c) {
//    int prev_y = pos1.y;
//    const int sgn_x = sign(pos2.x - pos1.x);
//    const int sgn_y = sign(pos2.y - pos1.y);
//
//    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
//    {
//        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
//        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
//            setPixel(sf::Vector2i(pos1.x + i, j), settings.color);
//        }
//        setPixel(sf::Vector2i(pos1.x + i, curr_y), settings.color);
//        prev_y = curr_y;
//    }
//    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {
//        setPixel(sf::Vector2i(pos2.x, j), settings.color);
//    }
//    setPixel(sf::Vector2i(pos2.x, pos2.y), settings.color);
//}
//
//void Brush::line(const sf::Vector2i& pos1, const sf::Vector2i& pos2) {
//    // calculate equation of the line
//    const int a = pos1.y - pos2.y;
//    const int b = pos2.x - pos1.x;
//    const int c = pos1.x * pos2.y - pos2.x * pos1.y;
//
//    const int dx = pos1.x - pos2.x;
//    const int dy = pos1.y - pos2.y;
//    const double dist = std::sqrt(dx * dx + dy * dy);
//    if(dist == 0) return;
//    const float normX = dx / dist;
//    const float normY = dy / dist;
//
//
//    sf::Vector2i p_pos1;
//    sf::Vector2i p_pos2;
//
//    int prev_y = pos1.y;
//    const int sgn_x = sign(pos2.x - pos1.x);
//    const int sgn_y = sign(pos2.y - pos1.y);
//
//    for(int i = 0; std::abs(pos2.x - pos1.x - i) > 0; i+=sgn_x)
//    {
//        int curr_y = calculate_y_from_x(pos1.x + i, a, b, c);
//        for(int j = prev_y; std::abs(curr_y - j) > 0; j += sgn_y) {
//            for(int z = 0; z <= settings.size; z++) {
//
//                const int xPerp = z * normX;
//                const int yPerp = z * normY;
//                p_pos1 = sf::Vector2i(pos1.x + i + yPerp, j - xPerp);
//                p_pos2 = sf::Vector2i(pos1.x + i - yPerp, j + xPerp);
//                setPixel(sf::Vector2i(p_pos1.x, p_pos1.y), settings.color);
//                setPixel(sf::Vector2i(p_pos2.x, p_pos2.y), settings.color);
//            }
//        }
//
//        for(int z = 0; z <= settings.size; z++) {
//
//            const int xPerp = z * normX;
//            const int yPerp = z * normY;
//            p_pos1 = sf::Vector2i(pos1.x + i + yPerp, curr_y - xPerp);
//            p_pos2 = sf::Vector2i(pos1.x + i - yPerp, curr_y + xPerp);
//            setPixel(sf::Vector2i(p_pos1.x, p_pos1.y), settings.color);
//            setPixel(sf::Vector2i(p_pos2.x, p_pos2.y), settings.color);
//        }
//        prev_y = curr_y;
//    }
//    for(int j = prev_y; std::abs(pos2.y - j) > 0; j += sgn_y) {
//
//        for(int z = 0; z <= settings.size; z++) {
//
//            const int xPerp = z * normX;
//            const int yPerp = z * normY;
//            p_pos1 = sf::Vector2i(pos2.x + yPerp, j - xPerp);
//            p_pos2 = sf::Vector2i(pos2.x - yPerp, j + xPerp);
//            setPixel(sf::Vector2i(p_pos1.x, p_pos1.y), settings.color);
//            setPixel(sf::Vector2i(p_pos2.x, p_pos2.y), settings.color);
//        }
//    }
//
//    for(int z = 0; z <= settings.size; z++) {
//
//        const int xPerp = z * normX;
//        const int yPerp = z * normY;
//        p_pos1 = sf::Vector2i(pos2.x + yPerp, pos2.y - xPerp);
//        p_pos2 = sf::Vector2i(pos2.x - yPerp, pos2.y + xPerp);
//        setPixel(sf::Vector2i(p_pos1.x, p_pos1.y), settings.color);
//        setPixel(sf::Vector2i(p_pos2.x, p_pos2.y), settings.color);
//    }
//
//    circle(pos1);
//    circle(pos2);
//}
//
//void Brush::setPixel(const sf::Vector2i &pos, const sf::Color &color) {
//    auto _pos = pos;
//    int xSize = (int)_internal.getSize().x;
//    int ySize = (int)_internal.getSize().y;
//    if(pos.x >= xSize || pos.y >= ySize) {
//        _internal = Utils::resize(_internal, sf::Vector2u(std::max(pos.x + 1, xSize), std::max(pos.y + 1, ySize)), sf::Color::Transparent);
//    }
//    _pos.x = std::max(pos.x, 0);
//    _pos.y = std::max(pos.y, 0);
//    _internal.setPixel(_pos.x, _pos.y, color);
//}

void Brush::setCanvas(Canvas &canvas) {
    _canvas = &canvas;
}