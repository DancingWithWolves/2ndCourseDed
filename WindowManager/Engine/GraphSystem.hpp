#ifndef GRAPHSYSTEM
#define GRAPHSYSTEM

struct Color {
    int r, g, b;
    Color(int r, int g, int b) : r(r), g(g), b(b) {}
};

#include "EventSystem.hpp"
#include <SFML/Graphics.hpp>
#include "../LayoutConstants.hpp"


//============================================================================\

void DrawRectangle(int x, int y, int width, int height, Color c);

//============================================================================/

#endif