#include <GL/glut.h>
#include <assert.h>
#include "../Debug.hpp"

extern int window_height;
extern int window_width;


void DrawRectangle(int x, int y, int width, int height)
{
    soft_assert(x + width <= window_width);
    soft_assert(y + height <= window_height);
    
    glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x, y + height);
        glVertex2i(x + width, y + height);
        glVertex2i(x + width, y);
    glEnd();

}