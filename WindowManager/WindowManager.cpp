#include "WindowManager.hpp"

//============================================================================\


Window::Window(Window* parent)
    : sweet_daddy(parent)
{
    if (sweet_daddy) {
        sweet_daddy->AddChild(this);
    }
    FULL_TRACE(MSG_TO_LOG(meow))
}

Window::~Window()
{
    for (auto& child : children) {
        // child->sweet_daddy = nullptr;
    }
    children.clear();

    if (sweet_daddy) {
        sweet_daddy->EraseChild(this);
    }
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

DrawableWindow::DrawableWindow(Window* parent, Color color)
    : Window(parent)
    , color(color)
{
    FULL_TRACE(MSG_TO_LOG(meow))
}

DrawableWindow::~DrawableWindow() 
{
    FULL_TRACE(MSG_TO_LOG(meow))
}
//============================================================================/

RectangleWindow::RectangleWindow(Window* parent, Color color, float x, float y, float width, float height)
    : DrawableWindow(parent, color)
    , x(x)
    , y(y)
    , width(width)
    , height(height)
{
    FULL_TRACE(MSG_TO_LOG(meow))
}

RectangleWindow::~RectangleWindow()
{
    printf("Suddenly I'm dead x(. My width was %f\n", width);
    FULL_TRACE(MSG_TO_LOG(meow))
}
void RectangleWindow::Draw()
{
    printf("draw with width %f and color %f %f %f\n", width, color.r, color.g, color.b);
    DrawRectangle(x, y, width, height, color);
}

//============================================================================\

ClickableWindow::ClickableWindow(Window* parent, Callback callback)
    : Window(parent)
    , callback(callback)
{
    FULL_TRACE(MSG_TO_LOG(meow))
}
ClickableWindow::~ClickableWindow()
{
    FULL_TRACE(MSG_TO_LOG(meow))
}
//============================================================================/