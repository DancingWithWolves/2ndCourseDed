#ifndef APPLICATION
#define APPLICATION

#include "Engine/GraphSystem.hpp"
#include <memory>
#include <queue>
#include <unordered_set>

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
        for (auto &i : cur->children) {
            DrawableWindow *new_dw = dynamic_cast<DrawableWindow*>(i);
            Draw(new_dw);
        }
    }
}


void EventManager::HandleEvent()
{
    Event& cur_event = EventManager::GetEventsQueue().front();
    switch (cur_event.GetType()) {
    case redraw: {
            DrawableWindow* cur_window = dynamic_cast<DrawableWindow*>(root_ptr);
            Draw(cur_window);
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
    EventManager::GetEventsQueue().pop();
}

#endif
