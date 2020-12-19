#include "Application.hpp"


int main(const int argc, const char* argv[])
{
    StartApp();
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

    delete button;
    delete child;
    delete rwptr;
    delete rwptr_2;
    
    return 0;
}