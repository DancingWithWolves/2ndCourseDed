#ifndef GRAPHSYSTEM
#define GRAPHSYSTEM

struct Color {
    float r, g, b;
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

#include "EventSystem.hpp"
#include <SFML/Graphics.hpp>
#include "../LayoutConstants.hpp"


//============================================================================\

void DrawRectangle(float x, float y, float width, float height, Color c);

//============================================================================/

#endif