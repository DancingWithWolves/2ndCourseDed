#pragma once

#include <algorithm>
#include <assert.h>
#include <forward_list>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#include "layoutConstants.h"

#include <GL/glut.h>


const char divider_str_in[] = "\n=================================================================\\\n";
const char divider_str_out[] = "=================================================================/\n\n";
FILE* LOG = nullptr;

#define MSG_TO_LOG(format, args...) \
    LOG = fopen(log_name, "at");    \
    fprintf(LOG, "%s", divider_str_in);   \
    fprintf(LOG, format, ##args);   \
    fprintf(LOG, "%s", divider_str_out);   \
    fclose(LOG);

#ifdef DEBUG
#define ON_DEBUG(param) param
#else
#define ON_DEBUG(param) ;
#endif

extern const int buttons_qty;

extern const int graphs_qty;

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

const char default_label[] = "график";
const char default_x_text[] = "x";
const char default_y_text[] = "y";

int window_width = 640;
int window_height = 480;

/*----------------------------------------------------------------------------------------
    *	\brief	Эта функция рисует текстовую строку на экран, используя шрифты глюта.
    *	\param	font	-	Шрифт. Один из следующих : 
    *
    *					GLUT_BITMAP_9_BY_15		\
    *					GLUT_BITMAP_8_BY_13		\	
    *					GLUT_BITMAP_TIMES_ROMAN_10	\
    *					GLUT_BITMAP_TIMES_ROMAN_24	\
    *					GLUT_BITMAP_HELVETICA_10	\
    *					GLUT_BITMAP_HELVETICA_12	/
    *					GLUT_BITMAP_HELVETICA_18	\
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

struct Drawable {

    Drawable(int x = 0, int y = 0, int width = 0, int height = 0)
        : x(x)
        , y(y)
        , width(width)
        , height(height)
    {
    }

    Drawable(const Drawable& from)
        : x(from.x)
        , y(from.y)
        , width(from.width)
        , height(from.height)
    {
    }

    bool CheckMouseOver(int mouse_x, int mouse_y)
    {
        if (mouse_x > x && mouse_x < x + width && mouse_y > y && mouse_y < y + height) {
            return true;
        }

        return false;
    }

    void virtual Draw() = 0;
    void virtual Resize(int new_window_width, int new_window_height) = 0;

    int x, y, width, height;
};

std::forward_list<Drawable*> drawable_list;

// Вспомогательная структура для графика

struct Point {
    float x;
    float y;

    const bool operator>(const Point& point)
    {
        if (this->x > point.x)
            return true;
        if (this->x == point.x)
            return this->y > point.y;
        return false;
    }

    const bool operator==(const Point& point)
    {
        if (this->x == point.x && this->y == point.y)
            return true;
        return false;
    }

    const bool operator<(const Point& point)
    {
        if (this->x < point.x)
            return true;
        if (this->x == point.x)
            return this->y < point.y;
        return false;
    }

    Point()
        : x(0)
        , y(0)
    {
    }

    Point(const float& x, const float& y)
        : x(x)
        , y(y)
    {
    }
};

// Имплементация графика

struct GraphManager : public Drawable {

    int range_x_low, range_x_up;
    int range_y_low, range_y_up;

    int graph_width, graph_height;
    int graph_x, margin_to_graph_x, graph_y, margin_to_graph_y;

    struct Graph {

        GraphManager* daddy;
        const size_t points_qty;
        Point* points;
        float y_max, y_min, x_max, x_min;

        Graph(GraphManager* daddy, const size_t points_qty, const float* x_values, const float* y_values, bool sorted = false)
            : points_qty(points_qty)
            , daddy(daddy)
        {
            assert(daddy);
            assert(x_values);
            assert(y_values);
            assert(points_qty > 0);
            ON_DEBUG(MSG_TO_LOG("Hello there! I am Graph[%p] being constructed\n", this))

            x_min = x_values[0];
            x_max = x_values[0];

            y_min = y_values[0];
            y_max = y_values[0];

            points = reinterpret_cast<Point*>(::operator new(sizeof(Point) * points_qty));

            Point* tmp_point = nullptr;

            for (size_t i = 0; i < points_qty; ++i)
                tmp_point = new (points + i) Point(x_values[i], y_values[i]);

            if (!sorted)
                std::sort(points, points + points_qty - 1);

            if (points_qty != 0 && points[points_qty - 1].x == 0) {
                ON_DEBUG(MSG_TO_LOG("Graph[%p]'s x_max = 0! Set it to 1.", this))
                points[points_qty - 1].x = 1; //Чтобы на 0 не поделить ненароком
            }

            for (size_t i = 0; i < points_qty; ++i) {
                if (points[i].x < x_min)
                    x_min = points[i].x;

                if (points[i].x > x_max)
                    x_max = points[i].x;

                if (points[i].y < y_min)
                    y_min = points[i].y;

                if (points[i].y > y_max)
                    y_max = points[i].y;
            }

            if (y_max == 0) {
                ON_DEBUG(MSG_TO_LOG("Graph[%p]'s y_max = 0! Set it to 1.", this))
                y_max = 1;
            }
            if (x_max == 0) {
                ON_DEBUG(MSG_TO_LOG("Graph[%p]'s x_max = 0! Set it to 1.", this))
                x_max = 1;
            }


            if (x_min < daddy->range_x_low
                || x_max < daddy->range_x_up
                || y_min < daddy->range_y_low
                || y_max > daddy->range_y_up) {
                daddy->ChangeRange(
                    std::min(x_min
                    , static_cast<float>(daddy->range_x_low))
                    , std::max(x_max, static_cast<float>(daddy->range_x_up))
                    , std::min(y_min, static_cast<float>(daddy->range_y_low))
                    , std::max(y_max, static_cast<float>(daddy->range_y_up)));
            }

            ON_DEBUG(MSG_TO_LOG("Meow there! I am Graph[%p] constructed\n", this))
        }

        ~Graph()
        {
            // ::operator delete (points);
        }

        void Draw()
        {
            assert(daddy);
            int range_x = daddy->range_x_up - daddy->range_x_low;
            assert(range_x > 0);

            int range_y = daddy->range_y_up - daddy->range_y_low;
            assert(range_y > 0);

            const float dx = daddy->graph_width / range_x;
            const float dy = daddy->graph_height / range_y;


            glBegin(GL_LINE_STRIP);

            for (size_t i = 0; i < points_qty; ++i) {
                glVertex2i(daddy->graph_x + (points[i].x - daddy->range_x_low) * dx, daddy->graph_y + daddy->graph_height - (points[i].y - daddy->range_y_low) * dy);
            }


            glEnd();
        }
    };

    GraphManager( int x = 0
                , int y = 0
                , int width = 0
                , int height = 0
                , int margin_to_graph_x = 0
                , int margin_to_graph_y = 0
                , const char* label = default_label
                , const char* x_axis_text = default_x_text
                , const char* y_axis_text = default_y_text
                , int range_x_low = 0
                , int range_x_up = 0
                , int range_y_low = 0
                , int range_y_up = 0)
        : Drawable(x, y, width, height)
        , margin_to_graph_x (margin_to_graph_x)
        , margin_to_graph_y (margin_to_graph_y)
        , graph_x (x + margin_to_graph_x)
        , graph_y (y + margin_to_graph_y)
        , graphs_qty(0)
        , range_x_low(range_x_low)
        , range_x_up(range_x_up)
        , range_y_low(range_y_low)
        , range_y_up(range_y_up)
    {
        this->graph_width = this->width - 2 * margin_to_graph_x;
        this->graph_height = this->height - 2 * margin_to_graph_y;
        this->label = strdup(label);

        this->x_axis_text = strdup(x_axis_text);
        this->y_axis_text = strdup(y_axis_text);

        TellMeEverythingIWannaHear();
    }

    GraphManager(const GraphManager& from)
        : Drawable(from)
        , graphs_qty(from.graphs_qty)
        , graph_height(from.graph_height)
        , graph_width(from.graph_width)
        , graph_x(from.graph_x)
        , graph_y(from.graph_y)
    {
        this->label = strdup(from.label);

        this->x_axis_text = strdup(from.x_axis_text);
        this->y_axis_text = strdup(from.y_axis_text);

        this->graphs = from.graphs;
    }

    ~GraphManager()
    {
        free(label);
        free(x_axis_text);
        free(y_axis_text);
    }

    void Draw()
    {
        glColor3f(BUTTON_COLOR);

        glBegin(GL_QUADS);
        glVertex2i(this->x, this->y);
        glVertex2i(this->x, this->y + this->height);
        glVertex2i(this->x + this->width, this->y + this->height);
        glVertex2i(this->x + this->width, this->y);
        glEnd();

        glColor3f(BUTTON_HIGHLITED_COLOR);
        glBegin(GL_QUADS);
        glVertex2i(this->graph_x, this->graph_y);
        glVertex2i(this->graph_x, this->graph_y + this->graph_height);
        glVertex2i(this->graph_x + this->graph_width, this->graph_y + this->graph_height);
        glVertex2i(this->graph_x + this->graph_width, this->graph_y);
        glEnd();

        glColor3f(1, 1, 1);
        for (auto& graph : graphs) {
            graph.Draw();
        }
    }

    void Resize(int new_width, int new_height)
    {

    }

    void ChangeRange(int new_x_low, int new_x_up, int new_y_low, int new_y_up)
    {
        SetRangeX(std::min(new_x_low, this->range_x_low), std::max(new_x_up, this->range_x_up));
        SetRangeY(std::min(new_y_low, this->range_y_low), std::max(new_y_up, this->range_y_up));
        ON_DEBUG(MSG_TO_LOG("GraphManager [%p]'s new range is:\n\tx_low %d\n\tx_up %d\n\ty_low %d\n\ty_up %d\n", this, this->range_x_low, this->range_x_up, this->range_y_low, this->range_y_up))
        Draw();
    }

    void SetRangeX(int new_low, int new_up)
    {
        this->range_x_low = new_low;
        this->range_x_up = new_up;
    }

    void SetRangeY(int new_low, int new_up)
    {
        this->range_y_low = new_low;
        this->range_y_up = new_up;
    }

    void AddGraph(const Graph& graph)
    {
        graphs.push_front(graph);
    }

    void AddGraph(size_t points_qty, const float* x_values, const float* y_values, bool sorted = true)
    {
        Graph graph(this, points_qty, x_values, y_values, sorted);
        graphs.push_front(graph);
    }

private:
    void TellMeEverythingIWannaHear()
    {
#ifndef DEBUG
        return nullptr;
#endif
        LOG = fopen(log_name, "at");
        fprintf(LOG, "%s\n", divider_str_in);

        fprintf(LOG, "Meow there! I am GraphManager [%p].\n", this);

        fprintf(LOG, "\tthis->x = %d;\n", this->x);

        fprintf(LOG, "\tthis->y = %d;\n", this->y);

        fprintf(LOG, "\tthis->width = %d;\n", this->width);

        fprintf(LOG, "\tthis->height = %d;\n", this->height);

        fprintf(LOG, "\tthis->label[%p] = %s;\n", this->label, this->label);

        fprintf(LOG, "\tthis->x_axis_text[%p] = %s;\n", this->x_axis_text, this->x_axis_text);

        fprintf(LOG, "\tthis->y_axis_text[%p] = %s;\n", this->y_axis_text, this->y_axis_text);

        fprintf(LOG, "\tthis->range_x_low = %lf;\n", this->range_x_low);
        fprintf(LOG, "\tthis->range_x_up = %lf;\n", this->range_x_up);
        fprintf(LOG, "\tthis->range_y_low = %lf;\n", this->range_y_low);
        fprintf(LOG, "\tthis->range_y_up = %lf;\n", this->range_y_up);

        // fprintf(LOG, "\tthis->points[%p] = %d;\n", this->points);

        // for (size_t i = 0; i < points_qty; ++i) {
        //     fprintf(LOG, "\t\tpoints[%lu]: x = %lf, y = %lf\n", i, points[i].x, points[i].y); //TODO: написать темплейтовую версию
        // }

        fprintf(LOG, "\n%s\n\n", divider_str_out);
        fclose(LOG);
    }

    char* label;

    char* x_axis_text;
    char* y_axis_text;

    std::forward_list<Graph> graphs;
    int graphs_qty;
};

// Имплементация кнопки
typedef void (*Callback)();

struct Clickable : public Drawable {

    Clickable(int x = 0, int y = 0, int width = 0, int height = 0, Callback callback = nullptr)
        : Drawable(x, y, width, height)
        , callback(callback)
        , pressed(false)
        , highlighted(false)
        , active(true)
    {
    }

    Clickable(const Clickable& from)
        : Drawable(from.x, from.y, from.width, from.height)
        , callback(from.callback)
        , pressed(false)
        , highlighted(false)
        , active(true)
    {
    }

    void virtual Passive(int mouse_x, int mouse_y) = 0;
    void virtual OnPress(int mouse_x, int mouse_y) = 0;
    void virtual OnRelease(int mouse_x, int mouse_y) = 0;
    bool pressed, highlighted, active;

protected:
    Callback callback;
};

struct Button : public Clickable {
    Button(int x = 0, int y = 0, int width = 0, int height = 0, Callback callback = nullptr, const char* text = nullptr)

        : Clickable(x, y, width, height, callback)
    {
        id = qty++;
        label = strdup(text);
        ON_DEBUG(MSG_TO_LOG("Created button with id '%lu'!\n", id);)
    }

    ~Button()
    {
        ON_DEBUG(MSG_TO_LOG("Deleted button [%p] with id '%lu' and label '%s'!\n", this, id, label);)
        free(label);
        --qty;
    }

    Button(const Button& from)
        : Clickable(from)
    {
        id = qty++;
        label = strdup(from.label);
        ON_DEBUG(MSG_TO_LOG("Copied Button with id '%lu'!\n", id);)
    }

    void Passive(int mouse_x, int mouse_y)
    {
        if (this->CheckMouseOver(mouse_x, mouse_y)) {

            // Если кнопка ещё не подсвечена, подсвечиваем путём glutPostRedisplay()
            if (this->highlighted == 0) {
                this->highlighted = 1;
                glutPostRedisplay();
            }
        } else {

            // Если курсор ТОЛЬКО ЧТО покинул область кнопки, а она ещё подсвечена, перерисовываем её
            if (this->highlighted) {
                this->highlighted = false;
                glutPostRedisplay();
            }
        }
    }

    void OnPress(int mouse_x, int mouse_y)
    {
        if (CheckMouseOver(mouse_x, mouse_y) && active) {
            ON_DEBUG(MSG_TO_LOG("button[%lu]->active = %d\n", GetId(), active))
            pressed = true;
        }
    }

    void OnRelease(int mouse_x, int mouse_y)
    {
        if (CheckMouseOver(TheMouse.xpress, TheMouse.ypress) && CheckMouseOver(mouse_x, mouse_y) && pressed) {
            ON_DEBUG(MSG_TO_LOG("Meow there! I'm '%lu' Button pressed!\n", id);)
            callback();
        }
        pressed = false;
    }

    size_t GetId() { return id; }

    void Draw()
    {
        int fontx = 0, fonty = 0;

        // Меняем цвет кнопки, если она под курсором
        if (this->highlighted)
            glColor3f(BUTTON_HIGHLITED_COLOR);
        else
            glColor3f(BUTTON_COLOR);

        glBegin(GL_QUADS);
        glVertex2i(this->x, this->y);
        glVertex2i(this->x, this->y + this->height);
        glVertex2i(this->x + this->width, this->y + this->height);
        glVertex2i(this->x + this->width, this->y);
        glEnd();

        glLineWidth(3);

        if (this->pressed)
            glColor3f(STRIP_COLOR_2);
        else
            glColor3f(STRIP_COLOR_1);

        glBegin(GL_LINE_STRIP);
        glVertex2i(this->x + this->width, this->y);
        glVertex2i(this->x, this->y);
        glVertex2i(this->x, this->y + this->height);
        glEnd();

        if (this->pressed)
            glColor3f(STRIP_COLOR_1);
        else
            glColor3f(STRIP_COLOR_2);

        glBegin(GL_LINE_STRIP);
        glVertex2i(this->x, this->y + this->height);
        glVertex2i(this->x + this->width, this->y + this->height);
        glVertex2i(this->x + this->width, this->y);
        glEnd();

        glLineWidth(1);

        // Вычисляем координаты строчки ровно по центру кнопки
        fontx = this->x + (this->width - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<unsigned char*>(this->label))) / 2;
        fonty = this->y + (this->height + 10) / 2;

        // Если кнопка нажата, смещаем текст вниз
        if (this->pressed) {
            fontx += 2;
            fonty += 2;
        }

        // Если курсор сейчас над кнопкой, мы смещаем текст и рисуем "тень"
        if (this->highlighted) {
            glColor3f(0, 0, 0);
            ::Font(GLUT_BITMAP_HELVETICA_10, this->label, fontx, fonty);
            fontx--;
            fonty--;
        }

        glColor3f(1, 1, 1);
        ::Font(GLUT_BITMAP_HELVETICA_10, this->label, fontx, fonty);
    }

    void Resize(int new_width, int new_height)
    {

    }
private:
    static size_t qty;
    size_t id;
    char* label;
};

size_t Button::qty = 0;

Button* buttons = nullptr;

GraphManager* graph_managers = nullptr;

/*----------------------------------------------------------------------------------------
*	Функция, отвечающая за отрисовку всех объектов на 2д-оверлей над 3д-полем.
*/
void Draw2D()
{
    for (auto i : drawable_list) {
        i->Draw();
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

    Draw2D();

    glLoadIdentity();
    glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
    *	Эта функция вызывается при изменении размеров экрана. "Под капотом" вызывает Draw().
    */
void Resize(int widht, int height)
{
    if (height == 0)
        height = 1; //Чтобы на 0 не поделить ненароком
    window_width = widht;
    window_height = height;

    int new_button_width = window_width / 3 - 12;

    for (size_t i = 0; i < buttons_qty; ++i) {
        buttons[i].width = new_button_width;
        if (i >= 1) {
            buttons[i].x = window_width / 3 * i + 5;
        }
    }

    int new_graph_managers_width = std::min(window_width / 2 - 2 * default_margin, window_height - button_height - 5 * default_margin);

    graph_managers[0].x = window_width / 2 - default_margin - new_graph_managers_width;
    graph_managers[1].x = window_width / 2 + default_margin;

    for (int i = 0; i < graphs_qty; ++i) {
        graph_managers[i].width = new_graph_managers_width;
        graph_managers[i].height = new_graph_managers_width;

        graph_managers[i].graph_x = graph_managers[i].x + graph_managers[i].margin_to_graph_x;
        graph_managers[i].graph_y = graph_managers[i].y + graph_managers[i].margin_to_graph_y;

        graph_managers[i].graph_width = new_graph_managers_width - 2 * graph_managers[i].margin_to_graph_x;
        graph_managers[i].graph_height = new_graph_managers_width - 2 * graph_managers[i].margin_to_graph_y;
    }
    
    

    glViewport(0, 0, widht, height);
}

/*----------------------------------------------------------------------------------------
    *	\brief	This function is called whenever a mouse Button is pressed or released
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
                buttons[i].OnPress(x, y);
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
                buttons[i].OnRelease(x, y);
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
    *	\brief	Эта функция вызывается каждый раз, когда мышка двигается и какая-то кнопка мыши нажата.
    *	\param	x	-	новая x-координата мышки.
    *	\param	y	-	новая y-координата мышки.
    */
void MouseMotion(int x, int y)
{

    TheMouse.x = x;
    TheMouse.y = y;

    for (int i = 0; i < buttons_qty; ++i) {
        buttons[i].Passive(x, y);
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
        buttons[i].Passive(x, y);
    }
}
