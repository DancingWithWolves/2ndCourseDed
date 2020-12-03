
#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include <unordered_set>

#include <iostream>

#include "Debug.hpp"


using std::unordered_set;

//============================================================================\

class Window {
private:
    Window* sweet_daddy;

protected:
    unordered_set<Window*> children;

public:
    Window() = delete;
    Window(Window* parent);
    virtual ~Window();

    virtual void AddChild(Window* child);

    virtual void EraseChild(Window* child);
};

unordered_set<Window*> windows;

//============================================================================/

//============================================================================\

class DrawableWindow : public Window {
public:
    DrawableWindow() = delete;
    DrawableWindow(Window* parent);
    virtual ~DrawableWindow() = default;

    virtual void Draw() = 0;
};


unordered_set<DrawableWindow*> drawable_windows;

//============================================================================/

//============================================================================\

class ClickableWindow : public Window {

protected:
    typedef void (*Callback)();
    Callback callback;

public:
    ClickableWindow(Window* parent, Callback callback);

    virtual ~ClickableWindow();

    bool virtual CheckMouseOver(int mouse_x, int mouse_y) = 0;
    void virtual Passive(int mouse_x, int mouse_y) = 0;
    void virtual OnPress(int mouse_x, int mouse_y) = 0;
    void virtual OnRelease(int mouse_x, int mouse_y) = 0;
    bool pressed, highlighted, active;
};


unordered_set<ClickableWindow*> clickable_windows;
//============================================================================/

#endif
