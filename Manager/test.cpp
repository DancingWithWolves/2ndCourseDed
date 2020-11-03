#include "Manager.hpp"

class Rectangle : public DrawableWindow {
protected:
    int x, y, width, height;
public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height){}

    void Draw() 
    {
        ON_DEBUG(printf("Rectangle::Draw, x = %d, y = %d, width = %d, height = %d\n", x, y, width, height));
        DrawRectangle(x, y, width, height);
    }
};

class Button : public Rectangle, public ClickableWindow {
public:
    Button(int x, int y, int width, int height, Callback callback) : Rectangle(x, y, width, height)
    {
        this->callback = callback;
    }
    bool virtual CheckMouseOver(int mouse_x, int mouse_y) 
    {
        if (mouse_x > x && mouse_x < x + width && mouse_y > y && mouse_y < y + height) {
            return true;
        }

        return false;
    }
    void Passive(int mouse_x, int mouse_y){}
    void OnPress(int mouse_x, int mouse_y){}
    void OnRelease(int mouse_x, int mouse_y)
    {
        if (CheckMouseOver(mouse_x, mouse_y)) {
            callback();
        } 
    }
     
};


void Callback()
{
    printf("I am callback\n");
}
int main(int argc, char* argv[])
{

    Button button (100, 100, 100, 100, Callback);
    AddDrawableWindow(dynamic_cast<DrawableWindow*>(&button));
    AddClickableWindow(dynamic_cast<ClickableWindow*>(&button));
    StartEngine(argc, argv);
    return 0;
}
