#ifndef APPLICATION
#define APPLICATION

#include "Engine/GraphSystem.hpp"
#include <memory>
#include <queue>
#include <unordered_set>
#include "WindowManager.hpp"
using std::unordered_set;

RectangleWindow* root_ptr;

struct Application {
    static RectangleWindow& AppWindow()
    {
        static RectangleWindow app_window(nullptr, Color(DEFAULT_COLOR), 0, 0, START_SIZE);
        return app_window;
    }
};

unordered_set<Window*> windows;
unordered_set<DrawableWindow*> drawable_windows;
unordered_set<ClickableWindow*> clickable_windows;

static void AddDrawable(DrawableWindow* dwptr)
{
    windows.insert(dwptr);
    drawable_windows.insert(dwptr);
}

void StartApp()
{
    root_ptr = &Application::AppWindow();

    DrawableWindow* root_drawable_ptr = dynamic_cast<DrawableWindow*>(&Application::AppWindow());
    AddDrawable(root_drawable_ptr);
}

void Run(const int argc, const char* argv[])
{
    Engine::StartEngine(argc, argv);
}

void Engine::MainLoop() { }

void Draw(DrawableWindow* cur)
{
    if (!cur)
        return;

    cur->Draw();

    if (!cur->children.empty()) {
        for (auto& i : cur->children) {
            DrawableWindow* new_dw = dynamic_cast<DrawableWindow*>(i);
            Draw(new_dw);
        }
    }
}

void EventManager::HandleEvent()
{
    Event* cur_event = EventManager::GetEventsQueue().front();
    switch (cur_event->GetType()) {
    case redraw: {
        DrawableWindow* cur_window = static_cast<DrawableWindow*>(root_ptr);
        Draw(cur_window);
    } break;
    case mouse_press:
        ON_DEBUG(if (the_mouse.button_pressed) printf("The mouse is pressed at %d, %d\n", the_mouse.xpress, the_mouse.ypress););
        
        for (auto& i : clickable_windows) {
            PointEvent* pe = static_cast<PointEvent*>(cur_event);
            i->OnPress(pe->GetX(), pe->GetY());
        }
        break;

    case mouse_release:
        ON_DEBUG(if (!the_mouse.button_pressed) printf("The mouse is released at %d, %d\n", the_mouse.x, the_mouse.y););

        for (auto& i : clickable_windows) {
            PointEvent* pe = static_cast<PointEvent*>(cur_event);
            i->OnRelease(pe->GetX(), pe->GetY());
        }
        break;

    case mouse_move:

        for (auto& i : clickable_windows) {
            PointEvent* pe = static_cast<PointEvent*>(cur_event);
            i->Passive(pe->GetX(), pe->GetY());
        }
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
    // TODO:
    delete EventManager::GetEventsQueue().front();
    EventManager::GetEventsQueue().pop();
}

class SimpleButton : public RectangleButton {
public:
    SimpleButton(Window* parent, Color color, int x, int y, int width, int height, const char label[])
        : RectangleButton(parent, color, x, y, width, height, label)
    {
    }

    void Passive(int mouse_x, int mouse_y) {}

    void OnPress(int mouse_x, int mouse_y) {}

    void Callback()
    {
        printf("Hello there! I am button [%p] being pressed\n", this);
    }
};

//===================================================================================================================\
//                 Дополнительные окна для конкретного приложения



#endif
