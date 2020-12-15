#pragma once

#include <iostream>
#include <vector>


#include "Engine/GraphManager.hpp"
#include "Engine/EventManager.hpp"

using std::vector;

class Window {
    // Events handling
public:
    Window() 
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }
    virtual ~Window() 
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }
};


class ContainterWindow : public Window {
protected:
    vector<Window*> children;
public:
    ContainterWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    ~ContainterWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    virtual void AddChild(Window *child)
    {
        children.push_back(child);
    }
};

class DrawableWindow : public Window {
public:
    DrawableWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    ~DrawableWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    virtual void Draw() = 0;
};

vector<DrawableWindow*> drawable_windows;

class ClickableWindow : public Window {
    
public:

    ClickableWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    ~ClickableWindow()
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }

    bool virtual CheckMouseOver(int mouse_x, int mouse_y) = 0;
    void virtual Passive(int mouse_x, int mouse_y) = 0;
    void virtual OnPress(int mouse_x, int mouse_y) = 0;
    void virtual OnRelease(int mouse_x, int mouse_y) = 0;
    bool pressed, highlighted, active;
protected:
    typedef void (*Callback)();
    Callback callback;
};




vector<ClickableWindow*> clickable_windows;


//------------------------------------------------------------------\
//                       Интерфейс с менеджером                     |
//------------------------------------------------------------------/

    void AddDrawableWindow(DrawableWindow *draw_ptr)
    {
        drawable_windows.push_back(draw_ptr);
    }

    void AddClickableWindow(ClickableWindow *click_ptr)
    {
        clickable_windows.push_back(click_ptr);
    }
//------------------------------------------------------------------/


//------------------------------------------------------------------\
//                       Интерфейс с движком                        |
//------------------------------------------------------------------/
    void Draw()
    {
        for (auto &i : drawable_windows) {
            i->Draw();
        }
    }

    void ResizeCallback(int widht, int height)
    {

    }

    //------------------------------------------------------------------\
    //                     Нажатие кнопок мыши                          |
    //------------------------------------------------------------------/
    void LeftMouseButtonPressed(int x, int y)
    {
        for (auto &i : clickable_windows) {
            i->OnPress(x, y);
        }
    }
    void MiddleMouseButtonPressed(int x, int y)
    {

    }
    void RightMouseButtonPressed(int x, int y)
    {

    }
    //------------------------------------------------------------------/


    //------------------------------------------------------------------\
    //                    Отпускание кнопок мыши                        |
    //------------------------------------------------------------------/
    void LeftMouseButtonReleased(int x, int y)
    {
        for (auto &i : clickable_windows) {
            i->OnRelease(x, y);
        }
    }
    void MiddleMouseButtonReleased(int x, int y)
    {

    }
    void RightMouseButtonReleased(int x, int y)
    {
        
    }
    //------------------------------------------------------------------/



    //------------------------------------------------------------------\
    //                   Пассивное движение мыши                        |
    //------------------------------------------------------------------/
    void ButtonPassiveMotionNoKey(int x, int y)
    {

    }
    void ButtonPassiveMotionKeyPressed(int x, int y)
    {

    }

//------------------------------------------------------------------/
