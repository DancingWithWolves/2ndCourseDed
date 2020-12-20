#ifndef GRAPHSYSTEM
#define GRAPHSYSTEM

#include <SFML/Graphics.hpp>

struct Color {
    int r, g, b;
    Color(int r, int g, int b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
};

//============================================================================\
//                        Класс - обёртка над битмапой
//============================================================================/

class Texture {
    sf::Texture texture;

public:
    bool LoadFromFile(const char* fname)
    {
        return texture.loadFromFile(fname);
    }
    int GetWidth() { return texture.getSize().x; }
    int GetHeight() { return texture.getSize().y; }
    sf::Texture &GetSFTexture() { return texture; }
};

//============================================================================\
//                        Контейнер с текстурой
//============================================================================/

class Sprite {
protected:
    sf::Sprite sf_sprite;

    int x, y, width, height;
    Texture texture;
    int showed_texture_x, showed_texture_y, showed_texture_width, showed_texture_height;

public:
    Sprite() = delete;
    Sprite(const char* texture_fname, int x, int y, int width, int height);

    sf::Sprite GetSFSprite() {return sf_sprite; }
    Texture GetTexture() { return texture; }

    int GetTextureWidth() { return texture.GetWidth(); }
    int GetTextureHeight() { return texture.GetHeight(); }

    int GetShowedTextureX() { return showed_texture_x; }
    int GetShowedTextureY() { return showed_texture_y; }
    int GetShowedTextureWidth() { return showed_texture_width; }
    int GetShowedTextureHeight() { return showed_texture_height; }

    void SetShowedTextureX(int x) { showed_texture_x = x; }
    void SetShowedTextureY(int y) { showed_texture_y = y; }
    void SetShowedTextureWidth(int width) { showed_texture_width = width; }
    void SetShowedTextureHeight(int height) { showed_texture_height = height; }

    int GetX() { return x; };
    int GetY() { return y; }
    void SetX(int x) { this->x = x; };
    void SetY(int y) { this->y = y; };

    int GetWidth() { return width; };
    int GetHeight() { return height; }
    void SetWidth(int x) { this->width = x; };
    void SetHeight(int y) { this->height = y; };
};


#include "../LayoutConstants.hpp"
#include "EventSystem.hpp"

//============================================================================\
//                          Функции рисования
//============================================================================/

void DrawRectangle(int x, int y, int width, int height, Color c);
void DrawSprite(Sprite& sprite);

#endif