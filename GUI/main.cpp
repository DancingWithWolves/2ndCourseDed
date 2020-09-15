#include "sortFunctions.h"
#include "windowManager.h"
#include <stdio.h>

int compares_qty = 0, swaps_qty = 0;

int compare(const void* arg_1, const void* arg_2)
{
    compares_qty++;
    return *(int*)arg_1 - *(int*)arg_2;
}
int buttons_qty = 3;

void AButtonCallback(){
    printf("lol\n");
}

void ShowBubbleSortStat()
{

}
void ShowInsertionSortStat()
{
    
}
void ShowQuickSortStat()
{
    
}

void Clear()
{
    for (int i = 0; i < buttons_qty; ++i) {
        buttons[i].~Button();
    }
    ::operator delete (buttons);
}


int main(int argc, char** argv)
{

    buttons = reinterpret_cast<Button*>(::operator new (sizeof(Button) * buttons_qty));
    
    Button *button;
    button = new (buttons) Button(5, 5, window_width / 3 - 10, 25, "Button 1", AButtonCallback);
    button = new (buttons + 1) Button(window_width / 3 + 5, 5, window_width / 3 - 10, 25, "Button 2", AButtonCallback);
    button = new (buttons + 2) Button(window_width * 2 / 3 + 5, 5, window_width / 3 - 10, 25, "Button 3", AButtonCallback);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("03 - Mouse Motion");

    glutDisplayFunc(Draw);
    glutReshapeFunc(Resize);
    // mouse click
    glutMouseFunc(MouseButton);
    // mouse motion
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MousePassiveMotion);

    atexit(Clear);
    glutMainLoop();
    
    return 0;
}