#include <iostream>
#include <vector>
#include <Manager.hpp>


#include <Engine/GraphManager.hpp>
#include <Engine/EventManager.hpp>

using std::vector;

class Window {

};

class DrawableWindow : public Window {
    virtual void Draw() = 0;
};

class ClickableWindow : public DrawableWindow {
    typedef void (*Callback)();

    void virtual Passive(int mouse_x, int mouse_y) = 0;
    void virtual OnPress(int mouse_x, int mouse_y) = 0;
    void virtual OnRelease(int mouse_x, int mouse_y) = 0;
    bool pressed, highlighted, active;

protected:
    Callback callback;
};

