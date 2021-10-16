//
// Created by Aron on 10/3/2021.
//


#include <cmath>
#include <imgui.h>
#include <iostream>
#include "canvas/canvas.h"
#include "brush.h"

Brush::Brush(Canvas &canvas) {
    setCanvas(canvas);
}

void Brush::setCanvas(Canvas &canvas) {
    _canvas = &canvas;
}

void BrushSettings::DrawUI() {
    ImGui::Begin("Brush settings");
    ImGui::SliderFloat("Radius of line", &size, 0, 100);
    ImGui::SliderInt("Feathering amount", &feathering, 0, 100);
    ImGui::SliderScalarN("Brush color", ImGuiDataType_U8, &color.r, 4, &sf::Color::Black.r, &sf::Color::White.r);
    ImGui::End();
}

sf::Shader Brush::renderShader{};

void Brush::initShader() {
    constexpr auto fragmentShader = R"(

    uniform float radius;
    uniform float feathering;
    uniform vec2 pt1;
    uniform vec2 pt2;
    uniform vec2 size;

    uniform int type;

    float DistToLine(vec2 pt1, vec2 pt2, vec2 testPt)
    {
      vec2 lineDir = pt2 - pt1;
      vec2 perpDir = vec2(lineDir.y, -lineDir.x);
      vec2 dirToPt1 = pt1 - testPt;
      return abs(dot(normalize(perpDir), dirToPt1));
    }

    float DistToCircle(vec2 pt1, vec2 point)
    {
        vec2 dist = point - pt1;
        return length(dist);
    }

    float getValueBasedOnDistance(float distance)
    {
        return 1 / pow(1 + feathering, distance);
    }

    void main()
    {
        float res;
        switch(type) {
            case 0:
                res = DistToLine(pt1, pt2, gl_FragCoord.xy) / (radius);
                break;
            case 1:
                res = DistToCircle(pt1, gl_FragCoord.xy) / (2*radius);
        }
        gl_FragColor = vec4(gl_Color.rgb, getValueBasedOnDistance(res) * gl_Color.a);
    }
)";
    renderShader.loadFromMemory(fragmentShader, sf::Shader::Fragment);
}

void Brush::prepareForDraw(const sf::Vector2f& pos1, const sf::Vector2f& pos2, int type) {
    renderShader.setUniform("pt1", pos1);
    renderShader.setUniform("pt2", pos2);
    renderShader.setUniform("radius", settings.size);
    renderShader.setUniform("feathering", static_cast<float>(settings.feathering) / 100.f);
    renderShader.setUniform("type", type);
}
