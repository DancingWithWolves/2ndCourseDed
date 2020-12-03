#include "WindowManager.hpp"
#include "GraphSystem.hpp"

//============================================================================\

Window::Window(Window* parent)
    : sweet_daddy(parent)
{
    if (sweet_daddy) {
        sweet_daddy->AddChild(this);
    }
    windows.insert(this);
    FULL_TRACE(MSG_TO_LOG(meow))
}

Window::~Window()
{
    for (auto& child : children) {
        child->~Window();
    }
    if (sweet_daddy) {
        sweet_daddy->EraseChild(this);
    }
    windows.erase(this);
    FULL_TRACE(MSG_TO_LOG(meow))
}

void Window::AddChild(Window* child)
{
    children.insert(child);
}

void Window::EraseChild(Window* child)
{
    children.erase(child);
}
//============================================================================/

//============================================================================\

DrawableWindow::DrawableWindow(Window* parent)
    : Window(parent)
{
    drawable_windows.insert(this);
    FULL_TRACE(MSG_TO_LOG(meow))
}

DrawableWindow::~DrawableWindow()
{

    FULL_TRACE(MSG_TO_LOG(meow))
}
//============================================================================/

class RectangleWindow : DrawableWindow {
    float x, y, width, height;

public:
    RectangleWindow(Window* parent)
        : DrawableWindow(parent)
    {
        FULL_TRACE(MSG_TO_LOG(meow))
    }

    ~RectangleWindow()
    {
        FULL_TRACE(MSG_TO_LOG(meow))
    }

    void Draw()
    {
        glColor3f(DEFAULT_COLOR);
        DrawRectangle(x, y, width, height);
    }
};

//============================================================================\

ClickableWindow::ClickableWindow(Window* parent, Callback callback)
    : Window(parent)
    , callback(callback)
{
    clickable_windows.insert(this);
    FULL_TRACE(MSG_TO_LOG(meow))
}

//============================================================================/