#include "EventSystem.hpp"
#include "../WindowManager.hpp"

sf::RenderWindow* Engine::system_window_ptr = nullptr;

Mouse the_mouse = Mouse();
//============================================================================\

void MainLoop();

void Engine::StartEngine(const int argc, const char* argv[])
{
    sf::RenderWindow app_window(sf::VideoMode(START_SIZE), argv[0]);
    Engine::system_window_ptr = &app_window;
    app_window.setTitle(argv[0]);
    app_window.clear();

    sf::Event event = {};
    while (app_window.isOpen()) {
        app_window.clear();

        Engine::MainLoop();
        while (app_window.pollEvent(event)) {

            switch (event.type) {

            case sf::Event::Closed:
                ON_DEBUG(printf("bye bye!\n"));
                app_window.close();
                system_window_ptr = nullptr;
                break;

            case sf::Event::MouseButtonPressed:

                the_mouse.button_pressed = true;
                the_mouse.xpress = event.mouseButton.x;
                the_mouse.ypress = event.mouseButton.y;

                EventManager::GetEventsQueue().push(new PointEvent(mouse_press, event.mouseButton.x, event.mouseButton.y));
                break;

            case sf::Event::MouseButtonReleased:

                the_mouse.button_pressed = false;
                the_mouse.xpress = -1;
                the_mouse.ypress = -1;

                EventManager::GetEventsQueue().push(new PointEvent(mouse_release, event.mouseButton.x, event.mouseButton.y));
                break;

            case sf::Event::MouseMoved:
                the_mouse.x = event.mouseMove.x;
                the_mouse.y = event.mouseMove.y;

                EventManager::GetEventsQueue().push(new PointEvent(mouse_move, event.mouseMove.x, event.mouseMove.y));
                break;

            default:
                break;
            }
        }
        EventManager::GetEventsQueue().emplace(new Event(redraw));
        while (!EventManager::GetEventsQueue().empty()) {
            EventManager::HandleEvent();
        }

        app_window.display();
    }
}

//============================================================================/

//============================================================================\

std::queue<Event*>& EventManager::EventManager::GetEventsQueue()
{
    static std::queue<Event*> event_queue;
    return event_queue;
}

//============================================================================/