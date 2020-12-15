#include "EventSystem.hpp"

sf::RenderWindow* Engine::system_window_ptr = nullptr;

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
                EventManager::GetEventsQueue().emplace(PointEvent(mouse_press, event.size.width, event.size.height));
                break;

            case sf::Event::MouseButtonReleased:
                EventManager::GetEventsQueue().emplace(PointEvent(mouse_release, event.size.width, event.size.height));
                break;

            case sf::Event::MouseMoved:
                EventManager::GetEventsQueue().emplace(PointEvent(mouse_move, event.size.width, event.size.height));
                break;

            default:
                break;
            }
        }
        EventManager::GetEventsQueue().emplace(Event(redraw));
        while (!EventManager::GetEventsQueue().empty()) {
            EventManager::HandleEvent();
        }

        app_window.display();
    }
}

//============================================================================/

//============================================================================\

std::queue<Event>& EventManager::EventManager::GetEventsQueue()
{
    static std::queue<Event> event_queue;
    return event_queue;
}

//============================================================================/