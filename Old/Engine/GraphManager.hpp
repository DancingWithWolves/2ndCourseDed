#include "../Debug.hpp"
#include <GL/glut.h>
#include <SFML/Graphics.hpp>

extern int window_height;
extern int window_width;

void DrawRectangle(int x, int y, int width, int height)
{
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x, y + height);
    glVertex2i(x + width, y + height);
    glVertex2i(x + width, y);
    glEnd();
}

class Texture {
    sf::Texture texture;

public:
    Texture(const char* fname)
    {
    }
};

void LoadTexture()
{
}