
#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include "Debug.hpp"
#include "Engine/GraphSystem.hpp"
#include <iostream>

#include <unordered_set>
using std::unordered_set;

//============================================================================\
//                        Абстрактное окно
//============================================================================/
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

//============================================================================\
//                        Интерфейс рисуемого окна
//============================================================================/

class DrawableWindow : public Window {

public:
    Color color;
    DrawableWindow(Window* parent, Color color);
    virtual ~DrawableWindow();

    virtual void Draw() = 0;
};

//============================================================================\
//                 Рисуемое окно с текстурой
//============================================================================/

class TextureWindow : public DrawableWindow {
public:
    Sprite sprite;

    TextureWindow(Window *parent, const char* texture_fname, int x, int y, int width, int height);
    virtual ~TextureWindow();
    virtual void Draw();
};


//============================================================================\
//                  Прямоугольное окно с заливкой
//============================================================================/

class RectangleWindow : public DrawableWindow {
protected:
    int x, y, width, height;

public:
    RectangleWindow(Window* parent, Color color, int x, int y, int width, int height);
    virtual ~RectangleWindow();
    void Draw();
    int GetX() { return x; };
    int GetY() { return y; }
    void SetX(int x) { this->x = x; };
    void SetY(int y) { this->y = y; };
};

//============================================================================\
//                        Интерфейс кликабельного окна
//============================================================================/

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

//============================================================================\
//                        Интерфейс прямоугольной кнопки
//============================================================================/

class RectangleButton : public ClickableWindow, public RectangleWindow {
    char* label;

public:
    RectangleButton(Window* parent, Color color, int x, int y, int width, int height, const char label[]);
    RectangleButton(const RectangleButton& from) = delete;
    RectangleButton& operator=(const RectangleButton& from) = delete;
    bool CheckMouseOver(int mouse_x, int mouse_y);
    virtual void Passive(int mouse_x, int mouse_y) = 0;
    virtual void OnPress(int mouse_x, int mouse_y) = 0;
    virtual void OnRelease(int mouse_x, int mouse_y);
    virtual void Callback() = 0;

    virtual ~RectangleButton();
};


//============================================================================\
//                        Имплементация перетаскиваемого окна
//============================================================================/

class VerticalScrollWindow : public ClickableWindow, public TextureWindow {
protected:
    TextureWindow slider;
public:
    VerticalScrollWindow(Window *parent, const char* texture_container_fname, int x, int y, int width, int height,
                         const char* texture_slider_fname, int slider_x, int slider_y, int slider_width, int slider_height);
    void MoveSlider(float prop_y);

    void Passive(int mouse_x, int mouse_y);
    void OnPress(int mouse_x, int mouse_y);
    void OnRelease(int mouse_x, int mouse_y);
    void Callback();

    bool CheckMouseOver(int mouse_x, int mouse_y);

    float prop_y;
    bool is_dragged;
};


#endif
