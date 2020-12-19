#include "GraphSystem.hpp"

void DrawRectangle(int x, int y, int width, int height, Color color = Color(DEFAULT_COLOR))
{
    sf::RectangleShape rs;
    rs.setSize(sf::Vector2f(width, height));
    rs.setPosition(sf::Vector2f(x, y));
    rs.setFillColor(sf::Color(color.r, color.g, color.b));
    
    if(Engine::system_window_ptr) {
        Engine::system_window_ptr->draw(rs);
    }

}