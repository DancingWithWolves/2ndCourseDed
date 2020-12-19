#include "WindowManager.hpp"
#include <string.h>

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
    , color(color) {
        FULL_TRACE(MSG_TO_LOG(meow))
    }

    DrawableWindow::~DrawableWindow() { FULL_TRACE(MSG_TO_LOG(meow)) } //============================================================================/

    RectangleWindow::RectangleWindow(Window * parent, Color color, float x, float y, float width, float height)
    : DrawableWindow(parent, color)
    , x(x)
    , y(y)
    , width(width)
    , height(height) { FULL_TRACE(MSG_TO_LOG(meow)) }

    RectangleWindow::~RectangleWindow()
{
    ON_DEBUG(printf("Suddenly I'm dead x(. My width was %.3f\n", width));
    FULL_TRACE(MSG_TO_LOG(meow))
}
void RectangleWindow::Draw()
{
    DrawRectangle(x, y, width, height, color);
}

//============================================================================\

ClickableWindow::ClickableWindow() {
    FULL_TRACE(MSG_TO_LOG(meow))
} ClickableWindow::~ClickableWindow() {
    FULL_TRACE(MSG_TO_LOG(meow))
}
//============================================================================/

//============================================================================\

Button::Button(Window* parent, Color color, float x, float y, float width, float height, const char label[])
    : RectangleWindow(parent, color, x, y, width, height)
{
    this->label = strdup(label);
    FULL_TRACE(MSG_TO_LOG(meow))
}

Button::~Button()
{
    free(label);
    FULL_TRACE(MSG_TO_LOG(meow))
}
bool Button::CheckMouseOver(int mouse_x, int mouse_y)
{
    return (mouse_x < this->x + this->width
        && mouse_x > this->x
        && mouse_y < this->y + this->height
        && mouse_y > this->y);
}

//============================================================================/