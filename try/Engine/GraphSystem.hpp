#include <GL/gl.h>
#include "../LayoutConstants.hpp"

//============================================================================\

void DrawRectangle(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x, y + height);
        glVertex2i(x + width, y + height);
        glVertex2i(x + width, y);
    glEnd();
}

//============================================================================/