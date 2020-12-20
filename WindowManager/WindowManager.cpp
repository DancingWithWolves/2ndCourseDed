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

    TextureWindow::TextureWindow(Window * parent, const char* texture_fname, int x, int y, int width, int height)
    : DrawableWindow(parent, Color(DEFAULT_COLOR))
    , sprite(texture_fname, x, y, width, height)
{
    Draw();
    // printf("sprite.showed_texture_x = %d, showed_texture_y = %d, showed_texture_width = %d, showed_texture_height = %d\n\n\n",
    // sprite.GetShowedTextureX(), sprite.GetShowedTextureY(), sprite.GetShowedTextureWidth(), sprite.GetShowedTextureHeight());
}

TextureWindow::~TextureWindow() { }

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
} ClickableWindow::~ClickableWindow() {
    FULL_TRACE(MSG_TO_LOG(meow))
}

//============================================================================\
//                        Имплементация перетаскиваемого окна
//============================================================================/

VerticalScrollWindow::VerticalScrollWindow(Window* parent, const char* texture_container_fname, int x, int y, int width, int height,
    const char* texture_slider_fname, int slider_x, int slider_y, int slider_width, int slider_height)
    : TextureWindow(parent, texture_container_fname, x, y, width, height)
    , slider(this, texture_slider_fname, slider_x, slider_y, slider_width, slider_height)
    , prop_y(0)
    , is_dragged(false)
{
}

void VerticalScrollWindow::MoveSlider(float prop_y)
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
    }
}

void VerticalScrollWindow::OnPress(int mouse_x, int mouse_y)
{
    if (the_mouse.button_pressed
        && mouse_x < slider.sprite.GetX() + slider.sprite.GetWidth()
        && mouse_x > slider.sprite.GetX()
        && mouse_y < slider.sprite.GetY() + slider.sprite.GetHeight()
        && mouse_y > slider.sprite.GetY()) {
        is_dragged = true;
    }
}
void VerticalScrollWindow::OnRelease(int mouse_x, int mouse_y)
{
    is_dragged = false;
}
void VerticalScrollWindow::Callback() { }

bool VerticalScrollWindow::CheckMouseOver(int mouse_x, int mouse_y)
{
    return (mouse_x < sprite.GetX() + sprite.GetWidth()
        && mouse_x > sprite.GetX()
        && mouse_y < sprite.GetY() + sprite.GetHeight()
        && mouse_y > sprite.GetY());
}
