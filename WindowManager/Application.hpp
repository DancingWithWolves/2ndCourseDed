#ifndef APPLICATION
#define APPLICATION

#include "Engine/GraphSystem.hpp"
#include "WindowManager.hpp"
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

    void Passive(int mouse_x, int mouse_y) { }

    void OnPress(int mouse_x, int mouse_y) { }

    void Callback()
    {
        printf("Hello there! I am button [%p] being pressed\n", this);
    }
};

//===================================================================================================================\
//                 Дополнительные окна для конкретного приложения



class ScrollableWindow;

//============================================================================\
//                      Имплементация прокручиваемого окна
//============================================================================/

class VerticalScrollWindow : public ClickableWindow, public TextureWindow {
protected:
    TextureWindow slider;

public:
    VerticalScrollWindow(Window* parent, const char* texture_container_fname, int x, int y, int width, int height,
        const char* texture_slider_fname, int slider_x, int slider_y, int slider_width, int slider_height)
        : TextureWindow(parent, texture_container_fname, x, y, width, height)
        , slider(this, texture_slider_fname, slider_x, slider_y, slider_width, slider_height)
        , prop_y(0)
        , is_dragged(false)
    {
    }

    void MoveSlider(float prop_y)
    {
        if (prop_y < 0) {
            prop_y = 0;
        }
        if (prop_y > 1) {
            prop_y = 1;
        }
        this->prop_y = prop_y;
        const int new_y = sprite.GetY() + static_cast<int>(prop_y * (sprite.GetHeight() - slider.sprite.GetHeight()));

        slider.sprite.SetY(new_y);
    }

    void Passive(int mouse_x, int mouse_y);

    void OnPress(int mouse_x, int mouse_y)
    {
        if (the_mouse.button_pressed
            && mouse_x < slider.sprite.GetX() + slider.sprite.GetWidth()
            && mouse_x > slider.sprite.GetX()
            && mouse_y < slider.sprite.GetY() + slider.sprite.GetHeight()
            && mouse_y > slider.sprite.GetY()) {
            is_dragged = true;
        }
    }
    void OnRelease(int mouse_x, int mouse_y)
    {
        is_dragged = false;
    }
    void Callback() {}

    bool CheckMouseOver(int mouse_x, int mouse_y)
    {
        return (mouse_x < sprite.GetX() + sprite.GetWidth()
            && mouse_x > sprite.GetX()
            && mouse_y < sprite.GetY() + sprite.GetHeight()
            && mouse_y > sprite.GetY());
    }

    float prop_y;
    bool is_dragged;
};

//============================================================================\
//                        Имплементация перетаскиваемого окна
//============================================================================/

class ScrollableWindow : public TextureWindow {
protected:
    VerticalScrollWindow scroll_window;

public:
    ScrollableWindow(Window* parent,
        const char* texture_container_fname, int x, int y, int width, int height,
        const char* texture_scroll_window_fname, int scroll_window_x, int scroll_window_y, int scroll_window_width, int scroll_window_height,
        const char* texture_slider_fname, int slider_x, int slider_y, int slider_width, int slider_height)
        : TextureWindow(parent, texture_container_fname, x, y, width, height)
        , scroll_window(this,
              texture_scroll_window_fname, scroll_window_x, scroll_window_y, scroll_window_width, scroll_window_height,
              texture_slider_fname, slider_x, slider_y, slider_width, slider_height)
    {
        clickable_windows.insert(&scroll_window);
    }

    void MoveView(float prop)
    {
        printf(meow);
        if (prop < 0) {
            prop = 0;
        }
        if (prop > 1) {
            prop = 1;
        }
        if (sprite.GetTextureHeight() > sprite.GetHeight()) {
            int new_y = static_cast<int>(prop * static_cast<float>(sprite.GetTextureHeight() - sprite.GetHeight()));
            sprite.SetShowedTextureY(new_y);
        }
    }
};

//============================================================================\
//                 Часть ребёнка, зависящая от родителя по иерархии окон
//============================================================================/
void VerticalScrollWindow::Passive(int mouse_x, int mouse_y)
    {
        if (the_mouse.button_pressed
            && is_dragged
            && mouse_x <= slider.sprite.GetX() + slider.sprite.GetWidth() + 10
            && mouse_x >= slider.sprite.GetX() - 10
            && mouse_y <= slider.sprite.GetY() + slider.sprite.GetHeight() + 10
            && mouse_y >= slider.sprite.GetY() - 10) {
            float prop = static_cast<float>(mouse_y - sprite.GetY()) / static_cast<float>(sprite.GetHeight());
            MoveSlider(prop);
            dynamic_cast<ScrollableWindow*>(sweet_daddy)->MoveView(prop);
        }
    }

#endif
