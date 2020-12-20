#include "Application.hpp"

void TestRectWindows(const int argc, const char*argv[])
{
    auto rwptr = new RectangleWindow(root_ptr, Color(SECOND_COLOR), 0, 0, 100, 100);
    auto rwptr_2 = new RectangleWindow(root_ptr, Color(SECOND_COLOR), 100, 100, 100, 100);
    AddDrawable(rwptr);
    AddDrawable(rwptr_2);

    auto button = new SimpleButton(root_ptr, Color(SECOND_COLOR), 200, 200, 200, 200, "sample button");
    AddDrawable(button);
    clickable_windows.insert(button);

    auto child = new RectangleWindow(rwptr, Color(DEFAULT_COLOR), 0, 0, 50, 50);
    AddDrawable(child);

    Run(argc, argv);

    // delete button;
    // delete child;
    // delete rwptr;
    // delete rwptr_2;
}


void TestTextureWindows(const int argc, const char*argv[])
{
    auto contain = new TextureWindow(root_ptr, "./Images/contain.png", 0, 0, 300, 324);
    auto road = new TextureWindow(root_ptr, "./Images/scrollRoad.png", 300, 0, 28, 324);
    auto button = new TextureWindow(road, "./Images/scrollButton.png", 302, 0, 24, 30);

    AddDrawable(contain);
    AddDrawable(road);
    AddDrawable(button);

    Run(argc, argv);

    delete contain;
    delete road;
    // delete button; -- не нужно, т.к. button -- дочернее окно.
}

void TestSlider(const int argc, const char* argv[])
{  
    auto contain = new TextureWindow(root_ptr, "./Images/contain.png", 0, 0, 658, 324);
    auto slider = new VerticalScrollWindow(root_ptr, "./Images/scrollRoad.png", 658, 0, 28, 324, "./Images/scrollButton.png", 660, 0, 24, 30);

    AddDrawable(contain);
    AddDrawable(slider);
    clickable_windows.insert(slider);

    Run(argc, argv);

    delete contain;
    delete slider;
}
int main(const int argc, const char* argv[])
{
    StartApp();
    
    TestSlider(argc, argv);

    // auto vsw = new VerticalScrollWindow ();
    
    return 0;
}