#include <iostream>
#include <SFML/Graphics.hpp>

bool mouse_pressed = false;

int main(const int argc, const char*argv[])
{

    sf::RenderWindow app_window(sf::VideoMode(1000, 1000), argv[0]);
    app_window.setTitle(argv[0]);
    app_window.clear();

    sf::Texture button_texture, road, contain;

    if (!button_texture.loadFromFile("scrollButton.png") || !road.loadFromFile("scrollRoad.png") || !contain.loadFromFile("contain.png")) {
        printf("Some shit happened with this damn texture!\n");
    }

    



    sf::Sprite button, road_sprite, contain_sprite;

    contain_sprite.setTexture(contain);
    contain_sprite.setTextureRect(sf::IntRect(0, 0, contain.getSize().x, road.getSize().y));
    
    button.setTexture(button_texture);
    button.setPosition(contain_sprite.getPosition().x + contain_sprite.getTextureRect().width, 0);

    road_sprite.setTexture(road);
    road_sprite.setPosition(contain_sprite.getPosition().x + contain_sprite.getTextureRect().width, 0);




    float y_prop = 0; // 
    int height = road_sprite.getTextureRect().height - button.getTextureRect().height;
    int max = road_sprite.getPosition().y + road_sprite.getTextureRect().height - button.getTextureRect().height;
    int min = road_sprite.getPosition().y;

    




    sf::Event event = {};
    while (app_window.isOpen()) {
        app_window.clear();
        app_window.draw(road_sprite);
        app_window.draw(contain_sprite);
        while (app_window.pollEvent(event)) {

            switch (event.type) {

            case sf::Event::Closed:
                app_window.close();
                break;

            case sf::Event::MouseButtonPressed:
                mouse_pressed = true;
                break;

            case sf::Event::MouseButtonReleased:
                mouse_pressed = false;
                break;

            case sf::Event::MouseMoved:
                if (mouse_pressed) {
                    float y_prop = (float) button.getPosition().y / (float) height;
                    int image_y = y_prop * contain_sprite.getTextureRect().height;

                    printf("y_prop = %f, image_y = %d\n", y_prop, image_y);
                    contain_sprite.setTextureRect(sf::IntRect(0, image_y, contain.getSize().x, road.getSize().y));

                    if (event.mouseMove.y > road_sprite.getPosition().y 
                    && event.mouseMove.y + button.getTextureRect().height < road_sprite.getPosition().y + road_sprite.getTextureRect().height
                    && event.mouseMove.x > button.getPosition().x
                    && event.mouseMove.x < button.getPosition().x + button.getTextureRect().width) {
                        button.setPosition(button.getPosition().x, event.mouseMove.y);
                    }
                }
                break;
            }
        }
        app_window.draw(button);
        app_window.display();
    }

    return 0;
}