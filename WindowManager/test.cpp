#include "Application.hpp"


int main(const int argc, const char* argv[])
{
    StartApp();
    auto rwptr = new RectangleWindow(root_ptr, Color(SECOND_COLOR), 0, 0, 100, 100);
    auto rwptr_2 = new RectangleWindow(root_ptr, Color(SECOND_COLOR), 100, 100, 100, 100);
    AddDrawable(rwptr);
    AddDrawable(rwptr_2);


    auto child = new RectangleWindow(rwptr, Color(DEFAULT_COLOR), 0, 0, 50, 50);
    AddDrawable(child);
    
    Run(argc, argv);

    delete child;
    delete rwptr;
    delete rwptr_2;
    
    return 0;
}