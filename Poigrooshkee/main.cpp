#include <SFML/Graphics.hpp>

int main(const int argc, const char* argv[])
{
    sf::RenderWindow rw (sf::VideoMode(1000, 1000), argv[1]);
    rw.clear();
    
    sf::RectangleShape rs;
    rs.setSize(sf::Vector2f(100, 100));
    rs.setPosition(sf::Vector2f(100, 100));
    rs.setFillColor(sf::Color(255, 255, 255));
    
    

    while (rw.isOpen()) {
        sf::Event event;
        while (rw.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                rw.close();
        }
        rw.draw(rs);
        rw.display();

    }
    return 0;
}