#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include "../Debug.hpp"


struct Mouse {
    int x;
    int y;
    int left_button_pressed;
    int middle_button_pressed;
    int right_button_pressed;

    //	хранит координаты в момент первого нажатия кнопки
    int xpress;
    int ypress;
};

Mouse TheMouse = Mouse(); //дефолтный конструктор всё занулит, благо, типы примитивные

int window_width = 640;
int window_height = 480;
//------------------------------------------------------------------\
//               Прототипы функций, зависящих от оболочек

void Resize(int width, int height);

void MouseButton(int mouse_button, int pressed, int x, int y);

void MouseMotion(int x, int y);
void MousePassiveMotion(int x, int y);

//------------------------------------------------------------------/



//------------------------------------------------------------------\
//                       Интерфейс с движком                        |
//------------------------------------------------------------------/
extern void Draw();

extern void ResizeCallback(int widht, int height);

extern void LeftMouseButtonPressed(int x, int y);
extern void MiddleMouseButtonPressed(int x, int y);
extern void RightMouseButtonPressed(int x, int y);

extern void LeftMouseButtonReleased(int x, int y);
extern void MiddleMouseButtonReleased(int x, int y);
extern void RightMouseButtonReleased(int x, int y);

extern void ButtonPassiveMotionNoKey(int x, int y);
extern void ButtonPassiveMotionKeyPressed(int x, int y);

    //------------------------------------------------------------------\
    //                       Старт движка                               |
    //------------------------------------------------------------------/

    void EngineDraw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, window_height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        
        Draw();

        glLoadIdentity();
        glutSwapBuffers();
    }

    void StartEngine(int& argc, char* argv[])
    {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(window_width, window_height);
        glutInitWindowPosition(200, 100);
        glutCreateWindow("Windows Manager");
        glutDisplayFunc(EngineDraw);
        glutReshapeFunc(Resize);
        glutMouseFunc(MouseButton);
        glutMotionFunc(MouseMotion);
        glutPassiveMotionFunc(MousePassiveMotion);

        glutMainLoop();
    }
    //------------------------------------------------------------------/

//------------------------------------------------------------------/



void Resize(int width, int height)
{
    if (height == 0)
        height = 1; //Чтобы на 0 не поделить ненароком

    window_width = width;
    window_height = height;

    ResizeCallback(width, height);
    glViewport(0, 0, width, height);
}

void Font(void* font, char* text, int x, int y)
{
    glRasterPos2i(x, y);

    while (*text != '\0') {
        glutBitmapCharacter(font, *text);
        ++text;
    }
}

void MouseButton(int mouse_button, int pressed, int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    if (pressed == GLUT_DOWN) {

        if (!(TheMouse.right_button_pressed || TheMouse.middle_button_pressed || TheMouse.left_button_pressed)) {
            TheMouse.xpress = x;
            TheMouse.ypress = y;
        }

        switch (mouse_button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.right_button_pressed = 1;
            LeftMouseButtonPressed(x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.middle_button_pressed = 1;
            MiddleMouseButtonPressed(x, y);
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.right_button_pressed = 1;
            RightMouseButtonPressed(x, y);
            break;
        }
    } else {

        switch (mouse_button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.left_button_pressed = 0;
            LeftMouseButtonReleased(x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.middle_button_pressed = 0;
            MiddleMouseButtonReleased(x, y);
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.right_button_pressed = 0;
            RightMouseButtonReleased(x, y);
            break;
        }
    }

    glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
    TheMouse.x = x;
    TheMouse.y = y;

    ButtonPassiveMotionNoKey(x, y);

    glutPostRedisplay();
}

void MousePassiveMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    ButtonPassiveMotionKeyPressed(x, y);
}
