#ifndef EVENTSYSTEM
#define EVENTSYSTEM

#include <SFML/Graphics.hpp>

struct Engine {
    static sf::RenderWindow* system_window_ptr;

    static void StartEngine(const int argc, const char* argv[]);

    static void MainLoop();
};

#include "../WindowManager.hpp"

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


//============================================================================\
//                                  Обработчик ивентов
//============================================================================/

class EventManager {

public:
    static std::queue<Event>& GetEventsQueue();
    static void HandleEvent();
};

//============================================================================\
//                              Основной цикл
//                          Перехват ивентов у SFML
//============================================================================/

#endif
