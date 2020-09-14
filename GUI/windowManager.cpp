
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <GL/glut.h>

const char log_name[] = "GUI.log";

// A structure to represent the mouse information

struct Mouse {
    int x;
    int y;
    int left_button_pressed;
    int middle_button_pressed;
    int right_button_pressed;

    int xpress; //	stores the x coord of when the first button press occurred
    int ypress; //	stores the y coord of when the first button press occurred
};

Mouse TheMouse = Mouse(); //TODO: правило трёх

int window_width = 640;
int window_height = 480;

struct Button {

    typedef void (*ButtonCallback)();

private:
    static size_t qty;
    size_t id;

public:
    Button(int x = 0, int y = 0, int width = 0, int height = 0, const char text[] = nullptr, ButtonCallback function = nullptr)
        : callbackFunction(function)
        , x(x)
        , y(y)
        , height(height)
        , width(width)
        , pressed(false)
        , highlighted(false)
    {
        this->label = strdup(text);
        this->id = qty;
        ++qty;
    }

    ~Button()
    {
        --qty;
        free(label);
    }
    Button(const Button& from)
        : callbackFunction(from.callbackFunction)
        , x(from.x)
        , y(from.y)
        , height(from.height)
        , width(from.width)
        , pressed(false)
        , highlighted(false)
    {
        this->label = strdup(from.label);
        ++qty;
    }

    // functions
    void OnPressed()
    {
        FILE* log = fopen(log_name, "at");
        fprintf(log, "Meow there! I'm '%ld' button pressed!\n", id);
        fclose(log);
    }

    // variables
    int x;
    int y;
    int width;
    int height;
    bool pressed;
    bool highlighted; // is the mouse cursor over the control?
    char* label;
    ButtonCallback callbackFunction; // A pointer to a function to call if the button is pressed
};

size_t Button::qty = 0;


/*----------------------------------------------------------------------------------------
    *	This is an example callback function. Notice that it's type is the same
    *	an the ButtonCallback type. We can assign a pointer to this function which
    *	we can store and later call.
    */
void TheButtonCallback()
{
    printf("God saves the cat 1!\n");
}
void TheButtonCallback2()
{
    printf("God saves the cat 2!\n");
}

void TheButtonCallback3()
{
    printf("God saves the cat 3!\n");
}

Button button_1 = Button(5, 5, window_width / 3 - 10, 25, "Button 1", TheButtonCallback);
Button button_2 = Button(window_width / 3 + 5, 5, window_width / 3 - 10, 25, "Button 2", TheButtonCallback2);
Button button_3 = Button(window_width * 2 / 3 + 5, 5, window_width / 3 - 10, 25, "Button 3", TheButtonCallback3);
/*----------------------------------------------------------------------------------------
    *	\brief	This function draws a text string to the screen using glut bitmap fonts.
    *	\param	font	-	the font to use. it can be one of the following : 
    *
    *					GLUT_BITMAP_9_BY_15		
    *					GLUT_BITMAP_8_BY_13			
    *					GLUT_BITMAP_TIMES_ROMAN_10	
    *					GLUT_BITMAP_TIMES_ROMAN_24	
    *					GLUT_BITMAP_HELVETICA_10	
    *					GLUT_BITMAP_HELVETICA_12	
    *					GLUT_BITMAP_HELVETICA_18	
    *
    *	\param	text	-	the text string to output
    *	\param	x		-	the x coordinate
    *	\param	y		-	the y coordinate
    */
void Font(void* font, char* text, int x, int y)
{
    glRasterPos2i(x, y);

    while (*text != '\0') {
        glutBitmapCharacter(font, *text);
        ++text;
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function is used to see if a mouse click or event is within a button 
    *			client area.
    *	\param	b	-	a pointer to the button to test
    *	\param	x	-	the x coord to test
    *	\param	y	-	the y-coord to test
    */
bool ButtonClickTest(Button* b, int x, int y)
{
    if (b) {
        if (x > b->x
            && x < b->x + b->width
            && y > b->y
            && y < b->y + b->height) {
            return true;
        }
    }

    return false;
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function handles if the button is pressed.
    *	\param	b	-	a pointer to the button to check.
    *	\param	x	-	the x location of the mouse cursor.
    *	\param	y	-	the y location of the mouse cursor.
    */
void ButtonPress(Button* b, int x, int y)
{
    if (b) {
        if (ButtonClickTest(b, x, y))
            b->pressed = true;
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function handles if the button is released.
    *	\param	b	-	a pointer to the button to check.
    *	\param	x	-	the x location of the mouse cursor.
    *	\param	y	-	the y location of the mouse cursor.
    */
void ButtonRelease(Button* b, int x, int y)
{
    if (b) {

        if (ButtonClickTest(b, TheMouse.xpress, TheMouse.ypress)
            && ButtonClickTest(b, x, y)) {

            b->OnPressed();
            if (b->callbackFunction)
                b->callbackFunction();
        }
        b->pressed = false;
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function draws the specified button.
    *	\param	b	-	a pointer to the button to check.
    *	\param	x	-	the x location of the mouse cursor.
    *	\param	y	-	the y location of the mouse cursor.
    */
void ButtonPassive(Button* b, int x, int y)
{
    if (b) {

        if (ButtonClickTest(b, x, y)) {
            /*
                *	If the cursor has just arrived over the control, set the highlighted flag
                *	and force a redraw. The screen will not be redrawn again until the mouse
                *	is no longer over this control
                */
            if (b->highlighted == 0) {
                b->highlighted = 1;
                glutPostRedisplay();
            }
        } else

            /*
            *	If the cursor is no longer over the control, then if the control
            *	is highlighted (ie, the mouse has JUST moved off the control) then
            *	we set the highlighting back to false, and force a redraw. 
            */
            if (b->highlighted) {
            b->highlighted = false;
            glutPostRedisplay();
        }
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function draws the specified button.
    *	\param	b	-	a pointer to the button to draw.
    */
void ButtonDraw(Button* b)
{
    int fontx = 0, fonty = 0;

    if (b != NULL) {
        /*
            *	We will indicate that the mouse cursor is over the button by changing its
            *	colour.
            */
        if (b->highlighted)
            glColor3f(0.7f, 0.7f, 0.8f);
        else
            glColor3f(0.6f, 0.6f, 0.6f);

        /*
            *	draw background for the button.
            */
        glBegin(GL_QUADS);
        glVertex2i(b->x, b->y);
        glVertex2i(b->x, b->y + b->height);
        glVertex2i(b->x + b->width, b->y + b->height);
        glVertex2i(b->x + b->width, b->y);
        glEnd();

        /*
            *	Draw an outline around the button with width 3
            */
        glLineWidth(3);

        /*
            *	The colours for the outline are reversed when the button. 
            */
        if (b->pressed)
            glColor3f(0.4f, 0.4f, 0.4f);
        else
            glColor3f(0.8f, 0.8f, 0.8f);

        glBegin(GL_LINE_STRIP);
        glVertex2i(b->x + b->width, b->y);
        glVertex2i(b->x, b->y);
        glVertex2i(b->x, b->y + b->height);
        glEnd();

        if (b->pressed)
            glColor3f(0.8f, 0.8f, 0.8f);
        else
            glColor3f(0.4f, 0.4f, 0.4f);

        glBegin(GL_LINE_STRIP);
        glVertex2i(b->x, b->y + b->height);
        glVertex2i(b->x + b->width, b->y + b->height);
        glVertex2i(b->x + b->width, b->y);
        glEnd();

        glLineWidth(1);

        /*
            *	Calculate the x and y coords for the text string in order to center it.
            */
        fontx = b->x + (b->width - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<unsigned char*>(b->label))) / 2;
        fonty = b->y + (b->height + 10) / 2;

        /*
            *	if the button is pressed, make it look as though the string has been pushed
            *	down. It's just a visual thing to help with the overall look....
            */
        if (b->pressed) {
            fontx += 2;
            fonty += 2;
        }

        /*
            *	If the cursor is currently over the button we offset the text string and draw a shadow
            */
        if (b->highlighted) {
            glColor3f(0, 0, 0);
            Font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
            fontx--;
            fonty--;
        }

        glColor3f(1, 1, 1);
        Font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
    }
}

/*----------------------------------------------------------------------------------------
    *	This function will be used to draw an overlay over the 3D scene.
    *	This will be used to draw our fonts, buttons etc......
    */
void Draw2D()
{
    ButtonDraw(&button_1);
    ButtonDraw(&button_2);
    ButtonDraw(&button_3);
}

/*----------------------------------------------------------------------------------------
    *	This is the main display callback function. It sets to 
    *	an orthographic projection and calls Draw2D().
    */
void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Draw2D();

    glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
    *	This function is called when the window is resized. All this does is simply 
    *	store the new width and height of the window which are then referenced by
    *	the draw function to set the correct viewing transforms 
    */
void Resize(int w, int h)
{
    window_width = w;
    window_height = h;

    button_1.width = window_width / 3 - 12;
    button_2.width = window_width / 3 - 12;
    button_3.width = window_width / 3 - 12;

    button_2.x = window_width / 3 + 5;
    button_3.x = window_width * 2 / 3 + 5;

    glViewport(0, 0, w, h);
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function is called whenever a mouse button is pressed or released
    *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
    *	\param	pressed	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
    *						or pressed respectivly. 
    *	\param	x		-	the x-coord of the mouse cursor.
    *	\param	y		-	the y-coord of the mouse cursor.
    */
void MouseButton(int mouse_button, int pressed, int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    if (pressed == GLUT_DOWN) {
        /*
            *	This holds the location of the first mouse click
            */
        if (!(TheMouse.right_button_pressed || TheMouse.middle_button_pressed || TheMouse.right_button_pressed)) {
            TheMouse.xpress = x;
            TheMouse.ypress = y;
        }

        switch (mouse_button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.right_button_pressed = 1;
            ButtonPress(&button_1, x, y);
            ButtonPress(&button_2, x, y);
            ButtonPress(&button_3, x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.middle_button_pressed = 1;
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.right_button_pressed = 1;
            break;
        }
    } else {

        switch (mouse_button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.right_button_pressed = 0;
            ButtonRelease(&button_1, x, y);
            ButtonRelease(&button_2, x, y);
            ButtonRelease(&button_3, x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.middle_button_pressed = 0;
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.right_button_pressed = 0;
            break;
        }
    }

    glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
    *	\param	x	-	the new x-coord of the mouse cursor.
    *	\param	y	-	the new y-coord of the mouse cursor.
    */
void MouseMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    ButtonPassive(&button_1, x, y);
    ButtonPassive(&button_2, x, y);
    ButtonPassive(&button_3, x, y);

    glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
    *	\param	x	-	the new x-coord of the mouse cursor.
    *	\param	y	-	the new y-coord of the mouse cursor.
    */
void MousePassiveMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    /*
        *	Check buttons to see if we should highlight it
        */
    ButtonPassive(&button_1, x, y);
    ButtonPassive(&button_2, x, y);
    ButtonPassive(&button_3, x, y);
}

int main(int argc, char** argv)
{

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

    glutMainLoop();
}

