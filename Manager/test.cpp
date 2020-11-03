#include "Manager.hpp"


const char log_name[] = "WindowManager.log";
class Rectangle : public DrawableWindow {
protected:
    int x, y, width, height;
public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height){}

    void Draw() 
    {
        DrawRectangle(x, y, width, height);
    }
};

class Button : public Rectangle, public ClickableWindow {
public:
    Button(int x, int y, int width, int height, Callback callback) : Rectangle(x, y, width, height)
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    
        this->callback = callback;
    }
    bool CheckMouseOver(int mouse_x, int mouse_y) 
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


class BigBoss : public ContainterWindow, public Rectangle {
public:

    BigBoss(int x, int y, int width, int height) : Rectangle(x, y, width, height) 
    {
        MSG_TO_LOG("Meow there from file <%s>, function <%s>, line %d!\n", meow)
    }
    void Draw()
    {
        glColor3f(0.4f, 0.4f, 0.4f);
        Rectangle::Draw();
        DrawableWindow *dw = nullptr;
        for (auto child : children) {
            if ( (dw = dynamic_cast<DrawableWindow*>(child)) != nullptr) {
                ON_DEBUG(MSG_TO_LOG("kek\n"));
                glColor3f(0.8f, 0.8f, 0.8f);
                dw->Draw();
            }
        }
    }
};

class TextWindow : Rectangle {
    
};

void Callback()
{
    printf("I am callback\n");
}


int main(int argc, char* argv[])
{

    BigBoss daddy(0, 0, window_width, window_height);
    AddDrawableWindow(dynamic_cast<DrawableWindow*>(&daddy));

    Rectangle rec(10, 10, window_width-20, window_height-20);
    daddy.AddChild(&rec);

    Window window;
    daddy.AddChild(&window);
    
    StartEngine(argc, argv);
    
    return 0;
}
