#ifndef EVENTSYSTEM
#define EVENTSYSTEM

#include "../WindowManager.hpp"
#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_set>
#include <vector>

//============================================================================\
//                                  Ивенты
//============================================================================/

typedef enum {
    undefined = -1,
    redraw,
    mouse_press,
    mouse_release,
    mouse_move,
    key_press,
    key_release,
    idle
} EventType;

struct Event {
    Event() = delete;
    Event(EventType type)
        : type(type)
    {
    }
    EventType GetType() { return type; }
    virtual ~Event() {};

private:
    EventType type = undefined;
};

struct PointEvent : public Event {
    PointEvent(EventType type, int x, int y)
        : Event(type)
        , x(x)
        , y(y)
    {
    }
    int GetX() { return x; }
    int GetY() { return y; }
    virtual ~PointEvent() {};

private:
    int x = 0, y = 0;
};

std::queue<Event> events_to_handle;

//============================================================================\
//                                  Обработчик ивентов
//============================================================================/

class EventManager {
    std::queue<Event> events_queue;

public:
    static void HandleEvent()
    {
        Event& cur_event = events_to_handle.front();
        switch (cur_event.GetType()) {
        case redraw:
            for (auto& i : drawable_windows) { //from WindowManager.hpp
                i->Draw();
            }
            break;

        case mouse_press:
            for (auto& i : clickable_windows) {
                PointEvent& pe = dynamic_cast<PointEvent&>(cur_event);
                i->OnPress(pe.GetX(), pe.GetY());
            }
            break;

        case mouse_release:
            break;

        case mouse_move:
            break;

        case key_press:
            break;

        case key_release:
            break;

        case idle:
            break;

        default:
            break;
        }
        events_to_handle.pop();
    }
};

//============================================================================\
//                              Основной цикл
//                          Перехват ивентов у SFML
//============================================================================/

void StartEngine(const int argc, const char* argv[])
{
    sf::RenderWindow app_window(sf::VideoMode(600, 600), argv[0]);
    app_window.clear();
    app_window.display();
    sf::Event event = {};
    while (app_window.isOpen()) {

        // ON_DEBUG(printf(meow));

        while (app_window.pollEvent(event)) {

            switch (event.type) {

            case sf::Event::Closed:
                ON_DEBUG(printf(meow));
                app_window.close();
                break;

            case sf::Event::MouseButtonPressed:
                events_to_handle.emplace(PointEvent(mouse_press, event.size.width, event.size.height));
                ON_DEBUG(printf(meow));
                break;

            case sf::Event::MouseButtonReleased:
                events_to_handle.emplace(PointEvent(mouse_release, event.size.width, event.size.height));
                break;

            case sf::Event::MouseMoved:
                events_to_handle.emplace(PointEvent(mouse_move, event.size.width, event.size.height));
                break;
            default:
                break;
            }
        }

        while (!events_to_handle.empty()) {
            EventManager::HandleEvent();
        }

        app_window.display();
    }
}

#endif
