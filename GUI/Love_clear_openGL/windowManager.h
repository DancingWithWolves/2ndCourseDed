#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG


#include <GL/glut.h>

#define MSG_TO_LOG(format, args...)    \
    FILE* log = fopen(log_name, "at"); \
    fprintf(log, format, ##args);      \
    fclose(log);

#ifdef DEBUG
#define ON_DEBUG(param) param
#else
#define ON_DEBUG(param) ;
#endif

// Количество кнопок
extern const int buttons_qty;

const char log_name[] = "GUI.log";

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
// TODO: правило трёх
Mouse TheMouse = Mouse(); //дефолтный конструктор всё занулит, благо, типы примитивные

int window_width = 640;
int window_height = 480;

struct Button {

    typedef void (*ButtonCallback)();

private:
    static size_t qty;
    size_t id;

public:
    static size_t GetQty() { return qty; }
    size_t GetId() { return id; }

    explicit Button(int x = 0, int y = 0, int width = 0, int height = 0, const char text[] = nullptr, ButtonCallback function = nullptr, bool active = true)
        : callback_function(function)
        , x(x)
        , y(y)
        , height(height)
        , width(width)
        , pressed(false)
        , highlighted(false)
        , active (active)
    {
        this->label = strdup(text);
        this->id = qty;
        ++qty;

        ON_DEBUG(MSG_TO_LOG("Created button with id '%lu'!\n", id);)
    }

    ~Button()
    {

        ON_DEBUG(MSG_TO_LOG("Deleted button [%p] with id '%lu' and label '%s'!\n", this, id, label);)

        --qty;
        free(label);
    }
    explicit Button(const Button& from)
        : callback_function(from.callback_function)
        , x(from.x)
        , y(from.y)
        , height(from.height)
        , width(from.width)
        , pressed(false)
        , highlighted(false)
        , active(from.active)
    {
        this->label = strdup(from.label);
        ++qty;
        this->id = qty;

        ON_DEBUG(MSG_TO_LOG("Copied button with id '%lu'!\n", id);)
    }

    void OnPressed()
    {
        ON_DEBUG(MSG_TO_LOG("Meow there! I'm '%ld' button pressed!\n", id);)
        callback_function();
    }

    int x;
    int y;
    int width;
    int height;
    bool pressed;
    bool highlighted;
    bool active;
    char* label;
    ButtonCallback callback_function;
};

size_t Button::qty = 0;
Button* buttons = nullptr;


/*----------------------------------------------------------------------------------------
    *	\brief	Эта функция рисует текстовую строку на экран, используя шрифты глюта.
    *	\param	font	-	Шрифт. Один из следующих : 
    *
    *					GLUT_BITMAP_9_BY_15		
    *					GLUT_BITMAP_8_BY_13			
    *					GLUT_BITMAP_TIMES_ROMAN_10	
    *					GLUT_BITMAP_TIMES_ROMAN_24	
    *					GLUT_BITMAP_HELVETICA_10	
    *					GLUT_BITMAP_HELVETICA_12	
    *					GLUT_BITMAP_HELVETICA_18	
    *
    *	\param	text	-	Строка на вывод
    *	\param	x		-	Координата x (центр текста)
    *	\param	y		-	Координата y (центр текста)
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
    *	\brief	Проверяет, нажата ли в текущий момент кнопка
    *	\param	b	-	Указатель на кнопку, которую проверяем
    *	\param	x	-	Нажатый x
    *	\param	y	-	Нажатый y
    */
bool ButtonClickTest(Button* b, int x, int y)
{
    ON_DEBUG(assert(b);)
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
    *	\brief	Обработчик нажатия кнопки
    *	\param	b	-	Указатель на кнопку, которую проверяем.
    *	\param	x	-	x-координата мышки.
    *	\param	y	-	y- координата мышки.
    */
void ButtonPress(Button* b, int x, int y)
{
    ON_DEBUG(assert(b);)
    if (b) {
        if (ButtonClickTest(b, x, y) && b->active) {
            ON_DEBUG(MSG_TO_LOG("button[%d]->active = %d\n", b->GetId(), b->active))
            b->pressed = true;
        }
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	Обработчик отпускание кнопки мыши.
    *	\param	b	-	Указатель на кнопку, которую проверяем.
    *	\param	x	-	x-координата мышки.
    *	\param	y	-	y-координата мышки.
    */
void ButtonRelease(Button* b, int x, int y)
{
    ON_DEBUG(assert(b);)
    if (b) {

        if (ButtonClickTest(b, TheMouse.xpress, TheMouse.ypress)
            && ButtonClickTest(b, x, y) && b->pressed) {
            b->OnPressed();
        }
        b->pressed = false;
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	Рисует кнопку.
    *	\param	b	-	Указатель на кнопку, которую проверяем.
    *	\param	x	-	x-координата мышки.
    *	\param	y	-	y-координата мышки.
    */
void ButtonPassive(Button* b, int x, int y)
{
    ON_DEBUG(assert(b);)
    if (b) {

        if (ButtonClickTest(b, x, y)) {

            // Если кнопка ещё не подсвечена, подсвечиваем путём glutPostRedisplay()
            if (b->highlighted == 0) {
                b->highlighted = 1;
                glutPostRedisplay();
            }

        } else {

            // Если курсор ТОЛЬКО ЧТО покинул область кнопки, а она ещё подсвечена, перерисовываем её
            if (b->highlighted) {
                b->highlighted = false;
                glutPostRedisplay();
            }
        }
    }
}

/*----------------------------------------------------------------------------------------
    *	\brief	Рисуем кнопку
    *	\param	b	-	Указатель на кнопку, которую надо нарисовать.
    */
void ButtonDraw(const Button* b)
{
    ON_DEBUG(assert(b);)
    int fontx = 0, fonty = 0;

    if (b != NULL) {

        // Меняем цвет кнопки, если она под курсором
        if (b->highlighted)
            glColor3f(0.7f, 0.7f, 0.8f);
        else
            glColor3f(0.6f, 0.6f, 0.6f);

        glBegin(GL_QUADS);
        glVertex2i(b->x, b->y);
        glVertex2i(b->x, b->y + b->height);
        glVertex2i(b->x + b->width, b->y + b->height);
        glVertex2i(b->x + b->width, b->y);
        glEnd();

        glLineWidth(3);

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

        // Вычисляем координаты строчки ровно по центру кнопки
        fontx = b->x + (b->width - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<unsigned char*>(b->label))) / 2;
        fonty = b->y + (b->height + 10) / 2;

        //    Если кнопка нажата, смещаем текст вниз
        if (b->pressed) {
            fontx += 2;
            fonty += 2;
        }

        // Если курсор сейчас над кнопкой, мы смещаем текст и рисуем "тень"
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
*	Функция, отвечающая за отрисовку всех объектов на 2д-оверлей над 3д-полем.
*/
void Draw2D()
{
    for (int i = 0; i < buttons_qty; ++i) {
        ButtonDraw(&buttons[i]);
        
    }
}

/*----------------------------------------------------------------------------------------
*	Главная рисующая функция-коллбек дисплея
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
    *	Эта функция вызывается при изменении размеров экрана. "Под капотом" вызывает Draw().
    */
void Resize(int w, int h)
{
    window_width = w;
    window_height = h;

    buttons[0].width = window_width / 3 - 12; // TODO: написать в общем случае
    buttons[1].width = window_width / 3 - 12;
    buttons[2].width = window_width / 3 - 12;

    buttons[1].x = window_width / 3 + 5;
    buttons[2].x = window_width * 2 / 3 + 5;

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

        if (!(TheMouse.right_button_pressed || TheMouse.middle_button_pressed || TheMouse.right_button_pressed)) {
            TheMouse.xpress = x;
            TheMouse.ypress = y;
        }

        switch (mouse_button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.right_button_pressed = 1;
            for (int i = 0; i < buttons_qty; ++i) {
                ButtonPress(&buttons[i], x, y);
            }
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

            for (int i = 0; i < buttons_qty; ++i) {
                ButtonRelease(&buttons[i], x, y);
            }

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
    *	\brief	Эта функция вызывается каждый раз, когда мышка двигается и какая-то кнопка нажата.
    *	\param	x	-	новая x-координата мышки.
    *	\param	y	-	новая y-координата мышки.
    */
void MouseMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    for (int i = 0; i < buttons_qty; ++i) {
        ButtonPassive(&buttons[i], x, y);
    }

    glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
    *	\brief	Эта функция вызывается каждый раз, когда мышка двигается и никакая кнопка не нажата.
    *	\param	x	-	новая x-координата мышки.
    *	\param	y	-	новая y-координата мышки.
    */
void MousePassiveMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    //	Проверяем, не подсветить ли кнопки
    for (int i = 0; i < buttons_qty; ++i) {
        ButtonPassive(&buttons[i], x, y);
    }
}
