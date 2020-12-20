#include "GraphSystem.hpp"

//============================================================================\
//                        Интерфейс контейнера с текстурой
//============================================================================/

Sprite::Sprite(const char* texture_fname, int x, int y, int width, int height)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
{   
    if (!texture.LoadFromFile(texture_fname)) {
        printf("Error loading texture!\n");
    } else {
        printf("Texture loaded successfully!\n");
    }

    // по умолчанию текстура отображается с координат 0 0 внутри окна
    showed_texture_x = 0;
    showed_texture_y = 0;
    showed_texture_width = std::min(texture.GetWidth(), width);
    showed_texture_height = std::min(texture.GetHeight(), height);

    sf_sprite.setTexture(texture.GetSFTexture());
    sf_sprite.setPosition(x, y);

    sf_sprite.setTextureRect(
        sf::IntRect(showed_texture_x,
        showed_texture_y,
        showed_texture_width,
        showed_texture_height)
    );

    
}

//============================================================================\
//                        Рисующие функции
//============================================================================/

void DrawRectangle(int x, int y, int width, int height, Color color = Color(DEFAULT_COLOR))
{
    sf::RectangleShape rs;
    rs.setSize(sf::Vector2f(width, height));
    rs.setPosition(sf::Vector2f(x, y));
    rs.setFillColor(sf::Color(color.r, color.g, color.b));

    if (Engine::system_window_ptr) {
        Engine::system_window_ptr->draw(rs);
    }
}

void DrawSprite(Sprite& sprite)
{
    // printf("\n\nsprite: x = %d\ny = %d\nwidth=%d\nheight=%d\ntx=%d\nty=%d\ntw=%d\nth=%d\n\n", 
    // sprite.GetX(), sprite.GetY(), sprite.GetWidth(), sprite.GetHeight(),
    // sprite.GetShowedTextureX(), sprite.GetShowedTextureY(), sprite.GetShowedTextureWidth(), sprite.GetShowedTextureHeight() );
    if (Engine::system_window_ptr) {
        Engine::system_window_ptr->draw(sprite.GetSFSprite());
    }
}