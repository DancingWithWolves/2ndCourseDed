#include "WindowManager.hpp"
#include <string.h>

//============================================================================\
//                        Интерфейс абстрактного окна
//============================================================================/

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

//============================================================================\
//                        Интерфейс риусемого окна
//============================================================================/

DrawableWindow::DrawableWindow(Window* parent, Color color)
    : Window(parent)
    , color(color) {
        FULL_TRACE(MSG_TO_LOG(meow))
    }

DrawableWindow::~DrawableWindow() { FULL_TRACE(MSG_TO_LOG(meow)) }

//============================================================================\
//                 Рисуемое окно с текстурой
//============================================================================/

TextureWindow::TextureWindow(Window * parent,
        const char* texture_fname, int x, int y, int width, int height)
    : DrawableWindow(parent, Color(DEFAULT_COLOR))
    , sprite(texture_fname, x, y, width, height) {
        Draw();
        // printf("sprite.showed_texture_x = %d, showed_texture_y = %d, showed_texture_width = %d, showed_texture_height = %d\n\n\n",
        // sprite.GetShowedTextureX(), sprite.GetShowedTextureY(), sprite.GetShowedTextureWidth(), sprite.GetShowedTextureHeight());
    }

TextureWindow::~TextureWindow(){}

void TextureWindow::Draw()
{
    DrawSprite(sprite);
}

//============================================================================\
//                  Интерфейс прямоугольного рисуемого окна
//============================================================================/

RectangleWindow::RectangleWindow(Window* parent, Color color, int x, int y, int width, int height)
    : DrawableWindow(parent, color)
    , x(x)
    , y(y)
    , width(width)
    , height(height) { FULL_TRACE(MSG_TO_LOG(meow)) }

    RectangleWindow::~RectangleWindow()
{
    ON_DEBUG(printf("Suddenly I'm dead x(. My width was %d\n", width));
    FULL_TRACE(MSG_TO_LOG(meow))
}
void RectangleWindow::Draw()
{
    DrawRectangle(x, y, width, height, color);
}

RectangleButton::RectangleButton(Window* parent, Color color, int x, int y, int width, int height, const char label[])
    : RectangleWindow(parent, color, x, y, width, height)
{
    this->label = strdup(label);
    FULL_TRACE(MSG_TO_LOG(meow))
}

RectangleButton::~RectangleButton()
{
    free(label);
    FULL_TRACE(MSG_TO_LOG(meow))
}
bool RectangleButton::CheckMouseOver(int mouse_x, int mouse_y)
{
    return (mouse_x < this->x + this->width
        && mouse_x > this->x
        && mouse_y < this->y + this->height
        && mouse_y > this->y);
}

void RectangleButton::OnRelease(int mouse_x, int mouse_y)
{
    if (CheckMouseOver(mouse_x, mouse_y)) {
        Callback();
    }
}

//============================================================================\
//                        Интерфейс кликабельного окна
//============================================================================/

ClickableWindow::ClickableWindow() {
    FULL_TRACE(MSG_TO_LOG(meow))
} ClickableWindow::~ClickableWindow()
{
    FULL_TRACE(MSG_TO_LOG(meow))
}