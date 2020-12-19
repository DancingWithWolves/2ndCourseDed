
#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include "Debug.hpp"
#include "Engine/GraphSystem.hpp"
#include <iostream>

#include <unordered_set>
using std::unordered_set;

//============================================================================\

class Window {
private:
    Window* sweet_daddy;

public:
    Window(Window* parent);
    virtual ~Window();

    unordered_set<Window*> children;

    bool HasChildren() { return !children.empty(); }
    virtual void AddChild(Window* child);

    virtual void EraseChild(Window* child);
};

//============================================================================/

//============================================================================\

class DrawableWindow : public Window {
public:
    Color color;
    DrawableWindow(Window* parent, Color color);
    virtual ~DrawableWindow();

    virtual void Draw() = 0;
};

//============================================================================/

//============================================================================\

class RectangleWindow : public DrawableWindow {
protected:
    float x, y, width, height;

public:
    RectangleWindow(Window* parent, Color color, float x, float y, float width, float height);
    virtual ~RectangleWindow();
    void Draw();
    float GetX() { return x; };
    float GetY() { return y; }
    void SetX(float x) { this->x = x; };
    void SetY(float y) { this->y = y; };
};
//============================================================================/

//============================================================================\

class ClickableWindow {

public:
    ClickableWindow();

    virtual ~ClickableWindow();

    bool virtual CheckMouseOver(int mouse_x, int mouse_y) = 0;
    void virtual Passive(int mouse_x, int mouse_y) = 0;
    void virtual OnPress(int mouse_x, int mouse_y) = 0;
    void virtual OnRelease(int mouse_x, int mouse_y) = 0;
    void virtual Callback() = 0;
    bool pressed, highlighted, active;
};

//============================================================================/

//============================================================================\

class RectangleButton : public ClickableWindow, public RectangleWindow {
    char* label;

public:
    RectangleButton(Window* parent, Color color, float x, float y, float width, float height, const char label[]);
    RectangleButton(const RectangleButton &from) = delete;
    RectangleButton& operator=(const RectangleButton& from) = delete;
    bool CheckMouseOver(int mouse_x, int mouse_y);
    virtual void Passive(int mouse_x, int mouse_y) = 0;
    virtual void OnPress(int mouse_x, int mouse_y) = 0;
    virtual void OnRelease(int mouse_x, int mouse_y);
    virtual void Callback() = 0;

    virtual ~RectangleButton();
};
//============================================================================/

#endif
